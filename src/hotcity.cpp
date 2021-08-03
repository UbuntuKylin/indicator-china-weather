/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * Authors:
 *  Kobe Lee    lixiang@kylinos.cn/kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
