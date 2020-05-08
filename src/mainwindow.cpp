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

    judgeSystemLanguage();

    //单例运行
    //checkSingle();

    //先注册结构体，这样才能作为信号与槽的参数
    qRegisterMetaType<ObserveWeather>();
    qRegisterMetaType<ForecastWeather>();
    qRegisterMetaType<LifeStyle>();

    //设置主界面样式
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint | Qt::Tool);
    this->setFocusPolicy(Qt::StrongFocus);//this->setFocusPolicy(Qt::NoFocus);
    this->setWindowTitle(tr("Kylin Weather"));
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    this->setWindowIcon(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/control_icons/indicator-china-weather.png")) );

    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    this->setStyleSheet("QWidget{border:none;border-radius:6px;}");

    //设置其他控件样式
    this->initControlQss();

    //创建托盘图标
    this->createTrayIcon();
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    //ui->widget_normal->show();

    //左上角按钮
    m_leftupcitybtn = new LeftUpCityBtn(ui->widget_normal);
    m_leftupcitybtn->move(20, 21);
    m_leftupcitybtn->show();

    //左上角搜索框
    m_leftupsearchbox = new LeftUpSearchBox(ui->widget_normal);
    m_leftupsearchbox->move(100, 18);
    m_leftupsearchbox->show();

    //主界面搜索列表
    m_searchView = new LeftUpSearchView(ui->widget_normal);
    m_delegate = new LeftUpSearchDelegate(m_searchView);
    m_proxyModel = new QSortFilterProxyModel(m_searchView);
    m_model = new QStandardItemModel();
    m_searchView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_searchView->move(100, 49);
    m_searchView->resize(178,205);
    m_searchView->hide();

    m_hintWidget = new PromptWidget(this);
    m_hintWidget->setIconAndText(":/res/control_icons/network_warn.png", tr("Network not connected"));
    m_hintWidget->move((this->width() - m_hintWidget->width())/2, 100);
    m_hintWidget->setVisible(false);

    m_locationWorker = new LocationWorker(this);
    m_weatherManager = new WeatherManager(this);

    initConnections(); //建立信号与槽的连接

    onRefreshMainWindowWeather();//软件启动时先获取一次网络数据

    m_refreshweather = new QTimer(this); //定时更新主界面天气
    m_refreshweather->setTimerType(Qt::PreciseTimer);
    QObject::connect(m_refreshweather, SIGNAL(timeout()), this, SLOT(onRefreshMainWindowWeather()));
    m_refreshweather->start((20*60)*1000); //set time interval to refresh weather
}

MainWindow::~MainWindow()
{
    delete ui;
}

//非中文系统环境无法启动
void MainWindow::judgeSystemLanguage()
{
    QLocale locale;
    //获取系统语言环境
    if( locale.language() == QLocale::Chinese ) {
        qDebug() << "Chinese system";
        return;
    } else {
        qDebug() << "Non-chinese system";
        exit(0);
    }
}

//单例模式
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

//配置文件是否存在
bool MainWindow::isFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    if(fileInfo.isFile()) {
        return true;
    }
    return false;
}

//初始化各控件样式
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
    //ui->btnCancel->hide();

    ui->lbCurrTmp->setStyleSheet("QLabel{border:none;background:transparent;font-size:110px;font-family:Microsoft YaHei;font-weight:300;color:rgba(255,255,255,1);line-height:100px;}");
    ui->lbCurrTmp->setAlignment(Qt::AlignCenter);

    ui->lbCurrTmpUnit->setStyleSheet("QLabel{border:none;background:transparent;font-size:24px;color:rgba(255,255,255,1);line-height:14px;}");
    ui->lbCurrTmpUnit->setAlignment(Qt::AlignCenter);

    ui->lbCurrWea->setStyleSheet("QLabel{border:none;background:transparent;font-size:20px;color:rgba(255,255,255,1);line-height:14px;}");

    ui->lbCurrHum->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,1);line-height:14px;}");
    ui->lbCurrHum->setAlignment(Qt::AlignCenter);

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

