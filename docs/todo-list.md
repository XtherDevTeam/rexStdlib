# libfs

- Where: `std.fs`

## Methods and members

### std.fs.open

- Type: function
- Call format: `std.fs.open(file, flags)`
- Result: `std.fs.file`
- Throws: `fsError` if file couldn't be opened.

### std.fs.file.read

- Type: function
- Call format: `std.fs.file.read(size)`
- Result: `vBytes` that contains bytes from file
- Throws: `fsError` if EoF

### std.fs.file.write

- Type: function
- Call format: `std.fs.file.write(dest)`
- Result: `null`
- Throws: `fsError` if file is invalid.

### std.fs.file.eof

- Type: function
- Call format: `std.fs.file.eof()`
- Result: `null`
- Throws: `fsError` if file is invalid

### std.fs.file.length

- Type: `vint`
- Default value: `0`

### std.fs.file.tell

- Type: function
- Call format: `std.fs.file.tell()`
- Result: `vint` that contains the current position of stream
- Throws: `fsError` if file is invalid

### std.fs.file.seek

- Type: function
- Call format: `std.fs.file.seek(pos)`
- Result: `null`
- Throws: `fsError` if file is invalid

### std.fs.file.close

- Type: function
- Call format: `std.fs.file.close()`
- Result: `null`
- Throws: `fsError` if file is invalid.

### std.fs.temp

- Type: function
- Call format: `std.fs.temp()`
- Result: `vStr` that contains the path of temp directory

### std.fs.access

- Type: function
- Call format: `std.fs.access(filePath, testFlag)`
- Result: `boolean`

### std.fs.stat

- Type: function
- Call format: `std.fs.stat(filePath)`
- Result: `str`
- Throws: `fsError` if something goes wrong

### std.fs.unlink

- Type: function
- Call format: `std.fs.unlink(filePath)`
- Result: `null`
- Throws: `fsError` if something goes wrong

### std.fs.mkdir

- Type: function
- Call format: `std.fs.mkdir(filePath)`
- Result: `null`
- Throws: `fsError` if something goes wrong

### std.fs.mkdirs

- Type: function
- Call format: `std.fs.mkdirs(filePath)`
- Result: `null`
- Throws: `fsError` if something goes wrong

### std.fs.remove

- Type: function
- Call format: `std.fs.remove(path)`
- Result: `null`
- Throws: `fsError` if something goes wrong

### std.fs.removeAll

- Type: function
- Call format: `std.fs.removeAll(path)`
- Result: `null`
- Throws: `fsError` if something goes wrong

### std.fs.listDir

- Type: function
- Call format: `std.fs.listDir(path)`
- Result: `vec` that contains `vstr`
- Throws: `fsError` if something goes wrong

### std.fs.realpath

- Type: function
- Call format: `std.fs.realpath(filePath)`
- Result: `vstr`
- Throws: `fsError` if something goes wrong

### std.fs.copy

- Type: function
- Call format: `std.fs.copy(src, dest)`
- Result: `null`
- Throws: `fsError` if something goes wrong

# libsqlite

- Where: `std.sqlite`

## Methods and members

### std.sqlite.open

- Type: function
- Call format: `std.sqlite.open(path)`
- Result: `std.sqlite.database`
- Throws: `sqliteError` if something goes wrong

### std.sqlite.database.close

- Type: function
- Call format: `std.sqlite.database.close()`
- Result: `null`
- Throws: `sqliteError` if something goes wrong

### std.sqlite.database.execute

- Type: function
- Call format: `std.sqlite.database.execute(sqlStmt)`
- Result: `vec` that contains objects of each column
- Throws: `sqliteError` if something goes wrong

### std.sqlite.database.executeScript

- Type: function
- Call format: `std.sqlite.database.executeScript(sqlStmts)`
- Result: `null`
- Throws: `sqliteError` if something goes wrong

### std.sqlite.database.begin

- Type: function
- Call format: `std.sqlite.database.begin()`
- Result: `null`
- Throws: `sqliteError` if something goes wrong

### std.sqlite.database.commit

- Type: function
- Call format: `std.sqlite.database.commit()`
- Result: `null`
- Throws: `sqliteError` if something goes wrong

### std.sqlite.database.rollback

- Type: function
- Call format: `std.sqlite.database.rollback()`
- Result: `null`
- Throws: `sqliteError` if something goes wrong

# libnet

- Where: `std.net`

## Methods and members

### std.net.resolve

- Type: function
- Call format: `std.net.resolve(url)`
- Result: `str` that contains the ip address
- Throws: `netError` if something goes wrong

### std.net.socket

- Type: function
- Call format: `std.net.socket()`
- Result: `socket` object
- Throws: `netError` if something goes wrong

### std.net.socket.connect

- Type: function
- Call format: `std.net.socket.connect(ip, port)`
- Result: `null`
- Throws: `netError` if something goes wrong

### std.net.socket.close

- Type: function
- Call format: `std.net.socket.close()`
- Result: `null`
- Throws: `netError` if something goes wrong

### std.net.socket.bind

- Type: function
- Call format: `std.net.socket.bind(ip, port)`
- Result: `null`
- Throws: `netError` if something goes wrong

### std.net.socket.listen

- Type: function
- Call format: `std.net.socket.listen(backlog)`
- Result: `null`
- Throws: `netError` if something goes wrong

### std.net.socket.accept

- Type: function
- Call format: `std.net.socket.accept()`
- Result: `socket` object of client
- Throws: `netError` if something goes wrong

### std.net.socket.send

- Type: function
- Call format: `std.net.socket.send(data)`
- Result: `null`
- Throws: `netError` if something goes wrong

### std.net.socket.recv

- Type: function
- Call format: `std.net.socket.recv(size)`
- Result: `vbytes` that contains received data
- Throws: `netError` if something goes wrong

### std.net.socket.setSendFlags

- Type: function
- Call format: `std.net.socket.setSendFlags(flag)`
- Result: `null`

### std.net.socket.getSendFlags

- Type: function
- Call format: `std.net.socket.getSendFlags()`
- Result: `vint`

### std.net.socket.setRecvFlags

- Type: function
- Call format: `std.net.socket.setRecvFlags(flag)`
- Result: `null`

### std.net.socket.getRecvFlags

- Type: function
- Call format: `std.net.socket.getRecvFlags()`
- Result: `vint`

### std.net.http.parseHttpHeaders

- Type: function
- Call format: `std.net.http.parseHttpHeaders(headers)`
- Result: `vObject` that contains the parsed headers.
- Throws: `httpError` if something goes wrong

### std.net.http.parseUrl

- Type: function
- Call format: `std.net.http.parseUrl(url)`
- Result: `vObject` that contains the components of url like `protocol`, `host`, `target` and `port`.
- Throws: `httpError` if something goes wrong
- Notes: If the url didn't provide port number, the field of `port` will be `null`.

### std.net.http.generateHttpHeader

- Type: function
- Call format: `std.net.http.generateHttpHeader(headers)`
- Result: `vstr` that contains the headers
- Throws: `httpError` if something goes wrong

### std.net.http.send

- Type: function
- Call format: `std.net.http.send(method, url)`
- Result: `vObject` that contains the request.
- Throws: `httpError` if something goes wrong
- Notes: use call-chain to set the details of the requests like `.recvAsStream()` `.sendFile()` `.sendJson()` `.callback()`

# libtime

- Where: `std.time`

### std.time.time

- Type: function
- Call format: `std.time.time([timestamp])`
- Result: `vObject` that contains the parsed time structure
- Throws: none
- Notes: timestamp is optional, function will get the current timestamp as the first param if user don't provide