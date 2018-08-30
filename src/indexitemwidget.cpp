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

#include "indexitemwidget.h"

#include <QApplication>
#include <QPainter>
#include <QDebug>

IndexItemWidget::IndexItemWidget(const QString &name, QWidget *parent)
    : QWidget(parent)
    , m_mouseHover(false)
    , m_indexName(name)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void IndexItemWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    this->m_mouseHover = true;
    emit this->requestShowMsg(m_indexName);
    update();
}

void IndexItemWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    this->m_mouseHover = false;
    update();
}

void IndexItemWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, m_mouseHover ? 20 : 0));

    const qreal ratio = qApp->devicePixelRatio();

    painter.setRenderHint(QPainter::Antialiasing, true);
    QPixmap icon = QPixmap(":/res/clothe_index.png");
    QRect iconRect(20, 2, icon.width()/ratio, icon.height()/ratio);
    painter.drawPixmap(iconRect, icon);

    painter.setRenderHint(QPainter::Antialiasing, false);

    QFont font = painter.font();
    font.setPixelSize(14);//font.setPointSize(14);
    QFontMetrics fm(font);
    QPen pen(Qt::white);
    painter.setPen(pen);
    painter.setFont(font);

    QString valueStr = "35.0KM";

    QRect valueRect(iconRect.right() + 5, iconRect.y(), fm.width(valueStr), 20);
    painter.setPen(QPen(QColor("#808080")));
    painter.drawText(valueRect, Qt::AlignVCenter | Qt::AlignLeft, valueStr);

    font.setPixelSize(12);//font.setPointSize(12);
    fm = QFontMetrics(font);
    QString unitText = "°C";
    painter.setPen(QPen(QColor("#cfcfcf")));
    painter.drawText(QRect(valueRect.x(), valueRect.bottom(), fm.width(unitText), 20), Qt::AlignVCenter | Qt::AlignLeft, unitText);
}
