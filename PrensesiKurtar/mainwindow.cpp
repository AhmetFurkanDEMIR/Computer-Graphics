#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // oyun ekranı, ana karakter ve prenses
    setScreen();

    // dusmanlar ve bunların hareketleri
    init__game();
}

MainWindow::~MainWindow()
{
    delete ui;

}

// oyun ekranı, ana karakter ve prenses
void MainWindow::setScreen(){

    puan = 0;
    healthy = 0;
    R = 1;
    L = 1;
    Location=670;
    time=120;

    medkitSpawnTime = 10 + QRandomGenerator::global()->generate() % 100;
    //medkitSpawnTime = 119;

    // ana karakter
    MainCharacter = new QLabel(this);
    QPixmap pix(":/gameCharacter/Images/Character/standing.png");
    MainCharacter->setPixmap(pix);
    MainCharacter->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    MainCharacter->setGeometry(Location,585,100,100);
    MainCharacter->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));
    MainCharacter->show();

    // prensesin oteleme ile ekrana girisi (prensesin giris animasyonu)
    thread = new QThread();
    QLabel *PrincessLabel = new QLabel(this);
    pr = new Princess(PrincessLabel);
    pr->moveToThread(thread);
    connect(thread, &QThread::started, pr, &Princess::process);
    //connect(pr, &Princess::finished, thread, &QObject::deleteLater);
    thread->start();

    // oyun bilgileri
    ui->message->setStyleSheet("QLabel { background-color : #2296af; color : green; }");
    color=qRgb(0,0,0);
    ui->message->setText("  Oyun Mesajı:Kendinizi ve Prensesi düşman nesnelerden koruyunuz;          Oyunun bitmesine "+QString::number(time)+" sn kaldı;               Can:"+QString::number(healthy)+";               Puanınız:"+QString::number(puan));

    QPixmap pixC(":/gameBackground/Images/GameBackground/0.png");
    imgCanv = pixC.toImage();


}

// dusmanlar ve bunların hareketleri
void MainWindow::init__game(){

        nesne_spawn_time=2000;
        dusman_spawn_time=3500;
        hiz_time = 100;

        // otomatik poligon cikartma
        timer_ObjSpawn = new QTimer(this);
        connect(timer_ObjSpawn, SIGNAL(timeout()), this, SLOT(spawn_obj()));
        timer_ObjSpawn->start(nesne_spawn_time);

        // poligon asagi kaydirma signal&slot
        // poligon ekrandan cikma kontrolu
        timer_label_obj = new QTimer(this);
        connect(timer_label_obj, SIGNAL(timeout()), SLOT(move_updateObject()));
        timer_label_obj->start(hiz_time);


        // ------------ //

        // otomatik dusman cikartma
        timer_time_enemy = new QTimer(this);
        connect(timer_time_enemy, SIGNAL(timeout()), this, SLOT(spawn_enemy()));
        timer_time_enemy->start(dusman_spawn_time);

        // dusman yana kaydirma signal&slot
        // dusman ekrandan cikma kontrolu
        timer_label_enemy = new QTimer(this);
        connect(timer_label_enemy, SIGNAL(timeout()), SLOT(move_updateEnemy()));
        timer_label_enemy->start(hiz_time);

        // -------------- //

        // ates etme signal&slot (Cember cizme)
        // ates ekrandan cikma kontrolu
        timer_fire_update = new QTimer(this);
        connect(timer_fire_update, SIGNAL(timeout()), this, SLOT(fireUpdate()));
        timer_fire_update->start(1);

        // karakterin olumu signal&slot (Dondurme animasyonu)
        deadEnemyTimer = new QTimer(this);
        connect(deadEnemyTimer, SIGNAL(timeout()), this, SLOT(deadEnemyChrt()));
        deadEnemyTimer->start(100);


        // Puan zaman ve oyun zorlugu signal&slot
        checkGameTimer = new QTimer(this);
        connect(checkGameTimer, SIGNAL(timeout()), this, SLOT(gameCheck()));
        checkGameTimer->start(1000);

        checkHizA=0;
        checkHizB=0;

}

