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

#include "searchtitlebar.h"
#include "searchinputedit.h"
#include "locationworker.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>

SearchTitleBar::SearchTitleBar(QWidget *parent) : QWidget(parent)
  , m_borderColor(QColor(255, 255, 255, 153))
  , m_borderRadius(1)
  , m_borderWidth(1)
  , m_bgBrush(QBrush(QColor("#1374e8")))
{
    this->initWidgets();

    m_searchTimer = new QTimer;
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(500);

    m_locationWorker = new LocationWorker;

    connect(m_searchTimer, SIGNAL(timeout()), this, SLOT(onSearchTimerOut()));

    connect(m_searchInputEdit, &SearchInputEdit::textChanged, this, [this] {
        emit this->requestResetViewAndModel();

        if (m_searchTimer->isActive()) {
            m_searchTimer->stop();
        }
        m_searchTimer->start();
    });
}

SearchTitleBar::~SearchTitleBar()
{
    m_locationWorker->deleteLater();

    if (m_searchTimer) {
        disconnect(m_searchTimer, SIGNAL(timeout()), this, SLOT(onSearchTimerOut()));
        if(m_searchTimer->isActive()) {
            m_searchTimer->stop();
        }
        delete m_searchTimer;
        m_searchTimer = nullptr;
    }

    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void SearchTitleBar::onSearchTimerOut()
{
    const QString inputText = m_searchInputEdit->text().trimmed();
    if (inputText.isEmpty())
        return;

    QList<LocationData> searchResultList;
    searchResultList = m_locationWorker->exactMatchCity(inputText);
    if (searchResultList.isEmpty()) {
        emit this->requestShowNoResultLabel();
    }
    else {
        setSearchResult(searchResultList);
    }
}

void SearchTitleBar::setSearchResult(const QList<LocationData> data)
{
    QList<LocationData> resultList;
    for (const LocationData &line : data) {
        if (resultList.indexOf(line) == -1) {
            resultList.append(line);
        }
    }

    emit this->sendSearchResults(resultList);
}

void SearchTitleBar::resetSearchInputEdit() const
{
    m_searchInputEdit->setText("");
    m_searchInputEdit->clearFocus();
}

void SearchTitleBar::initLeftContent()
{
    QWidget *w = new QWidget;
    m_lLayout = new QHBoxLayout(w);
    m_lLayout->setContentsMargins(0, 0, 0, 0);
    m_lLayout->setSpacing(0);
    m_toplayout->addWidget(w, 1, Qt::AlignLeft);
}

void SearchTitleBar::initMiddleContent()
{
    QWidget *w = new QWidget;
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_mLayout = new QHBoxLayout(w);
    m_mLayout->setContentsMargins(0, 0, 0, 0);
    m_mLayout->setSpacing(5);
    m_toplayout->addWidget(w);

    QLabel *m_iconLabel = new QLabel;
    m_iconLabel->setContentsMargins(0, 0, 0, 0);

    QPixmap iconPixmap = QPixmap(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/indicator-china-weather_min.png")).pixmap(QSize(16, 16)));
    m_iconLabel->setPixmap(iconPixmap);
    m_iconLabel->setFixedSize(iconPixmap.size());

    QLabel *m_titleLabel = new QLabel;
    m_titleLabel->setStyleSheet("QLabel{font-size:12px;color: rgb(255,255,255);background-color:transparent;}");
    m_titleLabel->setText(tr("Kylin Weather - Search"));

    m_mLayout->addWidget(m_iconLabel);
    m_mLayout->addWidget(m_titleLabel);
}

void SearchTitleBar::initRightContent()
{
    QWidget *w = new QWidget;
    m_rLayout = new QHBoxLayout(w);
    m_rLayout->setContentsMargins(0, 0, 5, 0);
    m_rLayout->setSpacing(0);
    m_toplayout->addWidget(w, 1, Qt::AlignRight);

    m_closeBtn = new QPushButton(this);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);
    m_closeBtn->setFixedSize(32,32);
    m_closeBtn->setStyleSheet("QPushButton{background-image:url(':/res/close_normal_btn.png');border:0px;}QPushButton:hover{background:url(':/res/close_hover_btn.png');}QPushButton:pressed{background:url(':/res/close_pressed_btn.png');}");

    m_rLayout->addWidget(m_closeBtn);

    connect(m_closeBtn, &QPushButton::clicked, this, &SearchTitleBar::requestCloseDialog);
}

void SearchTitleBar::initBottomContent()
{
    QWidget *w = new QWidget;
    m_bLayout = new QHBoxLayout(w);
    m_bLayout->setContentsMargins(10, 0, 0, 0);
    m_bLayout->setSpacing(10);
    m_layout->addWidget(w);

    m_searchInputEdit = new SearchInputEdit;
    m_searchInputEdit->setFixedSize(280, 32);
    m_bLayout->addWidget(m_searchInputEdit, 0, Qt::AlignCenter);
    m_searchInputEdit->setFocus();

    m_layout->addStretch();
}

void SearchTitleBar::initWidgets()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    this->setLayout(m_layout);

    m_toplayout = new QHBoxLayout;
    m_toplayout->setContentsMargins(0, 0, 0, 0);
    m_toplayout->setSpacing(0);

    m_layout->addLayout(m_toplayout);
    m_layout->addStretch();

    initLeftContent();
    initMiddleContent();
    initRightContent();
    initBottomContent();
}

void SearchTitleBar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

//    QPainterPath outBorderPath;
//    outBorderPath.addRoundedRect(this->rect(), m_borderRadius, m_borderRadius);
//    QPen pen(m_borderColor, m_borderWidth);
//    painter.setPen(pen);
//    painter.drawPath(outBorderPath);

//    QRect borderRect;
//    borderRect.setRect(this->rect().x() + m_borderWidth, this->rect().y() + m_borderWidth, this->rect().width() - m_borderWidth * 2, this->rect().height() - m_borderWidth * 2);
//    QPainterPath inBorderPath;
//    inBorderPath.addRoundedRect(borderRect, m_borderRadius, m_borderRadius);
//    painter.setClipPath(inBorderPath);

    painter.fillRect(0, 0, width(), height(), m_bgBrush);
}
