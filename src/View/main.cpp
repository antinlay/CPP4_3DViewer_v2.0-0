#include "view.h"
#include "glwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;
    w.show();
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    GLWidget gl;
    QObject::connect(&w, &View::uiEventSendFilePath, &gl, &GLWidget::getFilePath);
    return a.exec();
}
