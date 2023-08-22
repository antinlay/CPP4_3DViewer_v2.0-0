#include "objLoader.h"
#include "QtCore/qdebug.h"

GLuint ObjLoader::load(const QString &filename) {
    QVector<QString>().swap(coord);
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

        if (tokens.isEmpty() || tokens.size() < 4) {continue;}

        if (tokens[0] == "v") {

        }
    }

    for(int i = 0; i < coord.size(); ++i) {
//        if (coord[i][0])
    }
    return 0;
}
