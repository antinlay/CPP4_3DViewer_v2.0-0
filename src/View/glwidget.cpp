#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    resize(800, 600);
}

void GLWidget::getFilePath(QString& filePath) {
    if (!filePath.isEmpty()) {
        if (!this->isVisible()) {
            this->show();
        }
        filePath_ = filePath;
    }
}

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0, 0, 0, 1);

  // Load .obj file
  QFile objFile(filePath_);
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
    qDebug() << tokens[1].toFloat() << " " << tokens[2].toFloat() << " " << tokens[3].toFloat();
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

  int offset = 0;  // Add offset
  for (const QVector<int> &face : faces) {
    ibo.write(offset, face.constData(), face.size() * sizeof(int));
    offset += face.size() * sizeof(int);  // Update offset
  }

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

void GLWidget::resizeGL(int w, int h)  { glViewport(0, 0, w, h); }

void GLWidget::paintGL()  {
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

void GLWidget::wheelEvent(QWheelEvent *event)  {
  float delta = event->angleDelta().y() / 120.0f;
  scale += delta * 0.1f;
  update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)  {
  lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)  {
  QPoint delta = event->pos() - lastMousePos;
  rotationX += delta.y();
  rotationY += delta.x();
  lastMousePos = event->pos();
  update();
}
