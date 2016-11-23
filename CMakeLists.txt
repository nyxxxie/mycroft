cmake_minimum_required(VERSION 2.8.11)

# Include settings
include(MycroftConfig.cmake)

# Find requried libs
find_package(OpenSSL REQUIRED)
find_package(Capstone REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(PythonLibs 3.4 REQUIRED)

# Version
set(MYCROFT_PACKAGE_MAJOR_VERSION 0)
set(MYCROFT_PACKAGE_MINOR_VERSION 1)

# Enable testing
enable_testing()

# Include the directory with all application code
add_subdirectory(src)

# Include the directory with all library code
add_subdirectory(lib)

# Add testing stuff
add_subdirectory(tests)

# Add documentation stuff
add_subdirectory(doc)
