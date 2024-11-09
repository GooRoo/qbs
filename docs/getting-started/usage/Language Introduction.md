---
title: Language Introduction
---

# Language Introduction

Qbs uses project files (`*.qbs`) to describe the contents of a project.
A project contains one or more [[Product|products]]. A product is the target of a build
process, typically an application, library or maybe a tar ball.

!!! note
    Qbs source files are assumed to be UTF-8 encoded.

## The Obligatory Hello World Example

Qbs project files are written using a QML dialect.
A very simple C++ hello world project looks like this:
```qml title="helloworld.qbs"
Application {
    name: "helloworld"
    files: "main.cpp"
    Depends { name: "cpp" }
}
```

`Application` describes the product we want to build. In this case, an
application. This is just a shortcut for writing
```qml
Product {
    type: "application"
    // ...
}
```

The `name` is the name of the product. In this case it is also the
name of the produced executable (on Windows, the ".exe" extension is added by default).

In the property `files`, we specify the source files for our product.
Unlike QML, the right-hand side can be either a string or a string list.
A single string is converted to a stringlist containing just one element.
So we could have also written

```qml
files: [ "main.cpp" ]
```

`Depends` adds the dependency to the [[cpp]] module. This is necessary to
let Qbs know that we have a C++ project and want to compile main.cpp with a
C++ compiler. For more information about Qbs modules, see [[Modules]].


## Reusing Project File Code
QML-like inheritance works also in Qbs.

```qml title="CrazyProduct.qbs"
Product {
    property string craziness: "low"

    files: [
        "Crazy.h",
        "Crazy.cpp"
    ]
}
```
```qml title="hellocrazyworld.qbs"
import "CrazyProduct.qbs" as CrazyProduct

CrazyProduct {
    craziness: "enormous"
    name: "hellocrazyworld"
    files: base.concat(["main.cpp"])
    // ...
}
```

You can put JS code into separate `.js` files and then import them.

```qml title="helpers.js"
function planetsCorrectlyAligned()
{
    // implementation
}
```
```qml title="myproject.qbs"
import "helpers.js" as Helpers

Product {
    name: "myproject"
    Group {
        condition: Helpers.planetsCorrectlyAligned()
        file: "magic_hack.cpp"
    }
    // ...
}
```

See [[Special Property Values]] for more details about the [[base]] property.

## Modules

A _module_ is a collection of properties and language items that are used for
building a product if the product depends on (or loads) the module.

For example, the `cpp` module looks like this (simplified):
```qml
Module {
    name: "cpp"
    property string warningLevel
    property string optimization
    property bool debugInformation
    property pathList includePaths
    // ...
    FileTagger {
        patterns: "*.cpp"
        fileTags: ["cpp"]
    }
    Rule {...}  // compiler
    Rule {...}  // application linker
    Rule {...}  // static lib linker
    Rule {...}  // dynamic lib linker
}
```

The properties that can be set for the `cpp` module are used to control the behavior of
your C++ toolchain.
In addition, you can use FileTaggers and Rules that are explained later.

As soon as your product depends on a module, it can set the properties of the
module. You specify the optimization level for your product (and all build variants) like this:

```qml title="helloworld.qbs"
Application {
    name: "helloworld"
    files: ["main.cpp"]
    cpp.optimization: "ludicrousSpeed"
    Depends { name: "cpp" }
}
```

A module can depend on other modules. For example, the
[[Qt.core]] module depends on the [[cpp]] module. The module dependencies are transitive,
i.e. in a Product, all dependent modules are accessible:

```qml
Application {
    name: "helloworld"
    files: ["main.cpp"]
    Depends { name: "Qt.core" }
    // the "cpp" module is available since
    // "Qt.core" depends on "cpp".
    cpp.optimization: "ludicrousSpeed"
}
```

### Different Properties for a Single File

Not only the product, but all the source files of the product can have their own
set of module properties. For example, assume you have some files that are known to crash
your compiler if you turn on optimizations. You want to turn off
optimizations for just these files and this is how you do it:

```qml
Application {
    name: "helloworld"
    files: "main.cpp"
    Group {
        files: ["bad_file.cpp", "other_bad_file.cpp"]
        cpp.optimization: "none"
    }
    Depends { name: "cpp" }
}
```

### Selecting Files by Properties

Sometimes you have a file that is only going to be compiled on a certain platform.
This is how you do it:
```qml
Group {
    condition: qbs.targetOS.includes("windows")
    files: [
        "harddiskdeleter_win.cpp",
        "blowupmonitor_win.cpp",
        "setkeyboardonfire_win.cpp"
    ]
}
Group {
    condition: qbs.targetOS.includes("linux")
    files: [
        "harddiskdeleter_linux.cpp",
        "blowupmonitor_linux.cpp",
        "setkeyboardonfire_linux.cpp"
    ]
}
```

In the above example, [[qbs::targetOS|qbs.targetOS]] is a property of the
target of the the [[qbs]] module. The `qbs` module is always implicitly
loaded. Its main properties are:


- [[qbs::|buildVariant]] that specifies the name of the build variant
        for the current build.
- [[qbs::|targetOS]] that specifies the operating system you want to
        build the project for.


You can set these properties on the command line or by using a profile.

```sh
$ qbs                             # qbs.buildVariant:debug, profile:<default profile> (or profile:none, if no default profile exists)
$ qbs config:release              # qbs.buildVariant:release, profile:<default profile>
$ qbs config:debug config:release # builds two configurations of the project
$ qbs profile:none                # all module properties have their default values
```

