QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        audio.cpp \
        engine.cpp \
        main.cpp \
        sprite.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/release/ -lGL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/debug/ -lGL
else:unix: LIBS += -L$$PWD/../../../../usr/lib64/ -lGL

INCLUDEPATH += $$PWD/../../../../usr/include/GL
DEPENDPATH += $$PWD/../../../../usr/include/GL

HEADERS += \
    audio.h \
    audio.h \
    engine.h \
    matrix2d.h \
    sprite.h \
    sprite.h \
    testengine.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../GLFrameWork/build/Desktop-Desktop-Release/release/ -lGLFrameWork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../GLFrameWork/build/Desktop-Desktop-Release/debug/ -lGLFrameWork
else:unix: LIBS += -L$$PWD/../GLFrameWork/build/Desktop-Desktop-Release/ -lGLFrameWork

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../GLFrameWork/build/Desktop-Debug/release/ -lGLFrameWork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../GLFrameWork/build/Desktop-Debug/debug/ -lGLFrameWork
else:unix: LIBS += -L$$PWD/../GLFrameWork/build/Desktop-Debug/ -lGLFrameWork

INCLUDEPATH += $$PWD/../GLFrameWork/
DEPENDPATH += $$PWD/../GLFrameWork/


INCLUDEPATH += /usr/include/glm
DEPENDPATH += /usr/include/glm

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Logger/build/Desktop-Debug/release/ -lLogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Logger/build/Desktop-Debug/debug/ -lLogger
else:unix: LIBS += -L$$PWD/../Logger/build/Desktop-Debug/ -lLogger

INCLUDEPATH += $$PWD/../Logger
DEPENDPATH += $$PWD/../Logger

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/release/ -lSDL2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/debug/ -lSDL2
else:unix: LIBS += -L$$PWD/../../../../usr/lib64/ -lSDL2

INCLUDEPATH += $$PWD/../../../../usr/include/SDL2
DEPENDPATH += $$PWD/../../../../usr/include/SDL2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/release/ -lSDL2_mixer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/debug/ -lSDL2_mixer
else:unix: LIBS += -L$$PWD/../../../../usr/lib64/ -lSDL2_mixer

INCLUDEPATH += $$PWD/../../../../usr/include/SDL2
DEPENDPATH += $$PWD/../../../../usr/include/SDL2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/release/ -lGLEW
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/debug/ -lGLEW
else:unix: LIBS += -L$$PWD/../../../../usr/lib64/ -lGLEW

INCLUDEPATH += $$PWD/../../../../usr/include/GL
DEPENDPATH += $$PWD/../../../../usr/include/GL
