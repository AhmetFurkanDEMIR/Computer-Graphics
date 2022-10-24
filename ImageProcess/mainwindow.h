#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtMath>
#include <math.h>
#include <QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSelect_File_triggered();

    void on_actionImage_rotation_triggered();

    void on_actionScaling_triggered();

    void on_actionImage_Panning_triggered();

    void on_actionClear_Screen_triggered();

private:
    Ui::MainWindow *ui;
    QImage canvas;
    QImage imageC;
};
#endif // MAINWINDOW_H
