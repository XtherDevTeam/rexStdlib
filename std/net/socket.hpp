//
// Created by XIaokang00010 on 2022/12/30.
//

#ifndef REXSTDLIB_SOCKET_HPP
#define REXSTDLIB_SOCKET_HPP

#include <set>
#include <string>
#include <cstring>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <unistd.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

struct __WSAINIT {
    __WSAINIT () {
        WSADATA wsa_data;
        WSAStartup(MAKEWORD(2, 2), &wsa_data);
    }

    ~__WSAINIT () {
        WSACleanup();
    }
} ___WSAINIT;

#define inet_aton(a,b) inet_pton(AF_INET, (a), (b))
#define close_socket closesocket
#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define close_socket close

#endif

struct sslInfo {
    SSL *ssl{};
    SSL_CTX *ctx{};
    int fd{};
};

struct socketStartup {
    std::shared_ptr<std::set<int>> fdSet;
    std::shared_ptr<std::set<sslInfo *>> sslConnectionSet;

    socketStartup() {
        fdSet = std::make_shared<std::set<int>>();
        sslConnectionSet = std::make_shared<std::set<sslInfo *>>();
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();
    }

    ~socketStartup() {
        for (auto &i: *sslConnectionSet) {
            SSL_CTX_free(i->ctx);
            SSL_shutdown(i->ssl);
            SSL_free(i->ssl);
            delete i;
        }
        for (auto &i: *fdSet)
            close_socket(i);
        fdSet->clear();
    }
} socketStartupInstance;

namespace libnet {
    // Parse the given domain name and return the resolved IP address as a string
    std::string resolve(const std::string &domain_name) {
        // Set up hints for the getaddrinfo function
        addrinfo hints{};
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET; // We want IPv4 addresses
        hints.ai_socktype = SOCK_STREAM; // We want TCP sockets

        // Call getaddrinfo to resolve the domain name
        addrinfo *addr_list;
        int error = getaddrinfo(domain_name.c_str(), "80", &hints, &addr_list);
        if (error != 0) {
            throw std::runtime_error(std::string{"Failed to resolve domain name: "} + gai_strerror(error));
        }

        // Convert the resolved IP address to a string
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &((sockaddr_in *) addr_list->ai_addr)->sin_addr, ip_str, INET_ADDRSTRLEN);

        // Free the memory allocated by getaddrinfo
        freeaddrinfo(addr_list);

        // Return the resolved IP address
        return {(char *) ip_str};
    }

    int socket() {
        if (auto result = ::socket(AF_INET, SOCK_STREAM, 0); result < 0) {
            throw std::runtime_error("libnet: error creating socket");
        } else {
            socketStartupInstance.fdSet->insert(result);
            return result;
        }
    }

    void socketConnect(int fd, const std::string &ipAddr, unsigned short port) {
        struct sockaddr_in serv_addr{};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        if (inet_aton(ipAddr.c_str(), &serv_addr.sin_addr) == 0) {
            throw std::runtime_error("libnet: error parsing ip address");
        }
        if (connect(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            throw std::runtime_error("libnet: error connecting to server: " + ipAddr + " port " + std::to_string(port));
        }
    }

    void socketBind(int fd, const std::string &ipAddr, unsigned short port) {
        struct sockaddr_in serv_addr{};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        if (inet_aton(ipAddr.c_str(), &serv_addr.sin_addr) == 0) {
            throw std::runtime_error("libnet: error parsing ip address");
        }

        if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            throw std::runtime_error("libnet: error binding to address: " + ipAddr + " port " + std::to_string(port));
        }
    }

    void socketListen(int fd, int backlog = 5) {
        if (listen(fd, 5) < 0)
            throw std::runtime_error("libnet: error listening to the socket");
    }

    int socketAccept(int fd, struct sockaddr_in &clientAddr, socklen_t &clientAddrLen) {
        if (auto nfd = accept(fd, (struct sockaddr *) &clientAddr, &clientAddrLen); nfd > 0) {
            socketStartupInstance.fdSet->insert(nfd);
            return nfd;
        } else {
            throw std::runtime_error("libnet: error accepting incoming connection");
        }
    }

    void socketSend(int fd, const std::string &buf, int sendFlag = 0) {
        if (send(fd, buf.data(), buf.size(), sendFlag) < 0) {
            throw std::runtime_error("libnet: error sending message");
        }
    }

    void socketRecv(int fd, std::string &buf, int recvFlag = 0) {
        if (auto res = recv(fd, buf.data(), buf.size(), recvFlag); res < 0) {
            throw std::runtime_error("libnet: error receiving message");
        } else if (res == 0) {
            buf = {};
        } else {
            buf.resize(res);
        }
    }

    void socketClose(int fd) {
        close_socket(fd);
        socketStartupInstance.fdSet->erase(fd);
    }

    sslInfo *sslSocket(int fd) {
        auto *r = new sslInfo();
        if((r->ctx = SSL_CTX_new(TLS_method())) == nullptr)
            throw std::runtime_error("libnet: error creating ssl context");
        r->ssl = SSL_new(r->ctx);
        SSL_set_fd(r->ssl, fd);
        if (auto retCode = SSL_connect(r->ssl); retCode != 1) {
            throw std::runtime_error("libnet: error making ssl connection: ErrorCode: " + std::to_string(SSL_get_error(r->ssl,retCode)));
        }
        r->fd = fd;
        socketStartupInstance.sslConnectionSet->insert(r);
        return r;
    }



    void sslSend(sslInfo *fd, const std::string &data) {
        if (SSL_write(fd->ssl, data.data(), (int)data.size()) <= 0) {
            throw std::runtime_error("libnet: error sending message");
        }
    }

    void sslRecv(sslInfo *fd, std::string &dest) {
        if (auto res = SSL_read(fd->ssl, dest.data(), (int)dest.size()); res < 0) {
            throw std::runtime_error("libnet: error receiving message");
        } else if (res == 0) {
            dest = {};
        } else {
            dest.resize(res);
        }
    }

    void sslClose(sslInfo *fd) {
        SSL_CTX_free(fd->ctx);
        SSL_shutdown(fd->ssl);
        SSL_free(fd->ssl);
        close_socket(fd->fd);
        socketStartupInstance.fdSet->erase(fd->fd);
        socketStartupInstance.sslConnectionSet->erase(fd);
    }
}

#endif //REXSTDLIB_SOCKET_HPP
