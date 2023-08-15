#include <QApplication>
#include <QDebug>
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
#include <QWidget>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  GLWidget(QWidget *parent = nullptr)
      : QOpenGLWidget(parent),
        scale(1.0f),
        rotationX(0.0f),
        rotationY(0.0f),
        translation(0.0f, 0.0f, 0.0f) {}

 protected:
  void initializeGL() override {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    // Load .obj file
    QFile objFile(":/path/to/your/obj/file.obj");
    if (!objFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "Failed to open .obj file";
      return;
    }

    QVector<QVector3D> vertices;
    QVector<QVector<int>> faces;

    QTextStream in(&objFile);
    while (!in.atEnd()) {
      QString line = in.readLine().trimmed();
      QStringList tokens = line.split(' ');

      if (tokens.isEmpty()) continue;

      if (tokens[0] == "v") {
        float x = tokens[1].toFloat();
        float y = tokens[2].toFloat();
        float z = tokens[3].toFloat();
        vertices.append(QVector3D(x, y, z));
      } else if (tokens[0] == "f") {
        QVector<int> face;
        for (int i = 1; i < tokens.size(); ++i) {
          QStringList indices = tokens[i].split('/');
          int vertexIndex = indices[0].toInt() - 1;  // obj indices start from 1
          face.append(vertexIndex);
        }
        faces.append(face);
      }
    }

    objFile.close();

    // Create vertex array object
    vao.create();
    vao.bind();

    // Create vertex buffer object
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));

    // Create index buffer object
    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    for (const QVector<int> &face : faces)
      ibo.write(face.constData(), face.size() * sizeof(int));

    // Create shader program
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                    fragmentShaderSource);
    program.link();
    program.bind();

    // Set vertex attribute pointer
    program.enableAttributeArray(0);
    program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

    // Release all bindings
    vao.release();
    vbo.release();
    ibo.release();
    program.release();
  }

  void resizeGL(int w, int h) override { glViewport(0, 0, w, h); }

  void paintGL() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelMatrix;
    modelMatrix.scale(scale);
    modelMatrix.rotate(rotationX, QVector3D(1.0f, 0.0f, 0.0f));
    modelMatrix.rotate(rotationY, QVector3D(0.0f, 1.0f, 0.0f));
    modelMatrix.translate(translation);

    vao.bind();
    program.bind();

    program.setUniformValue("modelMatrix", modelMatrix);

    glDrawElements(GL_TRIANGLES, ibo.size() / sizeof(int), GL_UNSIGNED_INT,
                   nullptr);

    program.release();
    vao.release();
  }

  void wheelEvent(QWheelEvent *event) override {
    float delta = event->angleDelta().y() / 120.0f;
    scale += delta * 0.1f;
    update();
  }

  void mousePressEvent(QMouseEvent *event) override {
    lastMousePos = event->pos();
  }

  void mouseMoveEvent(QMouseEvent *event) override {
    QPoint delta = event->pos() - lastMousePos;
    rotationX += delta.y();
    rotationY += delta.x();
    lastMousePos = event->pos();
    update();
  }

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

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QSurfaceFormat format;
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  GLWidget widget;
  widget.resize(800, 600);
  widget.show();

  return app.exec();
}