To select files by build variant:
```qml
Group {
    condition: qbs.buildVariant == "debug"
    files: "debughelper.cpp"
}
```

To set properties for a build variant:
```qml
Properties {
    condition: qbs.buildVariant == "debug"
    cpp.debugInformation: true
    cpp.optimization: "none"
}
```
Or, to use a more QML-like style:
```qml
cpp.debugInformation: qbs.buildVariant == "debug"
cpp.optimization: qbs.buildVariant == "debug" ? "none" : "fast"
```

## Property Types

While properties in Qbs generally work the same way as in QML, the set of possible property
types has been adapted to reflect the specific needs of a build tool. The supported types
are as follows:

| Property type | Example                                                                                 | Description                                                                                       |
| ------------- | --------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- |
| `bool`        | `#!qml property bool someBoolean: false`                                                | The usual boolean values.                                                                         |
| `int`         | `#!qml property int theAnswer: 42`                                                      | Integral numbers.                                                                                 |
| `path`        | `#!qml property path aFile: "file.txt"`                                                 | File paths resolved relative to the directory the product they are associated with is located in. |
| `pathList`    | `#!qml property pathList twoFiles: ["file1.txt", "./file2.txt"]`                        | A list of `path` values.                                                                          |
| `string`      | `#!qml property string parentalAdvisory: "explicit lyrics"`                             | JavaScript strings.                                                                               |
| `stringList`  | `#!qml property stringList realWorldExample: ["no", "not really"]`                      | A list of JavaScript strings.                                                                     |
| `var`         | `#!qml property var aMap: ({ key1: "value1", key2: "value2" )}`                         | Generic data, as in QML.                                                                          |
| `varList`     | `#!qml property var aMapList: [{ key1: "value1", key2: "value2" , { key1: "value3" }]}` | A list of generic data, typically JavaScript objects.                                             |



## Overriding Property Values from the Command Line

Property values set in project files or profiles can be overridden on the command line.
The syntax is `<prefix>.<prop-name>:<prop-value>`. The following command lines
demonstrate how to set different kinds of properties:
```sh
$ qbs projects.someProject.projectProperty:false          # set a property of a project
$ qbs products.someProduct.productProperty:false          # set a property of a product
$ qbs modules.cpp.treatWarningsAsErrors:true              # set a module property for all products
$ qbs products.someProduct.cpp.treatWarningsAsErrors:true # set a module property for one product
```

Property values on the command line can also be expressed in JavaScript form, the same way
as you would write them in a project file. Make sure to take care of proper
quoting, so that the shell does not interpret any of the values itself. Properties of type
`stringList` can also be provided as comma-separated values, if none of the strings contain
special characters:
```sh
$ qbs projects.someProject.listProp:'["a", "b", "c"]'
$ qbs projects.someProject.listProp:a,b,c               # same as above
$ qbs projects.someProject.listProp:'["a b", "c"]'      # no CSV equivalent
```

## File Tags and Taggers

Qbs itself knows nothing about C++ files or file extensions. All source files
in a product are handled equally. However, you can assign `file tags` to an artifact
to act as a marker or to specify a file type.

An artifact can have multiple file tags.
For example, you can use the `Group` item to group files with the same file tags (or a set of
properties).

```qml
Product {
    Group {
        files: ["file1.cpp", "file2.cpp"]
        fileTags: ["cpp"]
    }
    Group {
        files: "mydsl_scanner.l"
        fileTags: ["flex", "foobar"]
    }
    // ...
}
```

When you load the `cpp` module, you also load the following item:
```qml
FileTagger {
    patterns: "*.cpp"
    fileTags: ["cpp"]
}
```
This construct means that each source file that matches the pattern `*.cpp` (and
has not explicitly set a file tag) gets the file tag `cpp`.

The above example can be simplified to
```qml
Product {
    Depends: "cpp"
    files: ["file1.cpp", "file2.cpp"]
    Group {
        files: "mydsl_scanner.l"
        fileTags: ["flex", "foobar"]
    }
    // ...
}
```

The `FileTagger` from the `cpp` module automatically assigns the `cpp`
file tag to the source files. Groups that just contain the `files`
property can be more simply expressed by using the `files` property of the product.

File tags are used by `rules` to transform one type of artifact into
another. For instance, the C++ compiler rule transforms artifacts with the file tag
`cpp` to artifacts with the file tag `obj`.

In addition, it is possible to use file taggers to tag files and specify custom file tags:
```qml
Product {
    Depends: "cpp"
    Group {
        overrideTags: false     // The overrideTags property defaults to true.
        fileTags: ["foobar"]
        files: ["main.cpp"]     // Gets the file tag "cpp" through a FileTagger item and
                                // "foobar" from this group's fileTags property.
    }
    // ...
}
```

## Rules

Qbs applies a _rule_ to a pool of artifacts (in the beginning it is just the set of
source files of the project) and chooses the ones that match the input file
tags specified by the rule. Then it creates output artifacts in the build graph that have other
filenames and file tags. It also creates a script that transforms the input artifacts into the
output artifacts. Artifacts created by one rule can (and typically do) serve as inputs to
another rule. In this way, rules are connected to one another via their input and output
file tags.

For examples of rules, see the `share/qbs/modules` directory in the Qbs
repository.

You can define rules in your own module to be provided along with
your project. Or you can put a rule directly into your project file.

For more information, see [[Rule]].
