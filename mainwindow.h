#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "lenswindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setLensWindow(LensWindow *input);
    void initializeValues();

private slots:
    void on_enterPushButton_clicked();

private:
    Ui::MainWindow *ui;
    LensWindow *lensWindow;
};

#endif // MAINWINDOW_H
