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

#include "texttip.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QDebug>

TextTip::TextTip(const QString &txt, QWidget *parent) : QFrame(parent)
  , m_radius(5)
  , m_background(QBrush(QColor(255,255,255,255)))
  , m_borderColor(QColor(224,224,224,130))
{
    this->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setContentsMargins(0, 0, 0, 0);
    this->setMinimumSize(260, 56);

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);

    m_frame = new QFrame();
    m_frame->setContentsMargins(0, 0, 0, 0);

    m_textLabel = new QLabel;
    m_textLabel->setWordWrap(true);//QLabel自动换行
    m_textLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080; font-size:12px;}");
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setText(txt);

    QHBoxLayout *h_layout = new QHBoxLayout(this->m_frame);
    h_layout->setContentsMargins(10, 5, 10, 5);
    h_layout->setSpacing(0);
    h_layout->addWidget(m_textLabel, 0, Qt::AlignVCenter);

    main_layout->addWidget(m_frame, 0, Qt::AlignVCenter);

    this->adjustSize();

    hide();
}

TextTip::~TextTip()
{

}

void TextTip::resetTipText(const QString &txt)
{
    m_textLabel->setText(txt);
}

void TextTip::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QBrush background =  this->m_background;
    QColor borderColor = this->m_borderColor;

    QMarginsF shadowMargins = QMarginsF(2.0, 2.0, 2.0, 2.0);
    QRectF bgRect = QRectF(rect()).marginsRemoved(shadowMargins);
    QPainterPath bgPath;
    bgPath.addRoundedRect(bgRect, this->m_radius, this->m_radius);
    painter.fillPath(bgPath, background);

    QPainterPath borderPath;
    QRectF borderRect = QRectF(rect());
    int borderRadius = this->m_radius;
    QMarginsF borderMargins(0.5, 0.5, 0.5, 0.5);
    borderRadius += 0.5;
    borderRect = borderRect.marginsAdded(borderMargins).marginsRemoved(shadowMargins);
    borderPath.addRoundedRect(borderRect, borderRadius, borderRadius);

    QPen borderPen(borderColor);
    borderPen.setWidthF(1.0);
    painter.strokePath(borderPath, borderPen);
}
