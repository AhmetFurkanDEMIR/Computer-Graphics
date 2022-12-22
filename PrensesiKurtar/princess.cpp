#include "princess.h"

Princess::Princess(QLabel *labelM)
{

    QPixmap pixa(":/gameCharacter/Images/Character/prenses.png");
    imageC= pixa.toImage();

    environment.load(":/gameBackground/Images/GameBackground/dead.png");

    labelM->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    labelM->setGeometry(100,100,90,90);
    labelM->setPixmap(QPixmap::fromImage(environment));
    labelM->setGeometry(1285,585,100,100);
    labelMM=labelM;

}

void Princess::process(){

    prensesOtele();


    emit finished();
}

// oteleme algoritmasi
void Princess::prensesOtele(){

    for(int i=100; i>0; i--){

        int xs = i;
        int ys = i;

        for(int i=0; i<100; i++){

            for(int j=0; j<100; j++){

                environment.setPixel(i+xs, j+ys, imageC.pixel(i,j));

            }

        }

        labelMM->setPixmap(QPixmap::fromImage(environment).scaled(100,100,Qt::KeepAspectRatio));

    }

}

void Princess::finished(){

}

QLabel * Princess::getLabel(){
    return labelMM;
}
