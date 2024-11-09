---
title: 'Appendix C: The JSON API'
---

# Appendix C: The JSON API

This API is the recommended way to provide Qbs support to an IDE.
It is accessible via the [[session]] command.

## Packet Format

All information is exchanged via _packets,_ which have the following
structure:

```
packet = "qbsmsg:" <payload length> [<meta data>] <line feed> <payload>
```

First comes a fixed string indentifying the start of a packet, followed
by the size of the actual data in bytes. After that, further meta data
might follow. There is none currently, but future extensions might add
some. A line feed character marks the end of the meta data section
and is followed immediately by the payload, which is a single JSON object
encoded in Base64 format. We call this object a _message._

## Messages

The message data is UTF8-encoded.

Most messages are either _requests_ or _replies._ Requests are messages
sent to Qbs via the session's standard input channel. Replies are messages
sent by Qbs via the session's standard output channel. A reply always
corresponds to one specific request. Every request (with the exception
of the [[#Closing the session|quit request]]) expects exactly one reply. A reply implies
that the requested operation has finished. At the very least, it carries
information about whether the operation succeeded, and often contains
additional data specific to the respective request.

Every message object has a `type` property, which is a string that uniquely
identifies the message type.

All requests block the session for other requests, including those of the
same type. For instance, if client code wishes to restart building the
project with different parameters, it first has to send a
[[#Cancelling an operation|cancel]] request, wait for the current build job's reply,
and only then can it request another build. The only other message beside
[[#Cancelling an operation|cancel]] that can legally be sent while a request
is currently being handled is the [[#Closing the session|quit]] message.

A reply object may carry an `error` property, indicating that the respective
operation has failed. If this property is not present, the request was successful.
The format of the `error` property is described [[#ErrorInfo|here]].

In the remainder of this page, we describe the structure of all messages
that can be sent to and received from Qbs, respectively. The property
tables may have a column titled _Mandatory,_ whose values indicate whether
the respective message property is always present. If this column is missing,
all properties of the respective message are mandatory, unless otherwise
noted.

## The `hello` Message

This message is sent by Qbs exactly once, right after the session was started.
It is the only message from Qbs that is not a response to a request.
The value of the `type` property is `"hello"`, the other properties are
as follows:

| Property         | Type   |
| ---------------- | ------ |
| api-level        | int    |
| api-compat-level | int    |
| lsp-socket       | string |

The value of `api-level` is increased whenever the API is extended, for instance
by adding new messages or properties.

The value of `api-compat-level` is increased whenever incompatible changes
are being done to this API. A tool written for API level `n` should refuse
to work with a Qbs version with an API compatibility level greater than `n`,
because it cannot guarantee proper behavior. This value will not change unless
it is absolutely necessary.

The value of `api-compat-level` is always less than or equal to the
value of `api-level`.

The value of `lsp-socket` is a path to a local domain socket (on Unix) or
a named pipe (on Windows). It provides a server implementing the
[Language Server Protocol](https://microsoft.github.io/language-server-protocol).

## Resolving a Project

To instruct Qbs to load a project from disk, a request of type
`resolve-project` is sent. The other properties are:

| Property                 | Type             | Mandatory                 |
| ------------------------ | ---------------- | ------------------------- |
| build-root               | [[#FilePath]]    | yes                       |
| configuration-name       | string           | no                        |
| data-mode                | [[#DataMode]]    | no                        |
| deprecation-warning-mode | string           | no                        |
| dry-run                  | bool             | no                        |
| environment              | [[#Environment]] | no                        |
| error-handling-mode      | string           | no                        |
| force-probe-execution    | bool             | no                        |
| log-time                 | bool             | no                        |
| log-level                | [[#LogLevel]]    | no                        |
| max-job-count            | int              | no                        |
| module-properties        | list of strings  | no                        |
| overridden-properties    | object           | no                        |
| project-file-path        | FilePath         | if resolving from scratch |
| restore-behavior         | string           | no                        |
| settings-directory       | string           | no                        |
| top-level-profile        | string           | no                        |
| wait-lock-build-graph    | bool             | no                        |

The `environment` property defines the environment to be used for resolving
the project, as well as for all subsequent Qbs operations on this project.

The `error-handling-mode` specifies how Qbs should deal with issues
in project files, such as assigning to an unknown property. The possible
values are `"strict"` and `"relaxed"`. In strict mode, Qbs will
immediately abort and set the reply's `error` property accordingly.
In relaxed mode, Qbs will continue to resolve the project if possible.
A [[#The warning Message|warning message]] will be emitted for every error that
was encountered, and the reply's `error` property will _not_ be set.
The default error handling mode is `"strict"`.

If the `log-time` property is `true`, then Qbs will emit [[#The log-data Message|log-data]] messages
containing information about which part of the operation took how much time.

The `module-properties` property lists the names of the module properties
which should be contained in the [[#ProductData|product data]] that
will be sent in the reply message. For instance, if the project to be resolved
is C++-based and the client code is interested in which C++ version the
code uses, then `module-properties` would contain `"cpp.cxxLanguageVersion"`.
Properties that have the value `undefined` will not be sent.

The `overridden-properties` property is used to override the values of
module, product or project properties. The possible ways to specify
keys are described [[Overriding Property Values from the Command Line|here]].

The `restore-behavior` property specifies if and how to make use of
an existing build graph. The value `"restore-only"` indicates that
a build graph should be loaded from disk and used as-is. In this mode,
it is an error if the build graph file does not exist.
The value `"resolve-only"` indicates that the project should be resolved
from scratch and that an existing build graph should be ignored. In this mode,
it is an error if the `"project-file-path"` property is not present.
The default value is `"restore-and-track-changes"`, which uses an
existing build graph if possible and re-resolves the project if no
build graph was found or if the parameters are different from the ones
used when the project was last resolved.

The `top-level-profile` property specifies which Qbs profile to use
for resolving the project. It corresponds to the `profile` key when
using the [[resolve]] command.

All other properties correspond to command line options of the [[resolve]]
command, and their semantics are described there.

When the project has been resolved, Qbs will reply with a `project-resolved`
message. The possible properties are:

| Property     | Type                     | Mandatory |
| ------------ | ------------------------ | --------- |
| error        | [[#ErrorInfo]]           | no        |
| project-data | [[#TopLevelProjectData]] | no        |

The `error-info` property is present if and only if the operation
failed. The `project-data` property is present if and only if
the conditions stated by the request's `data-mode` property
are fulfilled.

All other project-related requests need a resolved project to operate on.
If there is none, they will fail.

There is at most one resolved project per session. If client code wants to
open several projects or one project in different configurations, it needs
to start additional sessions.

## Building a Project

To build a project, a request of type `build-project` is sent. The other properties,
none of which are mandatory, are listed below:

| Property                   | Type                       |
| -------------------------- | -------------------------- |
| active-file-tags           | string list                |
| changed-files              | [[#FilePath]] list         |
| check-outputs              | bool                       |
| check-timestamps           | bool                       |
| clean-install-root         | bool                       |
| data-mode                  | [[#DataMode]]              |
| dry-run                    | bool                       |
| command-echo-mode          | string                     |
| enforce-project-job-limits | bool                       |
| files-to-consider          | [[#FilePath]] list         |
| install                    | bool                       |
| job-limits                 | list of objects            |
| keep-going                 | bool                       |
| log-level                  | [[#LogLevel]]              |
| log-time                   | bool                       |
| max-job-count              | int                        |
| module-properties          | list of strings            |
| products                   | list of strings or `"all"` |

All boolean properties except `install` default to `false`.

The `active-file-tags` and `files-to-consider` are used to limit the
build to certain output tags and/or source files.
For instance, if only C/C++ object files should get built, then
`active-file-tags` would be set to `"obj"`.

The objects in a `job-limits` array consist of a string property `pool`
and an int property `limit`.

If the `log-time` property is `true`, then Qbs will emit [[#The log-data Message|log-data]] messages
containing information about which part of the operation took how much time.

If `products` is an array, the elements must correspond to the
`full-display-name` property of previously retrieved [[#ProductData]],
and only these products will get built.
If `products` is the string `"all"`, then all products in the project will
get built.
If `products` is not present, then products whose
[[Product::builtByDefault|builtByDefault]] property is `false` will
be skipped.

The `module-properties` property has the same meaning as in the
[[#Resolving a Project|resolve-project]] request.

All other properties correspond to options of the [[build]] command.

When the build has finished, Qbs will reply with a `project-built`
message. The possible properties are:

| Property     | Type                     | Mandatory |
| ------------ | ------------------------ | --------- |
| error        | [[#ErrorInfo]]           | no        |
| project-data | [[#TopLevelProjectData]] | no        |

The `error-info` property is present if and only if the operation
failed. The `project-data` property is present if and only if
the conditions stated by the request's `data-mode` property
are fulfilled.

Unless the `command-echo-mode` value is `"silent"`, a message of type
`command-description` is emitted for every command to be executed.
It consists of two string properties `highlight` and `message`,
where `message` is the message to present to the user and `highlight`
is a hint on how to display the message. It corresponds to the
[[Command and JavaScriptCommand|Command]] property of the same name.

For finished process commands, a message of type `process-result`
might be emitted. The other properties are:

| Property             | Type            |
| -------------------- | --------------- |
| arguments            | list of strings |
| error                | string          |
| executable-file-path | [[#FilePath]]   |
| exit-code            | int             |
| stderr               | list of strings |
| stdout               | list of strings |
| success              | bool            |
| working-directory    | [[#FilePath]]   |

The `error` string is one of `"failed-to-start"`, `"crashed"`, `"timed-out"`,
`"write-error"`, `"read-error"` and `"unknown-error"`.
Its value is not meaningful unless `success` is `false`.

The `stdout` and `stderr` properties describe the process's standard
output and standard error output, respectively, split into lines.

The `success` property is `true` if the process finished without errors
and an exit code of zero.

The other properties describe the exact command that was executed.

This message is only emitted if the process failed or it has printed data
to one of the output channels.

## Cleaning a Project

To remove a project's build artifacts, a request of type `clean-project`
is sent. The other properties are:

| Property   | Type            |
| ---------- | --------------- |
| dry-run    | bool            |
| keep-going | bool            |
| log-level  | [[#LogLevel]]   |
| log-time   | bool            |
| products   | list of strings |

The elements of the `products` array correspond to a `full-display-name`
of a [[#ProductData]]. If this property is present, only the respective
products' artifacts are removed.

If the `log-time` property is `true`, then Qbs will emit [[#The log-data Message|log-data]] messages
containing information about which part of the operation took how much time.

All other properties correspond to options of the [[clean]] command.

None of these properties are mandatory.

After all artifacts have been removed, Qbs replies with a
`project-cleaned` message. If the operation was successful, this message
has no properties. Otherwise, a property `error` of type [[#ErrorInfo]]
indicates what went wrong.

## Installing a Project

Installing is normally part of the [[#Building a Project|build]]
process. To do it in a separate step, the `install` property
is set to `false` when building and a dedicated `install-project`
message is sent. The other properties are:

| Property           | Type            |
| ------------------ | --------------- |
| clean-install-root | bool            |
| dry-run            | bool            |
| install-root       | [[#FilePath]]   |
| keep-going         | bool            |
| log-level          | [[#LogLevel]]   |
| log-time           | bool            |
| products           | list of strings |
| use-sysroot        | bool            |

The elements of the `products` array correspond to a `full-display-name`
of a [[#ProductData]]. If this property is present, only the respective
products' artifacts are installed.

If the `log-time` property is `true`, then Qbs will emit [[#The log-data Message|log-data]] messages
containing information about which part of the operation took how much time.

If the `use-sysroot` property is `true` and `install-root` is not present,
then the install root will be [[qbs::sysroot|qbs.sysroot]].

All other properties correspond to options of the [[install]] command.

None of these properties are mandatory.

## Canceling an Operation

Potentially long-running operations can be aborted using the `cancel-job`
request. This message does not have any properties. There is no dedicated
reply message; instead, the usual reply for the request associated with
the currently running operation will be sent, with the `error` property
set to indicate that it was canceled.

If there is no operation in progress, this request will have no effect.
In particular, if it arrives after the operation that it was supposed to
cancel has already finished (i.e. there is a race condition), the reply
received by client code will not contain a cancellation-related error.

## Adding, Removing and Renaming Source Files

Source files can be added to, removed from and renamed in Qbs project files with
the `add-files`, `remove-files` and `rename-files` messages, respectively. These two
requests have the same set of properties:

| Property | Type      |
| -------- | --------- |
| files    | see below |
| group    | string    |
| product  | string    |

The `files` property specifies which files should be added, removed or renamed.
For the `add-files` and `remove-files` messages, this is a `FilePath` list.
For the `rename-files` message, this is a list of objects with two properties
`source-path` and `target-path`, each of which is a `FilePath`.

The `product` property corresponds to the `full-display-name` of
a [[#ProductData]] and specifies to which product to apply the operation.

The `group` property corresponds to the `name` of a [[#GroupData]]
and specifies to which group in the product to apply the operation.

After the operation has finished, Qbs replies with a `files-added`,
`files-removed` and `files-renamed` message, respectively. Again, the properties are
the same:

| Property     | Type               | Mandatory |
| ------------ | ------------------ | --------- |
| error        | [[#ErrorInfo]]     | no        |
| failed-files | [[#FilePath]] list | no        |

If the `error` property is present, the operation has at least
partially failed and `failed-files` will list the files
that could not be added or removed.

## The `get-run-environment` Message

This request retrieves the full run environment for a specific
executable product, taking into account the
[[Module::setupRunEnvironment|setupRunEnvironment]] scripts
of all modules pulled in by the product. The properties are as follows:

| Property         | Type             | Mandatory |
| ---------------- | ---------------- | --------- |
| base-environment | [[#Environment]] | no        |
| config           | list of strings  | no        |
| product          | string           | yes       |

The `base-environment` property defines the environment into which
the Qbs-specific values should be merged.

The `config` property corresponds to the [[--setup-run-env-config]]
option of the [[run]] command.

The `product` property specifies the product whose environment to
retrieve. The value must correspond to the `full-display-name`
of some [[#ProductData]] in the project.

Qbs will reply with a `run-environment` message. In case of failure,
it will contain a property `error` of type [[#ErrorInfo]], otherwise
it will contain a property `full-environment` of type [[#Environment]].

## The `get-generated-files-for-sources` Message

This request allows client code to retrieve information about
which artifacts are generated from a given source file.
Its sole property is a list `products`, whose elements are objects
with the two properties `full-display-name` and `requests`.
The first identifies the product to which the requests apply, and
it must match the property of the same name in a [[#ProductData]]
in the project.
The latter is a list of objects with the following properties:

| Property    | Type            | Mandatory |
| ----------- | --------------- | --------- |
| source-file | [[#FilePath]]   | yes       |
| tags        | list of strings | no        |
| recursive   | bool            | no        |

The `source-file` property specifies a source file in the respective
product.

The `tags` property constrains the possible file tags of the generated
files to be matched. This is relevant if a source files serves as input
to more than one rule or the rule generates more than one type of output.

If the `recursive` property is `true`, files indirectly generated
from the source file will also be returned. The default is `false`.
For instance, íf this property is enabled for a C++ source file,
the final link target (e.g. a library or an application executable)
will be returned in addition to the object file.

Qbs will reply with a `generated-files-for-sources` message, whose
structure is similar to the request. It also has a single object list
property `products`, whose elements consist of a string property
`full-display-name` and an object list property `results`.
The properties of these objects are:

| Property        | Type               |
| --------------- | ------------------ |
| source-file     | [[#FilePath]]      |
| generated-files | [[#FilePath]] list |

The `source-file` property corresponds to an entry of the same name
in the request, and the `generated-files` are the files which are
generated by Qbs rules that take the source file as an input,
taking the constraints specified in the request into account.

Source files for which the list would be empty are not listed.
Similarly, products for which the `results` list would be empty
are also omitted.

!!! note

    The results may be incomplete if the project has not been fully built.

## Closing a Project

A project is closed with a `release-project` message. This request has
no properties.

Qbs will reply with a `project-released` message. If no project was open,
the reply will contain an `error` property of type [[#ErrorInfo]].

## Closing the Session

To close the session, a `quit` message is sent. This request has no
properties.

Qbs will cancel all currently running operations and then close itself.
No reply will be sent.

## Progress Messages

While a request is being handled, Qbs may emit progress information in order
to enable client code to display a progress bar.

\target task-started
### The `task-started` Message

This is always the first progress-related message for a specific request.
It appears at most once per request.
It consists of a string property `description`, whose value can be displayed
to users, and an integer property `max-progress` that indicates which
progress value corresponds to 100 per cent.

\target task-progress
### The `task-progress` Message

This message updates the progress via an integer property `progress`.

\target new-max-progress
### The `new-max-progress` Message

This message is emitted if the original estimated maximum progress has
to be corrected. Its integer property `max-progress` updates the
value from a preceding [[#The task-started Message|task-started]] message.

## Messages for Users

There are two types of messages that purely contain information to be
presented to users.

\target log-data
### The `log-data` Message

This object has a string property `message`, which is the text to be
shown to the user.

\target warning-message
### The `warning` Message

This message has a single property `warning` of type [[#ErrorInfo]].

## The `protocol-error` Message

Qbs sends this message as a reply to a request with an unknown `type`.
It contains an `error` property of type [[#ErrorInfo]].

## Project Data

If a request can alter the build graph data, the associated reply may contain
a `project-data` property whose value is of type [[#TopLevelProjectData]].

### TopLevelProjectData

This data type represents the entire project. It has the same properties
as [[#PlainProjectData]]. If it is part of a `project-resolved` message,
these additional properties are also present:

| Property              | Type               |
| --------------------- | ------------------ |
| build-directory       | [[#FilePath]]      |
| build-graph-file-path | [[#FilePath]]      |
| build-system-files    | [[#FilePath]] list |
| overridden-properties | object             |
| profile-data          | object             |

The value of `build-directory` is the top-level build directory.

The `build-graph-file-path` value is the path to the build graph file.

The `build-system-files` value contains all Qbs project files, including
modules and JavaScript helper files.

The value of `overridden-properties` is the one that was passed in when
the project was last [[#Resolving a Project|resolved]].

The `profile-data` property maps the names of the profiles used in the project
to the respective property maps. Unless profile multiplexing is used, this
object will contain exactly one property.

### PlainProjectData

This data type describes a [[Project]] item. The properties are as follows:

| Property     | Type                       |
| ------------ | -------------------------- |
| is-enabled   | bool                       |
| location     | [[#FilePath]]              |
| name         | string                     |
| products     | [[#ProductData]] list      |
| sub-projects | [[#PlainProjectData]] list |

The `is-enabled` property corresponds to the project's
[[Project::condition|condition]].

The `location` property is the exact position in a Qbs project file
where the corresponding [[Project]] item was defined.

The `products` and `sub-projects` are what the project has pulled in via
its [[Project::references|references]] property.

### ProductData

This data type describes a [[Product]] item. The properties are as follows:

| Property                   | Type                      |
| -------------------------- | ------------------------- |
| build-directory            | [[#FilePath]]             |
| dependencies               | list of strings           |
| full-display-name          | string                    |
| generated-artifacts        | [[#ArtifactData]] list    |
| groups                     | [[#GroupData]] list       |
| is-enabled                 | bool                      |
| is-multiplexed             | bool                      |
| is-runnable                | bool                      |
| location                   | [[#Location]]             |
| module-properties          | [[#ModulePropertiesData]] |
| multiplex-configuration-id | string                    |
| name                       | string                    |
| properties                 | object                    |
| target-executable          | [[#FilePath]]             |
| target-name                | string                    |
| type                       | list of strings           |
| version                    | string                    |

The elements of the `dependencies` array correspond to the full-display-name
properties of the products that this product has pulled in via [[Depends]] items.

The `generated-artifacts` are files that are created by the [[Rule|rules]]
in this product.

The `groups` list corresponds to the [[Group]] items in this product.
In addition, a "pseudo-group" is created for the [[Product::files|files]]
property of the product itself. Its name is the same as the product's.

The `is-enabled` property corresponds to the product's
[[Product::condition|condition]]. A product may also get disabled
if it contains errors and Qbs was was instructed to operate in relaxed mode
when the project was [[#Resolving a Project|resolved]].

The `is-multiplexed` property is true if and only if the product is
[[Multiplexing|multiplexed]] over one ore more properties.

The `is-runnable` property indicates whether one of the product's
target artifacts is an executable file.
In that case, the file is available via the `target-executable` property.

The `location` property is the exact position in a Qbs project file
where the corresponding [[Product]] item was defined.

The `module-properties` object provides the values of the module properties
that were requested when the project was [[#Resolving a Project|resolved]].

The `name` property is the value given in the [[Product::name|Product item]],
whereas `full-display-name` is a name that uniquely identifies the
product in the entire project, even in the presence of multiplexing.
In the absence of multiplexing, it is the same as `name`. In either case,
it is suitable for being presented to users.

See the [[Product]] item documentation for a description of the other
properties.

### GroupData

This data type describes a [[Group]] item. The properties are:

| Property                        | Type                      |
| ------------------------------- | ------------------------- |
| is-enabled                      | bool                      |
| location                        | [[#Location]]             |
| module-properties               | [[#ModulePropertiesData]] |
| name                            | string                    |
| prefix                          | string                    |
| source-artifacts                | [[#ArtifactData]] list    |
| source-artifacts-from-wildcards | [[#ArtifactData]] list    |

The `is-enabled` property corresponds to the groups's
[[Group::condition|condition]]. However, if the group's product
is disabled, this property will always be `false`.

The `location` property is the exact position in a Qbs project file
where the corresponding [[Group]] item occurs.

The `module-properties` object provides the values of the module properties
that were requested when the project was [[#Resolving a Project|resolved]].
If no module properties are set on the Group level and the value would therefore
be the same as in the group's product, then this property is omitted.

The `source-artifacts` list corresponds the the files listed verbatim
in the group's [[Group::files|files]] property.

The `source-artifacts-from-wildcards` list represents the the files
expanded from wildcard entries in the group's [[Group::files|files]] property.

See the [[Group]] item documentation for a description of the other
properties.

### ArtifactData

This data type represents files that occur in the project, either as sources
or as outputs of a rules. Qbs project files, on the other hand, are not
artifacts. The properties are:

| Property          | Type                      |
| ----------------- | ------------------------- |
| file-path         | [[#FilePath]]             |
| file-tags         | list of strings           |
| install-data      | object                    |
| is-executable     | bool                      |
| is-generated      | bool                      |
| is-target         | bool                      |
| module-properties | [[#ModulePropertiesData]] |

The `install-data` property is an object whose `is-installable` property
indicates whether the artifact gets installed. If so, then the [[#FilePath]]
properties `install-file-path` and `install-root` provide further
information.

The `is-target` property is true if the artifact is a target artifact
of its product, that is, `is-generated` is true and `file-tags`
intersects with the [[Product::type|product type]].

The `module-properties` object provides the values of the module properties
that were requested when the project was [[#Resolving a Project|resolved]].
This property is only present for generated artifacts. For source artifacts,
the value can be retrieved from their [[#GroupData|group]].

The other properties should be self-explanatory.

### ModulePropertiesData

This data type maps fully qualified module property names to their
respective values.

## Other Custom Data Types

There are a number of custom data types that serve as building blocks in
various messages. They are described below.

### FilePath

A _FilePath_ is a string that describes a file or directory. FilePaths are
always absolute and use forward slashes for separators, regardless of
the host operating system.

### Location

A _Location_ is an object representing a file path and possibly also a position
within the respective file. It consists of the following properties:

| Property  | Type          | Mandatory |
| --------- | ------------- | --------- |
| file-path | [[#FilePath]] | yes       |
| line      | int           | no        |
| column    | int           | no        |

### ErrorInfo

An _ErrorInfo_ is an object representing error information. Its sole property
`items` is an array of objects with the following structure:

| Property    | Type          | Mandatory |
| ----------- | ------------- | --------- |
| description | string        | yes       |
| location    | [[#Location]] | no        |

### DataMode

This is the type of the `data-mode` property in a
[[#Resolving a project|resolve]] or [[#Building a project|build]]
request. It is used to indicate under which circumstances
the reply message should include the project data. The possible
values have string type and are as follows:

- `"never"`: Do not attach project data to the reply.
- `"always"`: Do attach project data to the reply.
- `"only-if-changed"`: Attach project data to the reply only
                              if it is different from the current
                              project data.

The default value is `"never"`.

### LogLevel

This is the type of the `log-level` property that can occur
in various requests. It is used to indicate whether the client would like
to receive [[#The log-data Message|log-data]] and/or [[#The warning Message|warning]] messages.
The possible values have string type and are as follows:

- `"error"`: Do not log anything.
- `"warning"`: Qbs may emit [[#The warning Message|warnings]], but no
                   [[#The log-data Message|log-data]] messages.
- `"info"`: In addition to warnings, Qbs may emit informational
                [[#The log-data Message|log-data]] messages.
- `"debug"`: Qbs may emit debug output. No messages will be generated;
                 instead, the standard error output channel will be used.

The default value is `"info"`.

### Environment

This data type describes a set of environment variables. It is an object
whose keys are names of environment variables and whose values are
the values of these environment variables.
