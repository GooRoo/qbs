// --8<-- [start:snippet0]
Project {
    property string version: "1.0.0"
    property bool installDebugInformation: true
    property bool withTests: false
    property stringList autotestArguments: []
    property stringList autotestWrapper: []

    name: "My Project"
    minimumQbsVersion: "2.0"
    // ...
    // --8<-- [end:snippet0]
    references: [
        "app/app.qbs",
        "lib/lib.qbs",
    ]
    qbsSearchPaths: "qbs"

    // --8<-- [start:snippet1]
    SubProject {
        filePath: "test/test.qbs"
        Properties {
            condition: parent.withTests
        }
    }
    // --8<-- [end:snippet1]

    // --8<-- [start:snippet2]
    AutotestRunner {
        condition: parent.withTests
        arguments: parent.autotestArguments
        wrapper: parent.autotestWrapper
    }
    // --8<-- [end:snippet2]
}
