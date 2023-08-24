#include "glwidget.h"
#include "view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  //    View w;
  GLWidget gl;
  gl.resize(1920, 1080);
  //    gl.loadModel("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/monkey2.obj");
  gl.show();
  return a.exec();
}
