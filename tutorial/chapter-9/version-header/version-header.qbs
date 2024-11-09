// --8<-- [start:snippet5]

// --8<-- [start:snippet0]
import qbs.TextFile

Product {
    name: "version_header"
    type: "hpp"

    Depends { name: "mybuildconfig" }
// --8<-- [end:snippet0]

// --8<-- [start:snippet1]
    Group {
        files: ["version.h.in"]
        fileTags: ["version_h_in"]
    }
// --8<-- [end:snippet1]

// --8<-- [start:snippet2]
    Rule {
        inputs: ["version_h_in"]
        Artifact {
            filePath: "version.h"
            fileTags: "hpp"
        }
// --8<-- [end:snippet2]
// --8<-- [start:snippet3]
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "generating " + output.fileName;
            cmd.highlight = "codegen";
            cmd.sourceCode = function() {
                var file = new TextFile(input.filePath, TextFile.ReadOnly);
                var content = file.readAll();

                content = content.replace(
                    "${PRODUCT_VERSION}",
                    product.mybuildconfig.productVersion);

                file = new TextFile(output.filePath, TextFile.WriteOnly);
                file.write(content);
                file.close();
            }
            return cmd;
        }
// --8<-- [end:snippet3]
    }

// --8<-- [start:snippet4]
    Export {
        Depends { name: "cpp" }
        cpp.includePaths: exportingProduct.buildDirectory
    }
// --8<-- [end:snippet4]
}
// --8<-- [end:snippet5]
