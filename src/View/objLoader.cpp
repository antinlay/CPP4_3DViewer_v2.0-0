#include "objLoader.h"
#include "QtCore/qdebug.h"

GLuint ObjLoader::load(const QString &filename) {
    QVector<QString>().swap(coord);
    QVector<QVector3D>().swap(vertex);
    QVector<QVector2D>().swap(uvs);
    QVector<QVector3D>().swap(normals);
    QVector<Facet>().swap(facets);

    QFile in(filename);

    if (!in.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "File not open";
    }

    char buff[256];

    while(!in.atEnd()) {
        in.readLine(buff, 256);
        if (buff[0] == 'v') {
            coord.push_back(buff);
        }
    }

    for(int i = 0; i < coord.size(); ++i) {
//        if (coord[i][0])
    }
    return 0;
}
