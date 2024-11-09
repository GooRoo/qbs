---
title: Custom Modules and Items
---

# Custom Modules and Items

Users of Qbs are not limited to the pre-defined [[List of Modules|modules]] and
[[List of Language Items|items]], they can also create their own. Here we describe how
to set up custom modules and items so that Qbs will find them.

## File System Layout

Items and modules are located under a common base directory, whose name and location is
completely arbitrary. We will refer to it as `search-path` here. This directory has two
subdirectories `modules` and `imports`, which contain Qbs modules and items, respectively.


## Custom Modules

To introduce a custom module `mymodule`, create a directory `search-path/modules/mymodule/`.

!!! note
    Module names are case-sensitive, and this also goes for the corresponding directory name.

Then, put a file containing an instance of the [[Module]] in there and give it the `.qbs`
extension. This module will be pulled in if a
[[Product|product]] declares a [[Depends|dependency]] on `mymodule`.


## Custom Items

To introduce a custom item `MyItem`, create the file `search-path/imports/MyItem.qbs`.

!!! note

    Item file names must start with a capital letter due to the fact that type names can
    only start with a capital letter. Otherwise, the file will be silently ignored.

## Making Qbs Aware of Custom Modules and Items

To be able to use your custom modules and items, you need to make them known to Qbs. You can
do this per project or globally.

### Project-specific Modules and Items

Let's assume you have a project that is located in `project_dir` and you have created some
modules in `project_dir/custom-stuff/modules/` as well as some items in
`project_dir/custom-stuff/imports/` that you want to use in the project.
To achieve this, your top-level project file should look like this:
```qml
// ...
Project {
    // ..
    qbsSearchPaths: "custom-stuff"
    // ..
}
```
!!! note
    For technical reasons, the custom modules and items will not be
    available in the file that contains the [[Project::qbsSearchPaths|Project.qbsSearchPaths]] property. Any product that wants to make use of
    them needs to be in a different file that is pulled in via the
    [[Project::references|Project.references]] property, for example.
    This is not a serious limitation, since every well-structured project will
    be split up in this manner.

### Making Custom Modules and Items Available Across Projects

What if your modules and items are generally useful and you want to access them in several
projects? In this case, it is best to add the location to your preferences.
For example:
```sh
qbs config preferences.qbsSearchPaths /usr/local/share/custom-qbs-extensions
```
