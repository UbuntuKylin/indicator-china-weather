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

#include "networkerrorwidget.h"

#include <QPushButton>
#include <QLabel>
#include <QDebug>

NetworkErrorWidget::NetworkErrorWidget(QFrame *parent) :
    QFrame(parent)
{
    this->setFixedSize(355, 340);

    QPushButton *m_retryBtn = new QPushButton;
    m_retryBtn->setStyleSheet("QPushButton{border:1px solid #e0e0e0;text-align:center;font-size:14px;color:#000000;background:transparent;}");
    m_retryBtn->setFixedSize(90, 25);
    m_retryBtn->setFocusPolicy(Qt::NoFocus);
    m_retryBtn->setText(tr("retry"));
    /*QFont font = m_btn->font();
    const QFontMetrics fm(font);
    m_retryBtn->setFixedWidth(fm.width(m_retryBtn->text()));*/

    connect(m_retryBtn, &QPushButton::clicked, this, [=] {
        emit this->requestRetryAccessWeather();
    });

    QLabel *m_tipIcon = new QLabel;
    m_tipIcon->setFixedSize(127 ,93);
    m_tipIcon->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    m_tipIcon->setPixmap(QPixmap(":/res/unlink.png"));
    QLabel *m_tipLabel = new QLabel;
    m_tipLabel->setFixedWidth(this->width());
    m_tipLabel->setWordWrap(true);
    m_tipLabel->setAlignment(Qt::AlignCenter);
    m_tipLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    m_tipLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_tipLabel->setText(tr("Current network exception, please check network settings"));//当前网络异常，请检查网络设置

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(20);
    this->setLayout(m_layout);

    m_layout->addStretch();
    m_layout->addWidget(m_tipIcon, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_tipLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_retryBtn, 0, Qt::AlignHCenter);
    m_layout->addStretch();
}

NetworkErrorWidget::~NetworkErrorWidget()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void NetworkErrorWidget::setErrorInfo(const QString &info)
{
    //m_tipLabel->setText(tr("Current network exception, please check network settings"));//当前网络异常，请检查网络设置
}
