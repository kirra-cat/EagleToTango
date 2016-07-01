#include "eagletotango.h"
#include "ui_eagletotango.h"

#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QTime>
#include <QThread>

#include <QDebug>

QString File_Name_Eagle_NET_List, Path_to_Eagle_NET_List;
qint8 Ignore_First_Lines = 0;
bool First_NET = true;

EagleToTango::EagleToTango(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EagleTango)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog);
    setWindowTitle(tr("Eagle to Tango-format"));
    setFixedSize(242,63);
}

EagleToTango::~EagleToTango()
{
    delete ui;
}

void EagleToTango::on_pushButtonOpenFile_clicked()
{
    File_Name_Eagle_NET_List = QFileDialog::getOpenFileName(this,
                                            tr("Открыть NET-лист Eagle CAD"),
                                            QString(),
                                            tr("NET-лист(*.net *.txt)"));
    if (File_Name_Eagle_NET_List.isEmpty())
    {
        ui->labelStatus->setText(tr("<font color=#ff2323>Net-лист не выбран.</font>"));
    }
    else
    {
        ui->labelStatus->setText(File_Name_Eagle_NET_List);
    }
}

void EagleToTango::on_pushButtonStartConversion_clicked()
{
    QTime CurrentTime = QTime::currentTime();

    QFile Eagle_NET_List(File_Name_Eagle_NET_List);

    QFileInfo Eagle_NET_List_File_Info(File_Name_Eagle_NET_List);
    QString Tango_File_Name =
            Eagle_NET_List_File_Info.fileName().left(Eagle_NET_List_File_Info.fileName().size() - 4) +
            "_tango_" + CurrentTime.toString("hh_mm_ss") + ".net";

    QFile Tango_NET_List(Eagle_NET_List_File_Info.path() + "/" + Tango_File_Name);
    QTextStream Tango_NET_Stream(&Tango_NET_List);

    if (Eagle_NET_List.open(QIODevice::ReadOnly | QIODevice::Text) &&
        Tango_NET_List.open(QIODevice::WriteOnly))
    {
        while(!Eagle_NET_List.atEnd())
        {
            QString ReadLine = Eagle_NET_List.readLine().simplified();
            QStringList Words = ReadLine.split(" ");

            if (Ignore_First_Lines++ < 8) continue;

            if (Words.size() == 5)
            {
              if (First_NET)
              {
                  First_NET = false;
                  Tango_NET_Stream << "[\n]\n(\n" << Words.at(0) << "\n";
                  Tango_NET_Stream << Words.at(1) << "," << Words.at(2) << "\n";
              }
              else
              {
                  Tango_NET_Stream << ")\n\n[\n]\n(\n";
                  Tango_NET_Stream << Words.at(0) << "\n";
                  Tango_NET_Stream << Words.at(1) << "," << Words.at(2) << "\n";
              }
            }

            if (Words.size() == 4)
            {
                Tango_NET_Stream << Words.at(0) << "," << Words.at(1) << "\n";
            }
        }

        Tango_NET_Stream << ")\n";

        Eagle_NET_List.close();
        Tango_NET_List.close();

        Ignore_First_Lines = 0;

        ui->labelStatus->setText("<font color=#2fab00>Файл: " + Tango_File_Name + "</font>");
    }
    else
    {
        qDebug() << tr("Don't open file") + File_Name_Eagle_NET_List;
        ui->labelStatus->setText(tr("<font color=#ff2323>Не удаётся открыть файл</font>"));
    }
}
