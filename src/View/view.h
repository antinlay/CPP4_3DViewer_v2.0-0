#ifndef VIEW_H
#define VIEW_H

#include <QFileDialog>

#include "./ui_view.h"
#include "glwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow {
  Q_OBJECT

 private slots:
  void openFile();

 public:
  View(QWidget *parent = nullptr);
  ~View();

 private:
  Ui::View *ui;
  GLWidget *gl;
};
#endif  // VIEW_H
