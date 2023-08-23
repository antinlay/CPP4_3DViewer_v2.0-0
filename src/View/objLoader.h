#ifndef OBJLOADER_H
#define OBJLOADER_H
#define GL_SILENCE_DEPRECATION

#include "OpenGL/gltypes.h"
#include <QFile>
#include <QVector3D>


class ObjLoader
{
public:
    struct Facet {
        struct Vertex {
            GLuint iV;
            GLuint iVt;
            GLuint iVn;
        };

        Vertex v[3];

        Facet(Vertex v1, Vertex v2, Vertex v3) {
            v[0] = v1;
            v[1] = v2;
            v[2] = v3;
         }
    };

//    QVector<QString> coord;
    QVector<QVector3D> vertex;
    QVector<QVector2D> uvs;
    QVector<QVector3D> normals;
    QVector<Facet> facets;

public:
    // single tone
    ObjLoader();
    ObjLoader(const ObjLoader &) = delete;
    ObjLoader(const ObjLoader &&) = delete;
    ObjLoader& operator=(const ObjLoader &) = delete;
    ObjLoader& operator=(const ObjLoader &&) = delete;

   public:
    static ObjLoader& Instance() {
        static ObjLoader singleInstance;
        return singleInstance;
    }
    GLuint load(const QString &filename);
    GLuint draw(GLfloat offset = 0);
};

#endif // OBJLOADER_H
