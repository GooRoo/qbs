---
title: Managing Qt Versions
---

# Managing Qt Versions

## Introduction

If your environment has the right `qmake` binary in its `PATH` and is also set up
properly for a matching toolchain, then you do not necessarily need a profile
to build projects with a Qt dependency. Otherwise, you should create one:

```sh
qbs setup-qt /usr/bin/qmake myqt
```

This will create the `myqt` profile which can then be used on
the command line:

```sh
qbs profile:myqt
```

!!! note

    If the `setup-toolchains` command has found more than one toolchain, you will need
    to manually link your Qt profile to one of them, like this:
    ```sh
    qbs config profiles.myqt.baseProfile <profile name>
    ```


## Multiple Qt Builds

To support multiple Qt builds, or in fact any combination of related settings, you need to
create several profiles. The following example illustrates how to set up
three different profiles, each for a different Qt build:

```sh
qbs setup-qt ~/dev/qt/4.7/bin/qmake qt47
qbs setup-qt ~/dev/qt/4.8/bin/qmake qt48
qbs setup-qt ~/dev/qt/5.0/qtbase/bin/qmake qt5
```

You can set the default Qt build like this:

```sh
qbs config defaultProfile qt5
```

To choose a Qt build that is different from the default, use:

```sh
qbs build profile:qt48
```

You can set other properties in a profile (not just Qt ones), in the same way
you override them from the command line. For example:

```powershell
qbs setup-qt C:\Qt\5.0.0\qtbase\bin\qmake.exe qt5
qbs config profiles.qt5.qbs.architecture x86_64
qbs config profiles.qt5.baseProfile msvc2010
```

The last example uses the inheritance feature of profiles. All settings in the profile
set as `baseProfile` are known in the derived profile as well.
They can of course be overridden there.
