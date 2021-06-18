#include "hotcity.h"

HotCity::HotCity(QWidget *parent,QString name, QString code)
    : QPushButton(parent)
{
    QSize size(30,18);
    this->setFixedSize(size);
    this->setText(name);
    this->setFocusPolicy(Qt::NoFocus);//设置焦点类型
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
//主题适配
void HotCity::onThemeChanged(QString str)
{
    if("ukui-dark" == str || "ukui-black" == str)
   {
        this->setStyleSheet("QPushButton{color:rgba(255,255,255,0.9);font-size:14px;}"
                        "QPushButton:Hover{color:#3E6CE5;font-size:14px;}"
                        "QPushButton:Pressed{color:#4260C4;font-size:14px;}");
    }
    else if ("ukui-default" == str || "ukui-white" == str || "ukui-light" == str)
    {
        this->setStyleSheet("QPushButton{color:#444444;font-size:14px;}"
                            "QPushButton:Hover{color:#3E6CE5;font-size:14px;}"
                            "QPushButton:Pressed{color:#4260C4;font-size:14px;}");
    }
}
