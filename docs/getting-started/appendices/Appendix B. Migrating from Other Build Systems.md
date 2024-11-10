---
title: 'Appendix B: Migrating from Other Build Systems'
---

# Appendix B: Migrating from Other Build Systems

You can use the [[create-project|qbs create-project]] command to
automatically generate Qbs project files from an arbitrary directory
structure. This is a useful starting point when migrating from other
build tools, such as qmake or CMake.

To use the tool, switch to the project directory and run the
`qbs create-project` command, which is located in the `bin` directory of
the Qbs installation directory (or the Qt Creator installation directory).

After generating the initial `.qbs` file, add the missing configuration
variables and functions to it, as described in the following sections.

## Migrating from qmake

The following sections describe the Qbs equivalents of qmake variable
values.

### `CONFIG`

Specify project configuration and compiler options.

#### `console`

Set the [[Product::consoleApplication|Product.consoleApplication]] property
to `true` for the [[Application]], [[CppApplication]],or [[QtApplication]]
item. For example:

```qml hl_lines="5"
Application {
    name: "helloworld"
    files: "main.cpp"
    Depends { name: "cpp" }
    consoleApplication: true
}
```

#### `ordered`

This qmake variable has no direct equivalent in Qbs. Instead, the build
order is determined by implicit and explicit dependencies between products.
To add an explicit dependency, add a [[Depends]] item to a
[[Product|product]]:

```qml
CppApplication {
    name: "myapp"
    Depends { name: "mylib" }
}
```

The `myapp` product depends on and links to the `mylib` product, and is
therefore built after it.

#### `qt`

In qmake, the Qt dependency is implicit, whereas in Qbs it is not.
If `CONFIG -= qt`, add a [[Depends]] item to specify that
the [[Product|product]] depends on the [[cpp]] module:

```qml
Product {
    Depends { name: "cpp" }
}
```

### `DEFINES`

Set the [[cpp::defines|cpp.defines]] property for the [[Product|product]].

!!! note

    To reference `cpp.defines`, you must specify a dependency on the
    [[cpp]] module.

```qml
Product {
    Depends { name: "cpp" }
    cpp.defines: ["SUPPORT_MY_FEATURES"]
}
```

### `DESTDIR`

We recommend that you use the [[Installing Files|installation mechanism]]
to specify the location of the target file:

```qml
Application {
    Group {
        name: "Runtime resources"
        files: "*.qml"
        qbs.install: true
        qbs.installDir: "share/myproject"
    }
    Group {
        name: "The App itself"
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
```

If that is not possible, you can use the [[Product::|destinationDirectory]]
property:

```qml
DynamicLibrary {
    name: "mydll"
    destinationDirectory: "libDir"
}
```

### `HEADERS`, `SOURCES`, `FORMS`, `RESOURCES`, `OTHER_FILES`

Include header, source, form, and resource files as well as any
other files as values of a [[Product::files|Product.files]]
or [[Group::files|Group.files]]  property:

```qml
QtApplication {
    name: "myapp"
    files: ["myapp.h", "myapp.cpp", "myapp.ui", "myapp.qrc", "readme.txt"]
}
```

Qbs uses [[FileTagger|file taggers]] to figure out what kind of file
it is dealing with.

### `ICON`

There is no direct equivalent in Qbs. If you add a [[Depends|dependency]]
to the [[ib]] module and add the `.xcassets` directory as a value of the
[[Product::files|Product.files]] property, Qbs takes care of setting the
application icon automatically when building for Apple platforms:

```qml
Application {
    name: "myapp"
    files [".xcassets"]
    Depends { name: "ib" }
}
```

Alternatively, you can set the icon name as the value of the
[[bundle::infoPlist|bundle.infoPlist]] parameter, specify a dependency to
the [[ib]] module, and add the application `.icns` file as a value of the
[[Product::|files]] property:

```qml
Application {
    name: "myapp"
    files ["myapp.icns"]
    Depends { name: "ib" }
    bundle.infoPlist: ({"CFBundleIconFile": "myapp"})
```

### `INCLUDEPATH`

Add the paths to the include files as values of the [[cpp::includePaths|cpp.includePaths]] property:

```qml
CppApplication {
    cpp.includePaths: ["..", "some/other/dir"]
}
```

### `LIBS`

