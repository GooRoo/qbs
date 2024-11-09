// --8<-- [start:snippet0]
MyLibrary {
    name: "mylib"
    files: [
        "lib.c",
        "lib.h",
        "lib_global.h",
    ]
    cpp.defines: base.concat(["CRUCIAL_DEFINE"])
}
// --8<-- [end:snippet0]
