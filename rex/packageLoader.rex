let loader = lambda() -> (pkgRoot, mod) {
    // 将libstd "挂载" 到 mod
    mod = nativeImport(format("${str}/libstd.${str}", pkgRoot, rexDylibSuffix));
    mod.zipfile = nativeImport(format("${str}/libstd_zipfile.${str}", pkgRoot, rexDylibSuffix));
};