#include "stateplot.h"
#include "ui_stateplot.h"

#include <QValueAxis>
#include <QSplineSeries>
#include <QVBoxLayout>

StatePlot::StatePlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatePlot)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,true); //关闭时自动删除

    m_chart= new Chart();
    m_chart->legend()->hide();
    m_chart->setTitleFont(QFont("Monospace",16));
    m_chart->m_axisX->setTitleFont(QFont("Monospace",12));
    m_chart->m_axisY->setTitleFont(QFont("Monospace",12));
    m_chart->m_series->setUseOpenGL(true);
    m_chart->setAnimationOptions(QChart::AllAnimations);
    chartView = new QChartView(m_chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    ui->gridLayout->addWidget(chartView);
    setLayout(ui->gridLayout);

}   

StatePlot::~StatePlot()
{
    // printf("delete state plot...\n");
    delete ui;
}

void StatePlot::generateData(std::vector<float> data){

    qreal x = m_chart->plotArea().width() / m_chart->m_axisX->tickCount();

    m_chart->m_x = data[0];  //实时x坐标
    m_chart->m_y = data[1];  //实时y坐标

    m_chart->m_series->append(m_chart->m_x, m_chart->m_y);
 
    //实现整体平移,必须指明是QChart的方法，不然就是整个界面都平移
    m_chart->scroll(x, 0);
}



