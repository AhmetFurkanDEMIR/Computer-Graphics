#include <QRandomGenerator>
#include <QLabel>
#include <QtMath>

class EnemyCharacter{

public:

    // Dusman goblin Constructor
    EnemyCharacter(QLabel *label_temp, int wid){

        x = -50;
        an=1;

        label=label_temp;

        label->move(x,585);

        QPixmap pix(":/enemy/Images/Enemy/R1.png");

        label->setPixmap(pix);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        label->setGeometry(x,585,55,90);
        label->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));

        label->show();
    }


    void new_move(){

        QPixmap pix(":/enemy/Images/Enemy/R"+QString::number(an)+".png");

        label->setPixmap(pix);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        label->setGeometry(getLabel()->pos().x()+6,585,100,100);
        label->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));


        if(an==8){
            an=1;
        }

        an+=1;

    }

    // dusman disari ciktimi kontrolu
    static int check(EnemyCharacter *bal, int H){

        if(bal->label->pos().x()>H+4){
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


    int x;
    int an;
    QLabel *label;
    QList<QPoint> mouses;

};
