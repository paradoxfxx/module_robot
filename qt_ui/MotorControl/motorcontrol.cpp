#include "motorcontrol.h"
#include "ui_motorcontrol.h"

MotorControl::MotorControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorControl)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(0);
    ui->pushButton_3->setEnabled(0);
    ui->pushButton_4->setEnabled(0);
    ui->tabWidget->setEnabled(0);

}

MotorControl::~MotorControl()
{
    delete ui;
}

void MotorControl::on_pushButton_clicked()
{
    ui->listWidget->addItem("motor start ...");
    ui->pushButton->setEnabled(0);
    ui->pushButton_2->setEnabled(1);
    ui->pushButton_3->setEnabled(1);
    ui->pushButton_4->setEnabled(1);
    ui->tabWidget->setEnabled(1);

}



void MotorControl::on_pushButton_4_clicked()
{
    ui->listWidget->addItem("clear error...");
}

void MotorControl::on_pushButton_3_clicked()
{
    ui->listWidget->addItem("motor quick stop...");
    ui->pushButton->setEnabled(1);
    ui->pushButton_2->setEnabled(0);
    ui->pushButton_3->setEnabled(0);
    ui->pushButton_4->setEnabled(0);
    ui->tabWidget->setEnabled(0);

}


void MotorControl::on_tabWidget_tabBarClicked(int index)
{
    if(index == 0){
        ui->listWidget->addItem("change op mode to \"profile position\"... ");
    }else if (index == 1) {
        ui->listWidget->addItem("change op mode to \"profile velocity\"... ");
    }else if (index == 2) {
        ui->listWidget->addItem("change op mode to \"profile torque\"... ");
    }
}

void MotorControl::on_pushButton_5_clicked()
{
    ui->listWidget->clear();
}

void MotorControl::on_pushButton_2_clicked(bool checked)
{
    if(checked){
        ui->listWidget->addItem("motor halt...");
    }else{
        ui->listWidget->addItem("motor continue...");
    }
}
