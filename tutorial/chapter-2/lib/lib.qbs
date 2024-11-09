// --8<-- [start:snippet0]
StaticLibrary {
    name: "mylib"
    files: [
        "lib.c",
        "lib.h",
    ]
    version: "1.0.0"
    install: true

    // --8<-- [start:snippet1]
    Depends { name: 'cpp' }
    cpp.defines: ['CRUCIAL_DEFINE']
    // --8<-- [end:snippet1]

    // --8<-- [start:snippet2]
    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [exportingProduct.sourceDirectory]
    }
    // --8<-- [end:snippet2]

    // --8<-- [start:snippet3]
    Depends { name: 'bundle' }
    bundle.isBundle: false
    // --8<-- [end:snippet3]
}
// --8<-- [end:snippet0]