// ana karakter olunce
// dondurme algoritmasi ile olum animasyonu
void MainWindow::mainDead(QString message){

    if(healthy!=0){
        healthy-=1;
        MainCharacter->move(1220,585);
        Location=1220;
        return;
    }

    puan-=20;

    QPixmap image(":/gameCharacter/Images/Character/mainDeadCr.png");
    QImage imageC = image.toImage();

    QImage canva;
    QPixmap imageCanva(":/gameCharacter/Images/Character/mainDead.png");
    canva=imageCanva.toImage();

    double x,y;
    double angle=90.0;

    for(int i=0; i<100; i++){

            for(int j=0; j<100; j++){

                x = i*cos(qDegreesToRadians(angle)) - j*sin(qDegreesToRadians(angle));
                y = i*sin(qDegreesToRadians(angle)) + j*cos(qDegreesToRadians(angle));

                canva.setPixel(round(x)+99, round(y), imageC.pixel(i,j));

            }

        }

    MainCharacter->setPixmap(QPixmap::fromImage(canva));
    MainCharacter->move(MainCharacter->pos().x(),595);

    timer_ObjSpawn->stop();
    timer_label_obj->stop();
    timer_time_enemy->stop();
    timer_label_enemy->stop();
    timer_fire_update->stop();
    deadEnemyTimer->stop();
    checkGameTimer->stop();

    delete timer_ObjSpawn;
    delete timer_label_obj;
    delete timer_time_enemy;
    delete timer_label_enemy;
    delete timer_fire_update;
    delete deadEnemyTimer;
    delete checkGameTimer;

    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setInformativeText("Puanınız: "+QString::number(puan)+" \nYeni bir oyun oynamak istiyormusunuz ?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {

        MainCharacter->hide();
        delete MainCharacter;

        deleteAll();
        setScreen();
        init__game();

    } else {
        QApplication::quit();
    }

}

// kullanıcının ates etmesi ve atesin DDA algoritması ile duz sekilde gitmesi
void MainWindow::fireUpdate(){

    int count = -1;

    foreach(Fire *bal, Fire_list){

        count++;

        // konum guncelleme
        bal->new_move();

        // ates kacti
        if(bal->check(bal->getLabel()->pos().x(), bal->getLabel()->pos().y())){

            puan-=1;

            Fire_list.at(count)->getLabel()->hide();
            delete Fire_list.at(count);
            Fire_list.removeAt(count);

        }

        // dusman karakter ile çakışma kontrolu
        else{

            int countA = -1;

            foreach(EnemyCharacter *dusman, EnemyChr_list){

                countA++;

                QRect droppedRect = bal->getLabel()->geometry();

                if (droppedRect.intersects(dusman->getLabel()->geometry())){

                    puan+=5;

                    EnemyChr_list_dead.append(EnemyChr_list.at(countA));
                    //EnemyChr_list.at(countA)->getLabel()->hide();
                    //delete EnemyChr_list.at(countA);
                    EnemyChr_list.removeAt(countA);

                    Fire_list.at(count)->getLabel()->hide();
                    delete Fire_list.at(count);
                    Fire_list.removeAt(count);

                    break;

                        }
                    }
                }

            }
}

// dusman goblinlerin olme efekti (olceklendirme algoritmasi ile olum animasyonu)
void MainWindow::deadEnemyChrt(){

    foreach(EnemyCharacter *bal, EnemyChr_list_dead){


        int countA = -1;

        QPixmap image(":/gameBackground/Images/GameBackground/deadCr.png");

        QImage imageC = image.toImage();

        QImage canva;

        double sx=1;
        double sy=1;

        for(int xx=0; xx<=8;xx++){

            countA+=1;

            sx-=0.1;
            sy-=0.1;

            QPixmap imageCanva(":/gameBackground/Images/GameBackground/dead.png");
            canva=imageCanva.toImage();

            int x,y;

            for(int i=0; i<imageC.width(); i++){

                    for(int j=0; j<imageC.height(); j++){

                        x=i*sx;
                        y=j*sy;

                        canva.setPixel(x, y, imageC.pixel(i,j));

                    }

                    QApplication::processEvents();
                    bal->getLabel()->setPixmap(QPixmap::fromImage(canva));

            }

        }

        bal->getLabel()->hide();

        //EnemyChr_list_dead.removeAt(countA);

    }


}


