# Requirements
Please install the following packages:

Package        | Notes
-------------- | ------------------------------------------------
cmake          |
sqlite-devel   |
capstone-devel |
python3-devel  |
openssl-devel  |
qt5-devel      |
gtest          |
doxygen        |
graphviz       | Optional, only *slightly* useful for development

> dnf install cmake sqlite-devel capstone-devel python3-devel openssl-devel qt5-devel gtest doxygen graphviz

GCC and GNU make are also required, but should (will) already be installed.

# Compile

To compile mycroft, execute the following sequence of commands:
* `git clone --recursive https://github.com/nyxxxie/mycroft.git`
* `mkdir mycroft/build`
* `cd mycroft/build`
* `cmake ..`
* `make`

If nothing fails, mycroft should have been compiled and the client executable
will be located in `build/src/client/qt`.  If you experience any problems when
following this process, PLEASE open an issue about it.

# Install
The makefile generated by cmake in the `mycroft/build` has an install target.
To install, run `make install`.  You can control where mycroft will be
installed to by passing the `-DCMAKE_INSTALL_PREFIX=prefix` to cmake when
running the above cmake command, where prefix is the directory to install to.
For example, to install systemwide prefix should be `/usr`.
The QT client follow the XDG directory standard, and will look for plugins,
configs, etc inside of `$HOME/.config/mycroft`.