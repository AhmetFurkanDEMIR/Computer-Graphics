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

   // algorithm check button
   void slot_SomethingChecked1();
   void slot_SomethingChecked2();
   void slot_SomethingChecked3();
   void mousePressEvent(QMouseEvent*);

private:
    Ui::MainWindow *ui;

    // algorithm check button
    void addCheckBox();
    QPointer<QAction> m_p_Act_Button1;
    QPointer<QAction> m_p_Act_Button2;
    QPointer<QAction> m_p_Act_Button3;


    // screen
    void __initScreen__();
    void drawDDA();
    void drawBresenham();
    void symmetry(int x, int y);
    void symmetryBresenham(int x, int y);

    void fill(int x, int y, QRgb ground, QRgb new_color);

    int algorithm = 0;

    QRgb color;
    QImage canvas;

    QPoint mouse;

    QString description = "";


};
#endif // MAINWINDOW_H
