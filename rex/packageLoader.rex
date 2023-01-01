let __module__ = nativeImport(format("${str}/libstd.${str}", rexPackage, rexDylibSuffix));
let zipfile = nativeImport(format("${str}/libstd_zipfile.${str}", rexPackage, rexDylibSuffix));
let fs = __module__.fs;
let sqlite = __module__.sqlite;
let net = __module__.net;