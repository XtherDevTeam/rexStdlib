let loader = lambda() -> (pkgRoot, mod) {
    if (rexPlatform == "win32") {
        mod = nativeImport(pkgRoot + "/libstd.dll");
    } else if (rexPlatform == "linux") {
        mod = nativeImport(pkgRoot + "/libstd.so");
    } else if (rexPlatform == "darwin") {
        mod = nativeImport(pkgRoot + "/libstd.dylib");
    }
};