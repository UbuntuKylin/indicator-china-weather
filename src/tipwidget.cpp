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

#include "tipwidget.h"

#include <QHBoxLayout>
#include <QDebug>

#include "preferences.h"
#include "global.h"
using namespace Global;

TipWidget::TipWidget(QWidget *parent)
    : QFrame(parent) {
    this->setStyleSheet("QFrame{border:none;background-color:transparent;color:#fff222;font-size:12px;}");
    this->setFixedHeight(22);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_icon = new QLabel(this);
    m_icon->setFixedSize(12, 12);
    m_icon->setScaledContents(true);//自动缩放,显示图像大小自动调整为Qlabel大小
    m_text = new QLabel(this);
    m_text->adjustSize();//QLabel自适应文字的大小
    m_text->setAlignment(Qt::AlignLeft);
    m_icon->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    m_text->setStyleSheet("QLabel{border:none;background-color:transparent;color:#fff222;font-size:12px;}");

    m_closeBtn = new QPushButton(this);
    m_closeBtn->setFixedSize(12, 12);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);
    m_closeBtn->setStyleSheet("QPushButton{background-image:url(':/res/delete_normal.png');border:0px;}QPushButton:hover{background:url(':/res/delete_hover_press.png');}QPushButton:pressed{background:url(':/res/delete_hover_press.png');}");
    m_closeBtn->setVisible(false);
    connect(m_closeBtn, &QPushButton::clicked, this, [=] () {
        m_preferences->m_serverNotify = false;
        this->setVisible(false);
    });

    QHBoxLayout *m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(15, 0, 5, 0);
    m_layout->setSpacing(5);

    m_layout->addWidget(m_icon, 0, Qt::AlignLeft | Qt::AlignVCenter);
    m_layout->addWidget(m_text, 1, Qt::AlignLeft | Qt::AlignVCenter);

    this->setLayout(m_layout);
}

TipWidget::~TipWidget()
{

}

void TipWidget::setLabelIcon(const QString &iconPath)
{
    QPixmap iconPix = QPixmap(iconPath);
    if (iconPix.isNull()) {
        iconPix = QPixmap(":/res/update_warn.png");
    }
    //Q_ASSERT(!iconPix.isNull());
    m_icon->setPixmap(iconPix);
}

void TipWidget::setLabelText(const QString &text)
{
    QFont font = m_text->font();
    const QFontMetrics fm(font);
    QString elided_text = fm.elidedText(text, Qt::ElideRight, this->width() - 40);
    m_text->setText(elided_text);
}

void TipWidget::enterEvent(QEvent *event)
{
    QFrame::enterEvent(event);

    m_closeBtn->move(1, 5);
    m_closeBtn->setVisible(true);
}

void TipWidget::leaveEvent(QEvent *event)
{
    QFrame::enterEvent(event);

    m_closeBtn->setVisible(false);
}