void MainWindow::initConnections()
{
    connect(m_leftupsearchbox, &LeftUpSearchBox::textChanged, this, [this] () {
        if (m_leftupsearchbox->text().size() == 0){
            m_searchView->hide();
        }else{
            m_searchView->show();
            onSearchBoxEdited();
        }
    });

    connect(m_searchView, SIGNAL(requestSetCityName(QString)), m_leftupcitybtn, SIGNAL(requestSetCityName(QString)) );

    connect(m_searchView, &LeftUpSearchView::requestSetNewCityWeather, this, [=] (QString id) {
        m_weatherManager->startGetTheWeatherData(id);
    });

    connect(m_leftupcitybtn, &LeftUpCityBtn::sendCurrentCityId, this, [=] (QString id) {
        m_weatherManager->startGetTheWeatherData(id);
    });

    connect(m_leftupcitybtn, SIGNAL(requestShowCollCityWeather()), m_weatherManager, SIGNAL(requestShowCollCityWeather()));

    //获取传过来的收藏城市的天气数据，并传给显示收藏城市窗口
    connect(m_weatherManager, SIGNAL(requestSetCityWeather(QString)), m_leftupcitybtn, SIGNAL(requestSetCityWeather(QString)));
    //connect(m_weatherManager, &WeatherManager::requestSetCityWeather, this, [=] (QString weather_data) {
    //   qDebug()<<weather_data;
    //});

    //收到信号带来的数据时，更新主界面天气数据
    connect(m_weatherManager, &WeatherManager::requestSetObserveWeather, this, [=] (ObserveWeather observerdata) {
        this->onSetObserveWeather(observerdata);
    });

    connect(m_weatherManager, &WeatherManager::requestSetForecastWeather, this, [=] (ForecastWeather forecastweather) {
        this->onSetForecastWeather(forecastweather);
    });

    connect(m_weatherManager, &WeatherManager::requestSetLifeStyle, this, [=] (LifeStyle lifestyle) {
        this->onSetLifeStyle(lifestyle);
    });

    //获取天气数据时发生了异常
    connect(m_weatherManager, &WeatherManager::responseFailure, this, [=] (int code) {
        onHandelAbnormalSituation("Get weather data failed!");

        m_hintWidget->setVisible(true);

        if (code == 0) {
            m_hintWidget->setIconAndText(":/res/control_icons/network_warn.png", tr("Incorrect access address"));
        } else {
            m_hintWidget->setIconAndText(":/res/control_icons/network_warn.png", QString(tr("Network error code:%1")).arg(QString::number(code)));
        }
    });

    //根据获取到网络探测的结果分别处理
    connect(m_weatherManager, &WeatherManager::nofityNetworkStatus, this, [=] (const QString &status) {
        if (status == "OK") {
            //m_weatherManager->startAutoLocationTask();//开始自动定位城市

            //CN101010100,beijing,北京,CN,China,中国
            QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
            QString collectPath = homePath.at(0) + "/.config/china-weather-data";
            if (!isFileExist(collectPath)){
                m_weatherManager->startGetTheWeatherData("101010100");

                QFile file(collectPath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    file.write("101010100,");
                    file.close();
                } else {
                    qDebug()<<"Can not write city id data to ~/.config/china-weather-data";
                }

            } else {
                QFile file(collectPath); //文件存在时根据保存的默认城市进行设置
                QString readCityId;
                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QByteArray cityId = file.readAll();
                    readCityId = (QString(cityId));
                    file.close();
                } else {
                    readCityId = "101010100,";
                }

                QStringList listCityId = readCityId.split(",");
                m_weatherManager->startGetTheWeatherData(listCityId.at(0));
            }
        } else {
            if (status == "Fail") {
                onHandelAbnormalSituation("Without wired Carrier");
            } else {
                onHandelAbnormalSituation("Unable to access the Internet");
            }
            emit m_weatherManager->responseFailure(404);
        }
    });

    //自动定位成功后，更新各个控件的默认城市数据，并开始获取天气数据
    connect(m_weatherManager, &WeatherManager::requestAutoLocationData, this, [=] (const CitySettingData &info, bool success) {
        if (success) {
            //自动定位城市成功后，更新各个ui，然后获取天气数据
        } else {
            //自动定位城市失败后，获取天气数据
        }
    });

    connect(m_hintWidget, &PromptWidget::requestRetryAccessWeather, this, [=] () {
        qDebug()<<"debug: retry to refreah mainwindow weather";
        onRefreshMainWindowWeather();
    });
}

