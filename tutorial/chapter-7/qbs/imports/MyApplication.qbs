// --8<-- [start:snippet0]
CppApplication {
    Depends { name: "mybuildconfig" }
    installDir: mybuildconfig.appInstallDir

    version: "1.0.0"
    // ...
// --8<-- [end:snippet0]

    consoleApplication: true
    install: true
    installDebugInformation: project.installDebugInformation
}
