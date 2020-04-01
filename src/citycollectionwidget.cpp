#include "citycollectionwidget.h"
#include "ui_citycollectionwidget.h"
#include "cityaddwidget.h"

CityCollectionWidget::CityCollectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::citycollectionwidget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    this->setStyleSheet("QWidget{border:none;border-radius:6px;}");

    ui->backwidget->setStyleSheet("QWidget{border:1px solid rgba(255,255,255,0.05);border-radius:6px;background:rgba(255,255,255,1);}");

    ui->lbLeftUpIcon->setStyleSheet("QLabel{border:none;background:transparent;background-image:url(':/res/control_icons/logo.png');}");

    ui->lbLeftUpTitle->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbLeftUpTitle->setText("麒麟天气");

    ui->btnCancel->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(:/res/control_icons/close_black.png);}");

    ui->lbCityCurrent->setStyleSheet("QLabel{border:none;background:transparent;font-size:18px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbCityCurrent->setText("当前城市");

    ui->lbCityCollect->setStyleSheet("QLabel{border:none;background:transparent;font-size:18px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbCityCollect->setText("收藏城市");

    ui->lbCityCount->setStyleSheet("QLabel{border:none;background:transparent;font-size:12px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbCityCount->setText("7/8");
}

CityCollectionWidget::~CityCollectionWidget()
{
    delete ui;
}

void CityCollectionWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
}

void CityCollectionWidget::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
}

void CityCollectionWidget::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
}

void CityCollectionWidget::on_pushButton_clicked()
{
    CityAddition *m_cityaddition = new CityAddition();
    m_cityaddition->show();
    this->hide();
}

void CityCollectionWidget::on_btnCancel_clicked()
{
    this->hide();
}