For libraries that are part of the project, use [[Depends]] items.

To pull in external libraries, use the [[cpp::libraryPaths|cpp.libraryPaths]] property for the Unix `-L` (library path) flags and the
[[cpp::dynamicLibraries|cpp.dynamicLibraries]] and [[cpp::staticLibraries|cpp.staticLibraries]] properties for the
`-l` (library) flags.

For example, `LIBS += -L/usr/local/lib -lm` would become:

```qml
CppApplication {
    cpp.libraryPaths: ["/usr/local/lib"]
    cpp.dynamicLibraries: ["m"]
}
```

### `OUT_PWD`

Use the [[Product::buildDirectory|Product.buildDirectory]] property
to refer to the base output directory of the generated artifacts.

### `PWD`

Corresponds to the the file-scope variable `path`.

### `_PRO_FILE_`

Corresponds to the file-scope variable `filePath` when used in a
[[Project|project]] or [[Product|product]].

### `_PRO_FILE_PWD_`

Corresponds to the [[Project::sourceDirectory|Project.sourceDirectory]] or
[[Product::sourceDirectory|Product.sourceDirectory]] property.

### `QMAKE_ASSET_CATALOGS`

Add a [[Depends|dependency]] to the [[ib]] module and add the `.xcassets`
directory as a value of the [[Product::|files]] property:

```qml
Application {
    name: "myapp"
    files [".xcassets"]
    Depends { name: "ib" }
}
```

### `QMAKE_BUNDLE_DATA`

For the time being, you can manually place files in the appropriate location
using the [[Installing Files|installation mechanism]]. Better solutions are
under development.

### `QMAKE_BUNDLE_EXTENSION`

Set the [[bundle::extension|bundle.extension]] property.

!!! note

    Unlike qmake, Qbs automatically prepends a period (.) to the property
    value.

### `QMAKE_{C,CXX,OBJECTIVE}_CFLAGS{_DEBUG,_RELEASE}`

Use the [[cpp::commonCompilerFlags|cpp.commonCompilerFlags]] property or
the properties corresponding to each compiler flags variable:

| qmake Variable                                      | cpp Module Property                                                         |
| --------------------------------------------------- | --------------------------------------------------------------------------- |
| `QMAKE_CFLAGS_DEBUG`<br/>`QMAKE_CFLAGS_RELEASE`     | [[cpp::cFlags\|cpp.cFlags]]                                                 |
| `QMAKE_CXXFLAGS_DEBUG`<br/>`QMAKE_CXXFLAGS_RELEASE` | [[cpp::cxxFlags\|cpp.cxxFlags]]                                             |
| `QMAKE_OBJECTIVE_CFLAGS`                            | [[cpp::objcFlags\|cpp.objcFlags]]<br/>[[cpp::objcxxFlags\|cpp.objcxxFlags]] |


Use [[Properties]] items or simple conditionals as values of the
[[qbs::buildVariant|qbs.buildVariant]] property to simulate the `_DEBUG`
and `_RELEASE` variants of the qmake variables.

### `QMAKE_FRAMEWORK_BUNDLE_NAME`

Set the [[bundle::bundleName|bundle.bundleName]] property (which is derived
from [[Product::targetName|Product.targetName]]) combined with
[[bundle::extension|bundle.extension]].

### `QMAKE_FRAMEWORK_VERSION`

Set the [[bundle::frameworkVersion|bundle.frameworkVersion]] property.

### `QMAKE_INFO_PLIST`

Include the `info.plist` file as a value of [[Product::|files]] property
and specify a dependency to the [[bundle]] module:

```qml
Application {
    name: "myapp"
    files ["info.plist"]
    Depends { name: "bundle" }
}
```

Qbs will automatically add any necessary properties to your `Info.plist`
file. Typically, it determines the appropriate values from the other
properties in the project, and therefore you do not need to use the
`Info.plist.in` → `Info.plist` configuration mechanism. Further, you almost
never need to embed placeholders into the source `Info.plist` file. Set the
[[bundle::processInfoPlist|bundle.processInfoPlist]] property to `false`
to disable this behavior:

```qml
\\ ...
bundle.processInfoPlist: false
```

In addition to, or instead of, using an actual `Info.plist` file, you can
add `Info.plist` properties using the [[bundle::infoPlist|bundle.infoPlist]] property. For example:

