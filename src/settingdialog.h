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
class QGroupBox;

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

    void setData();

public slots:
    virtual void accept();
    virtual void reject();

signals:
    void applied();
    void requeAddCityToMenu(const LocationData &data);
    void requestRemoveCityFromMenu(const QString &id);
    void requestSetDefaultCity();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QPoint m_dragPosition;
    bool m_mousePressed;

    QVBoxLayout *m_mainLayout = nullptr;
    SettingTitleBar *m_titleBar = nullptr;
    QStackedWidget *m_stackedWidget = nullptr;

    CityWidget *m_locationWidget = nullptr;
    /*QWidget *m_locationWidget = nullptr;
    CityListWidget *m_cityListWidget = nullptr;
    QPushButton *m_addCityBtn = nullptr;*/

    QWidget *m_systemWidget = nullptr;
    QGroupBox *m_variableGroup = nullptr;
    QGroupBox *m_fixedGroup = nullptr;
    SpinBox *m_spinBox = nullptr;

//    QPushButton *m_okBtn = nullptr;
};

#endif // SETTINGDIALOG_H
