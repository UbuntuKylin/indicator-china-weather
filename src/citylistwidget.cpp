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


#include "citylistwidget.h"
#include "cityitemwidget.h"
#include "data.h"

#include <QListWidgetItem>
#include <QScrollBar>
#include <QTimer>
#include <QApplication>
#include <QDebug>

CityListWidget::CityListWidget(QWidget *parent) :
    QWidget(parent)
    , m_layout(new QVBoxLayout)
    , m_timer(new QTimer(this))
{
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(1);
    this->setLayout(m_layout);

    m_timer->setSingleShot(true);
    m_timer->setInterval(15);
    connect(m_timer, &QTimer::timeout, this, &CityListWidget::updateCityListHeight, Qt::QueuedConnection);
}

CityListWidget::~CityListWidget()
{
    if (m_timer) {
        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(updateCityListHeight()));
        if(m_timer->isActive()) {
            m_timer->stop();
        }
        delete m_timer;
        m_timer = nullptr;
    }

    const int count = m_layout->count();
    for (int i = 0; i != count; ++i) {
        QLayoutItem *item = m_layout->takeAt(0);
        QWidget *w = item->widget();
        w->removeEventFilter(this);
        w->setParent(nullptr);
        delete item;
    }
}

void CityListWidget::appendItem(CityItemWidget *item)
{
    const int curIndex = m_layout->count();
    m_layout->insertWidget(curIndex, item);
    item->installEventFilter(this);

    m_timer->start();
}

void CityListWidget::removeItem(CityItemWidget *item)
{
    m_layout->removeWidget(item);
    item->removeEventFilter(this);

    m_timer->start();
}

int CityListWidget::itemCount() const
{
    return m_layout->count();
}

void CityListWidget::clearUI()
{
    const int count = m_layout->count();
    for (int i = 0; i != count; ++i) {
        QLayoutItem *item = m_layout->takeAt(0);
        QWidget *w = item->widget();
        w->removeEventFilter(this);
        w->setParent(nullptr);
        delete item;
    }

    m_timer->start();
}

CityItemWidget *CityListWidget::getItem(int index)
{
    if(index < 0) {
        return NULL;
    }

    if(index < itemCount()) {
        return qobject_cast<CityItemWidget *>(m_layout->itemAt(index)->widget());
    }

    return NULL;
}

void CityListWidget::updateCityListHeight()
{
    this->setFixedHeight(m_layout->sizeHint().height());
}

void CityListWidget::resetData()
{

}

bool CityListWidget::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Resize:
        m_timer->start();
        break;
    default:
        break;
    }

    return false;
}
