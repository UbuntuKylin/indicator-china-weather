/*
 * Copyright (C) 2013 ~ 2020 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
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

#include "leftupsearchview.h"
#include "weatherworker.h"

#include <QScrollBar>

LeftUpSearchView::LeftUpSearchView(QWidget *parent)
    : QListView(parent)
{
    this->setMouseTracking(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    this->setStyleSheet("QListView{margin:0px;padding:0px;border:none;border-radius:4px;outline:none;background-color:rgba(255,255,255,1);}"
                        "QListView::item:selected:!active{background-color:rgba(255,255,255,0.25);opacity:0.25}"
                        "QListView::item:selected:active{background-color:rgba(255,255,255,0.25);opacity:0.25;}"
                        "QListView::item:hover{background-color:rgba(255,255,255,0.1);opacity:0.1;}");
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background-color:transparent;border:0px;width:1px;}" \
                                             "QScrollBar::sub-line:vertical{subcontrol-origin:margin;border:none;height:6px;}" \
                                             "QScrollBar::up-arrow:vertical{subcontrol-origin:margin;background-color:transparent;height:6px;}" \
                                             "QScrollBar::sub-page:vertical{background-color:transparent;}" \
                                             "QScrollBar::handle:vertical{background-color:transparent;width:1px;}" \
                                             "QScrollBar::handle:vertical:hover{background-color:transparent;width:1px;}" \
                                             "QScrollBar::handle:vertical:pressed{background-color:transparent;width:1px;}" \
                                             "QScrollBar::add-page:vertical{background-color:transparent;}" \
                                             "QScrollBar::down-arrow:vertical{background-color:transparent;}" \
                                             "QScrollBar::add-line:vertical{subcontrol-origin:margin;border:none;height:6px;}");
}

LeftUpSearchView::~LeftUpSearchView()
{

}

void LeftUpSearchView::mouseReleaseEvent(QMouseEvent *e)
{
    send();
    return QListView::mouseReleaseEvent(e);
}

void LeftUpSearchView::send(){
    QModelIndexList sourceIndexList = this->selectionModel()->selectedIndexes();
    foreach (QModelIndex sourceIndex, sourceIndexList) {
        QVariant variant = sourceIndex.data(Qt::UserRole);
        ItemData data = variant.value<ItemData>();
        requestWeatherData(data.cityId);
    }
}

void LeftUpSearchView::requestWeatherData(QString cityId) {
    if (!cityId.isEmpty()) {
        m_cityid = cityId;
    } else {
        //如果为空时默认显示北京(101010100)天气
        m_cityid = "101010100";
    }

    emit requestSetNewCityWeather(m_cityid);
}
//主题适配
void LeftUpSearchView::ThemeLeftUpSearchView(QString str)
{
        if("ukui-dark" == str || "ukui-black" == str)
        {
            // 改下拉列表属性
       this->setStyleSheet("QListView{margin:0px;padding:0px;border:none;border-radius:6px;outline:none;background-color:rgba(255,255,255,0.2);}"
                           "QListView::item:selected:!active{background-color:rgba(255,255,255,0.2);border-radius:6px;}"
                           "QListView::item:selected:active{background-color:rgba(255,255,255,0.25);border-radius:6px;}"
                           "QListView::item:hover{background-color:rgba(255,255,255,0.1);border-radius:6px;}");
       this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background-color:transparent;border:0px;width:1px;}" \
                                                "QScrollBar::sub-line:vertical{subcontrol-origin:margin;border:none;height:6px;}" \
                                                "QScrollBar::up-arrow:vertical{subcontrol-origin:margin;background-color:transparent;height:6px;}" \
                                                "QScrollBar::sub-page:vertical{background-color:transparent;}" \
                                                "QScrollBar::handle:vertical{background-color:transparent;width:1px;}" \
                                                "QScrollBar::handle:vertical:hover{background-color:transparent;width:1px;}" \
                                                "QScrollBar::handle:vertical:pressed{background-color:transparent;width:1px;}" \
                                                "QScrollBar::add-page:vertical{background-color:transparent;}" \
                                                "QScrollBar::down-arrow:vertical{background-color:transparent;}" \
                                                "QScrollBar::add-line:vertical{subcontrol-origin:margin;border:none;height:6px;}");

    }else if("ukui-default" == str || "ukui-white" == str || "ukui-light" == str)
    {
            // 改下拉列表属性
        this->setStyleSheet("QListView{margin:0px;padding:0px;border:none;border-radius:6px;outline:none;background-color:rgba(255,255,255,0.2);}"
                            "QListView::item:selected:!active{background-color:rgba(255,255,255,0.2);border-radius:6px;}"
                            "QListView::item:selected:active{background-color:rgba(255,255,255,0.25);border-radius:6px;}"
                            "QListView::item:hover{background-color:rgba(255,255,255,0.1);border-radius:6px;}");
        this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background-color:transparent;border:0px;width:1px;}" \
                                                 "QScrollBar::sub-line:vertical{subcontrol-origin:margin;border:none;height:6px;}" \
                                                 "QScrollBar::up-arrow:vertical{subcontrol-origin:margin;background-color:transparent;height:6px;}" \
                                                 "QScrollBar::sub-page:vertical{background-color:transparent;}" \
                                                 "QScrollBar::handle:vertical{background-color:transparent;width:1px;}" \
                                                 "QScrollBar::handle:vertical:hover{background-color:transparent;width:1px;}" \
                                                 "QScrollBar::handle:vertical:pressed{background-color:transparent;width:1px;}" \
                                                 "QScrollBar::add-page:vertical{background-color:transparent;}" \
                                                 "QScrollBar::down-arrow:vertical{background-color:transparent;}" \
                                                 "QScrollBar::add-line:vertical{subcontrol-origin:margin;border:none;height:6px;}");
    }
}
void LeftUpSearchView::dealSearchBoxKeyPress(QString str){
    if(this->isHidden()) return ;
    if(str == "up"){
        if(-1 == this->currentIndex().row()){

            setCurrentIndex(model()->index(model()->rowCount() - 1,0));
        }else{
//            this->model()->index(this->model()->rowCount() - 1, 0);
            this->setCurrentIndex(model()->index(this->currentIndex().row() - 1,0));
        }
    }else if(str == "down"){
        if(this->currentIndex().row() == -1){
            setCurrentIndex(model()->index(0,0));
        }else{
            this->setCurrentIndex(model()->index(this->currentIndex().row() + 1,0));
        }
    }else if(str == "enter"){
        send();
    }
    return ;
}

