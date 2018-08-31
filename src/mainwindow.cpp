/*
 * Copyright (C) 2013 ~ 2018 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
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
#include "hintwidget.h"
#include "weatherworker.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QMouseEvent>
#include <QMenu>
#include <QScreen>
#include <QShortcut>
#include <QDebug>

#include "preferences.h"
#include "citieslist.h"
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
    , m_mousePressed(false)
    , m_weatherWorker(new WeatherWorker(this))
    , m_centralWidget(new QWidget(this))
    , m_titleBar(new TitleBar(this))
    , m_contentWidget(new ContentWidget(m_weatherWorker, this))
{
    this->setFixedSize(355, 552);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);//需要加上Qt::WindowMinimizeButtonHint，否则showMinimized无效
    this->setWindowTitle(tr("Kylin Weather"));
    this->setWindowIcon(QIcon(":/res/indicator-china-weather.png"));
    this->setStyleSheet("QMainWindow{color:white;background-image:url(':/res/background/weather-clear.png');background-repeat:no-repeat;}");

    global_init();

    m_layout = new QVBoxLayout(m_centralWidget);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);

    m_layout->addWidget(m_titleBar);//height:32
    m_layout->addWidget(m_contentWidget);//height:520
    this->setCentralWidget(m_centralWidget);

    this->initMenuAndTray();

    this->moveTopRight();

    //TODO:read background from ini file
    this->setStyleSheet("QMainWindow{color:white;background-image:url(':/res/background/weather-clear.png');background-repeat:no-repeat;}");
    this->m_contentWidget->setDayStyleSheets();
    this->m_titleBar->setDayStyleSheets();

    connect(m_titleBar, &TitleBar::requestShowSettingDialog, this, [=] {
        this->showSettingDialog();
    });

    m_hintWidget = new HintWidget(this);
    m_hintWidget->setIconAndText(":/res/network_warn.png", tr("Network not connected"));
    m_hintWidget->move((this->width() - m_hintWidget->width())/2, (this->height() - m_hintWidget->height())/2);
    m_hintWidget->setVisible(false);

    m_movieWidget = new HintWidget(tr("Getting data"), this, ":/res/link.gif", true);
    m_movieWidget->move((this->width() - m_hintWidget->width())/2, (this->height() - m_hintWidget->height())/2);
    m_movieWidget->setVisible(false);

    if (!m_weatherWorker->isNetWorkSettingsGood()) {
        m_contentWidget->setNetworkErrorPages();
    }
    else {
        m_movieWidget->setVisible(true);
        m_weatherWorker->refreshObserveWeatherData(m_preferences->currentCityId);
        m_weatherWorker->refreshForecastWeatherData(m_preferences->currentCityId);
    }

    connect(m_contentWidget, &ContentWidget::requestRetryWeather, this, [=] {
        m_movieWidget->setVisible(true);
        m_weatherWorker->refreshObserveWeatherData(m_preferences->currentCityId);
        m_weatherWorker->refreshForecastWeatherData(m_preferences->currentCityId);
    });

    connect(m_weatherWorker, &WeatherWorker::responseFailure, this, [=] (int code) {
        m_movieWidget->setVisible(false);
        m_hintWidget->setVisible(true);
        if (code == 0) {
            m_hintWidget->setIconAndText(":/res/network_warn.png", tr("Incorrect access address"));
        }
        else {
            m_hintWidget->setIconAndText(":/res/network_warn.png", QString(tr("Network error code:%1")).arg(QString::number(code)));
        }
    });

    connect(m_weatherWorker, &WeatherWorker::observeDataRefreshed, this, [=] (const ObserveWeather &data) {
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
}

MainWindow::~MainWindow()
{
    global_end();
}

void MainWindow::setOpacity(double opacity)
{
    this->setWindowOpacity(opacity);
}

void MainWindow::initMenuAndTray()
{
//    m_cityList << "北京" << "上海" << "长沙";
    if (m_preferences->m_cityList == NULL) {
        return;
    }
//    m_preferences->m_cityList->addCityToStringList("北京");
//    m_preferences->m_cityList->addCityToStringList("上海");
//    m_preferences->m_cityList->addCityToStringList("长沙");

    m_mainMenu = new QMenu(this);
    m_cityMenu = new QMenu(this);
    m_cityMenu->menuAction()->setText(tr("City"));
    m_cityActionGroup = new MenuActionGroup(this);
    connect(m_cityActionGroup, &MenuActionGroup::activated, this, [=] (int index) {
        m_cityActionGroup->setCurrentCheckedIndex(index);
//        m_cityMenu->menuAction()->setText(this->m_cityList.at(index));
        if (m_preferences->m_cityList->count() > 0 && m_preferences->m_cityList->count() >= index) {
            m_cityMenu->menuAction()->setText(m_preferences->m_cityList->cityName(index));
        }
    });
    this->refreshCityActions();

    m_mainMenu->addMenu(m_cityMenu);
    QAction *m_switchAciton = m_mainMenu->addAction(tr("Switch"));
    m_mainMenu->addSeparator();

    m_weatherAction = new QAction("N/A",this);
    m_temperatureAction = new QAction("N/A",this);
    m_sdAction = new QAction("N/A",this);
    m_aqiAction = new QAction("N/A",this);
    m_releaseTimeAction = new QAction(tr("Release time"),this);
    m_updateTimeAction = new QAction(tr("Refresh time"),this);
    m_mainMenu->addAction(m_weatherAction);
    m_mainMenu->addAction(m_temperatureAction);
    m_mainMenu->addAction(m_sdAction);
    m_mainMenu->addAction(m_aqiAction);
    m_mainMenu->addAction(m_releaseTimeAction);
    m_mainMenu->addAction(m_updateTimeAction);

    QAction *m_forecastAction = m_mainMenu->addAction(tr("Weather Forecast"));
    connect(m_forecastAction, &QAction::triggered, this, [=] {
        this->showNormal();
    });

    QAction *m_aboutAction = m_mainMenu->addAction(tr("Kylin Weather - About"));
    m_aboutAction->setIcon(QIcon(":/res/about_normal.png"));
    QAction *m_quitAction = m_mainMenu->addAction(tr("Exit"));
    m_quitAction->setIcon(QIcon(":/res/quit_normal.png"));

    //for test change weather background
    m_isDN = true;
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
    });
    connect(m_aboutAction, &QAction::triggered, this, [=] {
        AboutDialog dlg;
        dlg.exec();
    });
    connect(m_quitAction, &QAction::triggered, qApp, &QApplication::quit);

    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setToolTip(QString(tr("Kylin Weather")));
    m_systemTray->setIcon(QIcon(":/res/indicator-china-weather.png"));
    connect(m_systemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    m_systemTray->show();
    m_systemTray->setContextMenu(m_mainMenu);

    QShortcut *m_quitShortCut = new QShortcut(QKeySequence("Ctrl+Q"), this);
    connect(m_quitShortCut, SIGNAL(activated()), qApp, SLOT(quit()));

//    m_weatherWorker->requestPostHostInfoToWeatherServer("distro=ubuntu&version_os=16.04&version_weather=1.0&city=长沙");
//    m_weatherWorker->requestPingBackWeatherServer();
}

void MainWindow::showSettingDialog()
{
    if (!m_setttingDialog) {
        createSettingDialog();
    }

    m_setttingDialog->move((width() - m_setttingDialog->width()) / 2 + mapToGlobal(QPoint(0, 0)).x(),
                               (window()->height() - m_setttingDialog->height()) / 2 + mapToGlobal(QPoint(0, 0)).y());
    m_setttingDialog->show();
}

void MainWindow::createSettingDialog()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_setttingDialog = new SettingDialog;
    m_setttingDialog->setModal(false);
    connect(m_setttingDialog, SIGNAL(applied()), this, SLOT(applySettings()));
    connect(m_setttingDialog, &SettingDialog::requesetSetCurrentCity, this, [this] (const LocationData &data) {
        qDebug() << "main get city's id=" << data.id;
//        this->m_cityList.append(data.city);
        m_preferences->m_cityList->addCityToStringList(data.city);
        this->refreshCityActions();
    });
    connect(m_setttingDialog, &SettingDialog::requestRemoveCityFromMenu, this, [this] (const QString &name) {
        /*bool hasFound = false;
        for (int i=0; i<m_cityList.length(); i++) {
            if (m_cityList.at(i) == name) {
                this->m_cityList.removeAt(i);
                hasFound = true;
                break;
            }
        }
        if (hasFound) {
            this->refreshCityActions();
        }*/
        m_preferences->m_cityList->removeCityFromStringList(name);
        this->refreshCityActions();
    });

    QApplication::restoreOverrideCursor();
}

