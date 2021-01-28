#ifndef MENUMODULE_H
#define MENUMODULE_H
#define DEBUG_MENUMODULE 0  //DEBUG模式开关，代码上线之前务必关掉
#define FITTHEMEWINDOW "org.ukui.style"
#define APPDATA "org.china-weather-data.settings"


#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>
#include <QDebug>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QUrl>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QRect>
#include <QScreen>
#include <QGSettings>
#include <QMetaEnum>
#include "daemondbus.h"
#include "addcityaction.h"

class menuModule : public QWidget
{
    Q_OBJECT
public:
    explicit menuModule(QWidget *);
    void themeUpdate();
    AddCityAction *addCityAction = nullptr;



signals:
    void menuModuleClose();
    void menuModuleSetThemeStyle(QString);
public:
    QPushButton *menuButton = nullptr;

public:
//    程序在实例化的时候需要传递的信息字段,打开debug开关后这些字段会被自动填充
    QString appName = "appName字段未填充!"; //格式kylin-usb-creator
    QString appShowingName = "app展示名字段未填充"; //格式kylin usb creator ,用来在前端展示
    QString appVersion = "appVersion字段未填充!";
    QString appDesc = "appDesc字段未填充!";
    QString iconPath = "iconPath字段未填充!";
    QString confPath = "org.china-weather-data.settings";

private:
    QMenu *m_menu = nullptr;
    QMenu *themeMenu = nullptr;
    QSize iconSize;
//    QString appPath = "tools/kylin-usb-creator"; //拉起帮助菜单时使用appName字段
    QWidget *aboutWindow = nullptr;
    QGSettings *m_pGsettingThemeData = nullptr;
    QGSettings *m_pGsettingThemeStatus = nullptr;
    enum typeThemeStatus {
        themeAuto = 0,
        themeBlackOnly = 1,
        themeLightOnly = 2
    } themeStatus;
public slots:
    void dealSystemGsettingChange(const QString);
private:
    void init();
    QHBoxLayout* initTitleBar(); //关于窗口标题栏初始化
    QVBoxLayout* initBody();    // 关于窗口body初始化
    void initGsetting();
    void initAction();
    void setStyle();
    void triggerMenu(QAction* act); //主菜单动作4
    void triggerThemeMenu(QAction* act); //主题动作
    void aboutAction();
    void initAbout(); //关于窗口初始化
    void helpAction();
    void setThemeFromLocalThemeSetting(QList<QAction* >); //获取本地主题配置
    void setStyleByThemeGsetting(); //通过外部主题配置设置主题
    void setThemeStyle();
    void setThemeLight();
    void setThemeDark();

    void refreshThemeBySystemConf();    //通过系统配置更改主题
    QPushButton *titleBtnClose = nullptr;
    QLabel* titleText = nullptr;
    QLabel* bodyAppVersion = nullptr;
    QLabel* bodySupport = nullptr;
    QLabel* bodyAppName = nullptr;
};

#endif // MENUMODULE_H