```qml
\\ ...
bundle.infoPlist: ({
    "NSHumanReadableCopyright": "Copyright (c) 2017 Bob Inc",
    "Some other key", "Some other value, & XML special characters are no problem! >;) 非凡!"
})
```

### `QMAKE_LFLAGS`

Set the [[cpp::linkerFlags|cpp.linkerFlags]] property for the [[Product|product]].

### `QMAKE_{MACOSX,IOS,TVOS,WATCHOS}_DEPLOYMENT_TARGET`

For each qmake deployment target variable, use the corresponding property of
the [[cpp]] module:

| qmake Variable                    | cpp Module Property                                       |
| --------------------------------- | --------------------------------------------------------- |
| `QMAKE_MACOSX_DEPLOYMENT_TARGET`  | [[cpp::minimumMacosVersion\|cpp.minimumMacosVersion]]     |
| `QMAKE_IOS_DEPLOYMENT_TARGET`     | [[cpp::minimumIosVersion\|cpp.minimumIosVersion]]         |
| `QMAKE_TVOS_DEPLOYMENT_TARGET`    | [[cpp::minimumTvosVersion\|cpp.minimumTvosVersion]]       |
| `QMAKE_WATCHOS_DEPLOYMENT_TARGET` | [[cpp::minimumWatchosVersion\|cpp.minimumWatchosVersion]] |


### `QMAKE_RPATHDIR`

Set the [[cpp::rpaths|cpp.rpaths]] property for the [[Product|product]].

### `QMAKE_SONAME_PREFIX`

Use the [[cpp::sonamePrefix|cpp.sonamePrefix]] property for the [[Product|product]].

### `QML_IMPORT_PATH`

Used only for Qt Creator QML syntax highlighting. Inside a [[Product]],
[[Application]], [[CppApplication]], or [[QtApplication]], create a
`qmlImportPaths` property:

```qml
Product {
    name: "myProduct"
    property stringList qmlImportPaths: [sourceDirectory + "/path/to/qml/"]
}
```

### `QT`

Add a [[Depends]] item to the [[Product|product]] that specifies the
dependencies to [[Qt]] modules. For example:

```qml
QtApplication {
    Depends { name: "Qt.widgets" }
}
```

You could also use the following form that is equivalent to the previous
one:

```qml
QtApplication {
    Depends { name: "Qt"; submodules: "widgets" }
}
```

### `QTPLUGIN`

Building static applications often requires linking to static QPA plugins,
such as `qminimal`. You can use the following syntax to enable Qbs to
link to the required plugins:

```qml
QtApplication {
    name: "myapp"
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"] }
    Depends { name: "Qt.qminimal"; condition: Qt.core.staticBuild }
}
```

### `RC_FILE`

Add Windows resource files to the value of the [[Product::files|Product.files]] property.

### `TARGET`

Use the [[Product::targetName|Product.targetName]] property to specify the
base file name of target artifacts.

### `TEMPLATE`

#### `app`

Use [[Application]] or [[CppApplication]] as the [[Product|product]]:

```qml
CppApplication {
    name: "helloworld"
    files: "main.cpp"
}
```

This is roughly equivalent to:

```qml
Product {
    name: "helloworld"
    type: "application"
    files: "main.cpp"
    Depends { name: "cpp" }
}
```

#### `lib`

Use either [[DynamicLibrary]] or [[StaticLibrary]] as the [[Product|product]], depending on whether the value of `CONFIG` in the `.pro` file is
`shared` or `static`. For example, if the value is `shared`:

```qml
DynamicLibrary {
    name: "mydll"
    files: ["mySourceFile.cpp"]
    Depends { name: "cpp" }
}
```

#### `subdirs`

In a [[Project]] item, specify subdirectories as values of the
[[Project::|references]] property:

```qml
Project {
    references: [
        "app/app.qbs",
        "lib/lib.qbs"
    ]
}
```

### `message()`, `warning()`, `error()`, `log()`

You can use the [[Console API]] to print info, warning, error, and log messages to the console.

```qml
Product {
    name: {
        console.info("--> now evaluating the product name");
        return "theName";
    }
    Depends { name: "cpp" }
    cpp.includePath: { throw "An error occurred." }
}
```
