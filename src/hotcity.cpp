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
    this->setStyleSheet("QPushButton{color:#444444;font-size:14px;background:transparent;}"
                        "QPushButton:Hover{color:#3E6CE5;font-size:14px;background:transparent;}"
                        "QPushButton:Pressed{color:#4260C4;font-size:14px;background:transparent;}");
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
        this->setStyleSheet("QPushButton{color:rgba(255,255,255,0.9);font-size:14px;background:transparent}"
                            "QPushButton:Hover{color:#3E6CE5;font-size:14px;background:transparent;border:none}"
                            "QPushButton:Pressed{color:#4260C4;font-size:14px;background:transparent;border:none}"
                            );
    }
//    else if ("ukui-default" == str || "ukui-white" == str || "ukui-light" == str)
    else
    {
        this->setStyleSheet("QPushButton{color:rgba(0,0,0,1);font-size:14px;border:none}"
//        this->setStyleSheet("QPushButton{color:#444444;font-size:14px;}"
//                            "QPushButton:Hover{color:#3E6CE5;font-size:14px;background:transparent}"
                            "QPushButton:Hover{color:#3E6CE5;font-size:14px;border:none;background:transparent}"
//                            "QPushButton:Hover{color:rgba(0,0,0,1);font-size:14px;background:transparent}"
                            "QPushButton:Pressed{color:#4260C4;font-size:14px;background:transparent;border:none}");
    }
}
