set(PROJECT_DESCRIPTION  "Blender is a very fast and versatile 3D modeller/renderer.")
set(PROJECT_COPYRIGHT    "Copyright (C) 2001-2011 Blender Foundation")
set(PROJECT_CONTACT      "foundation@blender.org")
set(PROJECT_VENDOR       "Blender Foundation")
set(ORG_WEBSITE          "www.blender.org")

set(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
set(CPACK_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
set(CPACK_PACKAGE_CONTACT ${PROJECT_CONTACT})
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

set(CPACK_PACKAGE_NAME "mycroft")
set(CPACK_PACKAGE_VERSION "beta")
set(CPACK_PACKAGE_VENDOR "Nyxxie")
set(CPACK_PACKAGE_CONTACT "nyxxie@protonmail.ch")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "File format reverse engineering and rapid prototyping tool.")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsqlite3-dev, libpython3.5-dev, libcapstone-dev, libssl-dev, cmake, qtbase5-dev, libgtest-dev, doxygen")


