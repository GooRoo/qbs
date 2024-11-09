import qbs.FileInfo

Project {
    condition: qbs.targetOS.contains("unix")

    // --8<-- [start:snippet0]
    DynamicLibrary {
        Depends { name: "cpp" }
        Depends { name: "bundle" }
        name: "LibraryA"
        bundle.isBundle: false
        cpp.sonamePrefix: qbs.targetOS.contains("macos") ? "@rpath" : undefined
        cpp.rpaths: cpp.rpathOrigin
        cpp.cxxLanguageVersion: "c++11"
        cpp.minimumMacosVersion: "10.8"
        files: [
            "objecta.cpp",
            "objecta.h",
        ]
        install: true
        installDir: "examples/lib"
    }
    // --8<-- [end:snippet0]

    // --8<-- [start:snippet1]
    DynamicLibrary {
        Depends { name: "cpp" }
        Depends { name: "bundle" }
        Depends { name: "LibraryA" }
        name: "LibraryB"
        bundle.isBundle: false
        cpp.cxxLanguageVersion: "c++11"
        cpp.minimumMacosVersion: "10.8"
        cpp.sonamePrefix: qbs.targetOS.contains("macos") ? "@rpath" : undefined
        cpp.rpaths: cpp.rpathOrigin
        files: [
            "objectb.cpp",
            "objectb.h",
        ]
        install: true
        installDir: "examples/lib"
    }
    // --8<-- [end:snippet1]

    // --8<-- [start:snippet2]
    CppApplication {
        Depends { name: "bundle" }
        Depends { name: "LibraryA" }
        Depends { name: "LibraryB" }
        name: "rpaths-app"
        files: "main.cpp"
        consoleApplication: true
        bundle.isBundle: false
        cpp.rpaths: FileInfo.joinPaths(cpp.rpathOrigin, "..", "lib")
        cpp.cxxLanguageVersion: "c++11"
        cpp.minimumMacosVersion: "10.8"
        install: true
        installDir: "examples/bin"
    }
    // --8<-- [end:snippet2]
}
