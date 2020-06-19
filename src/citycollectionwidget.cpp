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

#include "citycollectionwidget.h"
#include "ui_citycollectionwidget.h"
#include "cityaddwidget.h"
#include "citycollectionitem.h"

#include <QFile>
#include <QApplication>
#include <QStringList>
#include <QThread>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QEventLoop>
#include <QFile>
#include <QApplication>
#include <QPainterPath>
#include <QStringList>
#include <QUrl>
#include <QVariant>

CityCollectionWidget::CityCollectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::citycollectionwidget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground); // set window background transparency

    // set style of widget and icon in taskbar
    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6); //set border radius
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    this->setStyleSheet("QWidget{border:none;border-radius:6px;}");
    this->setWindowIcon(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/control_icons/indicator-china-weather.png")) ); //set taskbar icon

    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    this->move((availableGeometry.width()-this->width())/2, (availableGeometry.height()-this->height())/2);

    ui->backwidget->setStyleSheet("QWidget{border:1px solid rgba(207,207,207,1);border-radius:6px;background:rgba(255,255,255,1);}");

    ui->lbLeftUpIcon->setStyleSheet("QLabel{border:none;background:transparent;background-image:url(':/res/control_icons/logo.png');}");

    ui->lbLeftUpTitle->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbLeftUpTitle->setText("麒麟天气");

    ui->btnCancel->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(:/res/control_icons/close_black.png);}"
                               "QPushButton:Hover{border:0px;background:transparent;background-image:url(:/res/control_icons/close_hover_btn.png);}"
                               "QPushButton:Pressed{border:0px;background:transparent;background-image:url(:/res/control_icons/close_pressed_btn.png);}");
    ui->btnCancel->setFocusPolicy(Qt::NoFocus);

    ui->lbCityCurrent->setStyleSheet("QLabel{border:none;background:transparent;font-size:18px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbCityCurrent->setText("当前城市");

    ui->lbCityCollect->setStyleSheet("QLabel{border:none;background:transparent;font-size:18px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbCityCollect->setText("收藏城市");

    ui->lbCityCount->setStyleSheet("QLabel{border:none;background:transparent;font-size:12px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbCityCount->setText("0/8");
    m_citynumber = 0;

    m_tipIcon = new QLabel(this);
    m_tipIcon->setFixedSize(127 ,93);
    m_tipIcon->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    m_tipIcon->setPixmap(QPixmap(":/res/control_icons/unlink.png"));
    m_tipIcon->move((this->width()-m_tipIcon->width())/2, 180);
    m_tipIcon->hide();
    m_tipLabel = new QLabel(this);
    m_tipLabel->setFixedWidth(this->width());
    m_tipLabel->setWordWrap(true);
    m_tipLabel->setAlignment(Qt::AlignCenter);
    m_tipLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    m_tipLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_tipLabel->setText("当前网络异常，请检查网络设置");//Current Network Exception, Please Check Network Settings
    m_tipLabel->move(0, 300);
    m_tipLabel->hide();

    m_cityaddition = new CityAddition(this);
    m_cityaddition->move(0, 0);
    m_cityaddition->hide();
    connect(m_cityaddition, SIGNAL(hideCityAddWiget()), this, SLOT(onHideCityAddWiget()) );
    connect(m_cityaddition, SIGNAL(requestChangeWidgetState()), this, SIGNAL(requestChangeWidgetState()) );
    connect(m_cityaddition, SIGNAL(requestAddNewCity(QString)), this, SLOT(onRequestAddNewCity(QString)) );

    m_networkManager = new QNetworkAccessManager(this);
}

CityCollectionWidget::~CityCollectionWidget()
{
    delete ui;
}

void CityCollectionWidget::onRequestSetCityWeather(QString weather_data)
{
    if (weather_data != "") {
        QStringList strList = weather_data.split(";");

        if (isAddCity) {
            //start moving control if task is add a new collect city
            if (strList.size() <= 2) {
                return;
            }

            int pos = strList.size() - 2; //the order number of last city in list
            QString addCityWeatherData = strList.at(pos); //get weather data for new cities

            QList<citycollectionitem *> cityItemList = ui->backwidget->findChildren<citycollectionitem *>();

            int itemNum = cityItemList.size();

            citycollectionitem *lastCityItem = cityItemList.at(itemNum-1); 
            delete lastCityItem; //delete last item
            itemNum -= 1;

            if (itemNum <= 1) {
                showCollectCity(35, 242, true, addCityWeatherData); //add a new collection city
                showCollectCity(35 + 170, 242, false, ""); //create add collect city item
            }

            if (itemNum > 1 && itemNum < 9) {
                int row(0), column(0);
                for (int i = 1;i < itemNum; i ++) {
                    column += 1;
                    if (column == 3) {
                        column = 0;
                        row += 1;
                    }
                }
                showCollectCity(35 + column*170, 242 + row*100, true, addCityWeatherData); //add a new collection city

                column += 1;
                if (column == 3) {
                    column = 0;
                    row += 1;
                }
                showCollectCity(35 + column*170, 242 + row*100, false, ""); //create add collect city item
            }

            if (itemNum >= 9) {
                //replace last collect city as new add city
                QList<citycollectionitem *> m_cityItemList = ui->backwidget->findChildren<citycollectionitem *>();
                citycollectionitem *m_lastCityItem = m_cityItemList.at(m_cityItemList.size() - 1); //delete last item
                delete m_lastCityItem;
                showCollectCity(35 + 1*170, 242 + 2*100, true, addCityWeatherData); //add a new collection city
                showCollectCity(35 + 2*170, 242 + 2*100, false, ""); //create add collect city item
            }

            isAddCity = false;
        } else {
            m_citynumber = strList.size()-2;
            QString citynumber = QString::number(m_citynumber) + "/8";
            ui->lbCityCount->setText(citynumber); //show number of collected cities

            int row = 0; //current row
            int column = 0; //cuerrent column
            for (int i=0; i< strList.size()-1; i++) {
                QString eachCityData = strList.at(i); //get real-time weather data of each city
                ObserveWeather observeweather;
                QJsonObject m_json;
                if (!eachCityData.isEmpty()) {
                    QStringList eachKeyList = eachCityData.split(",");
                    foreach (QString strKey, eachKeyList) {
                        if (!strKey.isEmpty()) {
                            m_json.insert(strKey.split("=").at(0), strKey.split("=").at(1)); //change data to json format
                        }
                    }
                }

                observeweather.tmp = m_json.value("tmp").toString();
                observeweather.cond_txt = m_json.value("cond_txt").toString();
                observeweather.cond_code = m_json.value("cond_code").toString();
                observeweather.id = m_json.value("id").toString();
                observeweather.city = m_json.value("location").toString();

                if (i==0) { //current city
                    citycollectionitem *m_currentcity = new citycollectionitem(ui->backwidget);
                    m_currentcity->move(35, 81);
                    m_currentcity->setItemWidgetState(true, true, m_citynumber);
                    m_currentcity->setCityWeather(observeweather);
                    m_currentcity->show();
                    connect(m_currentcity, SIGNAL(requestDeleteCity(QString)), this, SLOT(onRequestDeleteCity(QString)) );
                    connect(m_currentcity, SIGNAL(changeCurrentCity(QString)), this, SLOT(onChangeCurrentCity(QString)) );
                } else { //collected city
                    citycollectionitem *m_collecity = new citycollectionitem(ui->backwidget);
                    m_collecity->move(35 + column*170, 242 + row*100); //m_currentcity->move(35 + j*170, 242 + i*100);
                    m_collecity->setItemWidgetState(true, false, m_citynumber);
                    m_collecity->setCityWeather(observeweather);
                    m_collecity->show();
                    connect(m_collecity, SIGNAL(requestDeleteCity(QString)), this, SLOT(onRequestDeleteCity(QString)) );
                    connect(m_collecity, SIGNAL(changeCurrentCity(QString)), this, SLOT(onChangeCurrentCity(QString)) );

                    column += 1;
                    if (column == 3){
                        column = 0;
                        row += 1;
                    }
                }
            } //end for (int i=0; i< strList.size()-1; i++)
            //add collect city item
            citycollectionitem *m_lastitem = new citycollectionitem(ui->backwidget);
            m_lastitem->move(35 + column*170, 242 + row*100);
            m_lastitem->setItemWidgetState(false, false, m_citynumber);
            m_lastitem->show();
            connect(m_lastitem, SIGNAL(showCityAddWiget()), this, SLOT(onShowCityAddWiget()) );
        }
    } //end if (weather_data != "")
}

void CityCollectionWidget::showCollectCity(int x, int y, bool isShowNormal, QString weatherStr)
{
    //首先将 weatherStr中天气数据保存在ObserveWeather结构体中
    ObserveWeather observeweather;
    if (!weatherStr.isEmpty()) {
        QJsonObject m_json;
        if (!weatherStr.isEmpty()) {
            QStringList eachKeyList = weatherStr.split(",");
            foreach (QString strKey, eachKeyList) {
                if (!strKey.isEmpty()) {
                    //等号左边为键，右边为值
                    m_json.insert(strKey.split("=").at(0), strKey.split("=").at(1));
                }
            }
        }
        observeweather.tmp = m_json.value("tmp").toString();
        observeweather.cond_txt = m_json.value("cond_txt").toString();
        observeweather.cond_code = m_json.value("cond_code").toString();
        observeweather.id = m_json.value("id").toString();
        observeweather.city = m_json.value("location").toString();
    }

    citycollectionitem *m_currentcity = new citycollectionitem(ui->backwidget);
    m_currentcity->move(x, y); //m_currentcity->move(35 + j*170, 242 + i*100);
    m_currentcity->setItemWidgetState(isShowNormal, false, m_citynumber);
    if (isShowNormal) {
        m_currentcity->setCityWeather(observeweather);
    }
    m_currentcity->show();
    //m_currentcity->setCurrentWeather(cityId);
    connect(m_currentcity, SIGNAL(showCityAddWiget()), this, SLOT(onShowCityAddWiget()) );
    connect(m_currentcity, SIGNAL(requestDeleteCity(QString)), this, SLOT(onRequestDeleteCity(QString)) );
    connect(m_currentcity, SIGNAL(changeCurrentCity(QString)), this, SLOT(onChangeCurrentCity(QString)) );
}

void CityCollectionWidget::onRequestAddNewCity(QString cityId)
{
    m_cityaddition->hide(); //隐藏 搜索及添加城市窗口

    //判断已收藏城市中是否已经有需要添加的城市
    QString strSavedCity = readCollectedCity();
    QStringList listSavedCityId = strSavedCity.split(",");
    foreach (QString strCityId, listSavedCityId) {
        if (strCityId == cityId) { //若收藏城市列表中已有需要添加的城市，则返回
            return;
        }
    }

    //将新增城市写入列表
    if (listSavedCityId.size() == 10){ //包含最后一项为空字符串的项
        listSavedCityId.replace(8, cityId); //收藏城市已经有8个，替换最后一个收藏城市
    }else {
        listSavedCityId.append(cityId); //若收藏城市未满8个,将新添加的城市加到最后
        m_citynumber += 1;
        QString citynumber = QString::number(m_citynumber) + "/8";
        ui->lbCityCount->setText(citynumber);
        if (m_citynumber == 1) {
            QList<citycollectionitem *> cityitemlist = ui->backwidget->findChildren<citycollectionitem *>();
            citycollectionitem *firstitem = cityitemlist.at(0);
            firstitem->setItemWidgetState(true, false, m_citynumber);
        }
    }

    QString newStrCityId = "";
    foreach(QString str, listSavedCityId){
        if (str != ""){
            newStrCityId.append(str);
            newStrCityId.append(",");
        }
    }

    writeCollectedCity(newStrCityId);
    isAddCity = true;
    emit requestShowCollCityWeather(); //发信号从网络获取数据用于显示城市天气
}

void CityCollectionWidget::onRequestDeleteCity(QString cityId)
{
    qDebug()<<"debug: city id = "<<cityId;

    QString strSavedCity = readCollectedCity();
    QStringList listSavedCityId = strSavedCity.split(",");

    //若收藏窗口只有当前城市，不能删掉当前城市
    if (listSavedCityId.size() == 2) {
        return;
    }

    m_citynumber -= 1;
    QString citynumber = QString::number(m_citynumber) + "/8";
    ui->lbCityCount->setText(citynumber);

    //删掉对应的城市
    QList<citycollectionitem *> cityItemList = ui->backwidget->findChildren<citycollectionitem *>();
    for(int i = 0;i < cityItemList.size(); i ++){
        citycollectionitem *cityItem = cityItemList.at(i);
        if (i == 0) {
            if (cityItem->m_city_id == cityId) { //说明删除的是当前城市，以第一个收藏城市代替
                emit sendCurrentCityId(listSavedCityId.at(1)); //发信号更新主界面
                delete cityItem;
            }
        } else {
            if (cityItem->m_city_id == cityId) {
                delete cityItem;
            }
        }
    }

    //重新排列现有收藏城市
    int row(0), column(0);
    QList<citycollectionitem *> newCityItemList = ui->backwidget->findChildren<citycollectionitem *>();
    for(int i = 0;i < newCityItemList.size(); i ++){
        citycollectionitem *newCityItem = newCityItemList.at(i);
        if (i == 0) {
            newCityItem->move(35, 81); //当前城市
            newCityItem->setItemWidgetState(true, true, m_citynumber);
        } else {
            newCityItem->move(35 + column*170, 242 + row*100); //收藏城市
            column += 1;
            if (column == 3) {
                column = 0;
                row += 1;
            }
        }
    }

    //更新城市列表
    bool isflag = listSavedCityId.removeOne(cityId); //从列表中删除预删除的城市
    if (isflag) {
        qDebug()<<"delete one element from collected city list successfully";
        QString newStrCityId = "";
        foreach(QString str, listSavedCityId){
            if (str != ""){
                newStrCityId.append(str);
                newStrCityId.append(",");
            }
        }
        writeCollectedCity(newStrCityId); //将更新后的列表写入配置文件
    } else {
        qDebug()<<"delete one element from collected city list failed";
    }
}

void CityCollectionWidget::onChangeCurrentCity(QString cityId)
{
    emit sendCurrentCityId(cityId); //发信号更新主界面

    //QList<citycollectionitem *> cityItemList = ui->backwidget->findChildren<citycollectionitem *>();
    //foreach (citycollectionitem *cityItem, cityItemList) {
    //    delete cityItem;
    //}

    //更新城市列表
    QString strSavedCity = readCollectedCity();
    QStringList listSavedCityId = strSavedCity.split(",");

    QString firstId = listSavedCityId.at(0); //获取当前城市的id
    listSavedCityId.replace(0, cityId); //将 cityId设为第一个城市，即当前城市

    for (int i=1;i<listSavedCityId.size();i++){
        if (cityId == listSavedCityId.at(i)){
            listSavedCityId.replace(i, firstId); //如果收藏城市中有id和cityId相同的城市，将该城市id设为firstId
        }
    }

    QString newStrCityId = "";
    foreach(QString str, listSavedCityId){
        if (str != ""){
            newStrCityId.append(str);
            newStrCityId.append(",");
        }
    }

    writeCollectedCity(newStrCityId);

    emit requestChangeWidgetState(); //隐藏显示收藏城市列表窗口
    //emit requestShowCollCityWeather();
}

void CityCollectionWidget::writeCollectedCity(QString cityId)
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString collectPath = homePath.at(0) + "/.config/china-weather-data";

    QFile file(collectPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // open file succcessfully
        file.write(cityId.toUtf8());
        file.close();
    } else {
        //print information if write data failed
        qDebug()<<"Can not write city id data to ~/.config/china-weather-data";
    }
}

QString CityCollectionWidget::readCollectedCity()
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString collectPath = homePath.at(0) + "/.config/china-weather-data";

    QString readCityId;
    QFile file(collectPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // open file succcessfully
        QByteArray cityId = file.readAll();
        readCityId = (QString(cityId));
        file.close();
    } else {
        // using the following data if open file failed
        readCityId = "101010100,101020100,101030100,101040100,101280101,101280601,";
    }

    return readCityId;
}

void CityCollectionWidget::onShowCityAddWiget()
{
    m_cityaddition->show();
    m_cityaddition->raise();
}

void CityCollectionWidget::onHideCityAddWiget()
{
    m_cityaddition->hide();
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

void CityCollectionWidget::on_btnCancel_clicked()
{
    emit requestChangeWidgetState();
}
