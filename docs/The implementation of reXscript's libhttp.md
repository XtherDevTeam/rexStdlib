# The implementation of reXscript's libhttp

## 主要 API 列表

- `http.open(method, url[, optionalArgs])` 发起一个 HTTP 请求 需要设置请求方法以及目标 `url`，`optionalArgs` 接收一个 `vObject`，在内设置可选的请求参数。方法调用后返回一个 `response` 对象。

- `http.parseHttpHeader(headerStr)` 解析一个HTTP头字符串并将其转换为 `vObject`。
- `http.parseUrl(urlStr)` 解析一个 `url` 字符串并返回解析完的 `vObject`。
- `http.generateHttpHeader(headerObj)` 将一个 `header` 对象序列化为字符串。

## 请求的可选参数

- `stream` 设置是否为流式传输回应报文，可为 `vBool` 型。
- `chunkSize` 设置每次传输回应报文的分块大小，可为 `vInt` 型的正整数，默认为 `1024`。(**Notice: **这仅为建议大小，由于不可控的网络环境，单次获得的 `buffer` 大小可能会小于 `chunkSize`。在使用了 `chunked transfer encoding` 的 `HTTP` 回应中，该项不起作用)
- `body` 设置请求的载荷（即请求报文）可为 `vBytes`，`vStr`，可序列化为 `JSON` 的 `vObject` 对象也可以为一个 `fs.file` 对象。
- `sections` 头部的数据段，可以在这里设置 `User-Agent` 等。
- **NOTE: **不要试图在 `sections` 设置 `Content-Length` 等，会被直接 override 掉。

## Response 对象

- `http.response.headers` 存放了所返回的 `HTTP` 回应头
- `http.response.body` 存放了 `HTTP` 回应的报文，当 可选参数 `stream` 被设置为 `true` 时，该成员为空的 `vBytes` （当报文被 `recv()` 函数不带参数接收时，该项有数据）。
- `http.response.json()` 会将 `http.response.body` 使用 `json` 解析并返回解析后的对象，解析错误时会抛出 `jsonError`
- `http.response.recv([callback])` 用于流式传输 HTTP 回应报文，`callback` 参数为一个 `Callable` 对象，需接受一个 `data` 参数。也可以不传入`callback`，在内存接收报文并设置 `body` 的内容。
- `http.response.recvHeaders()` 用于接收 `Response` 头部，返回 `response` 的引用。

## 原理

### 请求的发送

`http.open` 会将所提供的参数组装成一个 `headerObj` 交给 `http.generateHttpHeader` 生成将要发送的 `http` 请求头部，然后会根据所给出的可选参数 `body` 的类型设置 `Content-Length`，并序列化 `body` 并发送。完成后会自动发送请求，等待服务端回应，只接收回应的 **头部** ，当 请求参数 `stream` 的值为 `false` 时，会自动调用 `recv` 函数接收报文，最后返回一个 `response` 对象。