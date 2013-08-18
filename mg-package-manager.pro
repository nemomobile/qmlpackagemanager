
TARGET=mg-package-manager

# Add more folders to ship with the application, here
folder_01.source = qml/package-manager
folder_01.target = qml
images.source = qml/images
images.target = qml
DEPLOYMENTFOLDERS = folder_01 images

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE6F8DC45

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

CONFIG += link_pkgconfig
#PKGCONFIG += PackageKit-Qt5
QT += dbus network qml quick widgets

INCLUDEPATH += /usr/include/PackageKit/packagekit-qt5/

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    packagemodel.cpp \
    iconprovider.cpp \
    packagemanager.cpp \
    packagemarkings.cpp \
    package.cpp \
    filterpackagemodel.cpp \
    transactionwrapper.cpp \
    packmancontext.cpp \
    packagelist.cpp \
    packagegroup.cpp \
    repository.cpp \
    networktest.cpp \
    packageinfo.cpp \
    detailsinfo.cpp \
    updatedetails.cpp

# Please do not modify the following two lines. Required for deployment.
#include(qmlapplicationviewer/qmlapplicationviewer.pri)
#qtcAddDeployment()

HEADERS += \
    packagemodel.h \
    iconprovider.h \
    packagemanager.h \
    packagemarkings.h \
    package.h \
    filterpackagemodel.h \
    transactionwrapper.h \
    packmancontext.h \
    packagelist.h \
    packagegroup.h \
    repository.h \
    networktest.h \
    packageinfo.h \
    detailsinfo.h \
    updatedetails.h

