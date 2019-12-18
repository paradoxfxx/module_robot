/********************************************************************************
** Form generated from reading UI file 'motorcontrol.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOTORCONTROL_H
#define UI_MOTORCONTROL_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MotorControl
{
public:
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QLabel *label_5;
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *label_4;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLineEdit *lineEdit_6;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QPushButton *pushButton_6;
    QLineEdit *lineEdit_7;
    QLabel *label_14;
    QWidget *tab_3;
    QLineEdit *lineEdit_8;
    QLabel *label_15;
    QLabel *label_16;
    QPushButton *pushButton_7;
    QWidget *tab_2;
    QLineEdit *lineEdit_9;
    QPushButton *pushButton_8;
    QLabel *label_17;
    QLabel *label_18;
    QPushButton *pushButton_5;
    QListWidget *listWidget;
    QLabel *label_7;
    QLineEdit *lineEdit_4;
    QLabel *label_8;

    void setupUi(QWidget *MotorControl)
    {
        if (MotorControl->objectName().isEmpty())
            MotorControl->setObjectName(QString::fromUtf8("MotorControl"));
        MotorControl->resize(800, 400);
        MotorControl->setMinimumSize(QSize(800, 400));
        MotorControl->setMaximumSize(QSize(800, 400));
        MotorControl->setSizeIncrement(QSize(0, 0));
        QFont font;
        font.setPointSize(14);
        MotorControl->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/think.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MotorControl->setWindowIcon(icon);
        pushButton_3 = new QPushButton(MotorControl);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(30, 200, 110, 40));
        pushButton_4 = new QPushButton(MotorControl);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(30, 80, 110, 40));
        label_5 = new QLabel(MotorControl);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(490, 290, 80, 30));
        pushButton = new QPushButton(MotorControl);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 20, 110, 40));
        label = new QLabel(MotorControl);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 290, 80, 30));
        label_2 = new QLabel(MotorControl);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(300, 290, 80, 30));
        label_3 = new QLabel(MotorControl);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(300, 350, 80, 30));
        label_6 = new QLabel(MotorControl);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(490, 350, 80, 30));
        label_4 = new QLabel(MotorControl);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(240, 290, 80, 30));
        pushButton_2 = new QPushButton(MotorControl);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(30, 140, 110, 40));
        pushButton_2->setCheckable(true);
        lineEdit = new QLineEdit(MotorControl);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(130, 290, 100, 30));
        lineEdit->setMinimumSize(QSize(100, 30));
        lineEdit->setMaximumSize(QSize(100, 30));
        lineEdit->setReadOnly(true);
        lineEdit_2 = new QLineEdit(MotorControl);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(380, 290, 100, 30));
        lineEdit_2->setMinimumSize(QSize(100, 30));
        lineEdit_2->setMaximumSize(QSize(100, 30));
        lineEdit_2->setReadOnly(true);
        lineEdit_3 = new QLineEdit(MotorControl);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(380, 350, 100, 30));
        lineEdit_3->setMinimumSize(QSize(100, 30));
        lineEdit_3->setMaximumSize(QSize(100, 30));
        lineEdit_3->setReadOnly(true);
        tabWidget = new QTabWidget(MotorControl);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(160, 10, 400, 250));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        lineEdit_6 = new QLineEdit(tab);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(210, 30, 100, 30));
        label_11 = new QLabel(tab);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(320, 30, 50, 30));
        label_12 = new QLabel(tab);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(40, 70, 170, 30));
        label_13 = new QLabel(tab);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(40, 30, 170, 30));
        pushButton_6 = new QPushButton(tab);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(120, 130, 150, 60));
        lineEdit_7 = new QLineEdit(tab);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(210, 70, 100, 30));
        label_14 = new QLabel(tab);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(320, 70, 50, 30));
        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        lineEdit_8 = new QLineEdit(tab_3);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));
        lineEdit_8->setGeometry(QRect(210, 50, 100, 30));
        label_15 = new QLabel(tab_3);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(40, 50, 170, 30));
        label_16 = new QLabel(tab_3);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(320, 50, 50, 30));
        pushButton_7 = new QPushButton(tab_3);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(120, 110, 150, 60));
        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        lineEdit_9 = new QLineEdit(tab_2);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));
        lineEdit_9->setGeometry(QRect(210, 50, 100, 30));
        pushButton_8 = new QPushButton(tab_2);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(120, 110, 150, 60));
        label_17 = new QLabel(tab_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(320, 50, 50, 30));
        label_18 = new QLabel(tab_2);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(40, 50, 170, 30));
        tabWidget->addTab(tab_2, QString());
        pushButton_5 = new QPushButton(MotorControl);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(700, 350, 80, 40));
        listWidget = new QListWidget(MotorControl);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(580, 30, 200, 310));
        label_7 = new QLabel(MotorControl);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(50, 350, 80, 30));
        lineEdit_4 = new QLineEdit(MotorControl);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(130, 350, 100, 30));
        lineEdit_4->setMinimumSize(QSize(100, 30));
        lineEdit_4->setMaximumSize(QSize(100, 30));
        lineEdit_4->setReadOnly(true);
        label_8 = new QLabel(MotorControl);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(240, 350, 80, 30));

        retranslateUi(MotorControl);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MotorControl);
    } // setupUi

    void retranslateUi(QWidget *MotorControl)
    {
        MotorControl->setWindowTitle(QApplication::translate("MotorControl", "MotorControl", nullptr));
        pushButton_3->setText(QApplication::translate("MotorControl", "QuickStop", nullptr));
        pushButton_4->setText(QApplication::translate("MotorControl", "Clear Error", nullptr));
        label_5->setText(QApplication::translate("MotorControl", "deg/s", nullptr));
        pushButton->setText(QApplication::translate("MotorControl", "Start", nullptr));
        label->setText(QApplication::translate("MotorControl", "Position", nullptr));
        label_2->setText(QApplication::translate("MotorControl", "Velocity", nullptr));
        label_3->setText(QApplication::translate("MotorControl", "Torque", nullptr));
        label_6->setText(QApplication::translate("MotorControl", "mN.m", nullptr));
        label_4->setText(QApplication::translate("MotorControl", "deg", nullptr));
        pushButton_2->setText(QApplication::translate("MotorControl", "Halt", nullptr));
#ifndef QT_NO_TOOLTIP
        lineEdit_6->setToolTip(QApplication::translate("MotorControl", "<html><head/><body><p>input range:(+ - 360deg )</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        lineEdit_6->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        lineEdit_6->setText(QString());
        label_11->setText(QApplication::translate("MotorControl", "deg", nullptr));
        label_12->setText(QApplication::translate("MotorControl", "Velocity Command", nullptr));
        label_13->setText(QApplication::translate("MotorControl", "Position Command", nullptr));
        pushButton_6->setText(QApplication::translate("MotorControl", "sent command", nullptr));
#ifndef QT_NO_TOOLTIP
        lineEdit_7->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        lineEdit_7->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        lineEdit_7->setWhatsThis(QApplication::translate("MotorControl", "<html><head/><body><p>input range: (0 - 100)deg/s</p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
        lineEdit_7->setText(QString());
        label_14->setText(QApplication::translate("MotorControl", "deg/s", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MotorControl", "pp", nullptr));
#ifndef QT_NO_WHATSTHIS
        lineEdit_8->setWhatsThis(QApplication::translate("MotorControl", "<html><head/><body><p>input range: (0 - 100)deg/s</p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
        lineEdit_8->setText(QString());
        label_15->setText(QApplication::translate("MotorControl", "Velocity Command", nullptr));
        label_16->setText(QApplication::translate("MotorControl", "deg/s", nullptr));
        pushButton_7->setText(QApplication::translate("MotorControl", "sent command", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MotorControl", "pv", nullptr));
#ifndef QT_NO_TOOLTIP
        lineEdit_9->setToolTip(QApplication::translate("MotorControl", "<html><head/><body><p>+-1200</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        lineEdit_9->setWhatsThis(QApplication::translate("MotorControl", "<html><head/><body><p>input range:(+ - 1200mN.m)</p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
        lineEdit_9->setText(QString());
        pushButton_8->setText(QApplication::translate("MotorControl", "sent command", nullptr));
        label_17->setText(QApplication::translate("MotorControl", "mN.m", nullptr));
        label_18->setText(QApplication::translate("MotorControl", "Torque Command", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MotorControl", "pt", nullptr));
        pushButton_5->setText(QApplication::translate("MotorControl", "Clear", nullptr));
        label_7->setText(QApplication::translate("MotorControl", "Current", nullptr));
        label_8->setText(QApplication::translate("MotorControl", "mA", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MotorControl: public Ui_MotorControl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOTORCONTROL_H
