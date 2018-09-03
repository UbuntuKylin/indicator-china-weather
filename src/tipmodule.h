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

#ifndef TIPMODULE_H
#define TIPMODULE_H

#include <QObject>

class QTimer;

class TipModule: public QObject
{
    Q_OBJECT
public:
    TipModule(QObject *parent = 0);
    ~TipModule();

public slots:
    void onDisplayTimeOut();

private:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

private:
    QWidget *m_parentWidget = nullptr;
    QWidget *m_tipWidget = nullptr;
    QTimer  *m_displayTimer = nullptr;
};

#endif // TIPMODULE_H
