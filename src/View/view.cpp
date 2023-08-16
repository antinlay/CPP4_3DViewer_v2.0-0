#include "view.h"

View::View(QWidget *parent, QOpenGLWidget *gl)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
}

View::~View()
{
    delete ui;
}

