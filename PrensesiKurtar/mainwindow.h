#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include <math.h>
#include "EnemyObject.cpp"
#include <QRandomGenerator>
#include <QMessageBox>
#include "EnemyCharacter.cpp"
#include "Fire.cpp"
#include <QtMath>
#include <QTimer>
#include "princess.h"
#include "Medkit.cpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    // cikis ve ekranı kucultme
    void on_closeButton_clicked();
    void on_minimizeButton_clicked();

    // ekran ayarlari
    void setScreen();

    // nesne uretimi icin timer
    void init__game();

    // karakter animasyonu ve ates etme konum fonksiyonu
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent*);

    // dusman nesnesi uretimi
    void spawn_obj();
    void move_updateObject();

    // dusman goblin uretimi
    void spawn_enemy();
    void move_updateEnemy();
    void deadEnemyChrt();

    // yeni oyun
    void deleteAll();

    // ates edilen cemberin yeni kordinatları
    void fireUpdate();

    // oyun hakkında
    void gameCheck();
    void mainDead(QString message);

    // medkitin donerek asagiya dusmesi
    void updateMedkit();


private:

    Ui::MainWindow *ui;
    QLabel *MainCharacter;
    Princess *pr;
    QThread* thread;
    QImage imgCanv;

    int R;
    int L;
    int Location;

    QList<QPoint> mouses;
    QPoint mouse;
    QRgb color;

    int puan;
    int time;

    QTimer *timer_time_obj;
    QTimer *timer_ObjSpawn;
    QTimer *timer_label_obj;
    QList<EnemyObject *> EnemyObject_list;

    QTimer *timer_time_enemy;
    QTimer *timer_EnemySpawn;
    QTimer *timer_label_enemy;
    QList<EnemyCharacter *> EnemyChr_list;
    QList<EnemyCharacter *> EnemyChr_list_dead;

    QTimer *timer_fire_update;
    QList<Fire *> Fire_list;
    QTimer * deadEnemyTimer;
    QTimer * checkGameTimer;

    int nesne_spawn_time;
    int dusman_spawn_time;
    int hiz_time;
    int healthy;

    int checkHizA;
    int checkHizB;

    int medkitSpawnTime;
    Medkit *md;
    QTimer *timer_medkitUpdate;

};
#endif // MAINWINDOW_H
