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

#include "indexitemwidget.h"

#include <QApplication>
#include <QPainter>
#include <QDebug>

IndexItemWidget::IndexItemWidget(const QString &name, const QString &iconPath, QWidget *parent)
    : QWidget(parent)
    , m_mouseHover(false)
    , m_iconPath(iconPath)
    , m_brf("-")
    , m_txt("-")
    , m_titleColor(QColor("#cfcfcf"))
    , m_txtColor(QColor("#808080"))
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    if (name == "comf") {
        m_indexName = QString(tr("comf"));
    }
    else if (name == "drsg") {
        m_indexName = QString(tr("drsg"));
    }
    else if (name == "flu") {
        m_indexName = QString(tr("flu"));
    }
    else if (name == "sport") {
        m_indexName = QString(tr("sport"));
    }
    else if (name == "trav") {
        m_indexName = QString(tr("trav"));
    }
    else if (name == "uv") {
        m_indexName = QString(tr("uv"));
    }
    else if (name == "cw") {
        m_indexName = QString(tr("cw"));
    }
    else if (name == "air") {
        m_indexName = QString(tr("air"));
    }
    else {
        m_indexName = name;
    }
}

void IndexItemWidget::refreshLifeStyle(const QString &brf, const QString &txt)
{
    this->m_brf = brf;
    this->m_txt = txt;
    update();
}

void IndexItemWidget::setDayStyleSheets()
{
    m_titleColor = QColor("#cfcfcf");
    m_txtColor = QColor("#808080");
    update();
}

void IndexItemWidget::setNightStyleSheets()
{
    m_titleColor = QColor("#808080");
    m_txtColor = QColor("#cdcdcd");
    update();
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
    QPixmap iconPix = QPixmap(this->m_iconPath);
    if (iconPix.isNull()) {
        iconPix = QPixmap(":/res/air_index.png");
    }
    //Q_ASSERT(!iconPix.isNull());

    QRect iconRect(20, 2, iconPix.width()/ratio, iconPix.height()/ratio);
    painter.drawPixmap(iconRect, iconPix);

    painter.setRenderHint(QPainter::Antialiasing, false);

    QFont font = painter.font();
    font.setPixelSize(12);//font.setPointSize(14);
    QFontMetrics fm(font);
    QPen pen(Qt::white);
    painter.setPen(pen);
    painter.setFont(font);

    QRect valueRect(iconRect.right() + 5, iconRect.y(), fm.width(this->m_brf), 20);
    painter.setPen(QPen(m_txtColor));
    painter.drawText(valueRect, Qt::AlignVCenter | Qt::AlignLeft, this->m_brf);

    font.setPixelSize(10);//font.setPointSize(12);
    QRect titleRect(valueRect.x(), valueRect.bottom(), this->width() - iconPix.width()/ratio - 5*2, 20);
    painter.setPen(QPen(m_titleColor));
    painter.drawText(titleRect, Qt::AlignVCenter | Qt::AlignLeft, this->m_indexName);
}
