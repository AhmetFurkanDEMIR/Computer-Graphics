#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QtMath>
#include <QMouseEvent>
#include <math.h>


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

   void mousePressEvent(QMouseEvent*);
   void on_pushButton_clicked();

   void on_actionFill_triggered();

private:
    Ui::MainWindow *ui;

    // screen
    void __initScreen__();
    void drawDDA(QPoint first, QPoint end);
    void fill(int x, int y, QRgb ground, QRgb new_color);

    QRgb color;
    QImage canvas;

    QList<QPoint> mouses;
    QPoint mouse;

    QString description = "";


};
#endif // MAINWINDOW_H
