Project {
    name: "My Project"
    minimumQbsVersion: "2.0"
    // --8<-- [start:snippet0]
    references: [
        "app/app.qbs",
        "lib/lib.qbs",
        "test/test.qbs",
    ]
    // --8<-- [end:snippet0]
    qbsSearchPaths: "qbs"
    AutotestRunner {
        timeout: 60
    }
}
