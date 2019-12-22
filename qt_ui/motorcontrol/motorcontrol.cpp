#include "motorcontrol.h"
#include "ui_motorcontrol.h"
#include <QMessageBox>
#include <QMetaType>

MotorControl::MotorControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MotorControl)
{
    ui->setupUi(this);

    ui->pushButton_2->setEnabled(0);
    ui->pushButton_3->setEnabled(0);
    ui->pushButton_4->setEnabled(0);
    ui->tabWidget->setEnabled(0);

    thread_ = new MotorThread();

    qRegisterMetaType<std::vector<float>>("std::vector<float>");

    connect(this,SIGNAL(motorStart()),thread_,SLOT(motorStart()));
    connect(this,SIGNAL(motorStop()),thread_,SLOT(motorStop()));
    connect(this,SIGNAL(motorHalt()),thread_,SLOT(motorHalt()));
    connect(this,SIGNAL(motorHaltContinue()),thread_,SLOT(motorHaltContinue()));
    connect(this,SIGNAL(motorQuickStop()),thread_,SLOT(motorQuickStop()));
    connect(this,SIGNAL(changeOpmode(uint8_t)),thread_,SLOT(changeOpmode(uint8_t)));
    connect(this,SIGNAL(sentMotorPos(float)),thread_,SLOT(sentMotorPos(float)));
    connect(this,SIGNAL(sentMotorVel(float)),thread_,SLOT(sentMotorVel(float)));
    connect(this,SIGNAL(sentMotorTorque(float)),thread_,SLOT(sentMotorTorque(float)));
    connect(thread_,SIGNAL(sentMotorOpenError(bool)),this,SLOT(motor_start_error(bool)));
    connect(thread_->sent_feedback_,SIGNAL(uploadFeedback(std::vector<float>)), \
            this,SLOT(get_feedback_data(std::vector<float>)));
}

MotorControl::~MotorControl()
{   
    // printf("delete motorcontrol...\n");
    delete thread_;
    delete ui;
}

void MotorControl::on_pushButton_clicked()
{
    /*motor start */
    ui->pushButton->setEnabled(0);
    ui->pushButton_2->setEnabled(1);
    ui->pushButton_3->setEnabled(1);
    ui->pushButton_4->setEnabled(1);
    ui->tabWidget->setEnabled(1);
    emit motorStart();
}

void MotorControl::on_pushButton_4_clicked()
{
    ui->listWidget->addItem("Motor Stop...");
    emit motorStop();
    ui->pushButton->setEnabled(1);
    ui->pushButton_2->setEnabled(0);
    ui->pushButton_3->setEnabled(0);
    ui->pushButton_4->setEnabled(0);
    ui->tabWidget->setEnabled(0);
}

void MotorControl::on_pushButton_2_clicked(bool checked)
{
    if(checked){
        ui->listWidget->addItem("motor halt...");
        emit motorHalt();
    }else{
        ui->listWidget->addItem("motor continue...");
        emit motorHaltContinue();
    }
}

void MotorControl::on_pushButton_3_clicked()
{
    ui->listWidget->addItem("motor quick stop...");
    emit motorQuickStop();
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
        emit changeOpmode(PROFILE_POSITION_MODE);
    }else if (index == 1) {
        ui->listWidget->addItem("change op mode to \"profile velocity\"... ");
        emit changeOpmode(PROFILE_VELOCITY_MODE);
    }else if (index == 2) {
        ui->listWidget->addItem("change op mode to \"profile torque\"... ");
        emit changeOpmode(TORQUE_PROFILE_MODE);
    }
}

void MotorControl::on_pushButton_5_clicked()
{
    ui->listWidget->clear();

}

void MotorControl::on_actionStatePlot_triggered()
{
    ui_state_plot = new StatePlot(NULL);
    ui_state_plot->show();
}

void MotorControl::on_actionQuit_triggered()
{
    this->close();
}

void MotorControl::on_actionHelp_triggered()
{
    QMessageBox::about(NULL, "About", "Elmo Controller V1.0.    \
                                     \n\nAuthor: Jony.          \
                                      \n\nMail: tanjony@qq.com"
                                      );

}

void MotorControl::get_feedback_data(std::vector<float> vector){
    printf("3333\n");
    ui->lineEdit->setText(QString::number(vector[0]));    //motor pos
    ui->lineEdit_2->setText(QString::number(vector[1]));  //motor vel
    ui->lineEdit_3->setText(QString::number(vector[2]));  //motor torque
    ui->lineEdit_4->setText(QString::number(vector[3]));  //current
    ui->lineEdit_10->setText(QString::number(vector[4]));  //joint position
    ui->lineEdit_5->setText(QString::number(vector[5]));  //joint velocity
    ui->lineEdit_11->setText(QString::number(vector[6]));  //joint torque
    ui->lineEdit_12->setText(QString::number(vector[7]));  //chip temperture

}

void MotorControl::motor_start_error(bool ifError){
    if(ifError){
        
        ui->pushButton->setEnabled(1);
        ui->pushButton_2->setEnabled(0);
        ui->pushButton_3->setEnabled(0);
        ui->pushButton_4->setEnabled(0);
        ui->tabWidget->setEnabled(0);
    }else{
        ui->listWidget->addItem("motor start.");
    }
}