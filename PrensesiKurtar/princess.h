#ifndef PRINCESS_H
#define PRINCESS_H

#include <QThread>
#include <QString>
#include <QRandomGenerator>
#include <QLabel>
#include <QtMath>

class Princess: public QObject
{
public:

    Princess(QLabel *labelM);

    // thread fonksiyon
    void process();

    QLabel * getLabel();
    void prensesOtele();

private:

    QLabel *labelMM;
    QImage environment;
    QImage imageC;

signals:
    void finished();
    void error(QString err);
};

#endif // PRINCESS_H