//创建托盘图标
void MainWindow::createTrayIcon()
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setToolTip(QString(tr("Kylin Weather")));
    m_trayIcon->setIcon(QIcon(":/res/control_icons/indicator-china-weather.png"));
    m_trayIcon->setVisible(true);
}

//托盘图标被点击
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::MiddleClick:
        if(this->isHidden()){
            //this->showNormal();
            //handleIconClicked(); //靠近任务栏显示
            handleIconClickedSub(); //显示在屏幕中央
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

//处理点击托盘图标事件
void MainWindow::handleIconClicked()
{
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    QRect screenGeometry = qApp->primaryScreen()->geometry();

    QDesktopWidget* desktopWidget = QApplication::desktop();
    //QRect deskMainRect = desktopWidget->availableGeometry(0);//获取可用桌面大小
    QRect screenMainRect = desktopWidget->screenGeometry(0);//获取设备屏幕大小
    //QRect deskDupRect = desktopWidget->availableGeometry(1);//获取可用桌面大小
    //QRect screenDupRect = desktopWidget->screenGeometry(1);//获取设备屏幕大小

    //qDebug()<<"screenGeometry: "<<screenGeometry;
    //qDebug()<<"availableGeometry: "<<availableGeometry;
    //qDebug()<<"deskMainRect: "<<deskMainRect;
    //qDebug()<<"screenMainRect: "<<screenMainRect;
    //qDebug()<<"deskDupRect: "<<deskDupRect;
    //qDebug()<<"screenDupRect: "<<screenDupRect;

    int m = m_weatherManager->getTaskBarHeight("height");
    int n = m_weatherManager->getTaskBarPos("position");
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
            if (screenGeometry.x() == 0){//主屏在左侧
                this->move(m + d, screenMainRect.y() + screenMainRect.height() - this->height());
            }else{//主屏在右侧
                this->move(screenMainRect.x() + m + d, screenMainRect.y() + screenMainRect.height() - this->height());
            }
        } else if (n == 3){
            //任务栏在右侧
            if (screenGeometry.x() == 0){//主屏在左侧
                this->move(screenMainRect.width() - this->width() - m - d, screenMainRect.y() + screenMainRect.height() - this->height());
            }else{//主屏在右侧
                this->move(screenMainRect.x() + screenMainRect.width() - this->width() - m - d, screenMainRect.y() + screenMainRect.height() - this->height());
            }
        }
    } else if (availableGeometry.x() == screenGeometry.x() && availableGeometry.y() == screenGeometry.y()) { //panel in right or bottom
        this->move(availableGeometry.x() + availableGeometry.width() - this->width() - d, availableGeometry.y() + availableGeometry.height() - this->height() - d);
    } else {
        if (availableGeometry.x() > 0) {//panel in left
            this->move(availableGeometry.x() + d, availableGeometry.y() + availableGeometry.height()  - this->height());
        }
        else {//panel in top
            this->move(availableGeometry.x() + availableGeometry.width() - this->width(), availableGeometry.y() + d);
        }
    }

    this->showNormal();
    this->raise();
    this->activateWindow();
}

void MainWindow::handleIconClickedSub()
{
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    this->move((availableGeometry.width() - this->width())/2, (availableGeometry.height() - this->height())/2);

    this->showNormal();
    this->raise();
    this->activateWindow();
}

//定时更新主界面天气
void MainWindow::onRefreshMainWindowWeather()
{
    //开始测试网络情况
    m_weatherManager->startTestNetwork();
}

//处理因网络异常等未获取到天气数据的情况
void MainWindow::onHandelAbnormalSituation(QString abnormalText){
    qDebug()<<"debug: network state: "<<abnormalText;
    setAbnormalMainWindow();
}

