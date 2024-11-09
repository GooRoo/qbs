import qbs.FileInfo

// --8<-- [start:snippet1]
// --8<-- [start:snippet0]
Module {
    property string appInstallDir: "bin"
    property string libDirName: "lib"
    property string libInstallDir: qbs.targetOS.contains("windows") ? appInstallDir : libDirName
    // --8<-- [end:snippet0]

    Depends { name: "cpp" }

    property bool enableRPath: true
    property stringList libRPaths: {
        if (enableRPath && cpp.rpathOrigin && product.installDir) {
            return [FileInfo.joinPaths(cpp.rpathOrigin, FileInfo.relativePath(
                                           FileInfo.joinPaths('/', product.installDir),
                                           FileInfo.joinPaths('/', libInstallDir)))];
        }
        return [];
    }

    cpp.rpaths: libRPaths
}
// --8<-- [end:snippet1]
