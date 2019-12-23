
#ifndef CHART_H
#define CHART_H
 
#include <QtCharts/QChart>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE
 
QT_CHARTS_USE_NAMESPACE
 
class Chart: public QChart
{
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~Chart();
 
public:
    QSplineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    qreal m_x;
    qreal m_y;
};

#endif