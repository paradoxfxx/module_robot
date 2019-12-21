#ifndef STATEPLOT_H
#define STATEPLOT_H

#include <QDialog>
#include <QtCharts>

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
    Ui::StatePlot *ui;
};

#endif // STATEPLOT_H
