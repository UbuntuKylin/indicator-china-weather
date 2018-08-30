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
#include "aboutdialog.h"

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <QScreen>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , mousePressed(false)
    , m_pixmap(QPixmap(":/res/indicator-china-weather.png"))
    , m_websiteLinkString("<a href='%1' style='text-decoration:none;font-size:14px;color:#1374e8;'>%2</a>")
{
    this->setStyleSheet("QDialog{border:1px solid white;border-radius:2px;background:rgba(255, 255, 255, 0.7);}QDialog:hover{background: rgba(255, 255, 255, 1.0);}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setWindowTitle(tr("Kylin Weather - About"));
    this->setWindowIcon(QIcon(":/res/indicator-china-weather.png"));
    this->setFixedWidth(456);

    QWidget *topWidget = new QWidget;
    QHBoxLayout *toplayout = new QHBoxLayout(topWidget);
    toplayout->setContentsMargins(0, 0, 0, 0);
    toplayout->setSpacing(0);
    QLabel *label = new QLabel();
    label->setStyleSheet("QLabel{background-color:transparent;font-size:16px;color:#000000;}");
    label->setStyleSheet("QLabel{background:transparent;border:none;}");
    label->setText(tr("Kylin Weather - About"));

    QPushButton *closeBtn = new QPushButton();
    closeBtn->setFocusPolicy(Qt::NoFocus);
    closeBtn->setFixedSize(32,32);
    closeBtn->setStyleSheet("QPushButton{background-image:url(':/res/close_normal_btn.png');border:0px;}QPushButton:hover{background:url(':/res/close_hover_btn.png');}QPushButton:pressed{background:url(':/res/close_pressed_btn.png');}");
    connect(closeBtn, &QPushButton::clicked, this, [=] {
        this->close();
    });
    toplayout->addWidget(label, 0, Qt::AlignVCenter | Qt::AlignLeft);
    toplayout->addStretch();
    toplayout->addWidget(closeBtn, 0, Qt::AlignTop | Qt::AlignRight);

    m_iconLabel = new QLabel();
    m_iconLabel->setContentsMargins(0, 0, 0, 0);
    m_iconLabel->setPixmap(m_pixmap);
    m_iconLabel->setFixedSize(m_pixmap.size());

    m_nameLabel = new QLabel();
    m_nameLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:16px;}");
    m_nameLabel->setText(qApp->applicationName());

    m_versionLabel = new QLabel();
    m_versionLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#00609a;font-size:12px;}");
    m_versionLabel->setText(qApp->applicationVersion());

    m_appWebsiteLabel = new QLabel();
    m_appWebsiteLabel->setStyleSheet("QLabel{border:none;background-color:transparent;font-size:12px;}");//color:#808080;
    m_appWebsiteLabel->setContextMenuPolicy(Qt::NoContextMenu);
    m_appWebsiteLabel->setOpenExternalLinks(false);
    m_appWebsiteLabel->setText(QString(m_websiteLinkString).arg("https://github.com/ukui/indicator-china-weather").arg("https://github.com/ukui/indicator-china-weather"));

    m_companyLabel = new QLabel();
    m_companyLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_companyLabel->setContextMenuPolicy(Qt::NoContextMenu);
    m_companyLabel->setOpenExternalLinks(false);
    m_companyLabel->setText(QString(m_websiteLinkString).arg("https://www.ubuntukylin.com").arg("www.ubuntukylin.com"));

    m_descriptionLabel = new QLabel();
    m_descriptionLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_descriptionLabel->setAlignment(Qt::AlignHCenter);
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_descriptionLabel->setText(tr("Real-time and three-day weather forecast information is obtained from Ubuntu Kylin weather server, which supports multi-city switching and automatic data updating.") + "\n");

    m_copyrightLabel = new QLabel();
    m_copyrightLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#00609a;font-size:12px;}");
    m_copyrightLabel->setText("Copyright © 2013-2018 Ubuntu Kylin Team, lixiang@kylinos.cn");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 2, 5, 20);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(topWidget, 0, Qt::AlignTop);
    mainLayout->addWidget(m_iconLabel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_nameLabel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_versionLabel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_appWebsiteLabel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_companyLabel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_descriptionLabel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_copyrightLabel, 0, Qt::AlignHCenter);

    connect(m_appWebsiteLabel, &QLabel::linkActivated, this, &AboutDialog::onLinkActivated);
    connect(m_companyLabel, &QLabel::linkActivated, this, &AboutDialog::onLinkActivated);
}

AboutDialog::~AboutDialog()
{

}

void AboutDialog::onLinkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void AboutDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    this->adjustSize();
}

void AboutDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
        event->accept();
    }

    QDialog::keyPressEvent(event);
}

QRect AboutDialog::getParentGeometry() const
{
    if (this->parentWidget()) {
        return this->parentWidget()->window()->geometry();
    } else {
        QPoint pos = QCursor::pos();

        for (QScreen *screen : qApp->screens()) {
            if (screen->geometry().contains(pos)) {
                return screen->geometry();
            }
        }
    }

    return qApp->primaryScreen()->geometry();
}

void AboutDialog::moveToCenter()
{
    QRect qr = geometry();
    qr.moveCenter(this->getParentGeometry().center());
    move(qr.topLeft());
}

void AboutDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->dragPosition = event->globalPos() - frameGeometry().topLeft();
        this->mousePressed = true;
    }

    QDialog::mousePressEvent(event);
}

void AboutDialog::mouseReleaseEvent(QMouseEvent *event)
{
    this->mousePressed = false;

    QDialog::mouseReleaseEvent(event);
}

void AboutDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (this->mousePressed) {
        move(event->globalPos() - this->dragPosition);
    }

    QDialog::mouseMoveEvent(event);
}
