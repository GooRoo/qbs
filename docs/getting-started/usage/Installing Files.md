---
title: Installing Files
---

# Installing Files

To install your project, specify the necessary information in the project file:

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

In this example, we want to install a couple of QML files and an executable.

When building, Qbs installs artifacts into the default root folder, namely
`<build root>/install-root`. The [[qbs::installPrefix|qbs.installPrefix]] and
[[qbs::installDir|qbs.installDir]] properties are appended to the root folder.
```sh
qbs build qbs.installPrefix:/usr
```
In this example, the executable will be installed into the `<build root>/install-root/usr/bin`
folder and the QML files will be installed into the
`<build root>/install-root/usr/share/myproject` folder.

To skip installation during the build, use the `--no-install` option.

To override the default location, use the `--install-root` option of the `qbs install`
command:
```sh
qbs build --no-install qbs.installPrefix:/usr
sudo qbs install --no-build --install-root /
```
In this example, artifacts will be installed directly into the `/usr` folder. Since the
`qbs install` command implies `build`, we use the `--no-build` parameter to ensure that
we do not accidentally rebuild the project, thereby changing the artifacts' owner to `root`.

Sometimes, it makes sense to install the application into a temporary root folder, keeping the
same folder structure within that root folder as in the examples above; for instance,
when building a Linux package such as `deb` or `rmp`. To install the application into the
`/tmp/myProjectRoot` folder, use the following command:

```sh
$ qbs install --install-root /tmp/myProjectRoot
```

In this example, the executable will be installed into the `/tmp/myProjectRoot/usr/bin` folder
and QML files will be installed into the `/tmp/myProjectRoot/usr/share/myproject` folder.

To remove all files from the install root prior to installing, use the `--clean-install-root`
parameter:

```sh
qbs install --clean-install-root --install-root /tmp/myProjectRoot
```

For more information about how the installation path is constructed, see
[[Installation Properties]].
