# Mycroft
Have you ever had the misfortune of needing to learn how to interact with a
file format that doesn't have a mature (or any!) library?  Have you ever wanted
to know what is going on inside of a random file you downloaded off the
internet?  Mycroft's job is to make the process of exploring and working with
file formats as easy and intuitive as possible, using an annotated hex editor
interface and a c-like scripting language for defining a file's structure.
Mycroft features extra goodies like cross-file xrefs, a plugin system,
relational metadata storage, and more.

Please note that Windows is not officially supported but is planned.

## Installation
Install documents have been created for all platforms in which mycroft has been
tested.  Please use the below documents to aid in installing for your specific
platform.

* [Gentoo](doc/install/gentoo.md)
* [Arch Linux](doc/install/archlinux.md)
* [Fedora](/doc/install/fedora.md)

Worst case, if no document exists for your platform, the general form for an
install consists of installing dependencies (openssl, sqlite, qt5, capstone),
creating a build directory inside of the cloned project dir, generating a
buildsystem file using cmake (on linux just running `cmake ..` inside the build
directory) and then building the executable (on linux, `make`).  Install
by copying the resultant executable to somewhere in your $PATH.  

If you experience issues compiling on an untested platform, feel free to open
an issue.
