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

#ifndef SETTINGTITLEBAR_H
#define SETTINGTITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class SettingTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit SettingTitleBar(QWidget *parent = 0);
    ~SettingTitleBar();

    void initLeftContent();
    void initMiddleContent();
    void initRightContent();
    void initBottomContent();
    void initWidgets();

public slots:
    void showWarnInfo(const QString &info);

signals:
    void requestCloseDialog();
    void requestSwitchPage(bool is_isFirs);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QVBoxLayout *m_layout = nullptr;
    QHBoxLayout *m_toplayout = nullptr;
    QHBoxLayout *m_lLayout = nullptr;
    QHBoxLayout *m_mLayout = nullptr;
    QHBoxLayout *m_rLayout = nullptr;
    QHBoxLayout *m_bLayout = nullptr;
    QPushButton *m_closeBtn = nullptr;
    QLabel *m_tipLabel = nullptr;
    QTimer *m_timer = nullptr;
    QColor m_borderColor;
    int m_borderRadius;
    int m_borderWidth;
    QBrush m_bgBrush;
};

#endif // SETTINGTITLEBAR_H
