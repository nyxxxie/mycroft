cmake_minimum_required(VERSION 2.8.11)

# Version
set(MYCROFT_PACKAGE_MAJOR_VERSION 0)
set(MYCROFT_PACKAGE_MINOR_VERSION 1)

enable_testing()

# Include src files
add_subdirectory(src)

# Create target to build documentation
#add_custom_target(doc)
#add_subdirectory(doc)
