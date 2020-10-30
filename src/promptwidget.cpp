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

#include "promptwidget.h"

#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>

PromptWidget::PromptWidget(QWidget *parent) : QFrame(parent)
    , m_iconLabel(new QLabel(this))
    , m_textLabel(new QLabel(this))
    , m_closeBtn(new QPushButton(this))
    , m_retryBtn(new QPushButton(this))
{
    this->setStyleSheet("QFrame{background:rgba(255,255,255,0.25);border-radius:6px;}"
                        "QFrame:hover{background:rgba(255,255,255,0.2);border-radius:6px;}");
    this->setFixedSize(223, 138);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *m_vlayout = new QVBoxLayout(this);
    m_vlayout->setContentsMargins(10, 10, 10, 10);
    m_vlayout->setSpacing(5);

    m_closeBtn->setFixedSize(12, 12);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);
    m_closeBtn->setStyleSheet("QPushButton{background-image:url(':/res/control_icons/delete_normal.png');border:0px;}"
                              "QPushButton:hover{background:url(':/res/control_icons/delete_hover_press.png');}"
                              "QPushButton:pressed{background:url(':/res/control_icons/delete_hover_press.png');}");
    m_closeBtn->setVisible(false);

    m_iconLabel->setFixedSize(45, 44);
    m_iconLabel->setStyleSheet("QLabel{border:none;background-color:transparent;}");

    m_textLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:14px;}");
    m_textLabel->setWordWrap(true);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_retryBtn->setFixedSize(90, 25);
    m_retryBtn->setFocusPolicy(Qt::NoFocus);
    m_retryBtn->setStyleSheet("QPushButton{border:1px solid #e0e0e0;text-align:center;font-size:14px;color:#ffffff;background:rgba(255,255,255,0);}"
                              "QPushButton:hover{border:1px solid #e0e0e0;text-align:center;font-size:14px;color:#ffffff;background:rgba(255,255,255,0.15);}");
    m_retryBtn->setText(tr("retry"));//重试

    m_vlayout->addWidget(m_iconLabel, 0, Qt::AlignHCenter);
    m_vlayout->addWidget(m_textLabel, 0, Qt::AlignHCenter);
    m_vlayout->addWidget(m_retryBtn, 0, Qt::AlignHCenter);

    connect(m_closeBtn, &QPushButton::clicked, this, [=] () {
        this->setVisible(false);
    });

    connect(m_retryBtn, &QPushButton::clicked, this, [=] () {
        this->setVisible(false);
        emit this->requestRetryAccessWeather();
    });
}

PromptWidget::PromptWidget(const QString &text, QWidget *parent, const QString &movieSouce, bool movie)
    : QFrame(parent)
    , m_iconLabel(new QLabel(this))
    , m_textLabel(new QLabel(this))
    , m_closeBtn(new QPushButton(this))
    , m_isMovie(movie)
{
    this->setStyleSheet("QFrame{background:rgba(255,255,255,0.25);border-radius:6px;}");
                        //"QFrame:hover{background:rgba(255,255,255,0.4);border-radius:6px;}");
    this->setFixedSize(223, 138);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *m_hlayout = new QHBoxLayout(this);
    m_hlayout->setContentsMargins(5, 10, 5, 10);
    m_hlayout->setSpacing(5);

    m_closeBtn->setFixedSize(12, 12);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);
    m_closeBtn->setStyleSheet("QPushButton{background-image:url(':/res/control_icons/delete_normal.png');border:0px;}"
                              "QPushButton:hover{background:url(':/res/control_icons/delete_hover_press.png');}"
                              "QPushButton:pressed{background:url(':/res/control_icons/delete_hover_press.png');}");
    m_closeBtn->setVisible(false);

    m_iconLabel->setFixedSize(16, 16);
    m_iconLabel->setStyleSheet("QLabel{border:none;background-color:transparent;}");

    m_textLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:14px;}");
    m_textLabel->setText(text);
    QFont font = m_textLabel->font();
    const QFontMetrics fm(font);
    m_textLabel->setFixedWidth(fm.width(m_textLabel->text()));

    if (m_isMovie) {
        m_movie = new QMovie(movieSouce);
        m_iconLabel->setMovie(m_movie);
        m_movie->start();
    }

    m_hlayout->addStretch();
    m_hlayout->addWidget(m_textLabel, 0, Qt::AlignVCenter | Qt::AlignRight);
    m_hlayout->addWidget(m_iconLabel, 0, Qt::AlignVCenter | Qt::AlignLeft);
    m_hlayout->addStretch();

    connect(m_closeBtn, &QPushButton::clicked, this, [=] () {
        m_movie->stop();
        this->setVisible(false);
    });
}

PromptWidget::~PromptWidget()
{
    if (m_movie) {
        delete m_movie;
    }
}

void PromptWidget::setIconAndText(const QString &iconPath, const QString &text)
{
    const qreal ratio = qApp->devicePixelRatio();
    QIcon icon = QIcon(iconPath);
    QPixmap iconPix = icon.pixmap(m_iconLabel->width()*ratio, m_iconLabel->height()*ratio);
    if (iconPix.isNull()) {
        iconPix = QPixmap(":/res/control_icons/network_warn.png");
    }
    //Q_ASSERT(!iconPix.isNull());
    iconPix = iconPix.scaled(m_iconLabel->width()*ratio, m_iconLabel->height()*ratio, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    iconPix.setDevicePixelRatio(ratio);
    m_iconLabel->setPixmap(iconPix);
    m_textLabel->setText(text);
}

void PromptWidget::enterEvent(QEvent *event)
{
    QFrame::enterEvent(event);

    //m_closeBtn->move(this->width() -12, 0);
    //m_closeBtn->setVisible(true);
}

void PromptWidget::leaveEvent(QEvent *event)
{
    QFrame::enterEvent(event);

    //m_closeBtn->setVisible(false);
}

bool PromptWidget::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress:
        if (m_isMovie && m_movie) {
            //m_movie->stop();
        }
        else {
            this->setVisible(false);
            emit this->requestRetryAccessWeather();
        }
        break;
    /*case QEvent::Show:
        if (m_isMovie && m_movie) {
            m_movie->start();
            this->setVisible(true);
        }
        break;*/
    default:
        break;
    }

    return QFrame::event(event);
}


void PromptWidget::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);

    if (m_isMovie && m_movie) {
        m_movie->start();
    }
}

void PromptWidget::hideEvent(QHideEvent* event)
{
  QFrame::hideEvent(event);

  if (m_isMovie && m_movie) {
      m_movie->stop();
  }
}
