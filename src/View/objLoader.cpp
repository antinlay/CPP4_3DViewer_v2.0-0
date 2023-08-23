#include "objLoader.h"
#include "OpenGL/gl.h"
#include "QtCore/qdebug.h"

GLuint ObjLoader::load(const QString &filename) {
    QVector<QVector3D>().swap(vertex);
    QVector<QVector2D>().swap(uvs);
    QVector<QVector3D>().swap(normals);
    QVector<Facet>().swap(facets);

    QFile objFile(filename);

    if (!objFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "File not open";
    }

    QTextStream in(&objFile);

    while(!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList tokens = line.split(' ');

        if (tokens.isEmpty() || tokens.size() < 3 || tokens.size() > 4) {
            continue;
        } else {
            if (tokens[0] == "v") {
                vertex.push_back(QVector3D(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat()));
            } else if (tokens[0] == "vn") {
                normals.push_back(QVector3D(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat()));
            } else if (tokens[0] == "vt") {
                uvs.push_back(QVector2D(tokens[1].toFloat(), tokens[2].toFloat()));
            } else if (tokens[0] == "f") {
                Facet::Vertex v1, v2, v3;

               QStringList vi1 = tokens[1].split('/');
               QStringList vi2 = tokens[2].split('/');
               QStringList vi3 = tokens[3].split('/');

               v1.iV = vi1[0].toUInt();
               v2.iV = vi2[0].toUInt();
               v3.iV = vi3[0].toUInt();

               v1.iVt = vi1[1].toUInt();
               v2.iVt = vi2[1].toUInt();
               v3.iVt = vi3[1].toUInt();

               v1.iVn = vi1[2].toUInt();
               v2.iVn = vi2[2].toUInt();
               v3.iVn = vi3[2].toUInt();

               facets.push_back(Facet(v1,v2,v3));
            }
        }
    }
    GLuint num = draw();
    return num;
}

GLuint ObjLoader::draw(GLfloat offset) {
    GLuint num = glGenLists(1);
    glNewList(num, GL_COMPILE);

    for (int i = 0; i < facets.size(); ++i) {
        Facet f = facets[i];

        glBegin(GL_TRIANGLES);

        QVector3D normal = normals[f.v[0].iVn - 1];
        glNormal3f(normal.x(), normal.y(), normal.z());

        glTexCoord2f(uvs[f.v[0].iVt - 1].x() + offset, uvs[f.v[0].iVt - 1].y() + offset);
        glVertex3f(vertex[f.v[0].iV - 1].x(), vertex[f.v[0].iV - 1].y(), vertex[f.v[0].iV - 1].z());

        normal = normals[f.v[1].iVn - 1];
        glNormal3f(normal.x(), normal.y(), normal.z());

        glTexCoord2f(uvs[f.v[1].iVt - 1].x() + offset, uvs[f.v[1].iVt - 1].y() + offset);
        glVertex3f(vertex[f.v[1].iV - 1].x(), vertex[f.v[1].iV - 1].y(), vertex[f.v[1].iV - 1].z());

        normal = normals[f.v[2].iVn - 1];
        glNormal3f(normal.x(), normal.y(), normal.z());

        glTexCoord2f(uvs[f.v[2].iVt - 1].x() + offset, uvs[f.v[2].iVt - 1].y() + offset);
        glVertex3f(vertex[f.v[2].iV - 1].x(), vertex[f.v[2].iV - 1].y(), vertex[f.v[2].iV - 1].z());

        glEnd();
    }

    glEndList();
    return num;
}
