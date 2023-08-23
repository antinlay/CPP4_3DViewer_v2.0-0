#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "QtGui/qevent.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QFile>
#include <QtCore/QTextStream>
#include <QtCore/QVector>
#include <QVector3D>
#include <QVector2D>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {
        rotationX = 0.0f;
        rotationY = 0.0f;
        rotationZ = 0.0f;
    }

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        loadObjFile("/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/Obj/Dragon.obj");
        rotationCenter = QVector3D(0, 0, 0);
    }

    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        QOpenGLShaderProgram shaderProgram;
        shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
        shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
        shaderProgram.link();
        shaderProgram.bind();

        QMatrix4x4 projectionMatrix;
        projectionMatrix.perspective(45.0f, width() / height(), 0.1f, 100.0f);

        QMatrix4x4 viewMatrix;
        viewMatrix.translate(trX, trY, trZ);
        viewMatrix.rotate(rotationX, 1.0f, 0.0f, 0.0f);
        viewMatrix.rotate(rotationY, 0.0f, 1.0f, 0.0f);
        viewMatrix.rotate(rotationZ, 0.0f, 0.0f, 1.0f);

        shaderProgram.setUniformValue("projectionMatrix", projectionMatrix);
        shaderProgram.setUniformValue("viewMatrix", viewMatrix);

        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);
        shaderProgram.enableAttributeArray("vertexPosition");

        normalBuffer.bind();
        shaderProgram.setAttributeBuffer("vertexNormal", GL_FLOAT, 0, 3);
        shaderProgram.enableAttributeArray("vertexNormal");
        textureCoordBuffer.bind();
        shaderProgram.setAttributeBuffer("textureCoord", GL_FLOAT, 0, 2);
        shaderProgram.enableAttributeArray("textureCoord");

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        shaderProgram.disableAttributeArray("vertexPosition");
        shaderProgram.disableAttributeArray("vertexNormal");
        shaderProgram.disableAttributeArray("textureCoord");

        vertexBuffer.release();
        normalBuffer.release();
        textureCoordBuffer.release();
        shaderProgram.release();
    }

    void resizeGL(int w, int h) override
    {
        glViewport(0, 0, w, h);
    }

    void loadObjFile(const QString& filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QVector<QVector3D> tempVertices;
        QVector<QVector3D> tempNormals;
        QVector<QVector2D> tempTextureCoords;
        QVector<unsigned int> vertexIndices;
        QVector<unsigned int> normalIndices;
        QVector<unsigned int> textureCoordIndices;

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            QStringList tokens = line.split(' ');

            if (tokens[0] == "v")
            {
                float x = tokens[1].toFloat();
                float y = tokens[2].toFloat();
                float z = tokens[3].toFloat();
                tempVertices.append(QVector3D(x, y, z));
            }
            else if (tokens[0] == "vn")
            {
                float nx = tokens[1].toFloat();
                float ny = tokens[2].toFloat();
                float nz = tokens[3].toFloat();
                tempNormals.append(QVector3D(nx, ny, nz));
            }
            else if (tokens[0] == "vt")
            {
                float u = tokens[1].toFloat();
                float v = tokens[2].toFloat();
                tempTextureCoords.append(QVector2D(u, v));
            }
            else if (tokens[0] == "f")
            {
//                qDebug() << "Tokens: " << tokens.size() - 1 << " " << tokens[1];
                for (int i = 1; i <= tokens.size() - 1; ++i)
                {
                    QStringList faceTokens = tokens[i].split('/');

                    if (faceTokens.size() >= 1) {
                        vertexIndices.append(faceTokens[0].toUInt() - 1);
                    }
                    if (faceTokens.size() >= 2) {
                        textureCoordIndices.append(faceTokens[1].toUInt() - 1);
                    }
                    if (faceTokens.size() == 3) {
                        normalIndices.append(faceTokens[2].toUInt() - 1);
                    }
                }
            }
        }

        file.close();

        for (int i = 0; i < vertexIndices.size(); ++i)
        {
            if (!tempVertices.isEmpty()) {
                vertices.append(tempVertices[vertexIndices[i]]);
            }
            if (!tempTextureCoords.isEmpty()) {
                textureCoords.append(tempTextureCoords[textureCoordIndices[i]]);
            }
            if (!tempNormals.isEmpty()) {
                normals.append(tempNormals[normalIndices[i]]);
            }
        }

        initializeBuffers();
    }

    void initializeBuffers() {
        if (!vertices.isEmpty()) {
            qDebug() << "vertic: " << vertices.size();
            vertexBuffer.create();
            vertexBuffer.bind();
            vertexBuffer.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));
        }
        if (!normals.isEmpty()) {
            qDebug() << "normals: " << normals.size();
            normalBuffer.create();
            normalBuffer.bind();
            normalBuffer.allocate(normals.constData(), normals.size() * sizeof(QVector3D));
        }
        if (!textureCoords.isEmpty()) {
            qDebug() << "textureCoords: " << textureCoords.size();
            textureCoordBuffer.create();
            textureCoordBuffer.bind();
            textureCoordBuffer.allocate(textureCoords.constData(), textureCoords.size() * sizeof(QVector2D));
        }
    }

    void mousePressEvent(QMouseEvent *event) override {
        lastMousePosition = event->pos();
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        int dx = event->position().x() - lastMousePosition.x();
        int dy = event->position().y() - lastMousePosition.y();

        if (event->buttons() & Qt::LeftButton)
        {
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

    void changeSceneScale(qreal scaleFactor){
        // Изменяем масштаб в матрице вида
        trZ += scaleFactor;
//        qDebug() << trZ;
        // Перерисовываем виджет
        update();
    }

    void wheelEvent(QWheelEvent *event) override
    {
        // Получаем значение прокрутки колесика мыши
        int delta = event->angleDelta().y();

        // Определяем коэффициент масштабирования
        qreal scaleFactor = 1.0;
        if (delta > 0)
        {
            // Увеличиваем масштаб при прокрутке вперед
            scaleFactor = 0.1;
        }
        else if (delta < 0)
        {
            // Уменьшаем масштаб при прокрутке назад
            scaleFactor = -0.1;
        }

        // Изменяем масштаб сцены
        changeSceneScale(scaleFactor);
    }

private:
    float trX= 0.0f, trY = 0.0f, trZ = -3.0f;
//    QMatrix4x4 viewMatrix {};
    QVector3D rotationCenter {};
    QOpenGLBuffer vertexBuffer {};
    QOpenGLBuffer normalBuffer {};
    QOpenGLBuffer textureCoordBuffer {};

    QVector<QVector3D> vertices {};
    QVector<QVector3D> normals {};
    QVector<QVector2D> textureCoords {};

    QPoint lastMousePosition {};
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

#endif // GLWIDGET_H
