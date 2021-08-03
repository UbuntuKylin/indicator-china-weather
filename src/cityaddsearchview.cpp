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

#include "cityaddsearchview.h"

#include <QScrollBar>
#include <QDebug>

// 设置收藏城市搜索框下拉列表里面item的显示样式

CityAddSearchView::CityAddSearchView(QWidget *parent)
    : QListView(parent)
{
    this->setMouseTracking(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFocusPolicy(Qt::NoFocus);//设置焦点类型

    //set style of listview widget
    this->setStyleSheet("QListView{margin:0px;padding:0px;border:1px solid rgba(207,207,207,1);border-radius:4px;outline:none;background:rgba(255,255,255,1);}"
                        "QListView::item:selected:active{background:rgba(238,242,253,1);}"
                        "QListView::item:hover{background:rgba(238,242,253,1);}");

    this->verticalScrollBar()->setProperty("drawScrollBarGroove",false);

    //set style of scroll bar
//    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background-color:rgb(255,255,255,100);border:0px;width:2px;}" \
//                                             "QScrollBar::sub-line:vertical{subcontrol-origin:margin;border:1px solid red;height:13px;}" \
//                                             "QScrollBar::up-arrow:vertical{subcontrol-origin:margin;background-color:blue;height:13px;}" \
//                                             "QScrollBar::sub-page:vertical{background-color:#EEEDF0;}" \
//                                             "QScrollBar::handle:vertical{background-color:#D1D0D2;width:10px;}" \
//                                             "QScrollBar::handle:vertical:hover{background-color:#14ACF5;width:10px;}" \
//                                             "QScrollBar::handle:vertical:pressed{background-color:#0B95D7;width:10px;}" \
//                                             "QScrollBar::add-page:vertical{background-color:#EEEDF0;}" \
//                                             "QScrollBar::down-arrow:vertical{background-color:yellow;}" \
//                                             "QScrollBar::add-line:vertical{subcontrol-origin:margin;border:1px solid green;height:18px;}");
}

CityAddSearchView::~CityAddSearchView()
{

}

void CityAddSearchView::mouseReleaseEvent(QMouseEvent *e)
{
    send();
    return QListView::mouseReleaseEvent(e);
}

void CityAddSearchView::send(){
    QModelIndexList sourceIndexList = this->selectionModel()->selectedIndexes();

    foreach (QModelIndex sourceIndex, sourceIndexList){
        QVariant variant = sourceIndex.data(Qt::UserRole);
        ItemData data = variant.value<ItemData>(); //get data of the selected item
        qDebug() << "Index : " << sourceIndex.row(); //show contents of the selected item
        emit requestClearLineEdit();
        emit requestAddNewCity(data.cityId);
    }
    return ;
}

void CityAddSearchView::dealSearchBoxKeyPress(QString str){
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
//主题适配
void CityAddSearchView::ThemeCitySearchView(QString str)
{
    if("ukui-dark" == str || "ukui-black" == str)
    {
   this->setStyleSheet("QListView{margin:0px;padding:0px;border-radius:4px;outline:none;background:rgba(48,48,51,1);}"
                       "QListView::item:selected:active{background:rgba(238,242,253,1);}"
                       "QListView::item:hover{background:rgba(238,242,253,1);}");
//   this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background-color:rgb(255,255,255,100);border:0px;width:2px;}" \
//                                            "QScrollBar::sub-line:vertical{subcontrol-origin:margin;border:1px solid red;height:13px;}" \
//                                            "QScrollBar::up-arrow:vertical{subcontrol-origin:margin;background-color:blue;height:13px;}" \
//                                            "QScrollBar::sub-page:vertical{background-color:#303033;}" \
//                                            "QScrollBar::handle:vertical{background-color:#303033;width:10px;}" \
//                                            "QScrollBar::handle:vertical:hover{background-color:#303033;width:10px;}" \
//                                            "QScrollBar::handle:vertical:pressed{background-color:#303033;width:10px;}" \
//                                            "QScrollBar::add-page:vertical{background-color:#303033;}" \
//                                            "QScrollBar::down-arrow:vertical{background-color:yellow;}" \
//                                            "QScrollBar::add-line:vertical{subcontrol-origin:margin;border:1px solid green;height:18px;}");


}
//    else if("ukui-default" == str || "ukui-white" == str  || "ukui-light" == str)
    else
    {
        this->setStyleSheet("QListView{margin:0px;padding:0px;border:1px solid rgba(207,207,207,1);border-radius:4px;outline:none;background:rgba(255,255,255,1);}"
//                            "QListView::item:selected:active{background:rgba(238,242,253,1);}"
                            "QListView::item:selected:active{background:#3790fa;}"
//                            "QListView::item:hover{background:rgba(238,242,253,1);}"
                            "QListView::item:hover{background:#b5d0f0;}"
                            );
//        this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background-color:rgb(255,255,255,100);border:0px;width:2px;}" \
//                                                 "QScrollBar::sub-line:vertical{subcontrol-origin:margin;border:1px solid red;height:13px;}" \
//                                                 "QScrollBar::up-arrow:vertical{subcontrol-origin:margin;background-color:blue;height:13px;}" \
//                                                 "QScrollBar::sub-page:vertical{background-color:#EEEDF0;}" \
//                                                 "QScrollBar::handle:vertical{background-color:#D1D0D2;width:10px;}" \
//                                                 "QScrollBar::handle:vertical:hover{background-color:#14ACF5;width:10px;}" \
//                                                 "QScrollBar::handle:vertical:pressed{background-color:#0B95D7;width:10px;}" \
//                                                 "QScrollBar::add-page:vertical{background-color:#EEEDF0;}" \
//                                                 "QScrollBar::down-arrow:vertical{background-color:yellow;}" \
//                                                 "QScrollBar::add-line:vertical{subcontrol-origin:margin;border:1px solid green;height:18px;}");
    }
}
