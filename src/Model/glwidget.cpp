#include "glwidget.h"

void GLWidget::getFileName(QString &fileName) {
    fileName_ = fileName;
}

float GLWidget::getWidth(const QVector<QVector3D>& vertices) {
    float minX = vertices[0].x();
    float maxX = vertices[0].x();

    for (int i = 1; i < vertices.size(); ++i) {
        float x = vertices[i].x();
        if (x < minX) {
            minX = x;
        }
        if (x > maxX) {
            maxX = x;
        }
    }

    return maxX - minX;
}

float GLWidget::getHeight(const QVector<QVector3D>& vertices) {
    float minY = vertices[0].y();
    float maxY = vertices[0].y();

    for (int i = 1; i < vertices.size(); ++i) {
        float y = vertices[i].y();
        if (y < minY) {
            minY = y;
        }
        if (y > maxY) {
            maxY = y;
        }
    }

    return maxY - minY;
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
    groups.clear();
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
  frontLightPosition = QVector3D(1.0f, 1.0f, 1.0f);
  backLightPosition = QVector3D(-1.0f, -1.0f, -1.0f);
  if (!groups.isEmpty()) {
      rotationCenter = groups[0].center;
  }
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

void GLWidget::normalizeObject(QMatrix4x4 &projectionMatrix, QVector<Group> &groups, QOpenGLShaderProgram &shaderProgram) {
    // Получаем размеры окна
    int windowWidth = width();
    int windowHeight = height();

    // Получаем размеры объекта
    float objectWidth = getWidth(groups.begin()->vertices); // Замените на соответствующий размер объекта
    float objectHeight = getHeight(groups.begin()->vertices); // Замените на соответствующий размер объекта

    // Вычисляем масштаб для нормализации объекта
    float scaleX = windowWidth / objectWidth;
    float scaleY = windowHeight / objectHeight;
    float scale = std::min(scaleX, scaleY);

    // Вычисляем смещение для центрирования объекта в окне
    float offsetX = (windowWidth - objectWidth * scale) / 2.0f;
    float offsetY = (windowHeight - objectHeight * scale) / 2.0f;

    // Применяем масштабирование и смещение к матрице проекции
//    projectionMatrix.ortho(0, windowWidth, 0, windowHeight, -1, 1);
    projectionMatrix.scale(scale);
    projectionMatrix.translate(offsetX, offsetY);

    // Устанавливаем матрицу проекции для отображения
    shaderProgram.setUniformValue("projectionMatrix", projectionMatrix);
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
//  normalizeObject(projectionMatrix, groups, shaderProgram);

  QMatrix4x4 viewMatrix;
  viewMatrix.translate(trX, trY, trZ);
  viewMatrix.rotate(rotationX, 1.0f, 0.0f, 0.0f);
  viewMatrix.rotate(rotationY, 0.0f, 1.0f, 0.0f);
  viewMatrix.rotate(rotationZ, 0.0f, 0.0f, 1.0f);

  shaderProgram.setUniformValue("projectionMatrix", projectionMatrix);
  shaderProgram.setUniformValue("viewMatrix", viewMatrix);
  shaderProgram.setUniformValue("frontLightPosition", frontLightPosition);
  shaderProgram.setUniformValue("backLightPosition", backLightPosition);
  shaderProgram.setUniformValue("rotationCenter", rotationCenter);

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
          // Создание и заполнение буфера индексов
//          QOpenGLBuffer indexBuffer;
//          indexBuffer.create();
//          indexBuffer.bind();
//          indexBuffer.allocate(group.indices.data(), group.indices.size() * sizeof(GLuint));

          // Отрисовка с использованием буфера индексов
//          glDrawElements(GL_LINES, group.indices.size(), GL_UNSIGNED_INT, 0);

          // Освобождение буфера индексов
//          indexBuffer.release();
//          indexBuffer.destroy();
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
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;
    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.toStdString().c_str());

    if (!err.empty()) {
        return;
    }

    if (!success) {
        return;
    }

    Group currentGroup;
    // Заполнение временных векторов данными из TinyObjLoader
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            if (index.vertex_index >= 0) {
                float x = attrib.vertices[3 * index.vertex_index + 0];
                float y = attrib.vertices[3 * index.vertex_index + 1];
                float z = attrib.vertices[3 * index.vertex_index + 2];
                currentGroup.vertices.append(QVector3D(x, y, z));
            }

            if (index.normal_index >= 0) {
                float nx = attrib.normals[3 * index.normal_index + 0];
                float ny = attrib.normals[3 * index.normal_index + 1];
                float nz = attrib.normals[3 * index.normal_index + 2];
                currentGroup.normals.append(QVector3D(nx, ny, nz));
            }

            if (index.texcoord_index >= 0) {
                float u = attrib.texcoords[2 * index.texcoord_index + 0];
                float v = attrib.texcoords[2 * index.texcoord_index + 1];
                currentGroup.textureCoords.append(QVector2D(u, v));
            }
            // Добавление индексов вершин в currentGroup.indices
            currentGroup.indices.append(currentGroup.vertices.size() - 1);
        }
    }

    QVector3D center(0.0f, 0.0f, 0.0f);
    for (const QVector3D& vertex : currentGroup.vertices) {
        center += vertex;
    }
    center /= currentGroup.vertices.size();
    currentGroup.center = center;

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
    scaleFactor = 1;
  } else if (delta < 0) {
    // Уменьшаем масштаб при прокрутке назад
    scaleFactor = -1;
  }

  // Изменяем масштаб сцены
  changeSceneScale(scaleFactor);
}
