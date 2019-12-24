#ifndef STATEPLOT_H
#define STATEPLOT_H

#include <QDialog>
#include "chart.h"
#include <QChartView>
#include <QDebug>

namespace Ui {
class StatePlot;
}

class StatePlot : public QDialog
{
    Q_OBJECT

public:
    explicit StatePlot(QWidget *parent = nullptr);

    ~StatePlot();

private:
    bool m_isTouching;

protected:
    void wheelEvent(QWheelEvent *event); 
    void keyPressEvent(QKeyEvent *event);
    // bool eventFilter(QObject *obj, QEvent *e);

public slots:
    void generateData(std::vector<float> data);


public:
    Ui::StatePlot *ui;
    Chart *m_chart;
    QChartView *chartView;

};

#endif // STATEPLOT_H
