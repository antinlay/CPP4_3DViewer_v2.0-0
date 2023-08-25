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
//      QVector<GLuint> vertexIndices;
//      QVector<GLuint> normalIndices;
//      QVector<GLuint> textureCoordIndices;
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
  bool dotLine = false;
  float trX = 0.0f, trY = 0.0f, trZ = -3.0f;
  QVector3D rotationCenter{};
  QOpenGLBuffer vertexBuffer{};
  QOpenGLBuffer normalBuffer{};
  QOpenGLBuffer textureCoordBuffer{};

  QPoint lastMousePosition{};
  GLfloat rotationX = 0.0f;
  GLfloat rotationY = 0.0f;
  GLfloat rotationZ = 0.0f;

  const QString vertexShaderSource = R"(
          attribute highp vec4 vertexPosition;
          attribute highp vec3 vertexNormal;
          attribute highp vec2 textureCoord;

          uniform highp mat4 projectionMatrix;
          uniform highp mat4 viewMatrix;
          uniform vec3 fixedLightPosition; // Добавляем позицию фиксированного источника света

          varying highp vec3 interpolatedNormal;
          varying highp vec2 interpolatedTextureCoord;

          void main()
          {
              gl_Position = projectionMatrix * viewMatrix * vertexPosition;
              interpolatedNormal = vertexNormal;
              interpolatedTextureCoord = textureCoord;

              // Передаем фиксированную позицию источника света во фрагментный шейдер
              vec4 fixedLightPositionInViewSpace = viewMatrix * vec4(fixedLightPosition, 1.0);
              gl_Position = projectionMatrix * viewMatrix * vertexPosition;
              interpolatedNormal = vertexNormal;
              interpolatedTextureCoord = textureCoord;
          }
      )";

  const QString fragmentShaderSource = R"(
            varying highp vec3 interpolatedNormal;
            varying highp vec2 interpolatedTextureCoord;
            uniform vec3 fixedLightPosition; // Добавляем фиксированную позицию источника света

            void main()
            {
                vec3 lightDirection = normalize(fixedLightPosition - gl_FragCoord.xyz); // Используем фиксированную позицию источника света
                vec3 normal = normalize(interpolatedNormal);

                float lightIntensity = dot(normal, lightDirection);

                lightIntensity = clamp(lightIntensity, 0.5, 1.0);

                vec3 objectColor = vec3(1.0, 0.9, 1.0);

                float shadowIntensity = 0.9;
                vec3 color = objectColor * lightIntensity * shadowIntensity;

                gl_FragColor = vec4(color, 1.0);
            }
        )";
};

#endif  // GLWIDGET_H
