cmake_minimum_required(VERSION 2.8.11)

# Include settings
include(MycroftConfig.cmake)

# Version
set(MYCROFT_PACKAGE_MAJOR_VERSION 0)
set(MYCROFT_PACKAGE_MINOR_VERSION 1)

enable_testing()

# Include the directory with all our code
add_subdirectory(src)

# Include the third party code library
add_subdirectory(third_party)

# Add testing stuff
add_subdirectory(tests)

# Create target to build documentation
#add_custom_target(doc)
#add_subdirectory(doc)
