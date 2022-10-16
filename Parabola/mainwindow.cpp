#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // screen
    __initScreen__();

}

void MainWindow::__initScreen__(){

    this->setWindowTitle("Circle Drawing Algorithms - Symmetrical Drawing");

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

    mouses.append(event->pos());

    description = "(X: "+QString::number(event->pos().x())+", Y: "+QString::number(event->pos().y())+")";
    ui->statusbar->showMessage(description);


}



void MainWindow::on_pushButton_clicked()
{

    mouses.append(mouses[0]);

    for(int i=0; i<mouses.length()-1; i++){

        drawDDA(mouses[i], mouses[i+1]);

    }

    mouses.clear();

}

void MainWindow::drawDDA(QPoint first, QPoint end){

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
