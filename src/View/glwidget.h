#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QFile>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QStringList>
#include <QVector3D>
#include <QVector>
#include <QWheelEvent>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 Q_OBJECT
 public:
  GLWidget(QWidget *parent = nullptr);
  void getFilePath(QString& filePath);
  QString filePath_ = "";

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

private slots:
//  void sendFilePath(QString &filePath) {}

 private:
  QOpenGLVertexArrayObject vao;
  QOpenGLBuffer vbo;
  QOpenGLBuffer ibo;
  QOpenGLShaderProgram program;

  const char *vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        uniform mat4 modelMatrix;
        void main()
        {
            gl_Position = modelMatrix * vec4(position, 1.0);
        }
    )";

  const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 color;
        void main()
        {
            color = vec4(1.0, 1.0, 1.0, 1.0);
        }
    )";

  float scale;
  float rotationX;
  float rotationY;
  QVector3D translation;
  QPoint lastMousePos;
};

#endif // GLWIDGET_H
