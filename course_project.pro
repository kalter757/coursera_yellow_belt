TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    condition_parser.cpp \
    condition_parser_test.cpp \
    database.cpp \
    date.cpp \
    node.cpp \
    token.cpp

HEADERS += \
    condition_parser.h \
    database.h \
    date.h \
    node.h \
    test_runner.h \
    token.h
