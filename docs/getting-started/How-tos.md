---
title: 'How-tos'
---

# How-tos

This page provides concrete instructions for common scenarios.

- [[#How do I build a Qt-based project?]]
- [[#How do I make my app build against my library?]]
- [[#How do I build release with debug information?]]
- [[#How do I separate and install debugging symbols?]]
- [[#How do I use precompiled headers?]]
- [[#How do I make use of rpaths?]]
- [[#How do I make sure my generated sources are getting compiled?]]
- [[#How do I run my autotests?]]
- [[#How do I use ccache?]]
- [[#How do I create a module for a third-party library?]]
- [[#How do I build against libraries that provide pkg-config files?]]
- [[#How do I create application bundles and frameworks on iOS, macOS, tvOS, and watchOS?]]
- [[#How do I apply C/C++ preprocessor macros to only a subset of the files in my product?]]
- [[#How do I disable a compiler warning?]]
- [[#How do I make the state of my Git repository available to my source files?]]
- [[#How do I limit the number of concurrent jobs for the linker only?]]
- [[#How do I add QML files to a project?]]
- [[#How do I define a reusable Group of files that can be included in other Qbs files?]]
- [[#How do I access properties of a base type?]]
- [[#How do I print the value of a property?]]
- [[#How do I debug Qbs scripts?]]
- [[#How do I sign an application for an Apple platform?]]


## How do I build a Qt-based project?

First of all, your project files need to declare [[Depends|dependencies]]
on [[Qt]] modules.

To build the project, you need a matching _profile._ The following commands
set up and use a Qt-specific profile:

```sh
$ qbs setup-qt /usr/bin/qmake qt
$ cd my_project
$ qbs profile:qt
```

If you plan to use this profile a lot, consider making it the default one:

```sh
$ qbs config defaultProfile qt
$ cd my_project
$ qbs
```

See [[Managing Qt Versions]] for more details.

!!! note
    These instructions are only relevant for building from the command line.
    If you use Qt Creator, profiles are set up automatically from the information in the Kit.

## How do I make my app build against my library?

This is achieved by introducing a _dependency_ between the two products using the
[[Depends]] item. Here is a simple, but complete example:

```qml
Project {
    CppApplication {
        name : "the-app"
        files : [ "main.cpp" ]
        Depends { name: "the-lib" }
    }
    DynamicLibrary {
        name: "the-lib"
        Depends { name: "cpp" }
        files: [
            "lib.cpp",
            "lib.h",
        ]
        Export {
            Depends { name: "cpp" }
            cpp.includePaths: [exportingProduct.sourceDirectory]
       }
    }
}
```

The product `the-lib` is a dynamic library. It expects other products to build against it, and
for that purpose, it exports an include path (via an [[Export]] item), so that the
source files in these products can include the library's header file.

The product `the-app` is an application that expresses its intent to link against `the-lib`
by declaring a dependency on it. Now `main.cpp` can include `lib.h` (because of the exported
include path) and the application binary will link against the library (because the linker
[[Rule|rule]] in the [[cpp]] module considers library dependencies as inputs).

!!! note
    In a non-trivial project, the two products would not be defined in the same file.
      Instead, you would put them into files of their own and use the
      [[Project::references|Project.references]] property to pull them into the project.
      The product definitions would stay exactly the same. In particular, their location
      in the project tree is irrelevant to the relationship between them.

### Choosing Between Dynamic and Statically-built Qt Projects

To build `"the-lib"` as either a dynamic or static library, depending on
how Qt was built, you can use the following code:

```qml
Product {
    name: "the-lib"
    type: Qt.core.staticBuild ? "staticlibrary" : "dynamiclibrary"

    Depends { name: "Qt.core" }
    // ...
}
```

## How do I build release with debug information?

You can simply use the `"profiling"` [[qbs::buildVariant|qbs.buildVariant]]:
```qml
qbs build qbs.buildVariant:profiling
```

## How do I separate and install debugging symbols?

First, you need to set the [[cpp::debugInformation|cpp.debugInformation]] and
[[cpp::separateDebugInformation|cpp.separateDebugInformation]]
properties to `true` or use some conditional expression in your product:
```qml
CppApplication {
    // ...
    cpp.debugInformation: qbs.buildVariant !== "release"
    cpp.separateDebugInformation: true
}
```

Now, you can install your [[Application|application]], [[DynamicLibrary|dynamic library]]
or [[LoadableModule|loadable module]] among with its debugging symbols as follows:

```qml
CppApplication {
    // ...
    install: true
    installDir: "bin"
    installDebugInformation: true
    debugInformationInstallDir: "bin"
}
```

If you are not using [[List of Convenience Items|convenience items]],
you can install debug symbols manually using the [[Group]] item. If the
[[cpp::separateDebugInformation|cpp.separateDebugInformation]] property is set to `true`,
Qbs will create debugging symbols with the corresponding file tags
`"debuginfo_app"` (for an application), `"debuginfo_dll"` (for a dynamic library),
or `"debuginfo_loadablemodule"` (for a macOS plugin).

```qml
Product {
    type: "application"
    Depends { name: "cpp" }
    cpp.debugInformation: qbs.buildVariant !== "release"
    cpp.separateDebugInformation: true
    Group {
        fileTagsFilter: cpp.separateDebugInformation ? ["debuginfo_app"] : []
        qbs.install: true
        qbs.installDir: "bin"
        qbs.installSourceBase: buildDirectory
    }
}
```

If you're building a shared library, you need to use the `"debuginfo_dll"` tag instead:

```qml
Product {
    type: "dynamic_library"
    // ...
    Group {
        fileTagsFilter: cpp.separateDebugInformation ? ["debuginfo_dll"] : []
        qbs.install: true
        qbs.installDir: "lib"
        qbs.installSourceBase: buildDirectory
    }
}
```

If you're building a macOS plugin, you need to use the `"debuginfo_loadablemodule"`
tag instead:

```qml
Product {
    type: "loadablemodule"
    // ...
    Group {
        fileTagsFilter: cpp.separateDebugInformation ? ["debuginfo_loadablemodule"] : []
        qbs.install: true
        qbs.installDir: "PlugIns"
        qbs.installSourceBase: buildDirectory
    }
}
```

## How do I use precompiled headers?

If you use a [[Group]] item to add a precompiled header file to a product
and mark it with the [[filetags-cpp|relevant file tag]] (`c_pch_src`,
`cpp_pch_src`, `objc_pch_src`, or `objcpp_pch_src`), it is used
automatically.

Only one precompiled header is allowed per product and language.

For example:

```qml
CppApplication {
    name: "the-app"
    files: ["main.cpp"]

    Group {
        files: ["precompiled-header.pch"]
        fileTags: ["cpp_pch_src"]
    }
}
```

## How do I make use of rpaths?

rpath designates the run-time search path used by the dynamic linker when loading
libraries on UNIX platforms. This concept does not apply to Windows.

Suppose you have a project with two dynamic library products `LibraryA` and `LibraryB`
and one dependent application product. Also, `LibraryB` depends on `LibraryA`. The
application is installed to the `bin` folder and the libraries are installed to the
`lib` folder next to the `bin` folder. You want the application to be able to find the
dependent libraries relative to its own location. This can be achieved by usage of the
[[cpp::rpaths|cpp.rpaths]] property.

First, you need to set [[cpp::rpaths|cpp.rpaths]] in your libraries so they can
find dependent libraries in the same folder where they are located. This can be
done as follows:

```qml hl_lines="6-7"
--8<-- "examples/rpaths/rpaths.qbs:snippet0"
```

We are setting [[cpp::rpaths|cpp.rpaths]] to [[cpp::rpathOrigin|cpp.rpathOrigin]] which
expands to `"$ORIGIN"` on Linux and to `"@loader_path"` on macOS.

On macOS you also need to set [[cpp::sonamePrefix|cpp.sonamePrefix]] to `"@rpath"` to
tell the dynamic linker to use RPATHs when loading this library.

`LibraryB` looks exactly the same:

```qml
--8<-- "examples/rpaths/rpaths.qbs:snippet1"
```

In a real project, it might be a good idea to move common properties to some base item
and inherit it in library items.

The application item is a bit different. It sets [[cpp::rpaths|cpp.rpaths]] to the
`"lib"` folder which is located one level up from the `bin` folder:

```qml hl_lines="9"
--8<-- "examples/rpaths/rpaths.qbs:snippet2"
```

## How do I make sure my generated sources are getting compiled?

The rules in a Qbs project do not care whether its inputs are actual source files
listed on the right-hand side of a [[Product::files|files]] property or artifacts
that were generated by another rule. For instance, the C++ compiler rule considers
all input files of type "cpp", no matter how they got into the product. The following
example project demonstrates this. One of its source files exists in the repository,
the other one is generated at build time. Both are getting compiled the same way.

!!! note
    Do not try to add the generated files to a `files` property. Declaring them
    as rule outputs is all that is needed to make Qbs know about them.

```qml
import qbs.TextFile

CppApplication {
    files: ["impl.cpp", "impl.h"]
    cpp.includePaths: sourceDirectory
    Rule {
        multiplex: true
        Artifact { filePath: "main.cpp"; fileTags: "cpp" }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "generating " + output.fileName;
            cmd.sourceCode = function() {
                var f = new TextFile(output.filePath, TextFile.WriteOnly);
                f.writeLine("#include <impl.h>");
                f.writeLine("int main()");
                f.writeLine("{");
                f.writeLine("    return functionFromImpl();");
                f.writeLine("}");
                f.close();
            };
            return cmd;
        }
    }
}
```

## How do I run my autotests?

There are two simple things you need to do in your project. Firstly, you
mark your test executables as such. This is done by adding the tag `"autotest"`
to the product type:

```qml
CppApplication {
    name: "test1"
    type: base.concat("autotest")
    // ...
}
```

The second step is to instantiate an [[AutotestRunner]] product in your project:

```qml
Project {
    // ...
    AutotestRunner { name: "run_my_tests" }
}
```

Building an AutotestRunner product does not produce artifacts, but triggers execution of all
applications whose products are tagged as autotests:

```sh
$ qbs -p run_my_tests
test1: PASS
test2: PASS
test3: FAIL
...
```

See the [[AutotestRunner|AutotestRunner documentation]] for how to fine-tune the behavior.

## How do I use ccache?

[ccache][ccache] is a popular C/C++ compiler cache on Unix to speed up compiling the
same content multiple times.

Qbs excels at tracking dependencies and avoiding needless recompilations, so
for linear development of one project and configuration using ccache
has little benefit. But if you switch between revisions of a project,
or build the same project with different configurations, a global cache like
ccache can speed up compilations significantly.

ccache can be used by setting up symbolic links to compiler executables
(such as `g++`, `gcc`) in the file system. In this setup, the use of ccache is
transparent to Qbs. If you prefer to call ccache explicitly, you should
set [[cpp::compilerWrapper|cpp.compilerWrapper]] to `ccache`.

!!! note

    Using precompiled headers might prevent ccache from actually
    using cached results. To work around this, you can set
    `sloppiness=pch_defines,time_macros` in your local ccache options.
    See the [ccache documentation about precompiled headers][ccache documentation about precompiled headers] for further details.

## How do I create a module for a third-party library?

If you have pre-built binary files in your source tree, you can create
modules for them and then introduce dependencies between your project and
the modules to pull in the functionality of a third-party library.

Create the following folder structure to store the module files:

```
$projectroot/modules/ThirdParty
```

Then create a file in the directory that specifies the module properties
for each supported toolchain. The filename must have the `.qbs` extension.
The module will be pulled in if a product declares a dependency on it.

In the following example, `lib1.dylib` is a multi-architecture library
containing both 32-bit and 64-bit code.

```qml title="ThirdParty.qbs"
Module {
    Depends { name: "cpp" }
    cpp.includePaths: ["/somewhere/include"]
    Properties {
        condition: qbs.targetOS.includes("android")
        cpp.dynamicLibraries: ["/somewhere/android/" + Android.ndk.abi + "/lib1.so"]
    }
    Properties {
        condition: qbs.targetOS.includes("macos")
        cpp.dynamicLibraries: ["/somewhere/macos/lib1.dylib"]
    }
    Properties {
        condition: qbs.targetOS.includes("windows") && qbs.architecture === "x86"
        cpp.dynamicLibraries: ["/somewhere/windows_x86/lib1.lib"]
    }
    Properties {
        condition: qbs.targetOS.includes("windows") && qbs.architecture === "x86_64"
        cpp.dynamicLibraries: ["/somewhere/windows_x86_64/lib1.lib"]
    }
}
```

Finally, declare dependencies on `ThirdParty` in your project:

```qml hl_lines="4"
CppApplication {
    name: "the-app"
    files: ["main.cpp"]
    Depends { name: "ThirdParty" }
}
```

## How do I create application bundles and frameworks on iOS, macOS, tvOS, and watchOS?

Creating an application bundle or framework is achieved by introducing a
dependency on the [[bundle]] module and setting the [[bundle::isBundle|bundle.isBundle]] property to `true`.

Here is a simple example for an application:

```qml
Application {
    Depends { name: "cpp" }
    Depends { name: "bundle" }
    bundle.isBundle: true
    name: "the-app"
    files: ["main.cpp"]
}
```

and for a framework:

```qml
DynamicLibrary {
    Depends { name: "cpp" }
    Depends { name: "bundle" }
    bundle.isBundle: true
    name: "the-lib"
    files: ["lib.cpp", "lib.h"]
}
```

Qbs also supports building static frameworks. You can create one by
replacing the [[DynamicLibrary]] item with a [[StaticLibrary]] item in the
example above.

!!! note

    When using the [[Application]] item (or convenience items, such as
    [[CppApplication]], [[DynamicLibrary]], and [[StaticLibrary]]), your
    products will be built as bundles on Apple platforms by default (this
    behavior is subject to change in a future release).

To explicitly control whether your product is built as a bundle, set the `bundle.isBundle`
property. Setting the [[Product::|consoleApplication]] property of your
product will also influence whether your product is built as a bundle.

Building your application against your framework is the same as linking a normal dynamic or
static library; see the [[#How do I make my app build against my library?]] section for an
example.

## How do I build against libraries that provide pkg-config files?

Just add a [[Depends]] item that matches the name of the pkg-config module,
set the [[Product::qbsModuleProviders]] property to `"qbspkgconfig"`,
and Qbs will employ
[pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config)
to find the headers and libraries if no matching Qbs module can be found. For instance,
to build against the OpenSSL library, you would write this:

```qml
qbsModuleProviders: "qbspkgconfig"
Depends { name: "openssl" }
```

That's it. The pkg-config behavior can be fine-tuned via the [[qbspkgconfig]] provider.

Internally, this functionality is implemented via [[Module Providers]].

## How do I apply C/C++ preprocessor macros to only a subset of the files in my product?

Use a [[Group]] item to define a subset of project files. To add
macros within the group, you need to use the `outer.concat` property,
because you are adding macros to those specified in the outer scope.

In the following example, `MACRO_EVERYWHERE` is defined for all files in
the [[Product]] unless a Group overrides the macro, whereas
`MACRO_GROUP` is only defined for `groupFile.cpp`.

```qml
Product {
    Depends { name: "cpp" }
    cpp.defines: ["MACRO_EVERYWHERE"]  // (1)
    Group {
        cpp.defines: outer.concat("MACRO_GROUP")  // (2)
        files: "groupFile.cpp"
    }
}
```

1. Applied for every file in the product.
2. Applied only for `groupFile.cpp`.

The `cpp.defines` statements inside a `Group` only apply to the files in
that `Group`, and therefore you cannot use a `Group` to include a bunch of
files and globally visible macros. The macros must be specified in a
[[Properties]] item at the same level as the `Group` if
they need to be visible to files outside the `Group`:

```qml
Product {
    Depends { name: "cpp" }
    Group {
        condition: project.supportMyFeature
        files: "myFile.cpp"
    }

    property stringList commonDefines: ["ONE", "TWO"]

    Properties {
        condition: project.supportMyFeature
        cpp.defines: commonDefines.concat("MYFEATURE_SUPPORTED")
    }
}
```

## How do I disable a compiler warning?

You can use the [[cpp::commonCompilerFlags|cpp.commonCompilerFlags]] property
to pass flags to the compiler. For example, to disable deprecation warnings:

```qml
CppApplication {
    // ...

    readonly property bool isMsvc: qbs.toolchain.includes("msvc")

    cpp.commonCompilerFlags: isMsvc ? "/wd4996" : "-Wno-deprecated-declarations"
}
```

It is also possible to disable all warnings at once by setting the
[[cpp::commonCompilerFlags|cpp.warningLevel]] property to `"none"`.
Usually this approach is discouraged, but it can be useful in some cases,
such as when compiling third party code:

```qml
Group {
    cpp.warningLevel: "none"

    files: [
        "3rdparty.h",
        "3rdparty.cpp"
    ]
}
```

## How do I make the state of my Git repository available to my source files?

Add a dependency to the [[vcs]] module to your product:
```qml
CppApplication {
    // ...
    Depends { name: "vcs" }
    // ...
}
```
Your source files will now have access to a macro whose value is a string representing the
current Git or Subversion HEAD:
```cpp
#include <vcs-repo-state.h>
#include <iostream>

int main()
{
    std::cout << "I was built from " << VCS_REPO_STATE << std::endl;
}
```

This value is also available via the [[vcs::repoState|vcs.repoState]]
property.

## How do I limit the number of concurrent jobs for the linker only?
<!-- [TODO] @GooRoo: \target job-pool-howto -->

While it is usually desirable to run as many compiler jobs as there are CPU cores,
the same is not true for linker jobs. The reason is that linkers are typically
I/O bound rather than CPU bound. When building large libraries, they also tend
to use up enormous amounts of memory. Therefore, we'd like to make sure that
only a few linkers are running at the same time without limiting other types
of jobs. In Qbs, this is achieved via _job pools_. There are several ways
to make use of them.

Firstly, you can provide a limit via the command line:

```sh
$ qbs --job-limits linker:4
```

The above call instructs Qbs to run at most four linker instances at the same
time, while leaving the general number of concurrent jobs at the default
value, which is derived from the number of CPU cores.
The `linker` string on the command line refers to the job pool of the same
name, which the [[cpp-job-pools|cpp module]] assigns to all its commands that
invoke a linker.

Secondly, you can set a limit via the settings, either generally
or for a specific profile:

```sh
$ qbs config preferences.jobLimit.linker 4
$ qbs config profiles.myprofile.preferences.jobLimit.linker 2
```

And finally, you can also set the limit per project or per product, using a
[[JobLimit]] item:

```qml
Product {
    name: "my_huge_library"
    JobLimit {
        jobPool: "linker"
        jobCount: 1
    }
    // ...
}
```

The above construct ensures that this specific library is never linked at
the same time as any other binary in the project.

Job limits set on the command line override those from the settings, which in turn
override the ones defined within a project. Use the `--enforce-project-job-limits`
option to give the job limits defined via `JobLimit` items maximum precedence.

## How do I add QML files to a project?

The simplest way to add QML files to a project is to add them to a
[Qt resource file](https://doc.qt.io/qt/resources.html):

```qml
QtGuiApplication {
    // ...

    files: "main.cpp"

    Group {
        prefix: "qml/"
        files: ["main.qml", "HomePage.qml"]
        fileTags: ["qt.qml.qml", "qt.core.resource_data"]
    }
}
```

In the example above, we declare each QML file as having the
[[filetags-qtcore|"qt.core.resource_data"]] file tag. This ensures
that it is added to a generated resource file.

## How do I define a reusable Group of files that can be included in other Qbs files?

Suppose you have an application and tests for that application, and that
the project is structured in the following way:

```
├── app
│   ├── app.qbs
│   ├── ...
│   └── qml
│       └── ui
│           ├── AboutPopup.qml
│           └── ...
├── my-project.qbs
└── tests
    ├── tst_app.cpp
    ├── ...
    └── tests.qbs
```

Both projects need access to the QML files used by the
application. To demonstrate how this can be done, we'll create a file
named `qml-ui.qbs` and put it in the `app/qml/ui` directory:

```qml
Group {
    prefix: path + "/"
    fileTags: ["qt.qml.qml", "qt.core.resource_data"]
    files: [
        "AboutPopup.qml",
        // ...
    ]
}
```

This Group is a variation of the one in the
[[#How do I add QML files to a project?|section above]].

If no prefix is specified, the file names listed in the `files` property
are resolved relative to the _importing_ product's (e.g. `app.qbs`)
directory. For that reason, we set the prefix to inform Qbs that the file
names should be resolved relative to the _imported_ item instead:
`qml-ui.qbs`. Conveniently, this also means that we don't need to specify
the path prefix for each file.

The application can then import the file like so:

```qml
import "qml/ui/qml-ui.qbs" as QmlUiFiles

QtGuiApplication {
    // ...

    files: "main.cpp"

    QmlUiFiles {}
}
```

The tests can use a relative path to import the file:

```qml
import "../app/qml/ui/qml-ui.qbs" as QmlUiFiles

QtGuiApplication {
    // ...

    files: "tst_app.cpp"

    QmlUiFiles {}
}
```

## How do I access properties of a base type?

You can use the [[Special Property Values#base|base]] property. For example, to append to a list of files
that come from the base type, you can use `base.concat()`:

```qml
// TestBase.qbs

QtGuiApplication {
    files: [
        "TestCaseBase.h",
        "TestCaseBase.cpp"
    ]
}
```

```qml
// tst_stuff.qbs
TestBase {
    files: base.concat(["tst_stuff.cpp"])
}
```

See [[Special Property Values]] for more details.

## How do I print the value of a property?

Use the [[Console API|console API]]. For example, suppose your project
is not built the way you expect it to be, and you suspect that
`qbs.targetOS` has the wrong value:

```qml
readonly property bool unix: qbs.targetOS.includes("unix")
```

To find out the value of `qbs.targetOS`, use `console.info()`:

```qml
readonly property bool unix: {
    console.info("qbs.targetOS: " + qbs.targetOS)
    return qbs.targetOS.includes("unix")
}
```

It is also possible to throw an exception with the text saying what is wrong - this might
be useful if the property contains invalid or unsupported value:

```qml
readonly property bool unix: {
    if (qbs.targetOS.includes("darwin"))
        throw "Apple platforms are not supported";
    return qbs.targetOS.includes("unix")
}
```

## How do I debug Qbs scripts?

To debug the value of a specific property, see the [[#How do I print the value of a property]]
section.

Similar debugging techniques could be used within [[Rule|Rules]] or `.js` files.

It is also possible to increase Qbs' logging level using the `--more-verbose` (`-v`) option
of the `qbs build` command:

```sh
qbs build -v config:release
```

Qbs uses the Qt Categorized Logging system which allows to configure logging categories
in [multiple ways](https://doc.qt.io/qt-6/qloggingcategory.html#configuring-categories). For
example, to enable debug logging for the `moduleloader` category, use the following command:
```sh
QT_LOGGING_RULES="qbs.moduleloader.debug=true" qbs resolve
```

To list all the files in the project directory and show whether they are known to qbs in the
respective configuration, use the `qbs status` command:
```sh
qbs status config:release
```

## How do I sign an application for an Apple platform?

To sign an application for an Apple platform, you need to use the [[codesign]] module.

```qml
Depends { name: "codesign" }
```

Several properties should be set to do signing as shown below.

Make sure that bundle and team indentifiers match the one used for signing:

```qml
bundle.identifierPrefix: "com.johndoe"
codesign.teamIdentifier: "John Doe"
```

It is also possible to use an ID of the team identifier instead of a name:

```qml
codesign.teamIdentifier: "1234ABCDEF"
```

Qbs will then try to find the matching signing identity and provisioning profile based on
[[codesign::signingType|codesign.signingType]].

It is also possible to specify [[codesign::signingIdentity|codesign.signingIdentity]]
manually:

```qml
codesign.signingIdentity: "Apple Development: johndoe@apple.com (ABCDEF1234)"
```

It is also possible to use an ID of the signing identity instead of a name:
```qml
codesign.signingIdentity: "ABCDEF1234567890ABCDEF1234567890ABCDEF12"
```

If Qbs cannot find the suitable provisioning profile, you can specify it manually as well:
```qml
codesign.provisioningProfile: "abcdef12-1234-5678-1111-abcdef123456"
```
