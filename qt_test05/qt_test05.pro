QT += qml quick
TARGET = qt_test05
!android: !ios: !blackberry: qtHaveModule(widgets): QT += widgets

include(src/src.pri)


OTHER_FILES += \
    main.qml

RESOURCES += \
    resources.qrc

HEADERS += \
    src/app.h
