#include "citycollectionitem.h"
#include "ui_citycollectionitem.h"

citycollectionitem::citycollectionitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::citycollectionitem)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{background:transparent;border:none;}");

    ui->lbBackImage->setStyleSheet("QWidget{background:rgba(61,107,229,1);border:none;}");

    ui->lbCityName->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(255,255,255,1);}");
    ui->lbCityName->setText("武汉");

    ui->lbTmp->setStyleSheet("QLabel{border:none;background:transparent;font-size:32px;font-weight:400;color:rgba(255,255,255,1);}");
    ui->lbTmp->setText("60");

    ui->lbTmpUnit->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(255,255,255,1);}");
    ui->lbTmpUnit->setText("℃");

    ui->lbwea->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(255,255,255,1);}");
    ui->lbwea->setText("晴");

    ui->btnDelete->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(:/res/control_icons/close_pressed_btn.png);}");
    ui->btnDelete->hide();

    ui->btnAddCity->setStyleSheet("QPushButton{border:0px;background:transparent;}");
    ui->btnAddCity->hide();

    ui->lbAddCity->setStyleSheet("QLabel{border:0px;background:transparent;background-image:url(:/res/control_icons/add.png);}");
    ui->lbAddCity->hide();
}

citycollectionitem::~citycollectionitem()
{
    delete ui;
}

void citycollectionitem::setItemWidgetState(bool isShowNormal)
{

    if (isShowNormal){
        ui->lbCityName->show();
        ui->lbTmp->show();
        ui->lbTmpUnit->show();
        ui->lbwea->show();
        ui->btnAddCity->hide();
        ui->lbAddCity->hide();
        ui->lbBackImage->setStyleSheet("QWidget{background-image:url(:/res/image/weather-clear.png);}");
    }else{
        ui->lbCityName->hide();
        ui->lbTmp->hide();
        ui->lbTmpUnit->hide();
        ui->lbwea->hide();
        ui->btnAddCity->show();
        ui->lbAddCity->show();
    }
}

void citycollectionitem::on_btnAddCity_clicked()
{
    emit showCityAddWiget();
}
