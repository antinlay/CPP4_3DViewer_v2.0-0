#include "glwidget.h"

void GLWidget::getFileName(QString &fileName) {
    fileName_ = fileName;
}

void GLWidget::reloadObjFile(const QString& filePath) {
    // Очистить существующие данные
    clearData();
    // Загрузить и отобразить новый файл obj
    loadObjFile(filePath);
    resize(1920, 1080);
    // Перерисовать виджет
    update();
}

void GLWidget::clearData() {
    vertices.clear();
    normals.clear();
    textureCoords.clear();

    // Очистить буферы OpenGL
    vertexBuffer.destroy();
    normalBuffer.destroy();
    textureCoordBuffer.destroy();
}

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  loadObjFile(fileName_);
  qDebug() << fileName_ << " OPA";
  rotationCenter = QVector3D(0, 0, 0);
}

void GLWidget::textureFromImg() {
    // Загрузка изображения .jpg
    QImage image("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/MetalCladdingFrame002/GLOSS_1K.jpg");
    if (image.isNull()) {
        qDebug() << "Failed to load image";
        return;
    }
    // Создание текстуры из изображения
//    QOpenGLTexture *m_texture = new QOpenGLTexture(image);
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QOpenGLShaderProgram shaderProgram;

  shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                        vertexShaderSource);
  shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                        fragmentShaderSource);
  shaderProgram.link();
  shaderProgram.bind();

  QMatrix4x4 projectionMatrix;
  projectionMatrix.perspective(45.0f, width() / height(), 0.01f, 500.0f);

  QMatrix4x4 viewMatrix;
  viewMatrix.translate(trX, trY, trZ);
  viewMatrix.rotate(rotationX, 1.0f, 0.0f, 0.0f);
  viewMatrix.rotate(rotationY, 0.0f, 1.0f, 0.0f);
  viewMatrix.rotate(rotationZ, 0.0f, 0.0f, 1.0f);

  shaderProgram.setUniformValue("projectionMatrix", projectionMatrix);
  shaderProgram.setUniformValue("viewMatrix", viewMatrix);

  for (const Group& group : groups) {
      vertexBuffer.bind();
      shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);
      shaderProgram.enableAttributeArray("vertexPosition");

      normalBuffer.bind();
      shaderProgram.setAttributeBuffer("vertexNormal", GL_FLOAT, 0, 3);
      shaderProgram.enableAttributeArray("vertexNormal");

      textureCoordBuffer.bind();
      shaderProgram.setAttributeBuffer("textureCoord", GL_FLOAT, 0, 2);
      shaderProgram.enableAttributeArray("textureCoord");

      if (dotLine) {
          glPointSize(10.0f);
          glDrawArrays(GL_POINTS, 0, group.vertices.size());

          glLineWidth(2.0f);
          glDrawArrays(GL_LINES, 0, group.vertices.size());
      } else {
          glDrawArrays(GL_TRIANGLES, 0, group.vertices.size());
      }

      shaderProgram.disableAttributeArray("vertexPosition");
      shaderProgram.disableAttributeArray("vertexNormal");
      shaderProgram.disableAttributeArray("textureCoord");

      vertexBuffer.release();
      normalBuffer.release();
      textureCoordBuffer.release();
  }
  shaderProgram.release();
}

void GLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void GLWidget::loadObjFile(const QString &filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {return;}

  Group currentGroup;
  QVector<QVector3D> tempVertices;
  QVector<QVector3D> tempNormals;
  QVector<QVector2D> tempTextureCoords;
  QVector<GLuint> vertexIndices;
  QVector<GLuint> normalIndices;
  QVector<GLuint> textureCoordIndices;

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine().trimmed();
    QStringList tokens = line.split(' ');

    if (tokens[0] == "v") {
      float x = tokens[1].toFloat();
      float y = tokens[2].toFloat();
      float z = tokens[3].toFloat();
      tempVertices.append(QVector3D(x, y, z));
    } else if (tokens[0] == "vn") {
      float nx = tokens[1].toFloat();
      float ny = tokens[2].toFloat();
      float nz = tokens[3].toFloat();
      tempNormals.append(QVector3D(nx, ny, nz));
    } else if (tokens[0] == "vt") {
      float u = tokens[1].toFloat();
      float v = tokens[2].toFloat();
      tempTextureCoords.append(QVector2D(u, v));
    } else if (tokens[0] == "f") {
      for (int i = 1; i <= tokens.size() - 1; ++i) {
        QStringList faceTokens = tokens[i].split("/");
        if (faceTokens.size() >= 1 && !faceTokens[0].isEmpty()) {
          int v = (faceTokens[0].toInt() < 0) ? abs(faceTokens[0].toInt()) : faceTokens[0].toInt();
          vertexIndices.append(v - 1);
        }
        if (faceTokens.size() >= 2 && !faceTokens[1].isEmpty()) {
          int v = (faceTokens[1].toInt() < 0) ? abs(faceTokens[1].toInt()) : faceTokens[1].toInt();
          textureCoordIndices.append(v - 1);
        }
        if (faceTokens.size() == 3 && !faceTokens[2].isEmpty()) {
          int v = (faceTokens[2].toInt() < 0) ? abs(faceTokens[2].toInt()) : faceTokens[2].toInt();
          normalIndices.append(v - 1);
        }
      }
    } else if (tokens[0] == "g") {
        if (!tempVertices.isEmpty()) {
            groups.append(currentGroup);
            currentGroup = Group();
        }
    }
  }

  file.close();

  qDebug() << vertexIndices.size();
  for (int i = 0; i < vertexIndices.size(); ++i) {
    if (!tempVertices.isEmpty()) {
      currentGroup.vertices.append(tempVertices[vertexIndices[i]]);
    }
    if (!tempTextureCoords.isEmpty()) {
      currentGroup.textureCoords.append(tempTextureCoords[textureCoordIndices[i]]);
    }
    if (!tempNormals.isEmpty()) {
      currentGroup.normals.append(tempNormals[normalIndices[i]]);
    }
  }

  if (!currentGroup.vertices.isEmpty()) {
      groups.append(currentGroup);
  }

  initializeBuffers();
}

void GLWidget::initializeBuffers() {
for (Group& group : groups) {
      if (!group.vertices.isEmpty()) {
        qDebug() << "vertic: " << group.vertices.size();
        vertexBuffer.create();
        vertexBuffer.bind();
        vertexBuffer.allocate(group.vertices.constData(),
                              group.vertices.size() * sizeof(QVector3D));
      }
      if (!group.normals.isEmpty()) {
        qDebug() << "normals: " << group.normals.size();
        normalBuffer.create();
        normalBuffer.bind();
        normalBuffer.allocate(group.normals.constData(),
                              group.normals.size() * sizeof(QVector3D));
      }
      if (!group.textureCoords.isEmpty()) {
        qDebug() << "textureCoords: " << group.textureCoords.size();
        textureCoordBuffer.create();
        textureCoordBuffer.bind();
        textureCoordBuffer.allocate(group.textureCoords.constData(),
                                    group.textureCoords.size() * sizeof(QVector2D));
      }
}
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  lastMousePosition = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->position().x() - lastMousePosition.x();
  int dy = event->position().y() - lastMousePosition.y();

  if (event->buttons() & Qt::LeftButton) {
    // Вычисляем расстояние между точкой вращения и камерой
    //            float distance = (trZ * QVector4D(rotationCenter, 1.0f)).z();
    //            qDebug() << "distance: " << distance;
    // Вычисляем углы поворота с учетом отдаления сцены
    float rotationFactor = trZ * 0.25f;

    rotationX += dy * rotationFactor;
    rotationY += dx * rotationFactor;
    update();
  }

  lastMousePosition = event->pos();
}

void GLWidget::changeSceneScale(qreal scaleFactor) {
  // Изменяем масштаб в матрице вида
  trZ += scaleFactor;
  //        qDebug() << trZ;
  // Перерисовываем виджет
  update();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  // Получаем значение прокрутки колесика мыши
  int delta = event->angleDelta().y();

  // Определяем коэффициент масштабирования
  qreal scaleFactor = 1.0;
  if (delta > 0) {
    // Увеличиваем масштаб при прокрутке вперед
    scaleFactor = 0.1;
  } else if (delta < 0) {
    // Уменьшаем масштаб при прокрутке назад
    scaleFactor = -0.1;
  }

  // Изменяем масштаб сцены
  changeSceneScale(scaleFactor);
}
