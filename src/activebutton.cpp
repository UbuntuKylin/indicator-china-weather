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

#include "activebutton.h"

#include <QDebug>

ActiveButton::ActiveButton(const QString &text, QWidget *parent) :
    QFrame(parent)
{
    this->setFixedHeight(28);

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(5);

    m_btn = new QPushButton(this);
    m_btn->setFixedHeight(24);
    m_btn->setFocusPolicy(Qt::NoFocus);
    m_btn->setStyleSheet("QPushButton{border:none;text-align:center;font-size:14px;color:#ffffff;background:transparent;}");
    m_btn->setText(text);
    QFont font = m_btn->font();
    const QFontMetrics fm(font);
    m_btn->setFixedWidth(fm.width(m_btn->text()) + 10);

    m_label = new QLabel(this);
    m_label->setFixedSize(m_btn->width(), 2);
    m_label->setStyleSheet("QLabel{border:none;background-color:#ffffff;}");
    m_label->setVisible(false);

    m_layout->addWidget(m_btn, 0, Qt::AlignCenter);
    m_layout->addStretch();
    m_layout->addWidget(m_label, 0, Qt::AlignBottom);

//    adjustSize();

    connect(m_btn, &QPushButton::clicked, this, &ActiveButton::btnClicked);
}

void ActiveButton::setActive(bool active)
{
    this->m_isActive = active;
    if (this->m_isActive) {
        m_label->setVisible(true);
    }
    else {
        m_label->setVisible(false);
    }
}
