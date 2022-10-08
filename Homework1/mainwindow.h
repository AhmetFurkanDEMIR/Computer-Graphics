#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
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

   void slot_SomethingChecked1();
   void slot_SomethingChecked2();
   void slot_SomethingChecked3();
   void mousePressEvent(QMouseEvent*);

private:
    Ui::MainWindow *ui;

    void addCheckBox();
    QPointer<QAction> m_p_Act_Button1;
    QPointer<QAction> m_p_Act_Button2;
    QPointer<QAction> m_p_Act_Button3;
    QPointer<QAction> m_p_Clear_Button3;

    void __initScreen__();
    void drawDDA();
    void drawBresenham();
    void drawImprovedBresenham();

    int algorithm = 0;
    int flagClick=0;

    QRgb color;
    QImage canvas;

    QPoint first, end;

    QString description = "";


};
#endif // MAINWINDOW_H
