#include "view.h"
#include "glwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    View w;
    GLWidget gl;
    gl.show();
    return a.exec();
}
