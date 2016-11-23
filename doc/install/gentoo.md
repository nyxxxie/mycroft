# Requirements
Please install the following packages (ebuild TBA):

Package           | USE flags
----------------- | ---------
dev-util/cmake    |
dev-db/sqlite     | static-libs
dev-libs/capstone |
dev-libs/openssl  |
dev-qt/qtcore     |
dev-qt/qtwidgets  |

GCC and GNU make are also required, but should (will) already be installed.


# Compile

To compile mycroft, execute the following sequence of commands:
* `git clone --recursive https://github.com/nyxxxie/mycroft.git`
* `mkdir mycroft/build`
* `cd mycroft/build`
* `cmake ..`
# `make`

If nothing fails, mycroft should have been compiled and the client executable
will be located in `build/src/client/qt`.  If you experience any problems when
following this process, PLEASE open an issue about it.

# Install
I haven't created an install target yet, so for now just install by copying
the compiled executable in `build/src/client/qt` to somewhere in your $PATH.
The QT client follow the XDG directory standard, and will look for plugins,
configs, etc inside of `$HOME/.config/mycroft`.
