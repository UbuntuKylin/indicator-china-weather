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

#include "cityaddwidget.h"
#include "ui_cityaddwidget.h"
#include "citycollectionwidget.h"
#include "data.h"

#include <QDebug>

CityAddition::CityAddition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cityaddition)
{
    ui->setupUi(this);

    this->setFixedSize(600, 580);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFocusPolicy(Qt::ClickFocus);//设置焦点类型
    this->setAttribute(Qt::WA_TranslucentBackground);// set window background transparency

    // set style of widget and icon in taskbar
    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6); //set border radius
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    this->setStyleSheet("QWidget{border:none;border-radius:6px;}");
    this->setWindowIcon(QIcon(":/res/control_icons/indicator-china-weather.png") ); //set taskbar icon

//    ui->backwidget->setStyleSheet("QWidget{border:1px solid rgba(207,207,207,1);border-radius:6px;background:rgba(255,255,255,1);}");

    ui->lbLeftUpIcon->setStyleSheet("QLabel{border:none;background:transparent;background-image:url(':/res/control_icons/logo.png');}");
    ui->lbLeftUpIcon->hide();

    ui->lbLeftUpTitle->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(68,68,68,1);}");

    ui->lbLeftUpTitle->hide();

    ui->btnReturn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/return_black.png);}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:#3d6be5;background-image:url(:/res/control_icons/return_white.png);}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:#3257ca;background-image:url(:/res/control_icons/return_white.png);}");
    ui->btnReturn->setFocusPolicy(Qt::NoFocus);

    ui->btnCancel->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(:/res/control_icons/close_black.png);}"
                               "QPushButton:Hover{border:0px;background:transparent;background-color:#E44C50;background-image:url(:/res/control_icons/close_normal_btn.png);}"
                               "QPushButton:Pressed{border:0px;background:transparent;background-color:#E44C50;background-image:url(:/res/control_icons/close_normal_btn.png);}");
    ui->btnCancel->setFocusPolicy(Qt::NoFocus);

    m_cityaddsearchbox = new CityAddSearchBox(this);
    m_cityaddsearchbox->move(45, 93);
    m_cityaddsearchbox->show();

    m_cityaddhotview = new CityAddHotView(this);
    connect(m_cityaddhotview,&CityAddHotView::setHotCity,this,&CityAddition::setHotCity);
    m_cityaddhotview->move(45,193);
    m_cityaddhotview->show();

    m_cityaddsearchview = new CityAddSearchView(ui->backwidget);
    m_cityaddsearchdelegate = new CityAddSearchDelegate(m_cityaddsearchview);
    m_darkcityaddsearchdelegate = new DarkCityAddSearchDelegate(m_cityaddsearchview);
    m_proxyModel = new QSortFilterProxyModel(m_cityaddsearchview);
    m_model = new QStandardItemModel();
    m_cityaddsearchview->move(35, 115);
    m_cityaddsearchview->resize(470,227);
    m_cityaddsearchview->hide();

    connect(m_cityaddsearchview, SIGNAL(requestClearLineEdit() ), this, SLOT(onRequestClearLineEdit()) );
    connect(m_cityaddsearchview, SIGNAL(requestAddNewCity(QString)), this, SIGNAL(requestAddNewCity(QString)) );


    connect(m_cityaddsearchbox, &CityAddSearchBox::textChanged, this, [this] () {
        if (m_cityaddsearchbox->text().size() == 0){
            m_cityaddsearchview->hide();
             m_cityaddhotview->show();
        }else{
            m_cityaddsearchview->show();
            m_cityaddhotview->hide();
            onSearchBoxEdited();
        }
    });

    m_locationWorker = new LocationWorker();
}

CityAddition::~CityAddition()
{
    delete ui;
}

void CityAddition::onSearchBoxEdited()
{
    searchCityName();
   if("ukui-dark" == ThemeStyle || "ukui-black" == ThemeStyle)
   {
        m_cityaddsearchview->setItemDelegate(m_darkcityaddsearchdelegate); //Set delegation for view
   }else if("ukui-default" == ThemeStyle || "ukui-white" == ThemeStyle || "ukui-light" == ThemeStyle)
   {
        m_cityaddsearchview->setItemDelegate(m_cityaddsearchdelegate);
   }
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterRole(Qt::UserRole);
    m_proxyModel->setDynamicSortFilter(true);
    m_cityaddsearchview->setModel(m_proxyModel); //Set up model for delegation
    m_cityaddsearchview->setViewMode(QListView::IconMode); //set icon display for item
    m_cityaddsearchview->setDragEnabled(false); //control does not allow dragging
}

