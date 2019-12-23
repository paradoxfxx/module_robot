
#include "chart.h"
 
Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(0),
    m_axisX(new QValueAxis()),
    m_axisY(new QValueAxis()),
    m_x(0),
    m_y(0)
{
    m_series = new QSplineSeries(this);
    QPen green(Qt::red);
    green.setWidth(1);
    m_series->setPen(green);
    m_series->append(m_x, m_y);
 
    addSeries(m_series);
 
    //x, y坐标的位置，可以自行调整
    addAxis(m_axisX, Qt::AlignBottom);
    addAxis(m_axisY, Qt::AlignLeft);
 
    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);

    m_axisX->setTickCount(6);  //设置大刻度线的数目，默认是5，不能小于2
     m_axisY->setTickCount(6);  //设置大刻度线的数目，默认是5，不能小于2

    //设置x, y坐标范围
    m_axisX->setRange(0, 10000);
    m_axisY->setRange(-100, 100);
}
 
Chart::~Chart()
{}

