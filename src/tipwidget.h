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

#ifndef TIPWIDGET_H
#define TIPWIDGET_H

#include <QLabel>
#include <QPushButton>

class TipWidget : public QFrame {
    Q_OBJECT
public:
    explicit TipWidget(QWidget *parent = 0);
    ~TipWidget();

    void setLabelIcon(const QString &iconPath);
    void setLabelText(const QString &text);

protected:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    QLabel *m_icon = nullptr;
    QLabel *m_text = nullptr;
    QPushButton *m_closeBtn = nullptr;
};
#endif // TIPWIDGET_H
