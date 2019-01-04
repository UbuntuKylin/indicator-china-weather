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

#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

#include "data.h"

class QVBoxLayout;
class QStackedWidget;
class SettingTitleBar;
//class CityListWidget;
class CityWidget;
class SpinBox;
class QSlider;
class QGroupBox;

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

    void setData();
    void refreshCityList(const QString &id);

public slots:
    virtual void accept();
    virtual void reject();

signals:
    void applied();
    void requestRefreshCityMenu(bool removedDefault);
    void requestRefreshWeatherById(const QString &id);
    //void requestSetDefaultCity();
    void requestChangeOpacity(int opacity);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event);

private:
    QPoint m_dragPosition;
    bool m_mousePressed;

    QVBoxLayout *m_mainLayout = nullptr;
    SettingTitleBar *m_titleBar = nullptr;
    QStackedWidget *m_stackedWidget = nullptr;

    CityWidget *m_cityWidget = nullptr;
    /*QWidget *m_cityWidget = nullptr;
    CityListWidget *m_cityListWidget = nullptr;
    QPushButton *m_addCityBtn = nullptr;*/

    QWidget *m_systemWidget = nullptr;
    QGroupBox *m_variableGroup = nullptr;
    QGroupBox *m_fixedGroup = nullptr;
    SpinBox *m_spinBox = nullptr;
    QSlider *m_opacitySlider = nullptr;

//    QPushButton *m_okBtn = nullptr;
};

#endif // SETTINGDIALOG_H
