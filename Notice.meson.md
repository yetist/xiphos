# Migrate to meson

[Meson](http://mesonbuild.com/index.html) is a cross platform building system designed to be as fast and user-friendly as possible. It supports many languages and compilers, including GCC, Clang, and Visual Studio.

I tried using meson to build Xiphos, and now it can work, at least on linux.

## How to compile

The easiest way to compile:

```
mkdir build
cd build/
meson
#mesonconf -DVar=Val
ninja
ninja install
```

This will automatically test your environment and will first use gtk3, gtkhtml-4.0, and webkitgtk-3.0 to compile.

If you want to specify more options, please see meson_options.txt.

For example, building a version with gtk2:

```
mesonconf -Dprefix=/usr -Dgtk=2
```

Build gtk3 version:

```
mesonconf -Dprefix=/usr -Dgtk=3
```

Build versions with gtk3 and webkit2:

```
mesonconf -Dprefix=/usr -Dgtk=3 -Denable-webkit2=true
```

## Problems to be solved

If the project moves to use meson to build, there are several code related things that need to be modified in addition to the currently added meson.build files:

* No help document was generated.

Because the meson's gnome.yelp component currently supports only `.page` source documents. If want to support the current XML source file format, should write a custom_target, which has not been done yet. See https://github.com/GNOME/yelp-tools/blob/master/help/C/index.page

* The desktop file (xiphos.desktop.in.in) should be modified to remove the '_'underlined front Name and Comments, or can not generate the correct international content in meson.

* The DBUS service support has been temporarily removed, and this will not compile the `xiphos-nav` command.

There are two reasons for doing so:

a. directly copies two files from one directory to another in waf, there is no corresponding function in meson. see [wscript#L507](
https://github.com/crosswire/xiphos/blob/master/wscript#L507) and [wscript#L508](https://github.com/crosswire/xiphos/blob/master/wscript#L508).

b. in waf, which generates two files of the same filename at the same time. See [src/webkit/wscript_build#L11](https://github.com/crosswire/xiphos/blob/master/src/webkit/wscript_build#L11) and [src/examples/wscript_build#L37](https://github.com/crosswire/xiphos/blob/master/src/examples/wscript_build#L37).

Meson can use gnome.genmarshal () to do the same function, but it doesn't allow two files of the same name to be generated in the same project.
