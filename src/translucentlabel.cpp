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

#include "translucentlabel.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QFocusEvent>
#include <QDebug>

TranslucentLabel::TranslucentLabel(bool showTip, QWidget *parent)
    : QLabel(parent), m_showTip(showTip)
{
    //使用颜色值设置背景
    this->setStyleSheet("QLabel{border-radius:4px;background-color:rgba(0,0,0,0.2);color:rgb(255,255,255);}");
    //使用图片设置背景
//    this->setStyleSheet("QLabel{background:transparent;background-image:url(':/res/min_bg.png');}");//border-image
    this->setFixedSize(89, 26);
    this->setFocusPolicy(Qt::NoFocus);

    if (m_showTip) {
        this->setFocusPolicy(Qt::ClickFocus);
        this->setMouseTracking(true);
        installEventFilter(this);
    }

    m_icon = new QLabel(this);
    m_icon->setStyleSheet("QLabel{border:none;background:transparent;background-color:transparent;}");
    //m_icon->setScaledContents(true);//自动缩放,显示图像大小自动调整为Qlabel大小
    m_icon->setFixedSize(12, 16);

    m_text = new QLabel(this);
    m_text->adjustSize();//QLabel自适应文字的大小
    m_text->setAlignment(Qt::AlignLeft);
    m_text->setStyleSheet("QLabel{border:none;background:transparent;background-color:transparent;color:#ffffff;font-size:12px;}");

    QHBoxLayout *m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(5, 0, 1, 0);
    m_layout->setSpacing(2);

    m_layout->addWidget(m_icon, 0, Qt::AlignLeft | Qt::AlignVCenter);
    m_layout->addWidget(m_text, 1, Qt::AlignLeft | Qt::AlignVCenter);

    this->setLayout(m_layout);
}

void TranslucentLabel::setLabelIcon(const QString &iconPath)
{
    QPixmap iconPix = QPixmap(iconPath);
    if (iconPix.isNull()) {
        iconPix = QPixmap(":/res/update_warn.png");
    }
    //Q_ASSERT(!iconPix.isNull());
    m_icon->setPixmap(iconPix);
}

void TranslucentLabel::setLabelText(const QString &text)
{
    QFont font = m_text->font();
    const QFontMetrics fm(font);
    QString elided_text = fm.elidedText(text, Qt::ElideRight, this->width() - m_icon->width() - 8);
    m_text->setText(elided_text);

}

bool TranslucentLabel::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter: {
        QWidget *widget = qobject_cast<QWidget *>(obj);
        if (widget) {
            widget->setCursor(QCursor(Qt::PointingHandCursor));
        }
        break;
    }
    case QEvent::Leave: {
        QWidget *widget = qobject_cast<QWidget *>(obj);
        if (!widget) {
            widget->unsetCursor();
        }
        break;
    }
    default:
        break;
    }

    return QObject::eventFilter(obj, event);
}


void TranslucentLabel::mousePressEvent(QMouseEvent *event)
{
    if (m_showTip) {
        emit this->clicked();
    }

    QLabel::mousePressEvent(event);
}

/*bool TranslucentLabel::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress:
        qDebug() << "press.";
        break;
    default:
        break;
    }

    return QWidget::event(event);
}

void TranslucentLabel::focusOutEvent(QFocusEvent *event)
{
    QPoint mousePos = mapToParent(mapFromGlobal(QCursor::pos()));
    if (!this->geometry().contains(mousePos)) {
        if (event && event->reason() == Qt::MouseFocusReason) {
            qDebug() << "focus out...........";
        }
    }

    QLabel::focusOutEvent(event);
}

void TranslucentLabel::focusInEvent(QFocusEvent *event)
{
    qDebug() << "focus in...........";
    QLabel::focusInEvent(event);
}*/
