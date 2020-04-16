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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<ObserveWeather>();
    qRegisterMetaType<ForecastWeather>();
    qRegisterMetaType<LifeStyle>();

    checkSingle();

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setFocusPolicy(Qt::StrongFocus);//this->setFocusPolicy(Qt::NoFocus);
    this->setWindowTitle(tr("Kylin Weather"));
    //this->setWindowIcon(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/control_icons/indicator-china-weather.png")) );

    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    this->setStyleSheet("QWidget{border:none;border-radius:6px;}");

    this->initControlQss(); //设置其他控件样式

    this->createTrayIcon(); //创建托盘图标
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    ui->widget_normal->show();

    //左上角按钮
    m_leftupcitybtn = new LeftUpCityBtn(ui->widget_normal);
    m_leftupcitybtn->move(20, 22);
    m_leftupcitybtn->show();

    //左上角搜索框
    m_leftupsearchbox = new LeftUpSearchBox(ui->widget_normal);
    m_leftupsearchbox->move(116, 17);
    m_leftupsearchbox->show();

    //主界面搜索列表
    m_searchView = new LeftUpSearchView(ui->widget_normal);
    m_delegate = new LeftUpSearchDelegate(m_searchView);
    m_proxyModel = new QSortFilterProxyModel(m_searchView);
    m_model = new QStandardItemModel();
    m_searchView->move(116, 49);
    m_searchView->resize(181,254);
    m_searchView->hide();

    connect(m_leftupsearchbox, &LeftUpSearchBox::textChanged, this, [this] () {
        if (m_leftupsearchbox->text().size() == 0){
            m_searchView->hide();
        }else{
            m_searchView->show();
            onSearchBoxEdited();
        }
    });

    connect(m_searchView, &LeftUpSearchView::requestSetObserveWeather, this, [=] (ObserveWeather observerdata) {
        this->onSetObserveWeather(observerdata);
    });
    connect(m_searchView, &LeftUpSearchView::requestSetForecastWeather, this, [=] (ForecastWeather forecastweather) {
        this->onSetForecastWeather(forecastweather);
    });
    connect(m_searchView, &LeftUpSearchView::requestSetLifeStyle, this, [=] (LifeStyle lifestyle) {
        this->onSetLifeStyle(lifestyle);
    });
    connect(m_searchView, SIGNAL(requestSetCityName(QString)), m_leftupcitybtn, SIGNAL(requestSetCityName(QString)) );

    connect(m_leftupcitybtn, &LeftUpCityBtn::sendCurrentCityId, this, [=] (QString id) {
        m_searchView->requestWeatherData(id);
    });

    m_locationWorker = new LocationWorker(this);
    m_weatherManager = new WeatherManager(this);

    //开始测试网络情况
    m_weatherManager->startTestNetwork();

    //根据获取到网络探测的结果分别处理
    connect(m_weatherManager, &WeatherManager::nofityNetworkStatus, this, [=] (const QString &status) {
        if (status == "OK") {
            qDebug()<<"debug: 网络正常";

            //CN101010100,beijing,北京,CN,China,中国
            QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
            QString collectPath = homePath.at(0) + "/.config/china-weather-data";
            if (!isFileExist(collectPath)){ //文件不存在时默认设置为北京
                m_searchView->requestWeatherData("101010100");

                QFile file(collectPath);
                file.open(QIODevice::WriteOnly | QIODevice::Text);
                file.write("101010100,");
                file.close();
            } else {//文件存在时根据保存的默认城市进行设置
                QFile file(collectPath);
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QByteArray cityId = file.readAll();
                QString readCityId = (QString(cityId));
                file.close();

                QStringList listCityId = readCityId.split(",");
                m_searchView->requestWeatherData(listCityId.at(0));
            }
        } else {
            if (status == "Fail") {//物理网线未连接
                qDebug()<<"debug: 物理网线未连接";
            } else {//互联网无法ping通
                qDebug()<<"debug: 互联网无法ping通";
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkSingle()
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString lockPath = homePath.at(0) + "/.config/china-weather-lock";
    int fd = open(lockPath.toUtf8().data(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fd < 0) { exit(1); }

    if (lockf(fd, F_TLOCK, 0)) {
        qDebug()<<"Can't lock single file, indicator-china-weather is already running!";
        exit(0);
    }
}

bool MainWindow::isFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}

void MainWindow::initControlQss()
{
    ui->centralwidget->setStyleSheet("#centralwidget{border:1px solid rgba(255,255,255,0.05);border-radius:6px;background:rgba(19,19,20,0);}");
    ui->centralwidget->setStyleSheet("#centralwidget{color:white;background-image:url(':/res/background/weather-clear.png');background-repeat:no-repeat;}");

    ui->btnMinimize->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/min_normal_btn.png);}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/min_hover_btn.png);}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/min_pressed_btn.png);}");

    ui->btnCancel->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/close_normal_btn.png);}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/close_hover_btn.png);}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/close_pressed_btn.png);}");

    ui->lbCurrTmp->setStyleSheet("QLabel{border:none;background:transparent;font-size:110px;color:rgba(255,255,255,1);line-height:80px;}");
    ui->lbCurrTmp->setAlignment(Qt::AlignCenter);
    //ui->lbCurrTmp->setText("12");
    ui->lbCurrTmpUnit->setStyleSheet("QLabel{border:none;background:transparent;font-size:20px;color:rgba(255,255,255,1);line-height:14px;}");
    ui->lbCurrTmpUnit->setAlignment(Qt::AlignCenter);
    //ui->lbCurrTmpUnit->setText("℃");
    ui->lbCurrWea->setStyleSheet("QLabel{border:none;background:transparent;font-size:20px;color:rgba(255,255,255,1);line-height:14px;}");
    ui->lbCurrWea->setAlignment(Qt::AlignCenter);
    //ui->lbCurrWea->setText("多云");
    ui->lbCurrHum->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,1);line-height:14px;}");
    ui->lbCurrHum->setAlignment(Qt::AlignCenter);
    //ui->lbCurrHum->setText("湿度 98%   东南风 1级");


    m_scrollarea = new QScrollArea(ui->centralwidget);
    m_scrollarea->setFixedSize(858, 220);
    m_scrollarea->move(4, 290);
    m_scrollarea->setStyleSheet("QScrollArea{border:none;border-radius:4px;background:transparent;color:rgba(255,255,255,1);}");
    m_scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_scrollarea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 2px 0px 2px;width:10px;background:rgba(255,255,255,0);border-radius:6px;}"
                                                     "QScrollBar::up-arrow:vertical{height:0px;}"
                                                     "QScrollBar::sub-line:vertical{border:0px solid;height:0px}"
                                                     "QScrollBar::sub-page:vertical{background:transparent;}"
                                                     "QScrollBar::handle:vertical{width:6px;background:rgba(255,255,255,0.2);border-radius:3px;}"
                                                     "QScrollBar::handle:vertical:hover{width:6px;background:rgba(255,255,255,0.2);border-radius:3px;}"
                                                     "QScrollBar::handle:vertical:pressed{width:6px;background:rgba(255,255,255,0.2);border-radius:3px;}"
                                                     "QScrollBar::add-page:vertical{background:transparent;}"
                                                     "QScrollBar::add-line:vertical{border:0px solid;height:0px}"
                                                     "QScrollBar::down-arrow:vertical{height:0px;}");

    m_scrollwidget = new QWidget(m_scrollarea);
    m_scrollwidget->resize(858, 450);
    m_scrollwidget->setStyleSheet("QWidget{border:none;border-radius:4px;background:transparent;color:rgba(255,255,255,1);}");
    m_scrollarea->setWidget(m_scrollwidget);
    m_scrollwidget->move(0, 0);

    m_information = new Information(m_scrollwidget);
    m_information->move(0,0);
}

