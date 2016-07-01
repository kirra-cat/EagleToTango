#ifndef EAGLETANGO_H
#define EAGLETANGO_H

#include <QDialog>

namespace Ui {
class EagleTango;
}

class EagleToTango : public QDialog
{
    Q_OBJECT

public:
    explicit EagleToTango(QWidget *parent = 0);
    ~EagleToTango();

private slots:
    void on_pushButtonOpenFile_clicked();

    void on_pushButtonStartConversion_clicked();

private:
    Ui::EagleTango *ui;
};

#endif // EAGLETANGO_H
