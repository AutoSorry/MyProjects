TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        command.cpp \
        expression.cpp \
        lexer.cpp \
        main.cpp \
        memory.cpp \
        statement.cpp

HEADERS += \
    command.h \
    expression.h \
    lexer.h \
    memory.h \
    statement.h

DISTFILES += \
    test1 \
    test2 \
    test3 \
    test4 \
    test5 \
    text