void MainWindow::createTrayIcon()
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setToolTip(QString(tr("Kylin Weather")));
    m_trayIcon->setIcon(QIcon(":/res/control_icons/indicator-china-weather.png"));
    m_trayIcon->setVisible(true);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    //    switch (reason) {
    //    case QSystemTrayIcon::Trigger:
    //    case QSystemTrayIcon::DoubleClick:
    //    case QSystemTrayIcon::MiddleClick:
    //        break;
    //    }

    switch(reason){
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::MiddleClick:
        handleIconClicked();
        if(this->isHidden()){
            this->showNormal();
        }else{
            this->hide();
        }
        break;
    case QSystemTrayIcon::DoubleClick:
        this->hide();
        break;
    case QSystemTrayIcon::Context:
        //右键点击托盘图标弹出菜单
        //showTrayIconMenu(); //显示右键菜单
        break;
    default:
        break;
    }
}

void MainWindow::handleIconClicked()
{
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    QRect screenGeometry = qApp->primaryScreen()->geometry();

    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskMainRect = desktopWidget->availableGeometry(0);//获取可用桌面大小
    QRect screenMainRect = desktopWidget->screenGeometry(0);//获取设备屏幕大小
    QRect deskDupRect = desktopWidget->availableGeometry(1);//获取可用桌面大小
    QRect screenDupRect = desktopWidget->screenGeometry(1);//获取设备屏幕大小

    //qDebug()<<"                                                  ";
    //qDebug()<<"m_trayIcon:"<<m_trayIcon->geometry();
    //qDebug()<<"screenGeometry: "<<screenGeometry;
    //qDebug()<<"availableGeometry: "<<availableGeometry;

    //qDebug()<<"deskMainRect: "<<deskMainRect;
    //qDebug()<<"screenMainRect: "<<screenMainRect;
    //qDebug()<<"deskDupRect: "<<deskDupRect;
    //qDebug()<<"screenDupRect: "<<screenDupRect;

    int m = 46;
    int n = 0;
    int d = 2; //窗口边沿到任务栏距离

    if (screenGeometry.width() == availableGeometry.width() && screenGeometry.height() == availableGeometry.height()){
        if(n == 0){
            //任务栏在下侧
            this->move(availableGeometry.x() + availableGeometry.width() - this->width(), screenMainRect.y() + availableGeometry.height() - this->height() - m - d);
        }else if(n == 1){
            //任务栏在上侧
            this->move(availableGeometry.x() + availableGeometry.width() - this->width(), screenMainRect.y() + screenGeometry.height() - availableGeometry.height() + m + d);
        } else if (n == 2){
            //任务栏在左侧
            this->move(m + d, screenMainRect.y() + screenMainRect.height() - this->height());
            //if (screenGeometry.x() == 0){//主屏在左侧
            //    this->move(screenGeometry.width() - availableGeometry.width() + m + d, screenMainRect.y() + screenMainRect.height() - this->height());
            //}else{//主屏在右侧
            //    this->move(screenGeometry.width() - availableGeometry.width() + m + d,screenDupRect.y() + screenDupRect.height() - this->height());
            //}
        } else if (n == 3){
            //任务栏在右侧
            this->move(screenMainRect.width() - this->width() - m - d, screenDupRect.y() + screenDupRect.height() - this->height());
            //if (screenGeometry.x() == 0){//主屏在左侧
            //    this->move(screenMainRect.width() + screenDupRect.width() - this->width() - m - d, screenDupRect.y() + screenDupRect.height() - this->height());
            //}else{//主屏在右侧
            //    this->move(availableGeometry.x() + availableGeometry.width() - this->width() - m - d, screenMainRect.y() + screenMainRect.height() - this->height());
            //}
        }
    } else if(screenGeometry.width() == availableGeometry.width() ){
        if (m_trayIcon->geometry().y() > availableGeometry.height()/2){
            //任务栏在下侧
            this->move(availableGeometry.x() + availableGeometry.width() - this->width(), screenMainRect.y() + availableGeometry.height() - this->height() - d);
        }else{
            //任务栏在上侧
            this->move(availableGeometry.x() + availableGeometry.width() - this->width(), screenMainRect.y() + screenGeometry.height() - availableGeometry.height() + d);
        }
    } else if (screenGeometry.height() == availableGeometry.height()){
        if (m_trayIcon->geometry().x() > availableGeometry.width()/2){
            //任务栏在右侧
            this->move(availableGeometry.x() + availableGeometry.width() - this->width() - d, screenMainRect.y() + screenGeometry.height() - this->height());
        } else {
            //任务栏在左侧
            this->move(screenGeometry.width() - availableGeometry.width() + d, screenMainRect.y() + screenGeometry.height() - this->height());
        }
    }
}

