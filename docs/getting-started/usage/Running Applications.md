---
title: Running Applications
---

# Running Applications

Qbs has the convenience [[run|qbs run]] command that simplifies running applications.

For example, entering the following command runs the Qt Creator application:

```sh
qbs run --products qtcreator
```

By default, running an application also builds it and installs it to a
location from where it can be run on the desktop or on a device. Also, this command
[[Module::setupRunEnvironment|sets up]] the environment so that the application can find
dependent libraries.

This is not the case when running the binary manually - you'll have to make sure that the
app will find its dependencies and/or is relocatable. You can achieve that by
[[How do I make use of rpaths?|configuring rpaths]] properly or setting the appropriate
environment variable for the respective host system.
