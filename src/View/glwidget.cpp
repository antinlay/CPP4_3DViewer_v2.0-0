#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  resize(800, 600);
  paintTimer = new QTimer(this);
  connect(paintTimer, &QTimer::timeout, this, QOverload<>::of(&GLWidget::repaint));
}

void GLWidget::initTextures(uint index, QImage &textureImg) {
    textureImg.convertTo(QImage::Format_RGBA8888);
    glBindTexture(GL_TEXTURE_2D, texture[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 1, GLsizei(textureImg.width()), GLsizei(textureImg.height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImg.bits());
}

void GLWidget::loadGLTextures() {
    glGenTextures(3, texture);

    QImage textureImg;
    textureImg.load("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/MetalCladdingFrame002/BUMP_1K.jpg");
    initTextures(0, textureImg);

    textureImg.load("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/MetalCladdingFrame002/BUMP_1K.jpg");
    initTextures(1, textureImg);

    textureImg.load("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/MetalCladdingFrame002/BUMP_1K.jpg");
    initTextures(2, textureImg);
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key()==Qt::Key_F) {
        ++textureCount%=3;
    }
    if (event->key()==Qt::Key_S) {
        if (paintTimer->isActive()) paintTimer->stop();
        else paintTimer->start();
    }
    if (event->key()==Qt::Key_M) {
        ++modelCount%=3;
    }
}



GLuint GLWidget::drawCube() {
    GLuint num = glGenLists(1);
    glNewList(num, GL_COMPILE);

    glBegin(GL_QUADS);
    // front facet
    glNormal3f(0.0f, 0.0f, 1.0f);
    // down left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    // down right
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    // up right
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    // up left
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

    // back facet
    glNormal3f(0.0f, 0.0f, -1.0f);
    // down left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    // down right
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    // up right
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    // up left
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

    // top facet
    glNormal3f(0.0f, 1.0f, 0.0f);
    // down left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // down right
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    // up right
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // up left
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

    // under facet
    glNormal3f(0.0f, 1.0f, 0.0f);
    // down left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // down right
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    // up right
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // up left
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

    // right facet
    glNormal3f(1.0f, 0.0f, 0.0f);
    // down left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    // down right
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    // up right
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // up left
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);

    // left facet
    glNormal3f(-1.0f, 0.0f, 0.0f);
    // down left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    // down right
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    // up right
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // up left
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    glEndList();
    return num;
}

void GLWidget::initLight() {
    GLfloat lightAmbient[] = { 0, 0, 0, 0.0 };
    GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightPosition[] = { 0.0, 0.0, 2.0, 1.0 };

    // setup parameters of light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // enable of light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
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
    model[0] = objloader::Instance().load("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/monkey2.obj");
    model[1] = objloader::Instance().load("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/sidor.obj");
    model[0] = drawCube();
    torus = objloader::Instance().load("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/torus.obj");
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

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTranslatef(0,0,-2.5);
    glDeleteLists(torus, 1);
    torus = objloader::Instance().draw(angle/100);
    glCallList(torus);
    glTranslatef(0,0,0.1f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texture[textureCount]);
    glCallList(model[modelCount]);
    angle += 0.3f;
}
