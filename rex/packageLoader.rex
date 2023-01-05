let __module__ = require(format("${str}/libstd.${str}", rexPkgRoot, rexDylibSuffix));
let zipfile = require(format("${str}/libstd_zipfile.${str}", rexPkgRoot, rexDylibSuffix));
let fs = __module__.fs;
let sqlite = __module__.sqlite;
let net = __module__.net;
let json = __module__.json;
let time = __module__.time;