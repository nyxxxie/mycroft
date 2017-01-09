cmake_minimum_required(VERSION 2.8.11)

create_test(project SRC
    main.cpp
    project.cpp
    LIBS ${MYCROFT_CORE_NAME}
    COPYFILES
)
