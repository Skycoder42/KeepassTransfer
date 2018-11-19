OUT_ROOT = $$shadowed($$SRC_ROOT_DIR)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_ROOT/lib/release/ -lkptransfer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_ROOT/lib/debug/ -lkptransfer
else:unix: LIBS += -L$$OUT_ROOT/lib/ -lkptransfer

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/lib/release/libkptransfer.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/lib/debug/libkptransfer.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/lib/release/kptransfer.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_ROOT/lib/debug/kptransfer.lib
else:unix: PRE_TARGETDEPS += $$OUT_ROOT/lib/libkptransfer.a

include($$SRC_ROOT_DIR/3rdparty/cryptopp/cryptopp.pri)
