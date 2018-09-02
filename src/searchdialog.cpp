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

#include "searchdialog.h"
#include "searchtitlebar.h"
#include "searchmodel.h"
#include "searchview.h"
#include "searchdelegate.h"

#include <QApplication>
#include <QDesktopServices>
#include <QStackedLayout>
#include <QMouseEvent>
#include <QDebug>

namespace {

const int CONTENT_MARGIN = 0;

}

SearchDialog::SearchDialog(QWidget *parent):
    QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(479, 421);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setWindowTitle(tr("Kylin Weather - Search"));
    this->setWindowIcon(QIcon(":/res/indicator-china-weather.png"));

    m_titleBar = new SearchTitleBar;
    m_titleBar->setFixedHeight(150);
    m_displayWidget = new QWidget;
//    m_displayWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    m_displayWidget->setContentsMargins(0, m_titleBar->height(), 0, 0);

    m_searchModel = new SearchModel;
    m_searchView = new SearchView;
    m_searchDelegate = new SearchDelegate;
    m_searchView->setFixedWidth(this->width() - 2*CONTENT_MARGIN);
    m_searchView->setItemDelegate(m_searchDelegate);
    m_searchView->setModel(m_searchModel);
    m_searchView->setVisible(false);

    m_noResultLabel = new QLabel;
    m_noResultLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_noResultLabel->setAlignment(Qt::AlignCenter);
    m_noResultLabel->setText(tr("No Result"));
    m_noResultLabel->setVisible(false);
    QFont font = m_noResultLabel->font();
    const QFontMetrics fm(font);
    m_noResultLabel->setFixedWidth(fm.width(m_noResultLabel->text()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_titleBar);
    layout->addWidget(m_displayWidget);

    QHBoxLayout *contentLayout = new QHBoxLayout(m_displayWidget);
    contentLayout->setContentsMargins(CONTENT_MARGIN, 0, CONTENT_MARGIN, 0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(m_searchView, 1, Qt::AlignHCenter);
    contentLayout->addWidget(m_noResultLabel, 1, Qt::AlignCenter);

    connect(m_searchView, &SearchView::clicked, this, [this](const QModelIndex &index) {
        QVariant data = index.data(Qt::UserRole);//SearchModel::data Qt::UserRole
        QString selectCityId = data.value<QString>();
        //qDebug() << "selectCityId=" << selectCityId;
        for (const LocationData &line : m_searchModel->locationList()) {//it must exits.
            if (line.id == selectCityId) {
                emit this->requestAddCityToMenu(line);
                break;
            }
        }
    });

    connect(m_titleBar, &SearchTitleBar::requestCloseDialog, this, [=] {
        this->close();
    });

    connect(m_titleBar, &SearchTitleBar::requestShowNoResultLabel, this, [=] {
        m_noResultLabel->setVisible(true);
    });
    connect(m_titleBar, &SearchTitleBar::requestResetViewAndModel, this, [=] {
        m_searchView->setVisible(false);
        m_noResultLabel->setVisible(false);
        m_searchModel->setLocationData(QList<LocationData>());
    });

    connect(m_titleBar, &SearchTitleBar::sendSearchResults, this, [=] (QList<LocationData> results) {
        m_searchModel->setLocationData(results);
        m_searchView->setVisible(true);
        m_noResultLabel->setVisible(false);
    });
}

SearchDialog::~SearchDialog()
{
    delete m_titleBar;
    delete m_noResultLabel;
    delete m_searchModel;
    delete m_searchView;
    delete m_searchDelegate;
}

void SearchDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = true;
        m_dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

void SearchDialog::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePressed = false;
}

void SearchDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mousePressed) {
        QPoint move_pos = event->globalPos();
        move(move_pos - m_dragPosition);
        event->accept();
    }
}
