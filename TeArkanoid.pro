TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    game.cpp \
    ball.cpp \
    paddle.cpp \
    brick.cpp \
    bonus.cpp \
    entity.cpp \
    enemy.cpp \
    rectentity.cpp \
    roundentity.cpp \
    lasergun.cpp


LIBS += `pkg-config --libs sfml-all`

QMAKE_CXXFLAGS += -std=gnu++14

HEADERS += \
    game.h \
    ball.h \
    paddle.h \
    brick.h \
    bonus.h \
    constants.h \
    utils.h \
    entity.h \
    enemy.h \
    rectentity.h \
    roundentity.h \
    lasergun.h
