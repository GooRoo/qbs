---
title: 'Building for macOS'
---

# Building for macOS


This topic describes the Qbs features specific to macOS.

--8<-- "docs/getting-started/usage/targets/Apple.md.in:xcode"

--8<-- "docs/getting-started/usage/targets/Apple.md.in:building-user-interfaces"

--8<-- "docs/getting-started/usage/targets/Apple.md.in:creating-app-bundles"

--8<-- "docs/getting-started/usage/targets/Apple.md.in:architectures-and-variants"


## Building macOS Disk Images

The [[AppleDiskImage]] and [[AppleApplicationDiskImage]] items have a
dependency on the [[dmg]] module. The former represents a
product that is a basic Apple disk image, while the latter extends the
former to create a drag 'n' drop disk image installer used for installing
single application bundles.

For example, the following code snippet creates a macOS disk image with a
custom background and icon layout:

```qml
AppleApplicationDiskImage {
    targetName: "cocoa-application-" + version
    version: "1.0"

    files: [
        "CocoaApplication/dmg.iconset",
        "CocoaApplication/en_US.lproj/LICENSE",
        // comment out the following line to use a solid-color background
        // (see dmg.backgroundColor below)
        "CocoaApplication/background*"
    ]

    dmg.backgroundColor: "#41cd52"
    dmg.badgeVolumeIcon: true
    dmg.iconPositions: [
        {"x": 200, "y": 200, "path": "Cocoa Application.app"},
        {"x": 400, "y": 200, "path": "Applications"}
    ]
    dmg.windowX: 420
    dmg.windowY: 250
    dmg.windowWidth: 600
    dmg.windowHeight: 422 // this includes the macOS title bar height of 22
    dmg.iconSize: 64
}
```

<figure markdown="span">
    ![[qbs-dmg.png]]
</figure>

In addition, Qbs supports multi-language license agreement prompts that
appear when the DMG is opened, with full Unicode and rich-text formatting
support.
