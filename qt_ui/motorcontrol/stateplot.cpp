#include "stateplot.h"
#include "ui_stateplot.h"

StatePlot::StatePlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatePlot)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,true); //关闭时自动删除

    
}

StatePlot::~StatePlot()
{
    // printf("delete state plot...\n");
    delete ui;
}
