#include <QRandomGenerator>
#include <QLabel>
#include <QtMath>


class Fire{

public:

    // karakterin ates Constructor
    // yeni konum doğru cizme algoritmasi yani DDA ile bulunur
    Fire(QLabel *label_temp, int x, int y, QPoint mouseE){

        mouse = mouseE;

        label=label_temp;

        label->setFixedWidth(17);
        label->setFixedHeight(17);

        label->move(x,y);

        color=qRgb(255,214,80);

        environment.load(":/gameBackground/Images/GameBackground/Fire.png");

        // ates yani cember cizimi
        drawBresenhamCember();
        fill(8,8, color, color);

        label->setPixmap(QPixmap::fromImage(environment));

        label->show();

        dx = mouse.x() - x;
        dy = mouse.y() - y;

        if(fabs(dx)>fabs(dy)){
            step=fabs(dx);
        }

        else{
            step=fabs(dy);
        }

        Xincrement=dx/step;
        Yincrement=dy/step;

        xx=x;
        yy=y;
        i=0;

    }


    void symmetry(int xx, int y){

        double mouseX = 8;
        double mouseY = 8;


        environment.setPixel(mouseX+xx,mouseY+y,color);
        environment.setPixel(mouseX-xx,mouseY+y,color);
        environment.setPixel(mouseX+xx,mouseY-y,color);
        environment.setPixel(mouseX-xx,mouseY-y,color);
        environment.setPixel(mouseX+y,mouseY+xx,color);
        environment.setPixel(mouseX-y,mouseY+xx,color);
        environment.setPixel(mouseX+y,mouseY-xx,color);
        environment.setPixel(mouseX-y,mouseY-xx,color);


    }

    void drawBresenhamCember(){

        int R = 8;
        int x=R, y=0, error=0;
        int error1, error2;

        while(x>y){

            symmetry(x,y);

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

        }
    }

    void fill(int x, int y, QRgb ground, QRgb new_color){


        if(environment.pixel(x,y)!=ground){

            environment.setPixel(x,y,new_color);

            fill(x+1, y, ground, new_color);
            fill(x-1, y, ground, new_color);
            fill(x, y+1, ground, new_color);
            fill(x, y-1, ground, new_color);
        }

    }

    void new_move(){

        xx+=Xincrement;
        yy+=Yincrement;

        getLabel()->move(xx, yy);

    }

    // ates disari ciktimi kontrolu
    static int check(int x, int y){

        if(x>1366 || x<0){
            return 1;
        }

        else if(y>768 || y<0){
            return 1;
        }

        else{
            return 0;
        }

    }

    QLabel * getLabel(){
        return label;
    }

private:


    QLabel *label;
    QImage environment;
    QRgb color;
    QPoint mouse;


    double dx;
    double dy;

    double Xincrement, Yincrement;
    double step;
    double xx,yy;

    int i;

};
