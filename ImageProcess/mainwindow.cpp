#include "inputdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSelect_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));

    QPixmap image(fileName);

    ui->label->setPixmap(image);
    ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QImage environment(ui->label->width(), ui->label->height(),QImage::Format_ARGB32);
    canvas=environment;

    imageC = image.toImage();


}


void MainWindow::on_actionImage_rotation_triggered()
{

    double angle;
    bool ok;
    int d = QInputDialog::getInt(this, tr("QInputDialog::getInt()"),
                                     tr("Angle:"), 0, 0, 360, 1, &ok);
    if (ok){

        angle = d;

    }

    else{
        return;
    }

    double x,y;

    for(int i=0; i<imageC.width(); i++){

        for(int j=0; j<imageC.height(); j++){

            x = i*cos(qDegreesToRadians(angle)) - j*sin(qDegreesToRadians(angle));
            y = i*sin(qDegreesToRadians(angle)) + j*cos(qDegreesToRadians(angle));

            canvas.setPixel(round(x)+imageC.width()+(ui->label_2->width())/8, round(y)+imageC.height()+(ui->label_2->height())/8, imageC.pixel(i,j));

        }

    }

    ui->label_2->setPixmap(QPixmap::fromImage(canvas));
    ui->label_2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);


}

void MainWindow::on_actionScaling_triggered()
{

    int value;

    bool ok;
    double d = QInputDialog::getDouble(this, tr("QInputDialog::getDouble()"),
                                           tr("Amount:"), 0, -2, 2, 1, &ok,
                                           Qt::WindowFlags(), 1);

    if (ok){

        value = (int)d;

    }

    else{
        return;
    }

    double x,y, sx,sy;

    if(value==-2){
        sx=0.2;
        sy=0.2;
    }

    else if(value==-1){

        sx=0.5;
        sy=0.5;

    }

    else if(value==0){
        sx=1;
        sy=1;
    }

    else if(value==1){
        sx=1.5;
        sy=1.5;
    }

    else if(value==2){
        sx=1.8;
        sy=1.8;
    }


    for(int i=0; i<imageC.width(); i++){

        for(int j=0; j<imageC.height(); j++){

            x=i*sx;
            y=j*sy;

            canvas.setPixel(x+(ui->label_2->width())/5, y+(ui->label_2->height())/5, imageC.pixel(i,j));

        }

    }

    ui->label_2->setPixmap(QPixmap::fromImage(canvas));
    ui->label_2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

}

void MainWindow::on_actionImage_Panning_triggered()
{

    bool ok;
    int xs, ys;

    QStringList list = InputDialog::getStrings(this, &ok);
    if (ok) {

        xs = list[0].toInt();
        ys = list[1].toInt();

    }

    else{
        return;
    }


    for(int i=0; i<imageC.width(); i++){

        for(int j=0; j<imageC.height(); j++){

            canvas.setPixel(i+xs, j+ys, imageC.pixel(i,j));

        }
    }

    ui->label_2->setPixmap(QPixmap::fromImage(canvas));
    ui->label_2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

}

void MainWindow::on_actionClear_Screen_triggered()
{

    QImage environment(ui->label_2->width(), ui->label_2->height(),QImage::Format_ARGB32);

    ui->label_2->setPixmap(QPixmap::fromImage(environment));
    canvas=environment;

}