void MainWindow::refreshCityActions()
{
    // clear orig action list
    m_cityActionGroup->clearAllActions();

    // add new action list
    //qDebug() << m_cityList;
    int i = 0;
//    foreach (QString city, m_cityList) {
    foreach (QString city, m_preferences->m_cityList->getCitiesList()) {
//        m_cityMenu->addAction(city);
        MenuActionGroupItem *cityAction = new MenuActionGroupItem(this, m_cityActionGroup, i);
        cityAction->setActionText(city);
        i++;
    }
    m_cityMenu->addActions(m_cityActionGroup->actions());
    m_cityMenu->menuAction()->setText("北京");
    m_cityActionGroup->setCurrentCheckedIndex(0);
}

void MainWindow::refreshTrayMenuWeather(const ObserveWeather &data)
{
    m_systemTray->setIcon(QIcon(QString(":/res/weather_icons/lightgrey/%1.png").arg(data.cond_code)));
    m_weatherAction->setText(data.cond_txt);
    m_temperatureAction->setText(QString(tr("Temperature:%1˚C")).arg(data.tmp));
    m_sdAction->setText(QString(tr("Relative humidity:%1")).arg(data.hum));
    m_aqiAction->setText(QString(tr("Air quality:%1")).arg(data.air));
    m_releaseTimeAction->setText(QString(tr("Release time:%1")).arg(data.updatetime));
//    m_updateTimeAction->setText(QString(tr("Refresh time:%1")).arg(data.updatetime));
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
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->show();
        break;
    default:
        break;
    }
}

void MainWindow::moveTopRight()
{
    QPoint pos = QCursor::pos();
    QRect primaryGeometry;
    for (QScreen *screen : qApp->screens()) {
        if (screen->geometry().contains(pos)) {
            primaryGeometry = screen->geometry();
        }
    }

    if (primaryGeometry.isEmpty()) {
        primaryGeometry = qApp->primaryScreen()->geometry();
    }

    this->move(primaryGeometry.x() + primaryGeometry.width() - this->width(), primaryGeometry.y());
    this->show();
    this->raise();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
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

    //m_weatherWorker->refreshForecastWeatherData(m_preferences->currentCityId);
}
