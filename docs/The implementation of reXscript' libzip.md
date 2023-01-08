# The implementation of reXscript' libzip

## Before you read this:

Thanks for the developers of `libzip`, I've found the solution of your product `libzip` couldn't compile on my machine for all day, and finally I only made it work on my linux machine. Therefore, I gave up.

## Backend

We used `kuba--/zip` for our backend.

It seemed a bit difficult to port to reXscript, like `callable` functions adapt in `C`.

## API

- `std.zip.open(path, mode)`	

  Type: `function`

  Returns: `vObject` that contains `std.zip.file`

  Usage: to open a zipfile
  
- `std.zip.extract(zipFile, path, callback)`	

  Type: `function`

  Returns: `vNull`

  Usage: to extract a zipfile to specified path.
  
  Callback format: `func(filename)` or `lambda (...) -> (filename)`

- `std.zip.file.entry(filename)`

  Type: `function`

  Returns: `vObject` that contains `std.zip.file.entry`

  Usage: to open a file in zip archive

  Note: Entry is single-instance only, you can't open two entries in the same time.

- `std.zip.file.entry.size()`

  Type: `function`

  Returns: `vObject` that contains the file size

  Usage: get the entry file size

- `std.zip.file.entry.crc32()`

  Type: `function`

  Returns: `vInt` that contains the result

  Usage: get the crc32 of the entry

- `std.zip.file.entry.isDir()`

  Type: `function`

  Returns: `vBool`

  Usage: to check if the entry is directory

- `std.zip.file.entry.extract(callback)`

  Type: `function`

  Returns: `vNull`

  Usage: extract a file in data stream

  Note: Callback receives one param : `vBytes` that contains the received data.

- `std.zip.file.entry.close()`

  Type: `function`

  Returns: `vNull`

  Usage: close a zip entry

- `std.zip.file.entries()`

  Type: `function`

  Returns: `vInt` that contains the entries count

  Usage: get the number of entries
  
- `std.zip.file.delete(entryName)`

  Type: `function`

  Returns: `vInt` that contains the entries count

  Usage: get the number of entries

- `std.zip.file.rexIter()`

  Type: `function`

  Returns: `vObject` that contains `std.zip.file.iter`

  Usage: to iterate all zip entries

- `std.zip.file.iter.next()`

  Type: `function`

  Returns: `vObject` that contains `std.zip.file.entry`

  Usage: to iterate all zip entries