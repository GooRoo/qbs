import qbs.FileInfo

// --8<-- [start:snippet0]
CppApplication {
    version: project.version
    consoleApplication: true
    install: true
    installDebugInformation: project.installDebugInformation
    // ...
    // --8<-- [end:snippet0]

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
}
