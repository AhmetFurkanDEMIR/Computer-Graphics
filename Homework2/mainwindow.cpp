#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // algorithm check button
    addCheckBox();

    // screen
    __initScreen__();

}

void MainWindow::slot_SomethingChecked1(){

    if(!m_p_Act_Button1 || !m_p_Act_Button2) {return;}


    m_p_Act_Button1->setChecked(true);
    m_p_Act_Button2->setChecked(false);

    algorithm = 0;
    color=qRgb(0,255,0);

    this->setWindowTitle("Circle Drawing Algorithms - Symmetrical Drawing");


}

void MainWindow::slot_SomethingChecked2(){

    if(!m_p_Act_Button1 || !m_p_Act_Button2) {return;}

    m_p_Act_Button2->setChecked(true);
    m_p_Act_Button1->setChecked(false);

    algorithm = 1;
    color=qRgb(255,0,0);

    this->setWindowTitle("Circle Drawing Algorithms - Bresenham");

}

void MainWindow::slot_SomethingChecked3()
{

    m_p_Act_Button3->setChecked(false);

    fill(mouse.x(),mouse.y(), color, color);

}


void MainWindow::addCheckBox(){

    m_p_Act_Button1 = new QAction("Symmetrical Drawing", this);
    m_p_Act_Button1->setCheckable(true);
    m_p_Act_Button1->setChecked(true);
    connect(m_p_Act_Button1, SIGNAL(triggered()), this, SLOT(slot_SomethingChecked1()));

    m_p_Act_Button2 = new QAction("Bresenham", this);
    m_p_Act_Button2->setCheckable(true);
    m_p_Act_Button2->setChecked(false);
    connect(m_p_Act_Button2, SIGNAL(triggered()), this, SLOT(slot_SomethingChecked2()));

    m_p_Act_Button3 = new QAction("Fill", this);
    m_p_Act_Button3->setCheckable(true);
    m_p_Act_Button3->setChecked(false);
    connect(m_p_Act_Button3, SIGNAL(triggered()), this, SLOT(slot_SomethingChecked3()));


    QMenu *p_menu = menuBar()->addMenu("Algorithms");
    p_menu->addAction(m_p_Act_Button1);
    p_menu->addAction(m_p_Act_Button2);
    p_menu->addAction(m_p_Act_Button3);
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

    mouse=event->pos();
    mouse.setY(mouse.y()-22);

    description = "(X: "+QString::number(mouse.x())+", Y: "+QString::number(mouse.y())+")";
    ui->statusbar->showMessage(description);

    if (algorithm==0){
        drawDDA();
    }

    else{
        drawBresenham();
    }

}

void MainWindow::symmetry(int x, int y){

    double mouseX = mouse.x();
    double mouseY = mouse.y();

    canvas.setPixel(mouseX+x,mouseY+y,color);
    canvas.setPixel(mouseX+x,mouseY-y,color);
    canvas.setPixel(mouseX+y,mouseY+x,color);
    canvas.setPixel(mouseX-y,mouseY+x,color);
    canvas.setPixel(mouseX-x,mouseY+y,color);
    canvas.setPixel(mouseX-x,mouseY-y,color);
    canvas.setPixel(mouseX-y,mouseY-x,color);
    canvas.setPixel(mouseX+y,mouseY-x,color);

}

void MainWindow::drawDDA(){

    int R = ui->lineEdit->text().toInt();

    double x,y;
    double pain = 0.0;

    while(pain<45){

        QApplication::processEvents();

        x = cos(qDegreesToRadians(pain))*R;
        y = sin(qDegreesToRadians(pain))*R;

        symmetry(x,y);
        pain+=0.1;

        ui->label->setPixmap(QPixmap::fromImage(canvas));

    }

}

void MainWindow::symmetryBresenham(int x, int y){

    double mouseX = mouse.x();
    double mouseY = mouse.y();

    canvas.setPixel(mouseX+x,mouseY+y,color);
    canvas.setPixel(mouseX-x,mouseY+y,color);
    canvas.setPixel(mouseX+x,mouseY-y,color);
    canvas.setPixel(mouseX-x,mouseY-y,color);
    canvas.setPixel(mouseX+y,mouseY+x,color);
    canvas.setPixel(mouseX-y,mouseY+x,color);
    canvas.setPixel(mouseX+y,mouseY-x,color);
    canvas.setPixel(mouseX-y,mouseY-x,color);

}

void MainWindow::drawBresenham(){

    int R = ui->lineEdit->text().toInt();
    int x=R, y=0, error=0;
    int error1, error2;

    while(x>y){

        QApplication::processEvents();

        symmetryBresenham(x,y);

        error1 = error + (2 * y + 1);
        error2 = error + (2 * y +1) - (2 * x - 1);

        if(fabs(error1)<fabs(error2)){
            y++;
            error=error1;
        }

        else{
            x--;
            y++;
            error=error2;
        }


        ui->label->setPixmap(QPixmap::fromImage(canvas));
    }

}

void MainWindow::fill(int x, int y, QRgb ground, QRgb new_color){

    QApplication::processEvents();

    if(canvas.pixel(x,y)!=ground){

        canvas.setPixel(x,y,new_color);
        ui->label->setPixmap(QPixmap::fromImage(canvas));

        fill(x+1, y, ground, new_color);
        fill(x-1, y, ground, new_color);
        fill(x, y+1, ground, new_color);
        fill(x, y-1, ground, new_color);
    }

}

