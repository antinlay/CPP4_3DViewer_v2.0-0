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
#include <QOpenGLTexture>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "/opt/goinfre/janiecee/Projects/tinyobjloader/tiny_obj_loader.h"

#include "QtGui/qevent.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  GLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}
  struct Group {
      QVector<QVector3D> vertices;
      QVector<QVector3D> normals;
      QVector<QVector2D> textureCoords;
      QVector<GLuint> indices;
      QVector3D center;
  };
  float getWidth(const QVector<QVector3D>& vertices);
  float getHeight(const QVector<QVector3D>& vertices);
  void getFileName(QString &fileName);
  void clearData();
  void reloadObjFile(const QString& filePath);
  void textureFromImg();
  void normalizeObject(QMatrix4x4 &projectionMatrix, QVector<Group> &groups, QOpenGLShaderProgram &shaderProgram);

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
  QVector<Group> groups{};
  QString fileName_{};
  bool dotLine = true;
  float trX = 0.0f, trY = 0.0f, trZ = -3.0f;
  QVector3D rotationCenter{};
  QOpenGLBuffer vertexBuffer{};
  QOpenGLBuffer normalBuffer{};
  QOpenGLBuffer textureCoordBuffer{};

  QPoint lastMousePosition{};
  GLfloat rotationX = 0.0f;
  GLfloat rotationY = 0.0f;
  GLfloat rotationZ = 0.0f;

  QVector3D frontLightPosition;
  QVector3D backLightPosition;

  const QString vertexShaderSource = R"(
      attribute highp vec4 vertexPosition;
      attribute highp vec3 vertexNormal;
      attribute highp vec2 textureCoord;

      uniform highp mat4 projectionMatrix;
      uniform highp mat4 viewMatrix;
      uniform vec3 frontLightPosition;
      uniform vec3 backLightPosition;

      varying highp vec3 interpolatedNormal;
      varying highp vec2 interpolatedTextureCoord;
      varying highp vec3 frontLightDirection;
      varying highp vec3 backLightDirection;

      void main()
      {
          gl_Position = projectionMatrix * viewMatrix * vertexPosition;
          interpolatedNormal = vertexNormal;
          interpolatedTextureCoord = textureCoord;

          vec4 vertexPositionInViewSpace = viewMatrix * vertexPosition;
          vec3 vertexPositionWorldSpace = vertexPositionInViewSpace.xyz / vertexPositionInViewSpace.w;
          frontLightDirection = normalize(frontLightPosition - vertexPositionWorldSpace);
          backLightDirection = normalize(backLightPosition - vertexPositionWorldSpace);
      }
  )";

  const QString fragmentShaderSource = R"(
      varying highp vec3 interpolatedNormal;
      varying highp vec2 interpolatedTextureCoord;
      varying highp vec3 frontLightDirection;
      varying highp vec3 backLightDirection;

      void main()
      {
          vec3 normal = normalize(interpolatedNormal);

          // Вычисляем освещенность от первого источника света (спереди)
          vec3 frontLightColor = vec3(1.0, 1.0, 1.0);
          float frontLightIntensity = dot(normal, frontLightDirection);
          frontLightIntensity = clamp(frontLightIntensity, 0.0, 1.0);
          vec3 frontLightContribution = frontLightIntensity * frontLightColor;

          // Вычисляем освещенность от второго источника света (сзади)
          vec3 backLightColor = vec3(1.0, 1.0, 1.0);
          float backLightIntensity = dot(normal, backLightDirection);
          backLightIntensity = clamp(backLightIntensity, 0.0, 1.0);
          vec3 backLightContribution = backLightIntensity * backLightColor;

          vec3 objectColor = vec3(1.0, 0.9, 1.0);
          vec3 ambientColor = vec3(0.2, 0.2, 0.2);

          vec3 finalColor = objectColor * (ambientColor + frontLightContribution + backLightContribution);

          gl_FragColor = vec4(finalColor, 1.0);
      }
  )";
};

#endif  // GLWIDGET_H