// Dusman goblinlerin hareketleri
void MainWindow::move_updateEnemy(){

    int count = -1;

    foreach(EnemyCharacter *bal, EnemyChr_list){

        count++;

        // konum guncelleme
        bal->new_move();

        if(bal->check(bal, 1400)){


            EnemyChr_list.at(count)->getLabel()->hide();
            delete EnemyChr_list.at(count);
            EnemyChr_list.removeAt(count);
        }

        // ana karakter ile çakışma kontrolu
        else{

            QRect droppedRect = MainCharacter->geometry();

            if (droppedRect.intersects(bal->getLabel()->geometry())){

                mainDead("Oyunu kaybettiniz!! goblin tarafından öldürüldünüz....");
            }
        }

    }


}

// yukardan dusen dusman ates ucgenlerinin hareketleri
void MainWindow::move_updateObject(){

    int count = -1;

    foreach(EnemyObject *bal, EnemyObject_list){

        count++;

        // konum guncelleme
        bal->new_move();

        // obje kacti
        if(bal->check(bal, 625)){

            puan+=2;

            EnemyObject_list.at(count)->getLabel()->hide();
            delete EnemyObject_list.at(count);
            EnemyObject_list.removeAt(count);
        }

        // ana karakter ile çakışma kontrolu
        else{

            QRect droppedRect = MainCharacter->geometry();

            if (droppedRect.intersects(bal->getLabel()->geometry())){

                mainDead("Oyunu kaybettiniz!! düşman nesnesi tarafından öldürüldünüz....");
            }
        }

    }

}

// yeni oyuna baslama
void MainWindow::deleteAll(){


    foreach(EnemyObject *bal, EnemyObject_list){

        bal->getLabel()->hide();
        EnemyObject_list.removeAll(bal);
    }

    foreach(EnemyCharacter *bal, EnemyChr_list){

        bal->getLabel()->hide();
        EnemyChr_list.removeAll(bal);
    }
}

// dusman goblinlerin uretimi
void MainWindow::spawn_enemy(){

    QLabel *label = new QLabel(this);

    EnemyCharacter *en = new EnemyCharacter(label, 1300);
    EnemyChr_list.append(en);

}

// dusman ates ucgenlerinin uretimi
void MainWindow::spawn_obj(){

    QLabel *label = new QLabel(this);

    EnemyObject *en = new EnemyObject(label, 1300);
    EnemyObject_list.append(en);

}

// karakter animasyonu
void MainWindow::keyPressEvent(QKeyEvent *event)
{

    int step = 20;

    if(event->key() == Qt::Key_D){

        if(Location>=1200){
            return;
        }

        QPixmap pix(":/gameCharacter/Images/Character/R"+QString::number(R)+".png");
        MainCharacter->setPixmap(pix);
        MainCharacter->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        MainCharacter->setGeometry(Location+step,585,100,100);
        Location+=step;
        MainCharacter->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));

        R+=1;

        if(R==10){
            R=1;
        }

    }

    else if(event->key() == Qt::Key_A){

        if(Location<=10){
            return;
        }

        QPixmap pix(":/gameCharacter/Images/Character/L"+QString::number(L)+".png");
        MainCharacter->setPixmap(pix);
        MainCharacter->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        MainCharacter->setGeometry(Location-step,585,100,100);
        Location-=step;
        MainCharacter->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));

        L+=1;

        if(L==10){
            L=1;
        }


    }
}

// karakter animasyonu
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QPixmap pix(":/gameCharacter/Images/Character/standing.png");
    MainCharacter->setPixmap(pix);
    MainCharacter->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    //Here is how to change position:
    MainCharacter->setGeometry(Location,585,100,100);
    MainCharacter->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));
}

