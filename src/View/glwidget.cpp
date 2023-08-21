#include "glwidget.h"
#include <QOpenGLFunctions>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  resize(800, 600);
  paintTimer = new QTimer(this);
  connect(paintTimer, &QTimer::timeout, this, QOverload<>::of(&GLWidget::repaint));
}

void GLWidget::initializeGL() {
    glEnable(GL_MULTISAMPLE);
    loadGLTextures();
    glEnable(GL_TEXTURE_2D);
    glClearColor(1,1,1,1);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    initLight();
//    model[0] = ObjLoader::instance().load("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/Cow.obj");
//    model[1] = ObjLoader::instance().load("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/cube.obj");
    model[0] = drawCube();
//    torus = ObjLoader::instance().load("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/Teddy.obj");
}

void GLWidget::resizeGL(int nWidth, int nHeight) {
    // setup view point
    glViewport(0,0, nWidth, nHeight);

    // init matrix of proection
//    qreal aspectratio = qreal(nWidth) / qreal(nHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    gluPerspective(90.0, aspectratio, 0.1, 100.0);

    // init matrix model view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
