#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwindow.h"
#include "leftupcitybtn.h"
#include "leftupsearchbox.h"
#include "leftupsearchview.h"
#include "leftupsearchdelegate.h"
#include "informationwidget.h"
#include "locationworker.h"
#include "weathermanager.h"
#include "promptwidget.h"
#include "data.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <xcb/xcb.h>

#include <QMainWindow>
#include <QStandardPaths>
#include <QMouseEvent>
#include <QDebug>
#include <QButtonGroup>
#include <QObjectUserData>
#include <QSortFilterProxyModel>
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QRect>
#include <QScreen>
#include <QDesktopWidget>
#include <QTime>
#include <QTimer>
#include <QFileInfo>
#include <QLocale>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onRefreshMainWindowWeather();
    void onHandelAbnormalSituation(QString abnormalText);

    void onSetForecastWeather(ForecastWeather m_forecastweather);
    void onSetObserveWeather(ObserveWeather m_observeweather);
    void onSetLifeStyle(LifeStyle m_lifestyle);

private slots:
    void on_btnMinimize_clicked();

    void on_btnCancel_clicked();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::MainWindow *ui;
    QScrollArea *m_scrollarea = nullptr;
    QWidget *m_scrollwidget = nullptr;
    LeftUpCityBtn *m_leftupcitybtn = nullptr;
    LeftUpSearchBox *m_leftupsearchbox = nullptr;
    WeatherManager* m_weatherManager = nullptr;
    PromptWidget *m_hintWidget = nullptr;
    PromptWidget *m_movieWidget = nullptr;
    LeftUpSearchView *m_searchView = nullptr;
    LeftUpSearchDelegate *m_delegate = nullptr;
    QSortFilterProxyModel* m_proxyModel = nullptr;
    QStandardItemModel *m_model = nullptr;
    LocationWorker *m_locationWorker = nullptr;
    Information *m_information;
    QSystemTrayIcon *m_trayIcon = nullptr;
    QTimer *m_refreshweather;

    void judgeSystemLanguage();
    void checkSingle();

    bool isFileExist(QString fullFileName);

    void createTrayIcon();
    void handleIconClicked();

    void onSearchBoxEdited();
    void searchCityName();

    void initControlQss();

    void setAbnormalMainWindow();

    QString convertCodeToBackgroud(int code);
    void convertCodeToTrayIcon(QString code);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // MAINWINDOW_H
