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

macro(CREATE_TEST_PY TARGET_NAME)
    set(options COPYFILES)
    set(oneval SRC)
    set(multivals)
    cmake_parse_arguments(ARG "${options}" "${oneval}" "${multivals}" ${ARGN} )

    set(TEST_TARGET test_${TARGET_NAME}_py)

    # TODO: I'm probably going to need to write some sort of script runner thing so python scripts have the libs they need

    if (PYTHONINTERP_FOUND)
        if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.1)
            add_test(
                NAME ${TEST_TARGET}
                COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/${ARG_SRC})
        else ()
            add_test(${TEST_TARGET}
                ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_SRC})
        endif ()
    endif ()
endmacro()
