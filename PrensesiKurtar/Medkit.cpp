#include <QRandomGenerator>
#include <QLabel>
#include <QtMath>

class Medkit{

public:

    // Dusman nesne Constructor
    Medkit(QLabel *label_temp, int wid){

        flagA = 0;
        sx=1.0;
        sy=1.0;
        aci=0.0;
        x= 10 + (QRandomGenerator::global()->generate() % (wid-68));
        label=label_temp;

        label->setFixedWidth(60);
        label->setFixedHeight(60);

        label->move(x,50);

        color=qRgb(0,0,0);

        environment.load(":/gameBackground/Images/GameBackground/objBack.png");

        // saglik cantasi cizme fonksiyonu (Poligon kullanildi)
        drawMetkit();

        label->setPixmap(QPixmap::fromImage(environment));

        label->show();
    }

    void drawDDA_Line(QPoint first, QPoint end){

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

            x+=Xincrement;
            y+=Yincrement;
            environment.setPixel(round(x),round(y),color);
            environment.setPixel(round(x)+1,round(y)+1,color);
            environment.setPixel(round(x)+1,round(y)+1,color);

        }

    }

    void drawMetkit(){

        mouses.append(QPoint(5,15));
        mouses.append(QPoint(55,15));

        mouses.append(QPoint(55,55));
        mouses.append(QPoint(5,55));
        mouses.append(QPoint(5,15));

        for(int i=0; i<mouses.length()-1; i++){

            drawDDA_Line(mouses[i], mouses[i+1]);

        }

        mouses.clear();
        color=qRgb(255,0,0);

        mouses.append(QPoint(10,35));
        mouses.append(QPoint(50,35));


        for(int i=0; i<mouses.length()-1; i++){

            drawDDA_Line(mouses[i], mouses[i+1]);

        }

        mouses.clear();

        mouses.append(QPoint(30,20));
        mouses.append(QPoint(30,50));


        for(int i=0; i<mouses.length()-1; i++){

            drawDDA_Line(mouses[i], mouses[i+1]);

        }

        mouses.clear();
        color=qRgb(0,0,0);

        mouses.append(QPoint(25,15));
        mouses.append(QPoint(25,10));

        mouses.append(QPoint(35,10));
        mouses.append(QPoint(35,15));


        for(int i=0; i<mouses.length()-1; i++){

            drawDDA_Line(mouses[i], mouses[i+1]);

        }
        mouses.clear();



    }

    void olcekle(){

        if(flagA==0){

            if(sx<=0.6){
                flagA=1;
            }

            sx-=0.1;
            sy-=0.1;

        }
        else{

            if(sx>=0.9){
                flagA=0;
            }

            sx+=0.1;
            sy+=0.1;

        }

        QPixmap imageCanva(":/gameBackground/Images/GameBackground/objBack.png");
        QImage canva=imageCanva.toImage();

        int x,y;

        for(int i=0; i<60; i++){

            for(int j=0; j<60; j++){

                x=i*sx;
                y=j*sy;

                    canva.setPixel(x, y, environment.pixel(i,j));

            }


        }



        getLabel()->setPixmap(QPixmap::fromImage(canva));

    }


    // nesne konumunu asagiya kaydirma
    void new_move(){

        getLabel()->move(x, getLabel()->pos().y()+6);
        olcekle();

    }

    // nesne disari ciktimi kontrolu
    static int check(Medkit *bal, int H){

        if(bal->getLabel()->pos().y()>H+4){
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

    // nesne konumu
    int x;
    double aci;
    double sx;
    double sy;
    int flagA;

    QLabel *label;
    QImage environment;
    QRgb color;
    QList<QPoint> mouses;

};
