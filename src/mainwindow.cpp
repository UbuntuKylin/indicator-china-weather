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
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    judgeSystemLanguage();
    // 用户手册功能
    mDaemonIpcDbus = new DaemonDbus();

    //先注册结构体，这样才能作为信号与槽的参数
    qRegisterMetaType<ObserveWeather>();
    qRegisterMetaType<ForecastWeather>();
    qRegisterMetaType<LifeStyle>();

    //设置主界面样式
    this->setFixedSize(885, 600);
    this->setWindowFlags(Qt::FramelessWindowHint);
    qDebug()<< this->windowState();
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint | Qt::Tool);
    this->setFocusPolicy(Qt::ClickFocus);//this->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    this->setWindowTitle(tr("Kylin Weather"));
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    this->setWindowIcon(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/control_icons/logo_24.png")) );

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setColor(QColor(0,0,0,127));
    shadow->setBlurRadius(13);
    ui->centralwidget->setGraphicsEffect(shadow);
    //给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
//    ui->centralwidget->setMargin(24);


    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    this->setStyleSheet("QWidget{border:none;border-radius:6px;}");
    titleWid = new QWidget(this);
    titleLayout = new QHBoxLayout();
    cityLabel = new QLabel(this);
    cityLabel->setStyleSheet("font:20px;");

    m_menu = new menuModule(this);
    connect(m_menu,&menuModule::menuModuleClose,this,&MainWindow::close);
//    setBtn = new QPushButton(this);
//    menu = new QMenu(this);
//    addCityAction = new AddCityAction(menu);
    m_menu->addCityAction->setText(tr("Add City"));
//    aboutAction = new QAction(tr("About"),menu);
//    actions<<addCityAction<<aboutAction;
//    menu->addActions(actions);
//    setBtn->setMenu(menu);
    connect(m_menu->addCityAction, &AddCityAction::requestSetCityName, this, [=] (QString cityName) {
        cityLabel->setText(cityName);//一会设置个label用于显示地名
    });




    //左上角按钮
    m_leftupcitybtn = new LeftUpCityBtn(ui->widget_normal);
    m_leftupcitybtn->hide();

    logoBtn = new QPushButton(ui->widget_normal);
    logolb = new QLabel(ui->widget_normal);
    logolb->setFixedSize(100,24);
    logoBtn->setFixedSize(24,24);
    logoBtn->setIconSize(QSize(24,24));//重置图标大小
    logoBtn->setIcon(QIcon(":/res/control_icons/logo_24.png"));
    logolb->setText(tr("Kylin Weather"));
    logolb->setStyleSheet("font-size:14px;");


    //左上角搜索框
    m_leftupsearchbox = new LeftUpSearchBox(ui->widget_normal);
    //设置其他控件样式
    this->initControlQss();

    //创建托盘图标
    this->createTrayIcon();
    //添加托盘菜单
    m_mainMenu = new QMenu;
//    m_mainMenu->addSeparator();
    m_openAction = new QAction(tr("Open Kylin Weather"),this);//打开麒麟天气
    m_quitAction = new QAction(tr("Exit"),this);//退出
    m_mainMenu->addAction(m_openAction);
//    m_openAction->setIcon(QIcon::fromTheme(QString("indicator-china-weather"), QIcon(QString(":/res/control_icons/indicator-china-weather_min.png"))));
    m_openAction->setIcon(QIcon(QString(":/res/control_icons/logo_24.png")) );
    m_mainMenu->addAction(m_quitAction);
    m_quitAction->setIcon(QIcon::fromTheme(QString("exit-symbolic"), QIcon(QString(":/res/control_icons/quit_normal.png"))) );
//    m_quitAction->setIcon(QIcon(QString(":/res/control_icons/quit_normal.png")));
    connect(m_openAction, &QAction::triggered, this, [=] {
        if(this->isHidden()){
        this->show();}
        else{
            return;
        }
    });
    //connect(m_quitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(m_quitAction, &QAction::triggered, this,&MainWindow::closeActivated);
    m_trayIcon->setContextMenu(m_mainMenu);

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    //主界面搜索列表
    m_searchView = new LeftUpSearchView(ui->widget_normal);
    m_delegate = new LeftUpSearchDelegate(m_searchView);
    m_proxyModel = new QSortFilterProxyModel(m_searchView);
    m_model = new QStandardItemModel();
    m_searchView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    m_searchView->move(100, 49);//2020.12.22
    m_searchView->resize(178,205);
    m_searchView->hide();
    m_searchView->move(605,49);

    m_hintWidget = new PromptWidget(this);
    m_hintWidget->setIconAndText(":/res/control_icons/network_warn.png", tr("Network not connected"));//网络未连接
    m_hintWidget->move((this->width() - m_hintWidget->width())/2, 100);
    m_hintWidget->setVisible(false);

    m_locationWorker = new LocationWorker(this);
    m_weatherManager = new WeatherManager(this);
    m_weatherManager->initConnectionInfo(); //get information about network connection

    initConnections(); //建立信号与槽的连接

    onRefreshMainWindowWeather();//软件启动时先获取一次网络数据

    m_refreshweather = new QTimer(this); //定时更新主界面天气
    m_refreshweather->setTimerType(Qt::PreciseTimer);
    QObject::connect(m_refreshweather, SIGNAL(timeout()), this, SLOT(onRefreshMainWindowWeather()));
    m_refreshweather->start((20*60)*1000); //set time interval to refresh weather
    initGsetting();//初始化Gsetting
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 实现键盘响应
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // F1快捷键打开用户手册
    if (event->key() == Qt::Key_F1) {
        if (!mDaemonIpcDbus->daemonIsNotRunning()){
            //F1快捷键打开用户手册，如kylin-recorder
            //由于是小工具类，下面的showGuide参数要填写"tools/indicator-china-weather"
            mDaemonIpcDbus->showGuide("tools/indicator-china-weather");
        }
    }
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

//初始化各控件样式
void MainWindow::initControlQss()
{
    //baibai Modify左上角标题栏布局
    m_leftupsearchbox->setFixedWidth(150);

    titleLayout->addSpacing(4);
    titleLayout->addWidget(logoBtn);//麒麟天气logo
    titleLayout->addSpacing(4);
    titleLayout->addWidget(logolb);//麒麟天气标签
    titleLayout->addStretch();//添加伸缩
    titleLayout->addWidget(m_leftupsearchbox);//麒麟天气搜索栏
    titleLayout->addSpacing(4);
//    titleLayout->addStretch();//添加伸缩
    titleLayout->addWidget(m_menu->menuButton);//设置按钮
    titleLayout->addWidget(ui->btnMinimize);
    titleLayout->addWidget(ui->btnCancel);
    titleLayout->setSpacing(4);
    titleLayout->setMargin(4);
    titleWid->setLayout(titleLayout);
    titleWid->setFixedWidth(865);
    titleWid->move(10,10);

//    setBtn->setFixedSize(30,30);
    //menu跟主题走
//    menu->setFixedSize(120,66);
//    menu->setStyleSheet("QMenu{border-radius:3px;background-color:white;color:black;}"
//                        "QMenu::item:selected {color:white;background-color: #2dabf9;}"
//                        "QMenu::item {font-size:14px;border-radius:4px;background-color: transparent;}");

//    setBtn->setIcon(QIcon::fromTheme("application-menu"));

//    setBtn->setStyleSheet("QPushButton{border-radius:4px;}"
//                          "QPushButton::hover{background-color:rgba(0,0,0,0.1)}"
//                          "QPushButton::pressed{background-color:rgba(0,0,0,0.15)}"
//                          "QPushButton::menu-indicator{image:None;}");

    ui->centralwidget->setStyleSheet("#centralwidget{border:1px solid rgba(38,38,38,0.15);border-radius:6px;background:rgba(19,19,20,0);}");
    ui->centralwidget->setStyleSheet("#centralwidget{color:white;background-image:url(':/res/background/weather-clear.png');background-repeat:no-repeat;}");
    ui->centralwidget->move(10,10);
    ui->centralwidget->setFixedSize(865,520);
    ui->btnMinimize->setIcon(QIcon::fromTheme("window-minimize-symbolic"));
    ui->btnMinimize->setFixedSize(30,30);
    ui->btnMinimize->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:transparent;background-color:rgba(0,0,0,0.1);}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:transparent;background-color:rgba(0,0,0,0.15);}");
    ui->btnMinimize->setFocusPolicy(Qt::NoFocus);//设置焦点类型

    ui->btnCancel->setIcon(QIcon::fromTheme("window-close-symbolic"));
    ui->btnCancel->setFixedSize(30,30);
    ui->btnCancel->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:transparent;background-color:#F86457;}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:transparent;background-color:#E44C50;}");
    ui->btnCancel->setFocusPolicy(Qt::NoFocus);//设置焦点类型

    ui->lbCurrTmp->setStyleSheet("QLabel{border:none;background:transparent;font-size:110px;font-weight:300;color:rgba(255,255,255,1);line-height:100px;}");
    ui->lbCurrTmp->setAlignment(Qt::AlignCenter);

    ui->lbCurrTmpUnit->setStyleSheet("QLabel{border:none;background:transparent;font-size:24px;color:rgba(255,255,255,1);line-height:14px;}");
    ui->lbCurrTmpUnit->setAlignment(Qt::AlignCenter);

    ui->lbCurrWea->setStyleSheet("QLabel{border:none;background:transparent;font-size:20px;color:rgba(255,255,255,1);line-height:14px;}");

    ui->lbCurrHum->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,1);line-height:14px;}");
    ui->lbCurrHum->setAlignment(Qt::AlignCenter);

    m_scrollarea = new QScrollArea(ui->centralwidget);
    m_scrollarea->setFocusPolicy(Qt::NoFocus);//设置焦点类型
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

    connect(m_leftupsearchbox,&LeftUpSearchBox::lineEditKeyEvent,m_searchView,&LeftUpSearchView::dealSearchBoxKeyPress);
    //1*****addCityAction替换原来的m_leftupcitybtn*****
    connect(m_searchView, SIGNAL(requestSetCityName(QString)), m_menu->addCityAction, SIGNAL(requestSetCityName(QString)) );
