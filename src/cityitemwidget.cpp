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

#include "cityitemwidget.h"

#include <QApplication>
#include <QDebug>
#include <QPixmap>
#include <QPainter>

CityItemWidget::CityItemWidget(CitySettingData info, QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFixedSize(469, 46);

    this->m_data = info;
    this->m_id = info.id;

//    item = new QListWidgetItem();
//    item->setSizeHint(QSize(this->width(), 36));

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(10, 0, 5, 0);
    m_layout->setSpacing(5);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(24, 24);
    m_iconLabel->setStyleSheet("QLabel{border:none;background-color:transparent;}");
//    m_iconLabel->setScaledContents(true);//自动缩放,显示图像大小自动调整为Qlabel大小
    m_cityLabel = new QLabel(this);
    m_cityLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:14px;}");
    m_temperatureLabel = new QLabel(this);
    m_temperatureLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:14px;}");
    m_defaultBtn = new QPushButton(this);
    m_defaultBtn->setFocusPolicy(Qt::NoFocus);
    m_defaultBtn->setStyleSheet("QPushButton{border:none;text-align:center;background:transparent;background-color:transparent;color:#0a9ff5;font-size:14px;}");
    m_defaultBtn->setText(tr("Set as default"));
    m_defaultBtn->setCursor(QCursor(Qt::PointingHandCursor));
    m_defaultBtn->setVisible(false);
    QFont font = m_defaultBtn->font();
    const QFontMetrics fm(font);
    m_defaultBtn->setFixedWidth(fm.width(m_defaultBtn->text()) + 10);

    m_deleteBtn = new QPushButton(this);
    m_deleteBtn->setFocusPolicy(Qt::NoFocus);
    m_deleteBtn->setFixedSize(16, 15);
    m_deleteBtn->setStyleSheet("QPushButton{background-image:url(':/res/delete_normal_btn.png');border:0px;}QPushButton:hover{background:url(':/res/delete_hover_pressed_btn.png');}QPushButton:pressed{background:url(':/res/delete_hover_pressed_btn.png');}");
    m_deleteBtn->setCursor(QCursor(Qt::PointingHandCursor));

    m_layout->addWidget(m_iconLabel);
    m_layout->addWidget(m_cityLabel);
    m_layout->addStretch();
    m_layout->addWidget(m_defaultBtn);
    m_layout->addSpacing(20);
    m_layout->addWidget(m_temperatureLabel);
    m_layout->addWidget(m_deleteBtn);
    m_layout->addSpacing(15);

    connect(m_defaultBtn, &QPushButton::clicked, this, [=] {
        emit this->requestSetDefaultCityById(this->getCityId());
    });
    connect(m_deleteBtn, &QPushButton::clicked, this, [=] {
        emit this->requestDeleteCity(this->getCityId());
    });

    QPixmap pixmap = QPixmap(info.icon);
    pixmap = pixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_iconLabel->setPixmap(pixmap);
    if (info.active) {
        m_cityLabel->setText(QString(tr("%1 [Default]")).arg(info.name));
    }
    else {
        m_cityLabel->setText(info.name);
    }

    m_temperatureLabel->setText(info.temperature);
}

//QListWidgetItem *CityItemWidget::getCityItem()
//{
//    return item;
//}

void CityItemWidget::setItemAction(bool active)
{
    if (active) {
        m_data.active = true;
        m_cityLabel->setText(QString(tr("%1 [Default]")).arg(m_data.name));
    }
    else {
        m_data.active = false;
        m_cityLabel->setText(m_data.name);
    }
}

void CityItemWidget::setItemWeather(const QString &temp, const QString &iconPath)
{
    m_data.icon = iconPath;
    m_data.temperature = temp;
    m_temperatureLabel->setText(m_data.temperature);
    QPixmap pixmap = QPixmap(m_data.icon);
    pixmap = pixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_iconLabel->setPixmap(pixmap);
}

void CityItemWidget::setItemHovered()
{
    m_mouseHover = true;
    update();
}

void CityItemWidget::unsetItemHovered()
{
    m_mouseHover = false;
    update();
}

QString CityItemWidget::getCityId()
{
    return this->m_id;
}

void CityItemWidget::enterEvent(QEvent *event)
{
//    emit this->enter();
    QWidget::enterEvent(event);

    this->m_mouseHover = true;
    update();
}

void CityItemWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    this->m_mouseHover = false;
    update();
}

void CityItemWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (m_mouseHover) {
        QPainterPath path;
        path.addRect(QRectF(this->rect().x(), this->rect().y(), this->rect().width(), this->rect().height()));
        painter.setOpacity(1.0);
        painter.fillPath(path, QColor("#f5fbff"));

        if (!m_data.active) {
            m_defaultBtn->setVisible(true);
        }
        else {
            m_defaultBtn->setVisible(false);
        }
    }
    else {
        m_defaultBtn->setVisible(false);
    }
}
