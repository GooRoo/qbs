---
title: 'Building for Android'
---

# Building for Android

To develop applications for Android, you need development tools provided by
the Android SDK from Google, and (optionally) a C/C++ toolchain provided by
the Android NDK.

!!! note

    Qbs does not yet support the Kotlin programming language.

## Creating Android Application Packages

On Android, applications are distributed in a specially structured type of
ZIP package called an APK. The following files should be created and bundled
into an APK:

- Android assets.
- Android resource files.
- AndroidManifest.xml, which provides meta-information about your
        application.
- Compiled Java code, which serves as the entry point into your
        application and that automatically executes the native code in your
        application (if there is any).
- Shared libraries containing native code.


You can use the [[Application]] item to build application
packages for Android.

If the [[qbs::targetPlatform|target platform]] is `"android"`, then the Application item has
a dependency on the [[Android.sdk]] module, which
contains the properties and rules to create Android application packages
from source files.

You can use the [[DynamicLibrary]] item to build native
Android libraries that are bundled into the APK. The `qbs.architectures`
property specifies the architectures to build for, with the default value
`armv7a`.
If you have only one native library, you can simply list its sources
within the main Application item, and it will get built and packaged
automatically.

The [[DynamicLibrary]] item, as well as the [[CppApplication]] item,
has a dependency on the [[Android.ndk]] module,
and contains the properties and rules to create native libraries.
