OUT_ROOT = $$shadowed($$SRC_ROOT_DIR)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_ROOT/3rdparty/qzxing/release/ -lQZXing
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_ROOT/3rdparty/qzxing/debug/ -lQZXing
else:unix: LIBS += -L$$OUT_ROOT/3rdparty/qzxing/ -lQZXing

INCLUDEPATH += $$PWD/src/src
DEPENDPATH += $$PWD/src/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/qzxing/release/libQZXing.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/qzxing/debug/libQZXing.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/qzxing/release/QZXing.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/qzxing/debug/QZXing.lib
else:unix: PRE_TARGETDEPS += $$OUT_ROOT/3rdparty/qzxing/libQZXing.a