void CityAddition::searchCityName()
{
    const QString inputText = m_cityaddsearchbox->text().trimmed(); //get data from search box
    if (inputText.isEmpty())
        return;

    QList<LocationData> searchResultList;
    searchResultList = m_locationWorker->exactMatchCity(inputText); //match cities in the city list file, and add the matched cities to the list

    if (searchResultList.isEmpty()) {
        qDebug()<<"fail to search city information";
        m_model->clear();//清空上次遗留结果
        m_cityaddsearchview->resize(470,47);//只保留一行
        QStandardItem *Item = new QStandardItem;
        ItemData itemData;

        itemData.cityName = QString("无匹配城市");//提示用户，将内容显示为无匹配城市
        itemData.cityProvince = QString("请重新输入");
        Item->setData(QVariant::fromValue(itemData),Qt::UserRole); //整体存取

        m_model->appendRow(Item); //追加Item
        m_cityaddsearchview->setAttribute(Qt::WA_TransparentForMouseEvents, true);//无结果时点击搜索栏无效果

    }
    else {
        delete m_model;
        m_model = new QStandardItemModel();
        int tempNumsOfCityInSearchResultList = 0;//列表中城市数量计数
        //Access data of each item in the lisearchResultListst, and add the acquired data to listview in turn
        foreach(LocationData m_locationdata, searchResultList){
            tempNumsOfCityInSearchResultList++;
            QStandardItem *Item = new QStandardItem;
            ItemData itemData;

            itemData.cityId = QString(m_locationdata.id); //get id of city
            itemData.cityName = QString(m_locationdata.city); //get the city name
            itemData.cityProvince = QString(m_locationdata.province); //get province of this city
            Item->setData(QVariant::fromValue(itemData),Qt::UserRole); //save city data

            m_model->appendRow(Item); //add item to listview
            m_cityaddsearchview->setAttribute(Qt::WA_TransparentForMouseEvents, false);//有结果时点击搜索栏有效果
        }
        if ( tempNumsOfCityInSearchResultList > 5 )//搜索栏默认大小为5行，搜索结果大于5时用滚轮滚动显示
        {
            m_cityaddsearchview->resize(470,227);
        }
        else if ( tempNumsOfCityInSearchResultList > 0 )//小于5时，有几个结果显示几行
        {
            m_cityaddsearchview->resize(470,tempNumsOfCityInSearchResultList * 45 + 2);
        }
    }
}

void CityAddition::onRequestClearLineEdit()
{
    m_cityaddsearchview->hide(); //hide search and add collect city widget
    m_cityaddsearchbox->setText("");

}

void CityAddition::on_btnCancel_clicked()
{
    emit requestChangeWidgetState();
}

void CityAddition::on_btnReturn_clicked()
{
    emit hideCityAddWiget();
}
//主题适配
void CityAddition::getStr(QString str)
{

    ThemeStyle = str;
    onSearchBoxEdited();
    m_cityaddsearchview->ThemeCitySearchView(str);
    m_cityaddhotview->ThemeCityHotView(str);
    m_cityaddsearchbox->ThemeCitySearchBox(str);

     if("ukui-dark" == str || "ukui-black" == str)
     {

    ui->backwidget->setStyleSheet("QWidget{border:1px solid rgba(38,38,38,0.15);border-radius:6px;background:rgba(31, 32, 34, 1);}");
    ui->btnCancel->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(:/res/control_icons/close_white.png);}"
                               "QPushButton:Hover{border:0px;background:transparent;background-color:#F86457;background-image:url(:/res/control_icons/close_white.png);}"
                               "QPushButton:Pressed{border:0px;background:transparent;background-color:#E44C50;background-image:url(:/res/control_icons/close_white.png);}");
    ui->btnReturn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/return_white.png);}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:#3d6be5;background-image:url(:/res/control_icons/return_white.png);}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:#3257ca;background-image:url(:/res/control_icons/return_white.png);}");
}
     else if("ukui-default" == str || "ukui-white" == str  || "ukui-light" == str)
     {
         ui->backwidget->setStyleSheet("QWidget{border:1px solid rgba(38,38,38,0.15);border-radius:6px;background:rgba(255,255,255,1);}");

         ui->btnCancel->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(:/res/control_icons/close_black.png);}"
                                    "QPushButton:Hover{border:0px;background:transparent;background-color:#F86457;background-image:url(:/res/control_icons/close_white.png);}"
                                    "QPushButton:Pressed{border:0px;background:transparent;background-color:#E44C50;background-image:url(:/res/control_icons/close_white.png);}");

         ui->btnReturn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/return_black.png);}"
                                    "QPushButton:Hover{border:0px;border-radius:4px;background:#3d6be5;background-image:url(:/res/control_icons/return_white.png);}"
                                    "QPushButton:Pressed{border:0px;border-radius:4px;background:#3257ca;background-image:url(:/res/control_icons/return_white.png);}");
     }

}
//鼠标点击外部，收起搜索列表
bool CityAddition::event(QEvent *event)
{
//    if(m_searchView == nullptr)return QWidget::event(event);
    if (event->type() == QEvent::MouseButtonPress)
    {
        m_cityaddsearchbox->clear();
    }
    return QWidget::event(event);
}
