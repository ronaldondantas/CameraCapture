#include "camerarecord.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CameraRecord w;
    w.show();

    return a.exec();
}
