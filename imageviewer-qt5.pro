QT += core
QT += widgets
qtHaveModule(printsupport): QT += printsupport
HEADERS       = imageviewer-qt5.h \
                qcustomplot.h
SOURCES       = imageviewer-qt5.cpp \
                imageviewer-main-qt5.cpp \
                qcustomplot.cpp
RESOURCES     = CG2_Uebung_01.pdf \
                CG2_Uebung_02.pdf \
                CG2_Uebung_03.pdf \
                CG2_Uebung_04.pdf \
                CG2_Uebung_05.pdf \
                lenna.ico \
                lenna.jpg \
                README.md
# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/imageviewer
INSTALLS += target

wince*: {
   DEPLOYMENT_PLUGIN += qjpeg qgif
}

RC_ICONS = "lenna.ico"
