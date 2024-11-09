---
title: 'Building for QNX'
---

# Building for QNX


To develop applications for the QNX Neutrino RTOS, you need to install the
QNX Software Development Platform (SDP) on a Linux, macOS, or Windows
development host. You can deploy the QNX Neutrino RTOS on a target system,
such as embedded hardware, a virtual machine, or a PC.

Qbs automatically determines the location of the SDP base directory if the
SDP is installed at one of the standard locations, such as `~/qnx700`,
`/opt/qnx700`, or `C:\qnx700`. In addition, Qbs uses the SDP and the
information it has about the host operating system to determine the location
of the QNX host and target directories.

If the QNX SDP path could not be determined automatically, you must add a
dependency to the [[qnx]] module to your application and set the
[[qnx::sdkDir|qnx.sdkDir]] property:

```qml
Application {
    name: "helloworld"
    files: "main.cpp"
    Depends { name: "cpp" }

    Depends { name: "qnx" }
    qnx.sdkDir: "/path/to/qnx700"
}
```

Alternatively, you can set the `qnx.sdkDir` property in a profile or on
the command line.

Qbs supports QNX SDP version 6.5 and above.

For more information about developing applications for the QNX Neutrino
RTOS, see the [QNX Product Documentation](http://www.qnx.com/developers/docs/).
