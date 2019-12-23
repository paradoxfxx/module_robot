#include "motorcontrol.h"
#include "ui_motorcontrol.h"
#include <QMessageBox>
#include <QMetaType>

bool MotorControl::plot_pos;
bool MotorControl::plot_vel;
bool MotorControl::plot_tor;
bool MotorControl::plot_temp;
std::vector<float> MotorControl::plot_data;

MotorControl::MotorControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MotorControl)
{
    ui->setupUi(this);

    plot_pos = false;
    plot_vel = false;
    plot_tor = false;
    plot_temp = false;

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
    
    ui->lineEdit->setText(QString::number(vector[0]));    //motor pos
    ui->lineEdit_2->setText(QString::number(vector[1]));  //motor vel
    ui->lineEdit_3->setText(QString::number(vector[2]));  //motor torque
    ui->lineEdit_4->setText(QString::number(vector[3]));  //current
    ui->lineEdit_10->setText(QString::number(vector[4]));  //joint position
    ui->lineEdit_5->setText(QString::number(vector[5]));  //joint velocity
    ui->lineEdit_11->setText(QString::number(vector[6]));  //joint torque
    ui->lineEdit_12->setText(QString::number(vector[7]));  //chip temperture
    // vector[8] time interval
    if(plot_pos){
        plot_data[0] = vector[8];
        plot_data[1] = vector[0];
        emit sentPlotPosData(plot_data);
    }

    if(plot_vel){
        plot_data[0] = vector[8];
        plot_data[1] = vector[1];
        emit sentPlotVelData(plot_data);
    }

    if(plot_tor){
        plot_data[0] = vector[8];
        plot_data[1] = vector[2];
        emit sentPlotTorData(plot_data);
    }

    if(plot_temp){
        plot_data[0] = vector[8];
        plot_data[1] = vector[7];
        emit sentPlotTempData(plot_data);
    }

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
void MotorControl::on_actionPostion_triggered()
{

        ui_state_plot_pos = new StatePlot(NULL);
        connect(this,SIGNAL(sentPlotPosData(std::vector<float>)), ui_state_plot_pos,SLOT(generateData(std::vector<float>))); 
        plot_pos = true;
        ui_state_plot_pos->m_chart->setTitle("Plot Position");
        ui_state_plot_pos->m_chart->m_axisX->setTitleText("Time(ms)");
        ui_state_plot_pos->m_chart->m_axisY->setTitleText("Position(deg)");
        ui_state_plot_pos->m_chart->m_axisY->setRange(-360, 360);
        ui_state_plot_pos->show();
}

void MotorControl::on_actionVeclocity_triggered()
{

    ui_state_plot_vel = new StatePlot(NULL);
    connect(this,SIGNAL(sentPlotVelData(std::vector<float>)), ui_state_plot_vel,SLOT(generateData(std::vector<float>)));
    plot_vel = true;
    ui_state_plot_vel->m_chart->setTitle("Plot Velocity");  
    ui_state_plot_vel->m_chart->m_axisX->setTitleText("Time(ms)");
    ui_state_plot_vel->m_chart->m_axisY->setTitleText("Veclocity(deg/s)");
    ui_state_plot_vel->m_chart->m_axisY->setRange(-100, 100);

    ui_state_plot_vel->show();

}

void MotorControl::on_actionTorque_triggered()
{   

    ui_state_plot_tor = new StatePlot(NULL);
    connect(this,SIGNAL(sentPlotTorData(std::vector<float>)), ui_state_plot_tor,SLOT(generateData(std::vector<float>)));  
    plot_tor = true; 
    ui_state_plot_tor->m_chart->setTitle("Plot Torque");
    ui_state_plot_tor->m_chart->m_axisX->setTitleText("Time(ms)");
    ui_state_plot_tor->m_chart->m_axisY->setTitleText("Torque(mN.m)");
    ui_state_plot_tor->m_chart->m_axisY->setRange(-50, 50);
    ui_state_plot_tor->show();

}

void MotorControl::on_actionTemperature_triggered()
{

    ui_state_plot_temp = new StatePlot(NULL);
    connect(this,SIGNAL(sentPlotTempData(std::vector<float>)), ui_state_plot_temp,SLOT(generateData(std::vector<float>)));
    static bool plot_temp = true;
    ui_state_plot_temp->m_chart->setTitle("Plot Temperature");
    ui_state_plot_temp->m_chart->m_axisX->setTitleText("Time(ms)");
    ui_state_plot_temp->m_chart->m_axisY->setTitleText("Temperature(Celsius)");
    ui_state_plot_temp->m_chart->m_axisY->setRange(0, 100);
    ui_state_plot_temp->show();

}
