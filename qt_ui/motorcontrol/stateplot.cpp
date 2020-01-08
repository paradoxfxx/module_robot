#include "stateplot.h"
#include "ui_stateplot.h"

#include <QValueAxis>
#include <QSplineSeries>
#include <QGraphicsView>

StatePlot::StatePlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatePlot)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,true); //关闭时自动删除
    m_isTouching = false;
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
    // chartView->setRubberBand(QChartView::HorizontalRubberBand);

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

    m_chart->m_x += data[0];  //实时x坐标
    m_chart->m_y = data[1];  //实时y坐标

    m_chart->m_series->append(m_chart->m_x, m_chart->m_y);
 
    //实现整体平移,必须指明是QChart的方法，不然就是整个界面都平移
    // m_chart->scroll(x, 0);
}

void StatePlot::wheelEvent(QWheelEvent *event) 
{
    qreal factor;
    if ( event->delta() > 0 )
        factor = 2.0;
    else
        factor = 0.5;
    
    QRectF r = QRectF(m_chart->plotArea().left(),m_chart->plotArea().top(),
                                        m_chart->plotArea().width()/factor,m_chart->plotArea().height()/factor);
    QPointF mousePos = mapFromGlobal(QCursor::pos());
    r.moveCenter(mousePos);
    m_chart->zoomIn(r);
    QPointF delta = m_chart->plotArea().center() -mousePos;
    m_chart->scroll(delta.x(),-delta.y());
}


void StatePlot::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()){

    case Qt::Key_Plus:
        m_chart->zoomOut();
        break;
    case Qt::Key_Minus:
        m_chart->zoomIn();
        break;

    case Qt::Key_Pause: /*Key_Minus*/
        m_chart->zoomIn();
        break;

    case Qt::Key_A:
        m_chart->scroll(-10, 0);
        break;
    case Qt::Key_D:
        m_chart->scroll(10, 0);
        break;
    case Qt::Key_W:
        m_chart->scroll(0, 10);
        break;
    case Qt::Key_S:
        m_chart->scroll(0, -10);
        break;

    case Qt::Key_Enter:
        m_chart->zoomReset();
        break;

    case Qt::Key_Return:    /*Key_Enter*/
        m_chart->zoomReset();
        break;
    default:
        // qDebug() << event->key() << "\n";
        break;
    }
}