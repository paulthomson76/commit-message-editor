# Commit Message Editor

A simple Git commit message editor with the following features:

* Separate subject and body fields
* Character counter for the subject field
* Message body is automatically word-wrapped at 72 characters

## Requirements

The project should build on any platform that's [supported by Qt 5](https://doc.qt.io/qt-5/supported-platforms.html).

## Building

```bash
$ qmake
$ make
```

## Installation and Use

1. Copy the `commit-message-editor` exectuable to a known location
2. Execute the following on the command line:

```bash
$ git config --global core.editor /path/to/commit-message-editor
```
