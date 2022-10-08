#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    addCheckBox();

    __initScreen__();

}

void MainWindow::slot_SomethingChecked1(){

    if(!m_p_Act_Button1 || !m_p_Act_Button2) {return;}


    m_p_Act_Button1->setChecked(true);
    m_p_Act_Button2->setChecked(false);
    m_p_Act_Button3->setChecked(false);

    algorithm = 0;
    flagClick = 0;
    color=qRgb(0,255,0);

    this->setWindowTitle("Line Drawing Algorithms - DDA");


}

void MainWindow::slot_SomethingChecked2(){

    if(!m_p_Act_Button1 || !m_p_Act_Button2) {return;}

    m_p_Act_Button2->setChecked(true);
    m_p_Act_Button1->setChecked(false);
    m_p_Act_Button3->setChecked(false);

    algorithm = 1;
    flagClick = 0;
    color=qRgb(255,0,0);

    this->setWindowTitle("Line Drawing Algorithms - Bresenham");

}

void MainWindow::slot_SomethingChecked3(){

    if(!m_p_Act_Button1 || !m_p_Act_Button2) {return;}

    m_p_Act_Button3->setChecked(true);
    m_p_Act_Button1->setChecked(false);
    m_p_Act_Button2->setChecked(false);

    algorithm = 2;
    flagClick = 0;
    color=qRgb(0,0,255);

    this->setWindowTitle("Line Drawing Algorithms - Improved Bresenham");

}

void MainWindow::addCheckBox(){

    m_p_Act_Button1 = new QAction("DDA", this);
    m_p_Act_Button1->setCheckable(true);
    m_p_Act_Button1->setChecked(true);
    connect(m_p_Act_Button1, SIGNAL(triggered()), this, SLOT(slot_SomethingChecked1()));

    m_p_Act_Button2 = new QAction("Bresenham", this);
    m_p_Act_Button2->setCheckable(true);
    m_p_Act_Button2->setChecked(false);
    connect(m_p_Act_Button2, SIGNAL(triggered()), this, SLOT(slot_SomethingChecked2()));


    m_p_Act_Button3 = new QAction("Improved Bresenham", this);
    m_p_Act_Button3->setCheckable(true);
    m_p_Act_Button3->setChecked(false);
    connect(m_p_Act_Button3, SIGNAL(triggered()), this, SLOT(slot_SomethingChecked3()));

    QMenu *p_menu = menuBar()->addMenu("Algorithms");
    p_menu->addAction(m_p_Act_Button1);
    p_menu->addAction(m_p_Act_Button2);
    p_menu->addAction(m_p_Act_Button3);


}


void MainWindow::__initScreen__(){

    this->setWindowTitle("Line Drawing Algorithms - DDA");

    QImage environment(1202,601,QImage::Format_ARGB32);
    ui->label->setFrameShape(QFrame::Box);
    ui->label->setPixmap(QPixmap::fromImage(environment));

    color=qRgb(0,255,0);
    canvas = environment;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(flagClick==0)
    {
        first=event->pos();
        first.setY(first.y()-22);
        flagClick++;

        description = "First (X: "+QString::number(first.x())+", Y: "+QString::number(first.y())+")";
        ui->statusbar->showMessage(description);
    }
    else
    {
        end=event->pos();
        end.setY(end.y()-22);

        description = description + ", End (X: "+QString::number(end.x())+", Y: "+QString::number(end.y())+")";
        ui->statusbar->showMessage(description);

        if (algorithm==0){
            drawDDA();
        }

        else if(algorithm==1){
            drawBresenham();
        }

        else{
            drawImprovedBresenham();
        }

        flagClick=0;
    }

}

void MainWindow::drawDDA(){

    double dx = end.x() - first.x();
    double dy = end.y() - first.y();

    double Xincrement, Yincrement;
    double step;
    double x,y;

    if(fabs(dx)>fabs(dy)){
        step=fabs(dx);
    }

    else{
        step=fabs(dy);
    }

    Xincrement=dx/step;
    Yincrement=dy/step;

    x=first.x();
    y=first.y();

    for(int i=0; i<step; i++)
    {

        QApplication::processEvents();
        x+=Xincrement;
        y+=Yincrement;
        canvas.setPixel(round(x),round(y),color);

        ui->label->setPixmap(QPixmap::fromImage(canvas));
    }

}

void MainWindow::drawBresenham(){


    double dx = fabs(end.x() - first.x());
    double dy = fabs(end.y() - first.y());

    double slope = dy/dx;
    double error = 0;

    int x = first.x(), y = first.y();

    for (; x<end.x(); x++){

        QApplication::processEvents();

        canvas.setPixel(round(x),round(y),color);

        error+=slope;
        if(error>=0.5){
            y++;
            error--;
        }

        ui->label->setPixmap(QPixmap::fromImage(canvas));
    }

}


void MainWindow::drawImprovedBresenham(){

    int dx = fabs(end.x() - first.x()), sx = first.x() < end.x() ? 1 : -1;
    int dy = fabs(end.y() - first.y()), sy = first.y() < end.y() ? 1 : -1;

    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    int x, y;
    x = first.x();
    y = first.y();

    while(true){

        QApplication::processEvents();

        canvas.setPixel(round(x),round(y),color);

        if (x == end.x() && y == end.y()){
          break;
        }

        e2 = err;

        if (e2 > -dx) {
            err -= dy;
            x += sx;

        }
        if (e2 < dy) {
            err += dx;
            y += sy;
        }

        ui->label->setPixmap(QPixmap::fromImage(canvas));
    }

}

