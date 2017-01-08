macro(CREATE_TEST TARGET_NAME)
    set(options COPYFILES)
    set(oneval)
    set(multivals LIBS SRC)
    cmake_parse_arguments(ARG "${options}" "${oneval}" "${multivals}" ${ARGN} )

    set(TEST_TARGET test_${TARGET_NAME})
    add_executable(${TEST_TARGET} ${ARG_SRC})
    add_dependencies(${TEST_BUILD_TARGET} ${TEST_TARGET})
    target_link_libraries(${TEST_TARGET} ${ARG_LIBS} gtest)

    if (${ARG_COPYFILES})
        add_custom_target(${TEST_TARGET}_copy_files
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)
        add_dependencies(${TEST_TARGET} ${TEST_TARGET}_copy_files)
    endif ()

    add_test(${TEST_TARGET} ${TEST_TARGET})
endmacro()

set(MYCROFT_TOOL_PYTESBENCH_BIN ${CMAKE_BINARY_DIR}/src/tools/${MYCROFT_TOOL_PYTESBENCH_NAME}/${MYCROFT_TOOL_PYTESBENCH_NAME})
macro(CREATE_TEST_PY TARGET_NAME)
    set(options COPYFILES)
    set(oneval SRC)
    set(multivals)
    cmake_parse_arguments(ARG "${options}" "${oneval}" "${multivals}" ${ARGN} )

    set(TEST_TARGET test_${TARGET_NAME}_py)

    if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.1)
        add_test(
            NAME ${TEST_TARGET}
            COMMAND ${MYCROFT_TOOL_PYTESBENCH_BIN} ${CMAKE_CURRENT_LIST_DIR}/${ARG_SRC})
    else ()
        add_test(${TEST_TARGET}
            ${MYCROFT_TOOL_PYTESBENCH_BIN} ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_SRC})
    endif ()
endmacro()
