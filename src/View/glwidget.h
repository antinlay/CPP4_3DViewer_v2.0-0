#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QFile>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QVector2D>
#include <QVector3D>
#include <QtCore/QTextStream>
#include <QtCore/QVector>

#include "QtGui/qevent.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  GLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {
    rotationX = 0.0f;
    rotationY = 0.0f;
    rotationZ = 0.0f;
  }

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
  void loadObjFile(const QString &filePath);
  void initializeBuffers();
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void changeSceneScale(qreal scaleFactor);
  void wheelEvent(QWheelEvent *event) override;

 private:
  bool dotLine = true;
  float trX = 0.0f, trY = 0.0f, trZ = -3.0f;
  QVector3D rotationCenter{};
  QOpenGLBuffer vertexBuffer{};
  QOpenGLBuffer normalBuffer{};
  QOpenGLBuffer textureCoordBuffer{};

  QVector<QVector3D> vertices{};
  QVector<QVector3D> normals{};
  QVector<QVector2D> textureCoords{};

  QPoint lastMousePosition{};
  GLfloat rotationX = 0;
  GLfloat rotationY = 0;
  GLfloat rotationZ = 0;

  const QString vertexShaderSource = R"(
        attribute highp vec4 vertexPosition;
        attribute highp vec3 vertexNormal;
        attribute highp vec2 textureCoord;

        uniform highp mat4 projectionMatrix;
        uniform highp mat4 viewMatrix;

        varying highp vec3 interpolatedNormal;
        varying highp vec2 interpolatedTextureCoord;

        void main()
        {
            gl_Position = projectionMatrix * viewMatrix * vertexPosition;
            interpolatedNormal = vertexNormal;
            interpolatedTextureCoord = textureCoord;
        }
    )";

  const QString fragmentShaderSource = R"(
        varying highp vec3 interpolatedNormal;
        varying highp vec2 interpolatedTextureCoord;

        void main()
        {
            gl_FragColor = vec4(interpolatedNormal, 1.0);
        }
    )";

};

#endif  // GLWIDGET_H
