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

#include "translucentlabel.h"

#include <QHBoxLayout>

TranslucentLabel::TranslucentLabel(QWidget *parent)
    : QLabel(parent) {

//    QFont font;
//    font.setPixelSize(12);
//    setFont(font);
//    setWordWrap(true);//QLabel自动换行
//    setAttribute(Qt::WA_TransparentForMouseEvents);//实现鼠标穿透，如果要被穿透窗口只有一层，即没有嵌套窗口，直接只用对子窗口设置该属性，即鼠标事件让父窗口响应

    this->setStyleSheet("QLabel{border-radius:4px;background-color: argb(60, 60, 60, 130);color: rgb(255, 255, 255);}");
    this->setFixedSize(80, 28);

    m_icon = new QLabel(this);
    m_text = new QLabel(this);
    m_text->adjustSize();//QLabel自适应文字的大小
    m_text->setAlignment(Qt::AlignLeft);
    m_icon->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    m_text->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");

    //m_icon->setScaledContents(true);//自动缩放,显示图像大小自动调整为Qlabel大小
    m_icon->setFixedSize(20, 20);

    QHBoxLayout *m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(5, 0, 5, 0);
    m_layout->setSpacing(5);

    m_layout->addWidget(m_icon, 0, Qt::AlignLeft | Qt::AlignVCenter);
    m_layout->addWidget(m_text, 1, Qt::AlignLeft | Qt::AlignVCenter);

    this->setLayout(m_layout);
}

void TranslucentLabel::setLabelIcon(const QString &iconPath)
{
    m_icon->setPixmap(QPixmap(iconPath));
}

void TranslucentLabel::setLabelText(const QString &text)
{
    m_text->setText(text);
}
