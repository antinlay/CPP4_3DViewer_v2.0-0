#ifndef VIEW_H
#define VIEW_H

#include "./ui_view.h"
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow {
    Q_OBJECT

signals:
    void uiEventSendFilePath(QString &filePath);

public:
    View(QWidget *parent = nullptr);
    ~View();
    void openFile();

private:
    Ui::View *ui;
};
#endif // VIEW_H
