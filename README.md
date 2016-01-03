### wxMMDViewer ver.0.0.1
VMD file or PMD file viewer with wxWidgets for hobby

Work with
* Linux    (GTK2 or 3)
* Windows  (?)

This program will not work with (because of OpenGL ?)
* Mac OS X

[![Build Status](https://travis-ci.org/Hiroyuki-Nagata/wxMMDViewer.svg?branch=master)](https://travis-ci.org/Hiroyuki-Nagata/wxMMDViewer)

### Build
```
  $ ./bootstrap
  $ ./configure
  $ make
```

### Debug

* You can see some debug logs
```
  $ ./configure CXXFLAGS=-DDEBUG_BUILD
  $ make
```
