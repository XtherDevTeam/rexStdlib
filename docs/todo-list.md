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