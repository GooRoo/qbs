---
title: Special Property Values
---

# Special Property Values

Depending on the context, Qbs provides the following special values for use in property
bindings and JavaScript code:

- [`base`](#base)
- [`exportingProduct`](#exportingproduct)
- [`filePath`](#filepath)
- [`importingProduct`](#importingproduct)
- [`module`](#module)
- [`original`](#original)
- [`outer`](#outer)
- [`path`](#path)
- [`product`](#product)
- [`project`](#project)

#### `base`
This value is useful when making use of inheritance. It stands for the value of the respective
property in the item one level up in the inheritance chain. For instance:
```qml title="MyProduct.qbs"
Product {
    Depends { name: "mymodule" }
    mymodule.someProperty: ["value1"]
}
```
and then in some other file:
```qml hl_lines="2"
MyProduct {
    mymodule.someProperty: base.concat(["value2"]) // (1)
}
```

1. `["value1", "value2"]`

#### `exportingProduct`
Within an [[Export]] item, you can use the `exportingProduct` variable to refer to
the product which defines the Export item:

```qml hl_lines="4"
Product {
    Export {
        Depends { name: "cpp" }
        cpp.includePaths: exportingProduct.sourceDirectory
    }
}
```

#### `filePath`

This value holds the full file path to the `.qbs` file it appears in. This property is
rarely used, but might be useful when debugging:
```qml hl_lines="3"
Product {
    property bool dummy: {
        console.info("I'm located at " + filePath);
    }
}
```

#### `importingProduct`
Within an [[Export]] item, you can use the `importingProduct` variable to refer to
the product that pulls in the resulting module:

```qml hl_lines="4"
Product {
    Export {
        Depends { name: "cpp" }
        cpp.includePaths: importingProduct.buildDirectory
    }
}
```
Usually, you should use the [[product]] variable instead for consistency with [[Module]] items.

#### `module`

This value holds the properties of the module that contains the current item:
```qml hl_lines="7"
Module {
    property bool enableGroup
    property bool enableRule
    Group {
        condition: enableGroup // (1)
        Rule {
            condition: module.enableRule // (2)
            // ...
        }
    }
}
```

1. Available without qualification because Module is a direct parent item.
2. Shortcut for `product.<module name>.enableRule`

#### `original`
On the right-hand side of a module property binding, this refers to the value of the property
in the module itself (possibly overridden from a profile). Use it to set a module property
conditionally:
```qml title="mymodule/mymodule.qbs"
Module {
    property string aProperty: "z"
}
```
```qml hl_lines="5"
Product {
    Depends { name: "mymodule" }
    Depends { name: "myothermodule" }
    // `y` if myothermodule.anotherProperty is `x`, `z` otherwise:
    mymodule.aProperty: myothermodule.anotherProperty === "x" ? "y" : original
}
```

!!! note
    In all but the most exotic cases, a [[Properties]] item is the preferred way to
      achieve the same result:

    ```qml
    Product {
        Depends { name: "mymodule" }
        Depends { name: "myothermodule" }
        Properties {
            condition: myothermodule.anotherProperty === "x"
            mymodule.aProperty: "y"
        }
    }
    ```

#### `outer`
This value is used in nested items, where it refers to the value of the respective property
in the surrounding item. It is only valid in [[Group]] and [[Properties]] items:
```qml hl_lines="7"
Product {
    Depends { name: "mymodule" }
    mymodule.someProperty: ["value1"]
    Group {
        name: "special files"
        files: ["somefile1", "somefile2"]
        mymodule.someProperty: outer.concat(["value"])  // (1)
    }
}
```

1. `["value1", "value2"]`

#### `path`

This value holds the path to the folder where the `.qbs` file is located. Use it to e.g. add
the product's directory to file paths:
```qml hl_lines="3"
Product {
    Depends { name: "cpp" }
    cpp.includePaths: path
}
```

#### `product`

This value holds the properties of the product that contains the current item or pulls in the
current module:
```qml hl_lines="4 8"
Module {
    Rule {
        Artifact {
            fileTags: product.type
            filePath: {
                var result = input.fileName;
                // module properties are available as well
                if (product.qbs.buildVariant === "debug")
                    result = result + "_debug";
                result = result + ".out";
                return result;
            }
        }
    }
}
```
Within the [[Export]] item, same as [[importingProduct.]]

#### `project`
This value holds the properties of the project that references the current item or pulls in the
current module:
```qml hl_lines="5"
Project {
    property bool enableProduct: true
    Product {
        name: "theProduct"
        condition: project.enableProduct
    }
}
```
If the nearest project in the project tree does not have the desired property, Qbs looks it
up in the parent project, potentially all the way up to the top-level project.
