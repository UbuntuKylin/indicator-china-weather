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

#include "menuactiongroup.h"

#include <QAction>
#include <QList>
#include <QWidget>

MenuActionGroupItem::MenuActionGroupItem(QObject *parent, MenuActionGroup *group, int index)
    : MenuAction(parent)
{
    this->setCheckable(true);
    this->setData(QVariant(index));

    if (group) {
        group->addAction(this);
    }
}

MenuActionGroup::MenuActionGroup(QObject *parent) : QActionGroup(parent)
{
    setExclusive(true);
    connect(this, &MenuActionGroup::triggered, this, [=] (QAction *action) {
        int value = action->data().toInt();
        emit activated(value);
    });
}

QString MenuActionGroup::setCurrentCheckedIndex(int index)
{
    QString text;
    QList <QAction *> actionList = actions();
    for (int i=0; i < actionList.count(); i++) {
        if ((!actionList[i]->isSeparator()) && (actionList[i]->data().toInt() == index)) {
            actionList[i]->setChecked(true);
            text = actionList[i]->text();
            break;
        }
    }

    return text;
}

int MenuActionGroup::getCurrentCheckedIndex()
{
    QAction *action = checkedAction();
    if (action) {
        return action->data().toInt();
    }
    else {
        return -1;
    }
}

void MenuActionGroup::uncheckAll()
{
    QList <QAction *> actionList = actions();
    for (int i=0; i < actionList.count(); i++) {
        actionList[i]->setChecked(false);
    }
}

void MenuActionGroup::clearAllActions()
{
    while (actions().count() > 0) {
        QAction *action = actions()[0];
        if (action) {
            removeAction(action);
            action->deleteLater();
        }
    }
}

void MenuActionGroup::setActionsEnabled(bool enable)
{
    QList <QAction *> actionList = actions();
    for (int i=0; i < actionList.count(); i++) {
        actionList[i]->setEnabled(enable);
    }
}