//    connect(m_searchView, SIGNAL(requestSetCityName(QString)), m_leftupcitybtn, SIGNAL(requestSetCityName(QString)) );

    connect(m_searchView, &LeftUpSearchView::requestSetNewCityWeather, this, [=] (QString id) {
        m_weatherManager->startGetTheWeatherData(id);
    });
    //2*****addCityAction替换原来的m_leftupcitybtn*****
    connect(m_menu->addCityAction,&AddCityAction::sendCurrentCityId, this, [=] (QString id) {
        if(this->isHidden()){
            handleIconClickedSub(); //显示在屏幕中央
        }
        m_weatherManager->startGetTheWeatherData(id);
    });
//    connect(m_leftupcitybtn, &LeftUpCityBtn::sendCurrentCityId, this, [=] (QString id) {
//        if(this->isHidden()){
//            handleIconClickedSub(); //显示在屏幕中央
//        }
//        m_weatherManager->startGetTheWeatherData(id);
//    });

    //3*****addCityAction替换原来的m_leftupcitybtn*****
    connect(m_menu->addCityAction, SIGNAL(requestShowCollCityWeather()), m_weatherManager, SIGNAL(requestShowCollCityWeather()));
//    connect(m_leftupcitybtn, SIGNAL(requestShowCollCityWeather()), m_weatherManager, SIGNAL(requestShowCollCityWeather()));

    //同步主界面和收藏界面当前城市信息
    //4*****addCityAction替换原来的m_leftupcitybtn*****
    connect(this,&MainWindow::updatecity,m_menu->addCityAction,&AddCityAction ::updatecity);
