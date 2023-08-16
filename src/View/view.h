#ifndef VIEW_H
#define VIEW_H

#include "./ui_view.h"
//#include <QMainWindow>
//#include <QtOpenGLWidgets/QOpenGLWidget>
//#include <QtOpenGL/QOpenGLBuffer>
//#include <QOpenGLFunctions>
//#include <QtOpenGL/QOpenGLShaderProgram>
//#include <QtOpenGL/QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr, QOpenGLWidget *gl = nullptr);
    ~View();

private:
    Ui::View *ui;
};
#endif // VIEW_H
