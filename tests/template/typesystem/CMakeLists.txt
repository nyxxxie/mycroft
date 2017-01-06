cmake_minimum_required(VERSION 2.8.11)

create_test(typesystem SRC
    main.cpp
    typesystem_defaults.cpp
    typesystem_created.cpp
    typesystem_python.cpp
    LIBS ${MYCROFT_CORE_NAME}
    COPYFILES
)

create_test_py(typesystem SRC typesystem.py)
