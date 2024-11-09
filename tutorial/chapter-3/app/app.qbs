// --8<-- [start:snippet1]
import qbs.FileInfo

CppApplication {
    Depends { name: "mylib" }
    name: "My Application"
    targetName: "myapp"
    files: "main.c"
    version: "1.0.0"

    consoleApplication: true
    install: true

    // --8<-- [start:snippet0]
    cpp.rpaths: {
        if (!cpp.rpathOrigin)
            return [];
        return [
            FileInfo.joinPaths(
                cpp.rpathOrigin,
                FileInfo.relativePath(
                    FileInfo.joinPaths("/", product.installDir),
                    FileInfo.joinPaths("/", "lib")))
        ];
    }
    // --8<-- [end:snippet0]
}
// --8<-- [end:snippet1]
