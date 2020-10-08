#include "hotcity.h"

HotCity::HotCity(QWidget *parent,QString name, QString code)
    : QPushButton(parent)
{
    QSize size(30,18);
    this->setFixedSize(size);
    this->setText(name);
    cityCode=code;
    connect(this,&HotCity::clicked,this,&HotCity::onHotCityClick);
    this->setStyleSheet("QPushButton{color:#444444;font-size:14px;}"
                        "QPushButton:Hover{color:#3E6CE5;font-size:14px;}"
                        "QPushButton:Pressed{color:#4260C4;font-size:14px;}");
}

void HotCity::onHotCityClick()
{
    emit hotCityClick(cityCode);
}
