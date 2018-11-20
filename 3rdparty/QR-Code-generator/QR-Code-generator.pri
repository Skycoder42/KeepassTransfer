OUT_ROOT = $$shadowed($$SRC_ROOT_DIR)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_ROOT/3rdparty/QR-Code-generator/release/ -lqrcodegen
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_ROOT/3rdparty/QR-Code-generator/debug/ -lqrcodegen
else:unix: LIBS += -L$$OUT_ROOT/3rdparty/QR-Code-generator/ -lqrcodegen

INCLUDEPATH += $$PWD/src/cpp
DEPENDPATH += $$PWD/src/cpp

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/QR-Code-generator/release/libqrcodegen.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/QR-Code-generator/debug/libqrcodegen.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/QR-Code-generator/release/libqrcodegen.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/QR-Code-generator/debug/libqrcodegen.lib
else:unix: PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/QR-Code-generator/libqrcodegen.a
