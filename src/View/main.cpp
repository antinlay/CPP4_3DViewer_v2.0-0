#include "view.h"
#include "glwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;
    GLWidget gl;
    w.show();
//    QSurfaceFormat format;
//    format.setVersion(3, 3);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    QSurfaceFormat::setDefaultFormat(format);
    return a.exec();
}