void MainWindow::updateMedkit(){

    md->new_move();

    if (md->check(md, 625)==1){

        md->getLabel()->hide();
        delete md;
        timer_medkitUpdate->stop();
        delete timer_medkitUpdate;

        return;

    }

    QRect droppedRect = MainCharacter->geometry();

    if (droppedRect.intersects(md->getLabel()->geometry())){

        healthy+=1;

        md->getLabel()->hide();
        delete md;
        timer_medkitUpdate->stop();
        delete timer_medkitUpdate;

        return;
    }


}

// sure, puan ve oyun hizi
void MainWindow::gameCheck(){

    ui->message->setText("  Oyun Mesajı:Kendinizi ve Prensesi düşman nesnelerden koruyunuz;          Oyunun bitmesine "+QString::number(time)+" sn kaldı;               Can:"+QString::number(healthy)+";               Puanınız:"+QString::number(puan));
    time-=1;

    if(time==medkitSpawnTime){

        QLabel *label = new QLabel(this);
        md = new Medkit(label,1300);

        timer_medkitUpdate = new QTimer(this);
        connect(timer_medkitUpdate, SIGNAL(timeout()), this, SLOT(updateMedkit()));
        timer_medkitUpdate->start(75);
    }

    if(time<0){

        timer_ObjSpawn->stop();
        timer_label_obj->stop();
        timer_time_enemy->stop();
        timer_label_enemy->stop();
        timer_fire_update->stop();
        deadEnemyTimer->stop();
        checkGameTimer->stop();

        delete timer_ObjSpawn;
        delete timer_label_obj;
        delete timer_time_enemy;
        delete timer_label_enemy;
        delete timer_fire_update;
        delete deadEnemyTimer;
        delete checkGameTimer;

        QMessageBox msgBox;
        msgBox.setText("Tebrikler Prensesi kurtardınız :)");
        msgBox.setInformativeText("Puanınız: "+QString::number(puan)+" \nYeni bir oyun oynamak istiyormusunuz ?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Ok) {

            MainCharacter->hide();
            delete MainCharacter;

            deleteAll();
            setScreen();
            init__game();

        } else {
            QApplication::quit();
        }

    }

    if(time>90 && time<110){

        checkHizA=1;

        if (checkHizA==1 && checkHizB==0){

            checkHizB=1;

            timer_ObjSpawn->stop();
            timer_label_obj->stop();
            timer_time_enemy->stop();
            timer_label_enemy->stop();

            delete timer_ObjSpawn;
            delete timer_label_obj;
            delete timer_time_enemy;
            delete timer_label_enemy;

            nesne_spawn_time=1200;
            dusman_spawn_time=1800;
            hiz_time = 80;

            timer_ObjSpawn = new QTimer(this);
            connect(timer_ObjSpawn, SIGNAL(timeout()), this, SLOT(spawn_obj()));
            timer_ObjSpawn->start(nesne_spawn_time);

            timer_label_obj = new QTimer(this);
            connect(timer_label_obj, SIGNAL(timeout()), SLOT(move_updateObject()));
            timer_label_obj->start(hiz_time);

            timer_time_enemy = new QTimer(this);
            connect(timer_time_enemy, SIGNAL(timeout()), this, SLOT(spawn_enemy()));
            timer_time_enemy->start(dusman_spawn_time);

            timer_label_enemy = new QTimer(this);
            connect(timer_label_enemy, SIGNAL(timeout()), SLOT(move_updateEnemy()));
            timer_label_enemy->start(hiz_time);


        }
    }

    else if(time>70 && time<90){

        if(checkHizA==1){
            checkHizA=2;
            checkHizB=0;
        }

        if(checkHizA==2 && checkHizB==0){

            ui->message->setStyleSheet("QLabel { background-color : #2296af; color : orange; }");

            checkHizB=1;

            timer_ObjSpawn->stop();
            timer_label_obj->stop();
            timer_time_enemy->stop();
            timer_label_enemy->stop();

            delete timer_ObjSpawn;
            delete timer_label_obj;
            delete timer_time_enemy;
            delete timer_label_enemy;

            nesne_spawn_time=1000;
            dusman_spawn_time=1500;
            hiz_time = 70;

            timer_ObjSpawn = new QTimer(this);
            connect(timer_ObjSpawn, SIGNAL(timeout()), this, SLOT(spawn_obj()));
            timer_ObjSpawn->start(nesne_spawn_time);

            timer_label_obj = new QTimer(this);
            connect(timer_label_obj, SIGNAL(timeout()), SLOT(move_updateObject()));
            timer_label_obj->start(hiz_time);

            timer_time_enemy = new QTimer(this);
            connect(timer_time_enemy, SIGNAL(timeout()), this, SLOT(spawn_enemy()));
            timer_time_enemy->start(dusman_spawn_time);

            timer_label_enemy = new QTimer(this);
            connect(timer_label_enemy, SIGNAL(timeout()), SLOT(move_updateEnemy()));
            timer_label_enemy->start(hiz_time);

        }

    }

    else if(time>50 && time<70){

        if(checkHizA==2){
            checkHizA=3;
            checkHizB=0;
        }

        if(checkHizA==3 && checkHizB==0){

            ui->message->setStyleSheet("QLabel { background-color : #2296af; color : brown; }");

            checkHizB=1;

            timer_ObjSpawn->stop();
            timer_label_obj->stop();
            timer_time_enemy->stop();
            timer_label_enemy->stop();

            delete timer_ObjSpawn;
            delete timer_label_obj;
            delete timer_time_enemy;
            delete timer_label_enemy;

            nesne_spawn_time=900;
            dusman_spawn_time=1200;
            hiz_time = 60;

            timer_ObjSpawn = new QTimer(this);
            connect(timer_ObjSpawn, SIGNAL(timeout()), this, SLOT(spawn_obj()));
            timer_ObjSpawn->start(nesne_spawn_time);

            timer_label_obj = new QTimer(this);
            connect(timer_label_obj, SIGNAL(timeout()), SLOT(move_updateObject()));
            timer_label_obj->start(hiz_time);

            timer_time_enemy = new QTimer(this);
            connect(timer_time_enemy, SIGNAL(timeout()), this, SLOT(spawn_enemy()));
            timer_time_enemy->start(dusman_spawn_time);

            timer_label_enemy = new QTimer(this);
            connect(timer_label_enemy, SIGNAL(timeout()), SLOT(move_updateEnemy()));
            timer_label_enemy->start(hiz_time);

        }

    }

    else if(time<50){

        if(checkHizA==3){
            checkHizA=4;
            checkHizB=0;
        }

        if(checkHizA==4 && checkHizB==0){

            ui->message->setStyleSheet("QLabel { background-color : #2296af; color : red; }");

            checkHizB=1;

            timer_ObjSpawn->stop();
            timer_label_obj->stop();
            timer_time_enemy->stop();
            timer_label_enemy->stop();

            delete timer_ObjSpawn;
            delete timer_label_obj;
            delete timer_time_enemy;
            delete timer_label_enemy;

            nesne_spawn_time=700;
            dusman_spawn_time=800;
            hiz_time = 40;

            timer_ObjSpawn = new QTimer(this);
            connect(timer_ObjSpawn, SIGNAL(timeout()), this, SLOT(spawn_obj()));
            timer_ObjSpawn->start(nesne_spawn_time);

            timer_label_obj = new QTimer(this);
            connect(timer_label_obj, SIGNAL(timeout()), SLOT(move_updateObject()));
            timer_label_obj->start(hiz_time);

            timer_time_enemy = new QTimer(this);
            connect(timer_time_enemy, SIGNAL(timeout()), this, SLOT(spawn_enemy()));
            timer_time_enemy->start(dusman_spawn_time);

            timer_label_enemy = new QTimer(this);
            connect(timer_label_enemy, SIGNAL(timeout()), SLOT(move_updateEnemy()));
            timer_label_enemy->start(hiz_time);

        }
    }

}

// ates edilecek nokta
void MainWindow::mousePressEvent(QMouseEvent *event)
{


    QLabel *label = new QLabel(this);

    Fire *en = new Fire(label, MainCharacter->pos().x(), MainCharacter->pos().y()+25, event->pos());
    Fire_list.append(en);


}

// cikis
void MainWindow::on_closeButton_clicked()
{
    exit(0);
}

void MainWindow::on_minimizeButton_clicked()
{
    this->showMinimized();
}
