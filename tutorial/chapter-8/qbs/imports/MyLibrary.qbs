// --8<-- [start:snippet0]
Library {
    Depends { name: "cpp" }
    Depends { name: "mybuildconfig" }
    // --8<-- [start:snippet1]
    type: mybuildconfig.staticBuild ? "staticlibrary" : "dynamiclibrary"
    // --8<-- [end:snippet1]
    version: "1.0.0"
    install: !mybuildconfig.staticBuild || mybuildconfig.installStaticLib
    installDir: mybuildconfig.libInstallDir

    // --8<-- [start:snippet2]
    readonly property string _nameUpper : name.replace(" ", "_").toUpperCase()
    property string libraryMacro: _nameUpper + "_LIBRARY"
    property string staticLibraryMacro: _nameUpper + "_STATIC_LIBRARY"
    cpp.defines: mybuildconfig.staticBuild ? [staticLibraryMacro] : [libraryMacro]
    // --8<-- [end:snippet2]
    cpp.sonamePrefix: qbs.targetOS.contains("darwin") ? "@rpath" : undefined

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [exportingProduct.sourceDirectory]
        // --8<-- [start:snippet3]
        cpp.defines: exportingProduct.mybuildconfig.staticBuild
            ? [exportingProduct.staticLibraryMacro] : []
        // --8<-- [end:snippet3]
    }

    Depends { name: "bundle" }
    bundle.isBundle: false
}
// --8<-- [end:snippet0]
