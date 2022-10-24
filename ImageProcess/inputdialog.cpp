#include "inputdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>

InputDialog::InputDialog(QWidget *parent) : QDialog(parent)
{
    QFormLayout *lytMain = new QFormLayout(this);

    for (int i = 0; i < 2; ++i)
    {

        QLabel *tLabel;

        if(i==0){
            tLabel = new QLabel(QString("X : ").arg(i), this);
        }

        else{
            tLabel = new QLabel(QString("Y : ").arg(i), this);
        }

        QLineEdit *tLine = new QLineEdit(this);
        lytMain->addRow(tLabel, tLine);

        fields << tLine;
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                   this, &InputDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &InputDialog::reject);
    Q_ASSERT(conn);

    setLayout(lytMain);
}

QStringList InputDialog::getStrings(QWidget *parent, bool *ok)
{
    InputDialog *dialog = new InputDialog(parent);

    QStringList list;

    const int ret = dialog->exec();
    if (ok)
        *ok = !!ret;

    if (ret) {
        foreach (auto field, dialog->fields) {
            list << field->text();
        }
    }

    dialog->deleteLater();

    return list;
}
