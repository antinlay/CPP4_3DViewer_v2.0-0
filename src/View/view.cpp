#include "view.h"

View::View(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
    connect(ui->actionOpen, &QAction::triggered, this, &View::openFile);
    connect(ui->openButton, &QPushButton::clicked, this, &View::openFile);
    // Установка стиля для QMainWindow с использованием SVG файла в качестве фона
//    QString styleSheet = "QWidget { background-image: url(/Users/janiecee/Documents/CPP4_3DViewer_v2.0-0/src/imgMenu.png); }";
//    ui->centralwidget->setStyleSheet(styleSheet);
}

View::~View()
{
  delete ui;
}

void View::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Choose file", QDir::fromNativeSeparators("../../../../Obj"), "Oblect files (*.obj)");
    if (!fileName.isEmpty()) {
      qDebug() << fileName;
//      gl->loadObjFile(fileName);
    }
}
