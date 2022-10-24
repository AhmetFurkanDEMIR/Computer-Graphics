#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;

class InputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InputDialog(QWidget *parent = nullptr);

    static QStringList getStrings(QWidget *parent, bool *ok = nullptr);

private:
    QList<QLineEdit*> fields;
};

#endif // INPUTDIALOG_H
