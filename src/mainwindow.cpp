/*
 * Copyright (C) 2013 ~ 2019 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
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
#include "titlebar.h"
#include "contentwidget.h"
#include "settingdialog.h"
#include "aboutdialog.h"
#include "promptwidget.h"
#include "weatherworker.h"
#include "maskwidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QMouseEvent>
#include <QMenu>
#include <QScreen>
#include <QShortcut>
#include <QDebug>
#include <QTimer>
#include <math.h>
#include <QStandardPaths>
#include <QFileInfo>

#include "preferences.h"
#include "global.h"
using namespace Global;

inline QString convertCodeToBackgroud(int code)
{
    if (code == 100 || code == 900) {
        return ":/res/background/weather-clear.png";
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
        return ":/res/background/weather-clear.png";
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
//    , m_mousePressed(false)
    , m_centralWidget(new QWidget(this))
    , m_titleBar(new TitleBar(this))
    , m_contentWidget(new ContentWidget(this))
    , m_pingbackTimer(new QTimer(this))
    , m_tipTimer(new QTimer(this))
    , m_autoRefreshTimer(new QTimer(this))
    , m_actualizationTime(0)
    , m_weatherWorker(new WeatherWorker(this))
    , m_maskWidget(new MaskWidget(this))//MaskWidget::Instance();
{
    judgeSystemLanguage();

    this->setFixedSize(355, 552);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setFocusPolicy(Qt::StrongFocus);//this->setFocusPolicy(Qt::NoFocus);
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
//    this->setAttribute(Qt::WA_TranslucentBackground, true);
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);//ubuntu 16.04 可能需要加上Qt::WindowMinimizeButtonHint，否则showMinimized无效
    this->setWindowTitle(tr("Kylin Weather"));
    //const auto ratio = qApp->devicePixelRatio();
    this->setWindowIcon(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/indicator-china-weather.png"))/*.pixmap(QSize(64, 64) * ratio)*/);
    this->setStyleSheet("QMainWindow{color:white;background-image:url(':/res/background/weather-clear.png');background-repeat:no-repeat;}");

    global_init();

    m_updateTimeStr = QString(tr("Refresh time"));

    m_layout = new QVBoxLayout(m_centralWidget);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);

    m_layout->addWidget(m_titleBar);//height:32
    m_layout->addWidget(m_contentWidget);//height:520
    this->setCentralWidget(m_centralWidget);

    this->initMenuAndTray();
    m_titleBar->setCityName(m_preferences->m_currentCity);

    //第一次启动麒麟天气时执行,创建china-weather-save文件
    //CN101010100,beijing,北京,CN,China,中国
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString savePath = homePath.at(0) + "/.config/china-weather-save";
    if (!isFileExist(savePath)){
        QFile file(savePath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("101010100");
        file.close();
    }

    m_currentDesktop = qgetenv("XDG_CURRENT_DESKTOP");
    if (m_currentDesktop.isEmpty()) {
        m_currentDesktop = qgetenv("XDG_SESSION_DESKTOP");
    }

    if (m_preferences->weather.cond_code.contains(QChar('n'))) {
        this->setStyleSheet("QMainWindow{color:white;background-image:url(':/res/background/weather-clear-night.png');background-repeat:no-repeat;}");
        m_contentWidget->setNightStyleSheets();
        m_titleBar->setNightStyleSheets();
    }
    else {
        QString styleSheetStr = QString("QMainWindow{color:white;background-image:url('%1');background-repeat:no-repeat;}").arg(convertCodeToBackgroud(m_preferences->weather.cond_code.toInt()));
        this->setStyleSheet(styleSheetStr);
        m_contentWidget->setDayStyleSheets();
        m_titleBar->setDayStyleSheets();
    }

    connect(m_titleBar, &TitleBar::requestShowSettingDialog, this, [=] {
        this->showSettingDialog();
    });

    m_hintWidget = new PromptWidget(this);
    m_hintWidget->setIconAndText(":/res/network_warn.png", tr("Network not connected"));
    m_hintWidget->move((this->width() - m_hintWidget->width())/2, (this->height() - m_hintWidget->height())/2);
    m_hintWidget->setVisible(false);

    m_movieWidget = new PromptWidget(tr("Getting data"), this, ":/res/link.gif", true);
    m_movieWidget->move((this->width() - m_hintWidget->width())/2, (this->height() - m_hintWidget->height())/2);
    m_movieWidget->setVisible(false);

    if (!m_weatherWorker->isNetWorkSettingsGood()) {//无网络连接
        m_contentWidget->setNetworkErrorPages();
        m_autoRefreshTimer->stop();
    }
    else {//有网络连接，开始检查互联网是否可以ping通
        m_weatherWorker->netWorkOnlineOrNot();//ping www.baidu.com
    }

    connect(m_weatherWorker, &WeatherWorker::nofityNetworkStatus, this, [=] (const QString &status) {
        if (status == "OK") {//互联网可以ping通
            m_weatherWorker->requestPostHostInfoToWeatherServer();
//            m_weatherWorker->startAutoLocationTask();//开始自动定位城市

            //选择默认城市
            QString m_cityId;
            QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
            QString savePath = homePath.at(0) + "/.config/china-weather-save";
            if (!isFileExist(savePath)){
                m_cityId = "101010100";
            } else {
                QFile file(savePath);
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QByteArray cityId = file.readAll();
                m_cityId = (QString(cityId));
                m_cityId = m_cityId.trimmed();
                file.close();
            }

            m_preferences->resetCurrentCityNameById(m_cityId);
            this->refreshCityActions();
            this->startGetWeather();
        }
        else {//互联网无法ping通
            m_hintWidget->setIconAndText(":/res/network_warn.png", status);
            m_contentWidget->setNetworkErrorPages();
        }
    });

    connect(m_contentWidget, &ContentWidget::requestRetryWeather, this, [=] {
        m_weatherWorker->requestPostHostInfoToWeatherServer();
        m_weatherWorker->startAutoLocationTask();//开始自动定位城市
    });

    connect(m_weatherWorker, &WeatherWorker::responseFailure, this, [=] (int code) {
        m_maskWidget->hide();
        m_autoRefreshTimer->stop();
        m_movieWidget->setVisible(false);
        m_hintWidget->setVisible(true);
        if (code == 0) {
            m_hintWidget->setIconAndText(":/res/network_warn.png", tr("Incorrect access address"));
        }
        else {
            m_hintWidget->setIconAndText(":/res/network_warn.png", QString(tr("Network error code:%1")).arg(QString::number(code)));
        }
        m_contentWidget->setNetworkErrorPages();
    });

    connect(m_weatherWorker, &WeatherWorker::requestDiplayServerNotify, this, [=] (const QString &notifyInfo) {
        if (!notifyInfo.isEmpty() && m_preferences->m_serverNotify)
            m_contentWidget->showServerNotifyInfo(notifyInfo);
    });

    connect(m_weatherWorker, &WeatherWorker::observeDataRefreshed, this, [=] (const ObserveWeather &data) {
        m_maskWidget->hide();
        m_autoRefreshTimer->start(m_preferences->m_updateFrequency * 1000 * 60);
        if (m_preferences->m_currentCity.isEmpty()) {
            m_preferences->m_currentCity = data.city;
        }
        m_movieWidget->setVisible(false);
        m_titleBar->setCityName(data.city);
        m_contentWidget->refreshObserveUI(data);
        this->refreshTrayMenuWeather(data);
        QString condCodeStr = data.cond_code;
        if (!condCodeStr.isEmpty()) {
            if (condCodeStr.contains(QChar('n'))) {
                this->setStyleSheet("QMainWindow{color:white;background-image:url(':/res/background/weather-clear-night.png');background-repeat:no-repeat;}");
                m_contentWidget->setNightStyleSheets();
                m_titleBar->setNightStyleSheets();
            }
            else {
                QString styleSheetStr = QString("QMainWindow{color:white;background-image:url('%1');background-repeat:no-repeat;}").arg(convertCodeToBackgroud(condCodeStr.toInt()));
                this->setStyleSheet(styleSheetStr);
                m_contentWidget->setDayStyleSheets();
                m_titleBar->setDayStyleSheets();
            }
        }
    });

    connect(m_weatherWorker, &WeatherWorker::forecastDataRefreshed, this, [=] (const QList<ForecastWeather> &datas, const LifeStyle &data) {
        m_maskWidget->hide();
        if (!m_autoRefreshTimer->isActive()) {
            m_autoRefreshTimer->start(m_preferences->m_updateFrequency * 1000 * 60);
        }

        m_movieWidget->setVisible(false);

        int len = datas.size();
        if (len > 3) {
            len = 3;
        }
        for (int i = 0; i < len; ++i) {
            m_contentWidget->refreshForecastUI(datas[i], i);
        }

        m_contentWidget->refreshLifestyleUI(data);
    });

    //自动定位成功后，更新各个控件的默认城市数据，并开始获取天气数据
    connect(m_weatherWorker, &WeatherWorker::requestAutoLocationData, this, [=] (const CitySettingData &info, bool success) {
        if (success) {//自动定位城市成功后，更新各个ui，然后获取天气数据
            if (m_setttingDialog) {
                m_setttingDialog->addCityItem(info);
                m_setttingDialog->refreshCityList(m_preferences->m_currentCity);
            }
            this->refreshCityActions();
            m_cityMenu->menuAction()->setText(info.name);
            this->startGetWeather();
        }
        else {//自动定位城市失败后，获取天气数据
            this->startGetWeather();
        }
    });

    m_tipTimer->setInterval(60*1000);
    m_tipTimer->setSingleShot(false);
    connect(m_tipTimer, &QTimer::timeout, this, static_cast<void (MainWindow::*)()>(&MainWindow::updateTimeTip));

    connect(m_autoRefreshTimer, &QTimer::timeout, this, [=] {
        this->startGetWeather();
    });


    double value = m_preferences->m_opacity*0.01;
    if (value < 0.6) {
        value = 0.60;
        m_preferences->m_opacity = 60;
    }
    this->setOpacity(value);

    this->setVisible(false);
}

