cmake_minimum_required(VERSION 2.8.11)

create_test(template SRC
    main.cpp
    template_basicentry.cpp
    template_somestructs.cpp
    LIBS ${MYCROFT_CORE_NAME}
    COPYFILES
)
