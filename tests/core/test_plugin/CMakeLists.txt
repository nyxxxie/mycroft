cmake_minimum_required(VERSION 2.8.11)

create_test(plugin SRC
    main.cpp
    tests.cpp
    LIBS ${MYCROFT_CORE_NAME}
)
