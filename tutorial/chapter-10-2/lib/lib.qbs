// --8<-- [start:snippet0]
DynamicLibrary {
    condition: {
        if (qbs.toolchainType === "msvc"
            || qbs.toolchainType === "gcc"
            || (qbs.toolchainType === "mingw" && cpp.compilerVersionMajor >= 13)
            || (qbs.toolchainType === "clang" && cpp.compilerVersionMajor >= 16))
            return true;
        console.info("Unsupported toolchainType " + qbs.toolchainType);
        return false;
    }
    name: "mylib"
    files: ["hello.cppm", "lib_global.h"]
    version: "1.0.0"
    install: true

    Depends { name: "cpp" }
    cpp.defines: "MYLIB_LIBRARY"
    cpp.sonamePrefix: qbs.targetOS.contains("darwin") ? "@rpath" : undefined
    cpp.cxxLanguageVersion: "c++20"
    cpp.forceUseCxxModules: true

    Depends { name: "bundle" }
    bundle.isBundle: false
}
// --8<-- [end:snippet0]