//更新主界面搜索列表
void MainWindow::onSearchBoxEdited()
{
    searchCityName();

    m_searchView->setItemDelegate(m_delegate); //为视图设置委托
    m_searchView->setSpacing(1); //为视图设置控件间距
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterRole(Qt::UserRole);
    m_proxyModel->setDynamicSortFilter(true);
    m_searchView->setModel(m_proxyModel); //为委托设置模型
    m_searchView->setViewMode(QListView::IconMode); //设置Item图标显示
    m_searchView->setDragEnabled(false); //控件不允许拖动
}
void MainWindow::searchCityName()
{
    const QString inputText = m_leftupsearchbox->text().trimmed();
    if (inputText.isEmpty())
        return;

    QList<LocationData> searchResultList;
    searchResultList = m_locationWorker->exactMatchCity(inputText);

    if (searchResultList.isEmpty()) {
        qDebug()<<"fail to search city information";
    }
    else {
        delete m_model;
        m_model = new QStandardItemModel();

        foreach(LocationData m_locationdata, searchResultList){
            QStandardItem *Item = new QStandardItem;

            ItemData itemData;

            itemData.cityId = QString(m_locationdata.id);
            itemData.cityName = QString(m_locationdata.city);
            itemData.cityProvince = QString(m_locationdata.province);
            Item->setData(QVariant::fromValue(itemData),Qt::UserRole); //整体存取

            m_model->appendRow(Item); //追加Item
        }
    }
}