//    connect(this,&MainWindow::updatecity,m_leftupcitybtn,&LeftUpCityBtn ::updatecity);

    //获取传过来的收藏城市的天气数据，并传给显示收藏城市窗口
    //5*****addCityAction替换原来的m_leftupcitybtn*****
    connect(m_weatherManager, SIGNAL(requestSetCityWeather(QString)), m_menu->addCityAction, SIGNAL(requestSetCityWeather(QString)));
//    connect(m_weatherManager, SIGNAL(requestSetCityWeather(QString)), m_leftupcitybtn, SIGNAL(requestSetCityWeather(QString)));

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
            m_hintWidget->setIconAndText(":/res/control_icons/network_warn.png", tr("Incorrect access address"));//访问地址异常
        } else {
            m_hintWidget->setIconAndText(":/res/control_icons/network_warn.png", QString(tr("Network error code:%1")).arg(QString::number(code)));//网络错误代码
        }
    });

    //根据获取到网络探测的结果分别处理
    connect(m_weatherManager, &WeatherManager::nofityNetworkStatus, this, [=] (const QString &status) {
        if (status == "OK") {
            //m_weatherManager->startAutoLocationTask();//开始自动定位城市

            //CN101010100,beijing,北京,CN,China,中国
            // m_weatherManager->startGetTheWeatherData("101010100");
            QStringList listCityId = getCityList().split(",");
            m_weatherManager->startGetTheWeatherData(listCityId.at(0));
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

    //网络连接由无到有时触发天气界面更新
    connect(m_weatherManager, &WeatherManager::newNetworkConnectionCreated, this, [=] () {
        qDebug()<<"需要更新天气界面";
        onRefreshMainWindowWeather();
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
    m_trayIcon->setIcon(QIcon::fromTheme(QString("999"), QIcon(QString(":/res/weather_icons/white/999.png"))) );
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
        m_mainMenu->show();
        break;
    default:
        break;
    }
}
void MainWindow::closeActivated()
{
    //托盘退出默认关闭开机自启
    QString autostart=QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0]+"/.config/autostart/indicator-china-weather.desktop";
    QFile file(autostart);
    if(!file.exists())
    {
        QString path="/etc/xdg/autostart/indicator-china-weather.desktop";
        QFileInfo file2(path);
        if(!file2.exists())
        {
            qDebug()<<"/etc/xdg/autostart/目录下无麒麟天气快捷方式";
        }
        else
        {
            QFile::copy(path,autostart);
            if(file.exists())
            {
                if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
                {
                    file.write("\nHidden=true\n");
                    file.close();
                }
            }
        }
    }

    qApp->quit();
    return;
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
    m_trayIcon->setIcon(QIcon::fromTheme(QString("999"), QIcon(QString(":/res/weather_icons/white/999.png"))) );
//    m_openAction->setIcon(QIcon::fromTheme(QString("999"), QIcon(QString(":/res/weather_icons/white/999.png"))) );
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
        m_model->clear();//清空上一次搜索结果
        m_searchView->resize(178,55);//只保留一行大小
        //m_searchView->hide();//或一行不保留，无提示
        QStandardItem *Item = new QStandardItem;
        ItemData itemData;
        itemData.cityName = QString("无匹配城市");//无匹配搜索结果时，提示用户无结果
        itemData.cityProvince = QString("请重新输入");
        Item->setData(QVariant::fromValue(itemData),Qt::UserRole); //整体存取

        m_model->appendRow(Item); //追加Item
        m_searchView->setAttribute(Qt::WA_TransparentForMouseEvents, true);//无结果时点击搜索栏无效果
        qDebug()<<"fail to search city information";
    }
    else {
        delete m_model;
        m_model = new QStandardItemModel();
        int tempNumsOfCityInSearchResultList = 0;//搜索列表中城市数量
        foreach(LocationData m_locationdata, searchResultList){
            tempNumsOfCityInSearchResultList++;//计数
            QStandardItem *Item = new QStandardItem;

            ItemData itemData;

            itemData.cityId = QString(m_locationdata.id);
            itemData.cityName = QString(m_locationdata.city);
            itemData.cityProvince = QString(m_locationdata.province);
            Item->setData(QVariant::fromValue(itemData),Qt::UserRole); //整体存取

            m_model->appendRow(Item); //追加Item
            m_searchView->setAttribute(Qt::WA_TransparentForMouseEvents,false);//有结果时点击搜索栏有效果
        }
        if ( tempNumsOfCityInSearchResultList > 4 )//默认显示4行，结果数大于4时，按默认大小显示
        {
            m_searchView->resize(178,205);
        }
        else//结果小于4时，按城市数量显示行数
        {
            m_searchView->resize(178,tempNumsOfCityInSearchResultList * 50 + 5);
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

//设置实况天气显示
void MainWindow::onSetObserveWeather(ObserveWeather m_observeweather)
{
    if (m_observeweather.tmp != "") {
        m_hintWidget->setVisible(false);
    }

    //主界面UI变化,控件间的距离自适应
    m_searchView->hide();
    m_leftupsearchbox->setText("");
    int m_size = m_observeweather.city.size();
//    m_leftupsearchbox->move(100 + 15*(m_size-2), 18);
    m_searchView->move(605 + 15*(m_size-2), 49);//2020.12.22修改搜索视图位置

    int code  = m_observeweather.cond_code.toInt();
    convertCodeToTrayIcon(m_observeweather.cond_code);
    QString picStr = convertCodeToBackgroud(code);
    QString picQss = "#centralwidget{color:white;background-image:url(" + picStr + ");background-repeat:no-repeat;}";
    ui->centralwidget->setStyleSheet(picQss);

    ui->lbCurrTmp->setText(m_observeweather.tmp);
    int m_size1 = m_observeweather.tmp.size();

    if(m_size1 == 3){

        cityLabel->setGeometry(420,70,80,25);
        ui->lbCurrTmp->setGeometry(351,85,148,100);
        ui->lbCurrTmpUnit->move(447 + 30*(m_size1-1), 95);
        ui->lbCurrWea->move(450 + 30*(m_size1-1), 165);

    }
    else if(m_size1 == 1 || m_size1 ==2){

    cityLabel->setGeometry(420,70,80,25);
    ui->lbCurrTmp->setGeometry(351,85,116,100);
    ui->lbCurrTmpUnit->move(451 + 30*(m_size1-1), 95);
    ui->lbCurrWea->move(454 + 30*(m_size1-1), 165);

}


    ui->lbCurrTmpUnit->setText("℃");

    ui->lbCurrWea->setText(m_observeweather.cond_txt);

    QString strHum = "湿度 " + m_observeweather.hum + "%   " + m_observeweather.wind_dir + " " + m_observeweather.wind_sc + "级";//  Humidity-湿度
    ui->lbCurrHum->setText(strHum);

    if (m_observeweather.city != "") {
        m_weatherManager->postSystemInfoToServer(); //将当前城市告诉给服务器
//        emit m_leftupcitybtn ->requestSetCityName(m_observeweather.city); //更新左上角按钮显示的城市
        emit m_menu->addCityAction->requestSetCityName(m_observeweather.city); //更新中间Label显示的城市
    }

    //更新保存城市列表文件china-weather-data
    QStringList readCityIdList = getCityList().split(",");

    //若收藏城市列表中已经有搜索的新城市，则去掉。减1因为readCityIdList最后一项为空
//    for (int i=1; i<readCityIdList.size()-1; i++) {
//        QString str = readCityIdList.at(i);
//        if (str == m_observeweather.id) {
//            readCityIdList.removeOne(m_observeweather.id);
//            break;
//        }
//    }

    //将上一个当前城市放入收藏列表中
//    QString oldCurrentCityId = readCityIdList.at(0);
//    if  (oldCurrentCityId != m_observeweather.id) {
//        if (readCityIdList.size() == 10) {
//            //收藏列表已经有8个城市，替换最后一个
//            readCityIdList.replace(8, oldCurrentCityId);
//        }
//        if (readCityIdList.size() <= 9) {
//            //收藏列表少于8个城市，将上一个当前城市放入末尾
//            readCityIdList.append(oldCurrentCityId);
//        }
//    }

    //将列表中第一个城市设置为当前搜索的新城市
    readCityIdList.replace(0, m_observeweather.id);

    QString writeCityId = "";
    foreach (QString strCity, readCityIdList) {
        if (strCity != "") {
            writeCityId.append(strCity);
            writeCityId.append(",");
        }
    }
    setCityList(writeCityId);
}

//根据天气情况设置托盘图标
void MainWindow::convertCodeToTrayIcon(QString code)
{
    if (code.isEmpty()) {
        m_trayIcon->setIcon(QIcon::fromTheme(QString("999"), QIcon(QString(":/res/weather_icons/white/999.png"))) );
//        m_openAction->setIcon(QIcon::fromTheme(QString("999"), QIcon(QString(":/res/weather_icons/white/999.png"))) );
        return;
    }

    //QString strIcon = QString(":/res/weather_icons/white/%1.png").arg(code);
    //m_trayIcon->setIcon(QIcon(strIcon));
    m_trayIcon->setIcon(QIcon::fromTheme(QString("%1").arg(code), QIcon(QString(":/res/weather_icons/white/%1.png").arg(code))) );
//    m_openAction->setIcon(QIcon::fromTheme(QString("%1").arg(code), QIcon(QString(":/res/weather_icons/white/%1.png").arg(code))));
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
    m_leftupsearchbox->clear();
    //this->setVisible(false);
}

void MainWindow::on_btnCancel_clicked()
{
    m_leftupsearchbox->clear();
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
    this->setCursor(Qt::ArrowCursor);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        this->setCursor(Qt::ClosedHandCursor);
        event->accept();
    }
}
//鼠标点击外部，收起搜索列表
bool MainWindow::event(QEvent *event)
{
//    if(m_searchView == nullptr)return QWidget::event(event);
    if (event->type() == QEvent::MouseButtonPress)
    {
        m_leftupsearchbox->clear();
    }
    return QWidget::event(event);
}

void MainWindow::initGsetting()
{
    if(QGSettings::isSchemaInstalled(CHINAWEATHERDATA))
    {
        m_pWeatherData = new QGSettings(CHINAWEATHERDATA);
        firstGetCityList=m_pWeatherData->get("citylist").toString();
        //监听key的value是否发生了变化
        connect(m_pWeatherData, &QGSettings::changed, this, [=] (const QString &key)
        {
            if (key == "citylist")
            {
                QString nowCityList=m_pWeatherData->get("citylist").toString();
                if(nowCityList.split(",").first()!=firstGetCityList.split(",").first())
                {
                    m_weatherManager->startGetTheWeatherData(nowCityList.split(",").at(0));
                    firstGetCityList=nowCityList;
                    emit updatecity();
                }
            }
        });
    }
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW))
    {
        m_pThemeStyle = new QGSettings(FITTHEMEWINDOW);
        connect(m_pThemeStyle,&QGSettings::changed,this, [=] (const QString &key)
        {
            if(key == "styleName")
            {

                    setThemeStyle();
            }
        });
    }
    setThemeStyle();
    return;

}
void MainWindow::setThemeStyle()
{
  if(m_pThemeStyle==nullptr)return;
  nowThemeStyle = m_pThemeStyle->get("styleName").toString();

  m_leftupsearchbox->ThemeLeftUpSearchBox(nowThemeStyle);
  m_searchView->ThemeLeftUpSearchView(nowThemeStyle);
//    if("ukui-dark" == nowThemeStyle|| "ukui-black" == nowThemeStyle)
//    {
//        m_mainMenu ->setStyleSheet("QMenu {border:1px solid rgba(207,207,207,1);border-radius:4px;background-color:rgba(255,255,255,0.6);margin:1px;padding:5px;}"
//                          "QMenu::item {color: rgba(0,0,0,0.6);}"
//                          "QMenu::item:selected {border-radius:4px;background-color:rgba(0,0,0,0.25);}"
//                          QMenu::item:pressed {border-radius:4px;background-color: rgba(0,0,0,0.25);}");
//        m_mainMenu ->setStyleSheet("QMenu {margin:2px;padding:5px;}");
//  }
//    else if("ukui-default" ==nowThemeStyle || "ukui-white" == nowThemeStyle || "ukui-light" == nowThemeStyle)
//    {
//        m_mainMenu ->setStyleSheet("QMenu {background-color:rgba(0,0,0,0.6);margin:1px;padding:5px;}"
//                                    "QMenu::item {color: rgb(225,225,225);}"
//                                    "QMenu::item:selected {border-radius:4px;background-color:rgba(255,255,255,0.25);}"
//                                    QMenu::item:pressed {border-radius:4px;background-color: rgba(255,255,255,0.25);}");
//          m_mainMenu ->setStyleSheet("QMenu {margin:2px;padding:5px;}");
//  }
//QMenu::icon{position:absolute;padding-left:10px;padding-top:5px;padding-bottom:5px;}
}
QString MainWindow::getCityList()
{
    QString str="";
    if (m_pWeatherData != nullptr) {
        QStringList keyList = m_pWeatherData->keys();
        if (keyList.contains("citylist")) {
            str = m_pWeatherData->get("citylist").toString();
        }
    }
    return str;
}

void MainWindow::setCityList(QString str)
{
    m_pWeatherData->set("citylist", str);
}
