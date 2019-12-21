#include "stateplot.h"
#include "ui_stateplot.h"

StatePlot::StatePlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatePlot)
{
    ui->setupUi(this);
}

StatePlot::~StatePlot()
{
    delete ui;
}
