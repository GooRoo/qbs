---
title: Building Applications
---

# Building Applications

This section assumes that Qbs is present in `PATH`. For the details how to install Qbs, see
the [[Installing]] page.

To build applications from the command line, enter the following commands:

```sh
cd examples/collidingmice
qbs
```

By default, Qbs uses all the CPU cores available to achieve maximum build
parallelization. To explicitly specify the number of concurrent jobs, use
the `-j` option. For example, to run 4 concurrent jobs, enter the following
command:

```sh
qbs -j4
```

The application is built using the default build profile that is set up
in your Qbs configuration.

You can use the [[config]] command to set the max number of jobs per profile.
For example, to set four jobs as the default option for a profile named
_Android,_ enter the following command:

```sh
qbs config profiles.Android.preferences.jobs 4
```

To build with other profiles than the default one, specify options for the
[[build]] command. For example, to build debug and release configurations with
the _Android_ profile, enter the following command:

```sh
qbs build profile:Android config:debug config:release
```

The position of the property assignment is important. In the example
above, the profile property is set for all build configurations that come
afterwards.

To set a property just for one build configuration, place the assignment after
the build configuration name.
In the following example, the property [[cpp::treatWarningsAsErrors|cpp.treatWarningsAsErrors]] is set to `true` for debug only and
[[cpp::optimization|cpp.optimization]] is set to `small` for release only.

```sh
qbs build config:debug modules.cpp.treatWarningsAsErrors:true config:release modules.cpp.optimization:small
```

Projects are built in the debug build configuration by default.
