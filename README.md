# Mycroft
Have you ever had the misfortune of needing to learn how to interact with a
file format that doesn't have a mature (or any!) library?  Have you ever wanted
to know what is going on inside of a random file you downloaded off the
internet?  Mycroft's job is to make the process of exploring and working with
file formats as easy and intuitive as possible, using a familiar hex editor
interface and a c-like scripting language for defining a file's structure.
Mycroft features extra goodies like multifile projects (in case one file
reference data in one or more others, for instance), a plugin system using
Python, and a relational metadata storage system.

## Compiling (Linux)
Mycroft uses the CMake build system.  To compile the project on linux, open a
terminal in this directory and run this command:

```mkdir build; cd build; cmake ..```

This will create a Makefile with several targets:

Command | Description
--- | ---
make | Compile project. |
make test | Run all unit tests.
make docs | Make documentation.

Generated documentation and binaries will be in their cooresponding locations
in the project directory (EG a target defined in lib/core/ will put compiled
files in build/lib/core)
