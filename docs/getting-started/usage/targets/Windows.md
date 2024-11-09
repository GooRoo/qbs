---
title: 'Building for Windows'
---

# Building for Windows


This topic describes the Qbs features specific to Windows.

!!! note

    Qbs does not currently support building applications using .NET
technologies and languages such as C#, F#, and Visual Basic. At this time we
recommend that you use MSBuild and the tools shipped with the various
implementations of the .NET platform.

## Windows Resources

The [[ico]] module contains rules and properties for building
Windows icon (.ico) and cursor (.cur) files from a set of raw PNGs.

## Universal Windows Platform

Building applications for the Universal Windows Platform is currently only
partially supported. Notably, support for building APPX packages is missing,
but will be added in a future release.

Relevant properties include:


- [[cpp::windowsApiFamily|cpp.windowsApiFamily]]
- [[cpp::windowsApiAdditionalPartitions|cpp.windowsApiAdditionalPartitions]]
- [[cpp::requireAppContainer|cpp.requireAppContainer]]


See the [[cpp]] module for more information.

!!! note

    Qbs does not (and will not) support building Windows Runtime
applications targeting Windows 8 or Windows 8.1. We encourage users to
instead build desktop applications for older versions of Windows, or migrate
to Windows 10 and the Universal Windows Platform.

## Building Windows Installers

The following modules contain properties and rules for building Windows
installers using a number of different technologies:


- [[innosetup]] - Inno Setup
- [[nsis]] - Nullsoft Scriptable Install System
- [[wix]] - Windows Installer XML Toolset
