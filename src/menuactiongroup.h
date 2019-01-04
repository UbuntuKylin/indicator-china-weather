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

#ifndef MENUACTIONGROUP_H
#define MENUACTIONGROUP_H

#include <QActionGroup>
#include <QWidget>

#include "menuaction.h"


class QAction;
class MenuActionGroup;

class MenuActionGroupItem : public MenuAction
{
    Q_OBJECT
public:
    MenuActionGroupItem(QObject *parent, MenuActionGroup *group, int index);
};

class MenuActionGroup : public QActionGroup
{
    Q_OBJECT

public:
    MenuActionGroup (QObject *parent);

    QString setCurrentCheckedIndex(int index);
    int getCurrentCheckedIndex();
    void uncheckAll();
    void clearAllActions();
    void setActionsEnabled(bool enable);

signals:
    void activated(int);
};

#endif // MENUACTIONGROUP_H
