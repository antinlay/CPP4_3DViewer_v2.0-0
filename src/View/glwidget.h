#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION
#include "objLoader.h"
#include <QOpenGLFunctions>
#include <QFile>
#include <QTimer>
#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget {
 Q_OBJECT
 public:
  QTimer *paintTimer;
  GLfloat angle = 0;
  GLuint model[3];
  GLuint texture[3];
  GLuint torus = 0;

  GLuint textureCount = 0;
  GLuint modelCount = 0;
  void initLight();
  GLuint drawCube();
  void loadGLTextures();
  void initTextures(uint index, QImage &texture);
//  int loadObject(const QString &filename);

 public:
  GLWidget(QWidget *parent = nullptr);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 private:
  ObjLoader *ol_;
};

#endif // GLWIDGET_H
