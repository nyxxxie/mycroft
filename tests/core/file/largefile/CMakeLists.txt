# This should only be run on filesystems supporting sparse files!

cmake_minimum_required(VERSION 2.8.11)

create_test(file SRC
    main.cpp
    largefile.cpp
    LIBS ${MYCROFT_CORE_NAME} ${OPENSSL_LIBRARIES}
    COPYFILES
)
