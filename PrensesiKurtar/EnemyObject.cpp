#include <QRandomGenerator>
#include <QLabel>
#include <QtMath>

class EnemyObject{

public:

    // Dusman nesne Constructor
    EnemyObject(QLabel *label_temp, int wid){

        x= 9 + (QRandomGenerator::global()->generate() % (wid-68));
        label=label_temp;

        label->setFixedWidth(60);
        label->setFixedHeight(60);

        label->move(x,50);

        //int a=QRandomGenerator::global()->generate()%6;

        if(QRandomGenerator::global()->generate()%2==0){
            color=qRgb(255,214,80);
        }

        else{
            color=qRgb(255,93,52);
        }


        environment.load(":/gameBackground/Images/GameBackground/objBack.png");

        // ucgen cizme fonksiyonu (Poligon kullanildi)
        triangle();

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

        }

    }

    void triangle(){

        mouses.append(QPoint(5,5));
        mouses.append(QPoint(55,5));
        mouses.append(QPoint(30,30));
        mouses.append(QPoint(5,5));

        for(int i=0; i<mouses.length()-1; i++){

            drawDDA_Line(mouses[i], mouses[i+1]);

        }

        fill(30,16, color, color);

        mouses.clear();

        mouses.append(QPoint(5,26));
        mouses.append(QPoint(55,26));
        mouses.append(QPoint(30,50));
        mouses.append(QPoint(5,26));

        for(int i=0; i<mouses.length()-1; i++){

            drawDDA_Line(mouses[i], mouses[i+1]);

        }

        fill(30,45, color, color);

        mouses.clear();

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

    // nesne konumunu asagiya kaydirma
    void new_move(){

        getLabel()->move(x, getLabel()->pos().y()+6);

    }

    // nesne disari ciktimi kontrolu
    static int check(EnemyObject *bal, int H){

        if(bal->label->pos().y()>H+4){
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

    QLabel *label;
    QImage environment;
    QRgb color;
    QList<QPoint> mouses;

};
