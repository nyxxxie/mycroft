cmake_minimum_required(VERSION 2.8.11)

create_test(context SRC
    main.cpp
    context.cpp
    LIBS ${MYCROFT_CORE_NAME}
)
