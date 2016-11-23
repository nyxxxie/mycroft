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


# Compile
cd into the project directory and create a new directory called "build".  
Execute `cmake ..` and then finally `make`.  If you experience any problems 
when running the cmake command, PLEASE open an issue about it.

# Install
I haven't created an install target yet, so for now just install by copying
the compiled executable in `build/src/client/qt` to somewhere in your $PATH.
The QT client follow the XDG directory standard, and will look for plugins,
configs, etc inside of `$HOME/.config/mycroft`.