//处理异常时的主界面显示
void MainWindow::setAbnormalMainWindow()
{
    m_trayIcon->setIcon(QIcon(":/res/control_icons/indicator-china-weather.png"));

    ui->lbCurrTmp->setText("");
    ui->lbCurrTmpUnit->setText("");
    ui->lbCurrWea->setText("");
    ui->lbCurrHum->setText("");

    ForecastWeather abnormalForecastweather;
    for (int i=0; i<7; i++) {
        abnormalForecastweather.uv_index = "N/A";
        abnormalForecastweather.wind_spd = "N/A";
        abnormalForecastweather.sr = "N/A";
        abnormalForecastweather.wind_sc = "N/A";
        abnormalForecastweather.ms = "N/A";
        abnormalForecastweather.cond_txt_d = "N/A";
        abnormalForecastweather.vis = "N/A";
        abnormalForecastweather.ss = "N/A";
        abnormalForecastweather.hum = "N/A";
        abnormalForecastweather.cond_txt_n = "N/A";
        abnormalForecastweather.pop = "N/A";
        abnormalForecastweather.wind_deg = "N/A";
        abnormalForecastweather.pcpn = "N/A";
        abnormalForecastweather.wind_dir = "N/A ";
        abnormalForecastweather.cond_code_d = "999";
        abnormalForecastweather.mr = "N/A";
        abnormalForecastweather.date = "N/A";
        abnormalForecastweather.tmp_max = "N/A";
        abnormalForecastweather.cond_code_n = "999";
        abnormalForecastweather.pres = "N/A";
        abnormalForecastweather.tmp_min = "N/A";

        onSetForecastWeather(abnormalForecastweather);
    }

    LifeStyle abnormalLifestyle;
    abnormalLifestyle.drsg_brf = "N/A";
    abnormalLifestyle.flu_brf = "N/A";
    abnormalLifestyle.uv_brf = "N/A";
    abnormalLifestyle.cw_brf = "N/A";
    abnormalLifestyle.air_brf = "N/A";
    abnormalLifestyle.sport_brf = "N/A";
    onSetLifeStyle(abnormalLifestyle);
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
    if (m_observeweather.tmp != "") {
        m_hintWidget->setVisible(false);
    }

    //主界面UI变化,控件间的距离自适应
    m_searchView->hide();
    m_leftupsearchbox->setText("");
    int m_size = m_observeweather.city.size();
    m_leftupsearchbox->move(100 + 15*(m_size-2), 18);
    m_searchView->move(100 + 15*(m_size-2), 49);

    int code  = m_observeweather.cond_code.toInt();
    convertCodeToTrayIcon(m_observeweather.cond_code);
    QString picStr = convertCodeToBackgroud(code);
    QString picQss = "#centralwidget{color:white;background-image:url(" + picStr + ");background-repeat:no-repeat;}";
    ui->centralwidget->setStyleSheet(picQss);

    ui->lbCurrTmp->setText(m_observeweather.tmp);
    int m_size1 = m_observeweather.tmp.size();
    ui->lbCurrTmpUnit->move(451 + 30*(m_size1-1), 95);
    ui->lbCurrWea->move(454 + 30*(m_size1-1), 165);


    ui->lbCurrTmpUnit->setText("℃");

    ui->lbCurrWea->setText(m_observeweather.cond_txt);

    QString strHum = "湿度 " + m_observeweather.hum + "%   " + m_observeweather.wind_dir + " " + m_observeweather.wind_sc + "级";
    ui->lbCurrHum->setText(strHum);

    if (m_observeweather.city != "") {
        m_weatherManager->postSystemInfoToServer(); //将当前城市告诉给服务器
        emit m_leftupcitybtn->requestSetCityName(m_observeweather.city); //更新左上角按钮显示的城市
    }

    //更新保存城市列表文件china-weather-data
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString collectPath = homePath.at(0) + "/.config/china-weather-data";

    QFile readFile(collectPath);
    readFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray cityId = readFile.readAll();
    QString readCityId = (QString(cityId));
    readFile.close();

    QStringList readCityIdList = readCityId.split(",");
    for (int i=1; i<readCityIdList.size()-1; i++) { //减1因为readCityIdList最后一项为空
        QString str = readCityIdList.at(i);
        if (str == m_observeweather.id) {
            readCityIdList.removeOne(m_observeweather.id);
            break;
        }
    }
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

    //QString strIcon = QString(":/res/weather_icons/white/%1.png").arg(code);
    //m_trayIcon->setIcon(QIcon(strIcon));
    m_trayIcon->setIcon(QIcon::fromTheme(QString("%1").arg(code), QIcon(QString(":/res/weather_icons/white/%1.png").arg(code))) );
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
    //this->setVisible(false);
}

void MainWindow::on_btnCancel_clicked()
{
    this->setVisible(false);
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
}