MainWindow::~MainWindow()
{
    if (m_tipTimer->isActive()) {
        m_tipTimer->stop();
        delete m_tipTimer;
    }

    global_end();
}

void MainWindow::judgeSystemLanguage()
{
    QLocale locale;
    //获取系统语言环境
    if( locale.language() == QLocale::Chinese )   {
        qDebug() << "Chinese system";
        return;
    } else {
        qDebug() << "Non-chinese system";
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

void MainWindow::updateTimeTip()
{
    QDateTime time = QDateTime::currentDateTime();
    int timeIntValue = time.toTime_t();
    int ut = int((round(timeIntValue - m_actualizationTime)/60));
    if (ut == 0 || m_actualizationTime == 0) {
        m_updateTimeStr = QString(tr("Refresh time:Just updated"));
    }
    else {
        if (ut < 2) {
            m_updateTimeStr = QString(tr("Refresh time:%1 minute ago")).arg(QString::number(ut));
        }
        else {
            m_updateTimeStr = QString(tr("Refresh time:%1 minutes ago")).arg(QString::number(ut));
        }
    }

    if (QDateTime::currentDateTime().toTime_t()-m_actualizationTime > m_preferences->m_updateFrequency*1000*60) {
        m_actualizationTime = QDateTime::currentDateTime().toTime_t();
    }

    m_updateTimeAction->setText(m_updateTimeStr);

    //m_weatherWorker->requestPingBackWeatherServer();
}

void MainWindow::setOpacity(double opacity)
{
    this->setWindowOpacity(opacity);
}

void MainWindow::startGetWeather()
{
    m_maskWidget->showMask();
    m_tipTimer->stop();
    m_movieWidget->setVisible(true);
    m_titleBar->setCityName(m_preferences->m_currentCity);
    m_weatherWorker->refreshObserveWeatherData(m_preferences->m_currentCityId);
    m_weatherWorker->refreshForecastWeatherData(m_preferences->m_currentCityId);
}

void MainWindow::initMenuAndTray()
{
    m_mainMenu = new QMenu(this);
    m_cityMenu = new QMenu(this);
    m_cityMenu->menuAction()->setText(tr("City"));
    m_cityActionGroup = new MenuActionGroup(this);
    connect(m_cityActionGroup, &MenuActionGroup::activated, this, [=] (int index) {
        QString cur_cityName = m_cityActionGroup->setCurrentCheckedIndex(index);
        if (m_preferences->citiesCount() > 0 && m_preferences->citiesCount() >= index) {
            m_cityMenu->menuAction()->setText(cur_cityName/*m_preferences->cityName(index)*/);

            m_preferences->setCurrentCityIdAndName(cur_cityName/*index*/);

            if (m_setttingDialog) {
                m_setttingDialog->refreshCityList(m_preferences->m_currentCity);
            }
            this->startGetWeather();
        }
    });
    this->refreshCityActions();

    m_mainMenu->addMenu(m_cityMenu);
    //QAction *m_switchAciton = m_mainMenu->addAction(tr("Switch"));
    m_mainMenu->addSeparator();

    m_weatherAction = new QAction("N/A",this);
    m_temperatureAction = new QAction("N/A",this);
    m_sdAction = new QAction("N/A",this);
    m_aqiAction = new QAction("N/A",this);
    m_releaseTimeAction = new QAction(tr("Release time"),this);
    m_updateTimeAction = new QAction(m_updateTimeStr,this);
    m_mainMenu->addAction(m_weatherAction);
    m_mainMenu->addAction(m_temperatureAction);
    m_mainMenu->addAction(m_sdAction);
    m_mainMenu->addAction(m_aqiAction);
    m_mainMenu->addAction(m_releaseTimeAction);
    m_mainMenu->addAction(m_updateTimeAction);

    QAction *m_forecastAction = m_mainMenu->addAction(tr("Weather Forecast"));
    connect(m_forecastAction, &QAction::triggered, this, [=] {
        if (!this->isVisible()) {
            this->movePosition();
        }
        else {
            this->setVisible(false);
        }
    });

    m_mainMenu->addSeparator();
    QAction *m_settingAction = m_mainMenu->addAction(tr("Settings"));
    m_settingAction->setIcon(QIcon(":/res/prefs.png"));
    QAction *m_aboutAction = m_mainMenu->addAction(tr("Kylin Weather - About"));
    m_aboutAction->setIcon(QIcon(":/res/about_normal.png"));
    QAction *m_quitAction = m_mainMenu->addAction(tr("Exit"));
    m_quitAction->setIcon(QIcon(":/res/quit_normal.png"));

    //for test change weather background
    /*m_isDN = true;
    connect(m_switchAciton, &QAction::triggered, this, [=] {
        if (m_isDN) {
            m_isDN = false;
            QString currentBg = QString("QMainWindow{color:white;background-image:url('%1');background-repeat:no-repeat;}").arg(":/res/background/weather-clear-night.png");
            this->setStyleSheet(currentBg);
            m_contentWidget->setNightStyleSheets();
            m_titleBar->setNightStyleSheets();
        }
        else {
            m_isDN = true;
            QString currentBg = QString("QMainWindow{color:white;background-image:url('%1');background-repeat:no-repeat;}").arg(":/res/background/weather-clear.png");
            this->setStyleSheet(currentBg);
            m_contentWidget->setDayStyleSheets();
            m_titleBar->setDayStyleSheets();
        }
    });*/

    connect(m_settingAction, &QAction::triggered, this, [=] {
        this->showSettingDialog();
    });
    connect(m_aboutAction, &QAction::triggered, this, [=] {
        AboutDialog dlg;
        dlg.exec();
    });
    connect(m_quitAction, &QAction::triggered, qApp, &QApplication::quit);

    // 初始化托盘所有Icon
    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setToolTip(QString(tr("Kylin Weather")));
    m_systemTray->setIcon(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/indicator-china-weather.png")));
    connect(m_systemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    m_systemTray->show();
    m_systemTray->setContextMenu(m_mainMenu);

    QShortcut *m_quitShortCut = new QShortcut(QKeySequence("Ctrl+Q"), this);
    connect(m_quitShortCut, SIGNAL(activated()), qApp, SLOT(quit()));
}

void MainWindow::showSettingDialog()
{
    if (!m_setttingDialog) {
        createSettingDialog();
    }
    m_setttingDialog->moveToCenter();
}

void MainWindow::createSettingDialog()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_setttingDialog = new SettingDialog;
    m_setttingDialog->setModal(false);
    connect(m_setttingDialog, SIGNAL(applied()), this, SLOT(applySettings()));
    connect(m_setttingDialog, &SettingDialog::requestRefreshCityMenu, this, [this] (bool removedDefault) {
        this->refreshCityActions();

        if (removedDefault) {//刪除了默认城市后，重新设置了列表中第一个城市为默认城市后，从服务端获取该城市的天气
            this->startGetWeather();
        }

        m_preferences->save();
    });
    connect(m_setttingDialog, &SettingDialog::requestRefreshWeatherById, this, [this] (const QString &id) {
        m_preferences->resetCurrentCityNameById(id);
        m_preferences->save();
        this->refreshCityActions();
        this->startGetWeather();
    });
    connect(m_setttingDialog, &SettingDialog::requestChangeOpacity, this, [this] (int opcatity) {
        double value = opcatity*0.01;
        if (value < 0.6) {
            value = 0.60;
            m_preferences->m_opacity = 60;
        }
        this->setOpacity(value);
    });

    QApplication::restoreOverrideCursor();
}

void MainWindow::refreshCityActions()
{
    // clear orig action list
    m_cityActionGroup->clearAllActions();

    // add new action list
    int i = 0;
    int currentIndex = 0;
    foreach (QString city, m_preferences->getCitiesList()) {
        if (city == m_preferences->m_currentCity) {
            currentIndex = i;
        }

        MenuActionGroupItem *cityAction = new MenuActionGroupItem(this, m_cityActionGroup, i);
        cityAction->setActionText(city);
        i++;
    }
    m_cityMenu->addActions(m_cityActionGroup->actions());
    m_cityMenu->menuAction()->setText(m_preferences->m_currentCity);
    m_cityActionGroup->setCurrentCheckedIndex(currentIndex);
}

void MainWindow::refreshTrayMenuWeather(const ObserveWeather &data)
{
    //m_systemTray->setIcon(QIcon::fromTheme(QString("%1").arg(data.cond_code)) );
    m_systemTray->setIcon(QIcon::fromTheme(QString("%1").arg(data.cond_code), QIcon(QString(":/res/weather_icons/white/%1.png").arg(data.cond_code))) );
    m_weatherAction->setText(data.cond_txt);
    //m_temperatureAction->setText(QString(tr("Temperature:%1˚C")).arg(data.tmp));
    m_temperatureAction->setText(QString(tr("Temperature:%1")).arg(data.tmp) + "˚C");
    m_sdAction->setText(QString(tr("Relative humidity:%1")).arg(data.hum));
    if (data.air.isEmpty() || data.air.contains("Unknown")) {
        m_aqiAction->setText(QString(tr("Air quality:%1")).arg(QString(tr("Unknown"))));
    }
    else {
        m_aqiAction->setText(QString(tr("Air quality:%1")).arg(data.air));
    }
    m_releaseTimeAction->setText(QString(tr("Release time:%1")).arg(data.updatetime));

    m_actualizationTime = 0;
    this->updateTimeTip();
    m_tipTimer->start();
}

void MainWindow::applySettings()
{

}

void MainWindow::resetWeatherBackgroud(const QString &imgPath)
{
    QString weatherBg = imgPath;
    if(!QFileInfo(weatherBg).exists())
        weatherBg = ":/res/background/weather-clear.png";
    QString currentBg = QString("QMainWindow{color:white;background-image:url('%1');background-repeat:no-repeat;}").arg(weatherBg);
    this->setStyleSheet(currentBg);
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    /*case QSystemTrayIcon::DoubleClick:
    {
        QRect rect = m_systemTray->geometry();
        QMenu *currentMenu = m_systemTray->contextMenu();
        if (currentMenu->isHidden()) {
            currentMenu->popup(QPoint(rect.x()+8, rect.y()));
        }
    }
        break;*/
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::MiddleClick:
        if (this->isVisible()) {
            this->setVisible(false);
        }
        else {
            this->movePosition();
        }
        break;
    default:
        break;
    }
}

void MainWindow::movePosition()
{
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    QRect screenGeometry = qApp->primaryScreen()->geometry();

    //panel in bottom or right
    if (availableGeometry.x() == screenGeometry.x() && availableGeometry.y() == screenGeometry.y()) {
        this->move(availableGeometry.x() + availableGeometry.width() - this->width(), availableGeometry.height() - this->height());
    }
    else {
        if (availableGeometry.x() > 0) {//panel in left
            this->move(availableGeometry.x(), availableGeometry.y() + availableGeometry.height()  - this->height());
        }
        else {//panel in top
            this->move(availableGeometry.x() + availableGeometry.width() - this->width(), availableGeometry.y());
        }
    }
    this->showNormal();
    this->raise();
    this->activateWindow();

    /*for (QScreen *screen : qApp->screens()) {
        if (screen->geometry().contains(pos)) {
        }
    }*/
}

/*void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        this->m_mousePressed = true;
    }

    if (m_hintWidget->isVisible())
        m_hintWidget->setVisible(false);

    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    this->m_mousePressed = false;

    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (this->m_mousePressed) {
        move(event->globalPos() - this->m_dragPosition);
    }

    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    //m_weatherWorker->refreshForecastWeatherData(m_preferences->m_currentCityId);
}*/

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        if (this->isVisible()) {
            this->setVisible(false);
        }
        event->accept();
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::focusOutEvent(QFocusEvent *event)
{
    if (event->reason() == Qt::ActiveWindowFocusReason) {
        this->setVisible(false);
    }

    return QMainWindow::focusOutEvent(event);
}