//设置生活指数
void MainWindow::onSetLifeStyle(LifeStyle m_lifestyle)
{
    m_information->onSetLifeStyle(m_lifestyle);
}

//设置预报天气
void MainWindow::onSetForecastWeather(ForecastWeather m_forecastweather)
{
    m_information->onSetForecastWeather(m_forecastweather);
}

//设置实况天气
void MainWindow::onSetObserveWeather(ObserveWeather m_observeweather)
{
    //主界面UI变化
    m_searchView->hide();
    m_leftupsearchbox->setText("");

    int code  = m_observeweather.cond_code.toInt();
    convertCodeToTrayIcon(m_observeweather.cond_code);
    QString picStr = convertCodeToBackgroud(code);
    QString picQss = "#centralwidget{color:white;background-image:url(" + picStr + ");background-repeat:no-repeat;}";
    ui->centralwidget->setStyleSheet(picQss);

    ui->lbCurrTmpUnit->setText("℃");

    QString strHum = "湿度 " + m_observeweather.hum + "%   " + m_observeweather.wind_dir + " " + m_observeweather.wind_sc + "级";
    ui->lbCurrHum->setText(strHum);

    ui->lbCurrTmp->setText(m_observeweather.tmp);

    ui->lbCurrWea->setText(m_observeweather.cond_txt);

    emit m_leftupcitybtn->requestSetCityName(m_observeweather.city);

    //更新保存城市列表文件china-weather-data
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString collectPath = homePath.at(0) + "/.config/china-weather-data";

    QFile readFile(collectPath);
    readFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray cityId = readFile.readAll();
    QString readCityId = (QString(cityId));
    readFile.close();

    QStringList readCityIdList = readCityId.split(",");
    readCityIdList.replace(0, m_observeweather.id);

    QString writeCityId = "";
    foreach (QString strCity, readCityIdList) {
        if (strCity != ""){
            writeCityId.append(strCity);
            writeCityId.append(",");
        }
    }
    QFile writeFile(collectPath);
    writeFile.open(QIODevice::WriteOnly | QIODevice::Text);
    writeFile.write(writeCityId.toUtf8().data());
    writeFile.close();
}

//根据天气情况设置托盘图标
void MainWindow::convertCodeToTrayIcon(QString code)
{
    if (code.isEmpty()) {
        m_trayIcon->setIcon(QIcon(":/res/control_icons/indicator-china-weather.png"));
        return;
    }

    QString strIcon = QString(":/res/weather_icons/white/%1.png").arg(code);
    m_trayIcon->setIcon(QIcon(strIcon));
}

//根据天气情况设置主界面背景贴图
QString MainWindow::convertCodeToBackgroud(int code)
{
    if (code == 100 || code == 900) {
        QTime current_time = QTime::currentTime();
        int hour = current_time.hour();
        if (hour>=6 && hour<= 18){
            return ":/res/background/weather-clear.png";
        } else {
            return ":/res/background/weather-clear-night.png";
        }
    }
    else if (code <= 103 && code >= 101) {
        return ":/res/background/weather-few-clouds.png";
    }
    else if (code == 104 || code == 901) {
        return ":/res/background/weather-overcast.png";
    }
    else if (code <= 204 && code >= 200) {
        return ":/res/background/weather-clear.png";
    }
    else if (code <= 213 && code >= 205) {
        return ":/res/background/weather-overcast.png";
    }
    else if (code <= 399 && code >= 300) {
        return ":/res/background/weather-rain.png";
    }
    else if (code <= 499 && code >= 400) {
        return ":/res/background/weather-snow.png";
    }
    else if (code <= 502 && code >= 500) {
        return ":/res/background/weather-fog.png";
    }
    else if (code <= 508 && code >= 503) {
        return ":/res/background/weather-sandstorm.png";
    }
    else if (code <= 515 && code >= 509) {
        return ":/res/background/weather-fog.png";
    }
    else {
        QTime current_time = QTime::currentTime();
        int hour = current_time.hour();
        if (hour>=6 && hour<= 18){
            return ":/res/background/weather-clear.png";
        } else {
            return ":/res/background/weather-clear-night.png";
        }
    }
}

void MainWindow::on_btnMinimize_clicked()
{
    QWidget::showMinimized();
}

void MainWindow::on_btnCancel_clicked()
{
    QApplication* app;
    app->exit(0);
}
