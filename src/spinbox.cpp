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

#include <QResizeEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>

#include "spinbox.h"

SpinBox::SpinBox(QWidget *parent) :
    QSpinBox(parent)
    , m_value(1)
    , m_frame(new QFrame(this))
{
    QSpinBox::lineEdit()->setVisible(false);//hide QSpinBox's lineEdit, replace it with m_valueLabel
    this->setStyleSheet("QSpinBox{min-height:21px;color:#808080;font-size:14px;border:1px solid #7c8487;border-radius:0;padding-left:28px;padding-right:28px;background-color:transparent;}QSpinBox::up-button{border:none;background:transparent;}QSpinBox::up-arrow {border:none;background:transparent;}QSpinBox::down-button{border:none;background: transparent;}QSpinBox::down-arrow {border:none;background:transparent;}");//放置左右按钮的偏移设置： padding-left:28px;padding-right:28px;
    this->setSingleStep(1);
    //this->setRange(1, 60);

    m_frame->setStyleSheet("QFrame{background-color:transparent;border:none;}");//border:1px solid #7c8487;border-radius:0;

    QHBoxLayout *m_layout = new QHBoxLayout(m_frame);
    m_layout->setContentsMargins(1, 0, 1, 0);
    m_layout->setSpacing(0);

    QPushButton *m_addBtn = new QPushButton;
    QPushButton *m_subtractBtn = new QPushButton;
    m_addBtn->setFocusPolicy(Qt::NoFocus);
    m_subtractBtn->setFocusPolicy(Qt::NoFocus);
    m_addBtn->setFixedSize(19,21);
    m_subtractBtn->setFixedSize(19,21);
    m_addBtn->setStyleSheet("QPushButton{background-image:url(':/res/add_normal_btn.png');border:0px;}QPushButton:hover{background:url(':/res/add_hover_btn.png');}QPushButton:pressed{background:url(':/res/add_pressed_btn.png');}");
    m_subtractBtn->setStyleSheet("QPushButton{background-image:url(':/res/subtract_normal_btn.png');border:0px;}QPushButton:hover{background:url(':/res/subtract_hover_btn.png');}QPushButton:pressed{background:url(':/res/subtract_pressed_btn.png');}");

    QLabel *m_valueLabel = new QLabel;
    m_valueLabel->setStyleSheet("QLabel{font-size:14px;color:#808080;background-color:transparent;border:none}");
    m_valueLabel->setText(this->text());

    m_layout->addWidget(m_subtractBtn, 0, Qt::AlignVCenter | Qt::AlignLeft);
    m_layout->addStretch();
    m_layout->addWidget(m_valueLabel, 1, Qt::AlignCenter);
    m_layout->addStretch();
    m_layout->addWidget(m_addBtn, 0, Qt::AlignVCenter | Qt::AlignRight);

    connect(this, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=] (int value) {
        m_valueLabel->setText(this->text());
        this->m_value = value;
    });
    connect(m_addBtn, &QPushButton::clicked, this, &SpinBox::stepUp);
    connect(m_subtractBtn, &QPushButton::clicked, this, &SpinBox::stepDown);
}

SpinBox::~SpinBox()
{

}

void SpinBox::setValue(int value)
{
    if (this->m_value == value)
        return;

    this->m_value = value;
}

void SpinBox::resizeEvent(QResizeEvent *event)
{
    QSpinBox::resizeEvent(event);

    m_frame->setFixedWidth(event->size().width());
    m_frame->setFixedHeight(event->size().height());
}
