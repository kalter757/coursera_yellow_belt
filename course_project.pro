TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
#    condition_parser.cpp \
#    condition_parser_test.cpp \
#    token.cpp \
    date.cpp \
    database.cpp
#    node.cpp

HEADERS += \
#    condition_parser.h \
#    token.h \
    date.h \
    database.h \
#    node.h \
    test_runner.h
