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

#include "settingdialog.h"
#include "settingtitlebar.h"
#include "citywidget.h"
#include "spinbox.h"
#include "searchtitlebar.h"
#include "searchmodel.h"
#include "searchview.h"
#include "searchdelegate.h"

#include <QApplication>
#include <QDesktopServices>
#include <QStackedWidget>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QSlider>
#include <QGroupBox>
#include <QDebug>
#include <QPainter>
#include <QStackedLayout>
#include <QScreen>

#include <QMatrix3x3>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QWidget>
#include <QStyleFactory>

#include "preferences.h"
#include "global.h"
using namespace Global;

namespace {
const int BUTTON_WIDGET_HEIGHT = 30;
const int GROUP_BOX_MARGIN = 20;
const int ANIMATION_DELAY_TIME_INTERVAL = 300;
const int CONTENT_MARGIN = 0;

void animationFromBottomToTop(QWidget *topWidget, QWidget *bottomWidget)
{
    bottomWidget->setVisible(true);
    topWidget->setVisible(true);

    QRect topStartRect = QRect(0, 0, topWidget->width(), topWidget->height());
    QRect topEndRect = topStartRect;
    topEndRect.moveTop(-topWidget->height());

    QPropertyAnimation *topAnim = new QPropertyAnimation(topWidget, "geometry");
    topAnim->connect(topAnim, &QPropertyAnimation::finished, topAnim, &QPropertyAnimation::deleteLater);
    topAnim->connect(topAnim, &QPropertyAnimation::finished, topWidget, &QWidget::hide);
    topAnim->setDuration(ANIMATION_DELAY_TIME_INTERVAL);
    topAnim->setEasingCurve(QEasingCurve::InOutCubic);
    topAnim->setStartValue(topStartRect);
    topAnim->setEndValue(topEndRect);
    topAnim->start();

    QRect bottomStartRect = QRect(0, topWidget->height(), bottomWidget->width(), bottomWidget->height());
    QRect bottomEndRect = bottomStartRect;
    bottomEndRect.moveTo(0, 0);

    QPropertyAnimation *bottomAnim = new QPropertyAnimation(bottomWidget, "geometry");
    bottomAnim->connect(bottomAnim, &QPropertyAnimation::finished, bottomAnim, &QPropertyAnimation::deleteLater);
    bottomAnim->setEasingCurve(QEasingCurve::InOutCubic);
    bottomAnim->setDuration(ANIMATION_DELAY_TIME_INTERVAL);
    bottomAnim->setStartValue(bottomStartRect);
    bottomAnim->setEndValue(bottomEndRect);
    bottomAnim->start();

    QGraphicsOpacityEffect *bottomOpacity = new QGraphicsOpacityEffect(bottomWidget);
    bottomWidget->setGraphicsEffect(bottomOpacity);
    bottomOpacity->setOpacity(0);

    QPropertyAnimation *bottomOpacityAnim = new QPropertyAnimation(bottomOpacity, "opacity");
    bottomOpacityAnim->connect(bottomOpacityAnim, &QPropertyAnimation::finished, bottomOpacityAnim, &QPropertyAnimation::deleteLater);
    bottomOpacityAnim->connect(bottomOpacityAnim, &QPropertyAnimation::finished, bottomWidget, [ = ] () {
        bottomWidget->setGraphicsEffect(nullptr);
    });
    bottomOpacityAnim->setEasingCurve(QEasingCurve::InCubic);
    bottomOpacityAnim->setDuration(ANIMATION_DELAY_TIME_INTERVAL);
    bottomOpacityAnim->setStartValue(0.0);
    bottomOpacityAnim->setEndValue(1.0);
    bottomOpacityAnim->start();

    QGraphicsOpacityEffect *topOpacity = new QGraphicsOpacityEffect(topWidget);
    topWidget->setGraphicsEffect(topOpacity);
    topOpacity->setOpacity(1.0);

    QPropertyAnimation *topOpacityAnim = new QPropertyAnimation(topOpacity, "opacity");
    topOpacityAnim->connect(topOpacityAnim, &QPropertyAnimation::finished, topOpacityAnim, &QPropertyAnimation::deleteLater);
    topOpacityAnim->connect(topOpacityAnim, &QPropertyAnimation::finished, topWidget, [ = ] () {
        topWidget->setGraphicsEffect(nullptr);
    });
    topOpacityAnim->setEasingCurve(QEasingCurve::InCubic);
    topOpacityAnim->setDuration(ANIMATION_DELAY_TIME_INTERVAL);
    topOpacityAnim->setStartValue(1.0);
    topOpacityAnim->setEndValue(0.0);
    topOpacityAnim->start();
}

void animationFromTopToBottom(QWidget *topWidget, QWidget *bottomWidget)
{
    bottomWidget->setVisible(true);
    topWidget->setVisible(true);

    QRect topStartRect = QRect(0, 0, topWidget->width(), topWidget->height());
    QRect topEndRect = topStartRect;
    topEndRect.moveTo(0, topWidget->height());

    QPropertyAnimation *topAnim = new QPropertyAnimation(topWidget, "geometry");
    topAnim->connect(topAnim, &QPropertyAnimation::finished, topAnim, &QPropertyAnimation::deleteLater);
    topAnim->connect(topAnim, &QPropertyAnimation::finished, topWidget, &QWidget::hide);
    topAnim->setDuration(ANIMATION_DELAY_TIME_INTERVAL);
    topAnim->setEasingCurve(QEasingCurve::InOutCubic);
    topAnim->setStartValue(topStartRect);
    topAnim->setEndValue(topEndRect);
    topAnim->start();

    QRect bottomStartRect = QRect(0, -topWidget->height(), bottomWidget->width(), bottomWidget->height());
    QRect bottomEndRect = bottomStartRect;
    bottomEndRect.moveBottom(topWidget->height() - 1);

    QPropertyAnimation *bottomAnim = new QPropertyAnimation(bottomWidget, "geometry");
    bottomAnim->connect(bottomAnim, &QPropertyAnimation::finished, bottomAnim, &QPropertyAnimation::deleteLater);
    bottomAnim->setEasingCurve(QEasingCurve::InOutCubic);
    bottomAnim->setDuration(ANIMATION_DELAY_TIME_INTERVAL);
    bottomAnim->setStartValue(bottomStartRect);
    bottomAnim->setEndValue(bottomEndRect);
    bottomAnim->start();

    QGraphicsOpacityEffect *bottomOpacity = new QGraphicsOpacityEffect(bottomWidget);
    bottomWidget->setGraphicsEffect(bottomOpacity);
    bottomOpacity->setOpacity(0);

    QPropertyAnimation *bottomOpacityAnim = new QPropertyAnimation(bottomOpacity, "opacity");
    bottomOpacityAnim->connect(bottomOpacityAnim, &QPropertyAnimation::finished, bottomOpacityAnim, &QPropertyAnimation::deleteLater);
    bottomOpacityAnim->connect(bottomOpacityAnim, &QPropertyAnimation::finished, bottomWidget, [ = ]() {
        bottomWidget->setGraphicsEffect(nullptr);
    });
    bottomOpacityAnim->setEasingCurve(QEasingCurve::InCubic);
    bottomOpacityAnim->setDuration(ANIMATION_DELAY_TIME_INTERVAL);
    bottomOpacityAnim->setStartValue(0.0);
    bottomOpacityAnim->setEndValue(1.0);
    bottomOpacityAnim->start();

    QGraphicsOpacityEffect *topOpacity = new QGraphicsOpacityEffect(topWidget);
    topWidget->setGraphicsEffect(topOpacity);
    topOpacity->setOpacity(1.0);

    QPropertyAnimation *topOpacityAnim = new QPropertyAnimation(topOpacity, "opacity");
    topOpacityAnim->connect(topOpacityAnim, &QPropertyAnimation::finished, topOpacityAnim, &QPropertyAnimation::deleteLater);
    topOpacityAnim->connect(topOpacityAnim, &QPropertyAnimation::finished, bottomWidget, [ = ] () {
        topWidget->setGraphicsEffect(nullptr);
    });
    topOpacityAnim->setEasingCurve(QEasingCurve::InCubic);
    topOpacityAnim->setDuration(ANIMATION_DELAY_TIME_INTERVAL);
    topOpacityAnim->setStartValue(1.0);
    topOpacityAnim->setEndValue(0.0);
    topOpacityAnim->start();
}
}

SettingDialog::SettingDialog(QWidget *parent):
    QDialog(parent)
//    , m_settingLayout(new QVBoxLayout(this))
//    , m_stackedWidget(new QStackedWidget)
//    , m_okBtn(new QPushButton(this))
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(479, 421);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setWindowTitle(tr("Kylin Weather - Setting"));
    this->setWindowIcon(QIcon(":/res/indicator-china-weather.png"));
    //Why????? setStyleSheet将导致添加城市后，列表没有自动拉伸，出现重叠，但是qDebug打印的高度确实增加了，但是使用paintEvent可以
    //this->setStyleSheet("QDialog{border:1px solid #000000;border-radius:2px;background:rgba(255, 255, 255, 0.7);}QDialog:hover{background: rgba(255, 255, 255, 1.0);}");


    initSettings();
    initSearch();

    QStackedLayout *contentLayout = new QStackedLayout(this);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setMargin(0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(m_settingFrame);
    contentLayout->addWidget(m_searchFrame);
    m_settingFrame->setVisible(true);
}

SettingDialog::~SettingDialog()
{
    if (m_settingFrame) {
        delete m_settingFrame;
        m_settingFrame = nullptr;
    }
    if (m_searchFrame) {
        delete m_searchFrame;
        m_searchFrame = nullptr;
    }

    QLayoutItem *child;
    while ((child = m_settingLayout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    delete m_searchTitleBar;
    delete m_noResultLabel;
    delete m_searchModel;
    delete m_searchView;
    delete m_searchDelegate;
}

QRect SettingDialog::getParentGeometry() const
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

void SettingDialog::moveToCenter()
{
    QRect qr = geometry();
    qr.moveCenter(this->getParentGeometry().center());
    move(qr.topLeft());
    this->show();
}

void SettingDialog::initSettings()
{
    m_settingFrame = new QFrame;
    m_settingFrame->setFixedSize(this->size());
    m_settingFrame->setContentsMargins(0, 0, 0, 0);

    m_settingLayout = new QVBoxLayout(m_settingFrame);
    m_stackedWidget = new QStackedWidget;
    m_settingLayout->setContentsMargins(1, 1, 1, 1);
    m_settingLayout->setSpacing(0);


    m_settingTitleBar = new SettingTitleBar(m_settingFrame);
    m_settingTitleBar->setFixedHeight(100);
    m_cityWidget = new CityWidget(m_settingFrame);//m_cityWidget = new QWidget;
//    m_cityWidget->setContentsMargins(0, 0, 0, 0);
    m_cityWidget->setFixedHeight(this->height() - m_settingTitleBar->height() - 2/* - BUTTON_WIDGET_HEIGHT*/);
    m_cityWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_systemWidget = new QWidget(m_settingFrame);
    m_systemWidget->setStyleSheet("QWidget{border-radius: 0px; background-color:#ffffff;}");
    m_systemWidget->setFixedHeight(this->height() - m_settingTitleBar->height() - 2/* - BUTTON_WIDGET_HEIGHT*/);
    m_systemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(m_cityWidget, &CityWidget::responseCityError, m_settingTitleBar, &SettingTitleBar::showWarnInfo);

    /*m_okBtn->setFixedSize(90, 25);
    m_okBtn->setFocusPolicy(Qt::NoFocus);
    m_okBtn->setText(tr("Close"));*/

    //------------------
    /*m_cityListWidget = new CityListWidget(m_cityWidget);
    m_addCityBtn = new QPushButton(m_cityWidget);
    m_addCityBtn->setFocusPolicy(Qt::NoFocus);
    m_addCityBtn->setStyleSheet("QPushButton{font-size:12px;color:#808080;background:transparent;text-align:left;}");//margin-left:18px;border:1px solid rgba(0, 0, 0, 0.08);   QPushButton:hover{background-color:#f5fbff;}
    m_addCityBtn->setFixedWidth(this->width());
    m_addCityBtn->setText("Add");
    m_addCityBtn->setIconSize(QSize(24, 24));
    m_addCityBtn->setIcon(QIcon(":/res/add.png"));

    QVBoxLayout *m_cityLayout = new QVBoxLayout(m_cityWidget);
    m_cityLayout->setContentsMargins(0, 0, 0, 0);
    m_cityLayout->setSpacing(0);
    m_cityLayout->addWidget(m_cityListWidget, 1, Qt::AlignTop);
    m_cityLayout->addWidget(m_addCityBtn, 0, Qt::AlignBottom);*/

    //-------------------------
    m_variableGroup = new QGroupBox(m_systemWidget);
    m_variableGroup->setStyleSheet("QGroupBox{border:1px solid #e0e0e0;border-radius:2px;margin-top:15px;font-size:14px;}QGroupBox:title{subcontrol-origin: margin;subcontrol-position: top left;padding: 6px 3px;color:#00609a;font-weight:bold;}");
    m_variableGroup->setTitle(tr("Variable configuration"));
    m_variableGroup->setFixedWidth(this->width() - 2*GROUP_BOX_MARGIN);
//    m_variableGroup->setContentsMargins(10, 10, 10, 10);

    QLabel *updateFreqLabel = new QLabel(m_systemWidget);
    updateFreqLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    updateFreqLabel->setStyleSheet("QLabel{color:#808080;font-size:14px;text-align:left;}");
    updateFreqLabel->setText(tr("Update frequency") + "(15-60)");
    m_spinBox = new SpinBox(m_systemWidget);
    m_spinBox->setFixedWidth(80);
    m_spinBox->setContextMenuPolicy(Qt::NoContextMenu);
    m_spinBox->setRange(15, 60);
    m_spinBox->setSpinValue(m_preferences->m_updateFrequency);

    QHBoxLayout *m_freqLayout = new QHBoxLayout;
    m_freqLayout->setSpacing(5);
    m_freqLayout->addWidget(updateFreqLabel);
    m_freqLayout->addWidget(m_spinBox);

    QLabel *opacityLabel = new QLabel;
    opacityLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    opacityLabel->setStyleSheet("QLabel{color:#808080;font-size:14px;text-align:left;}");
    opacityLabel->setText(tr("Background transparency"));
    m_opacitySlider = new QSlider(Qt::Horizontal, m_systemWidget);
    m_opacitySlider->setStyleSheet("QSlider::groove:horizontal,QSlider::add-page:horizontal{height:3px;border-radius:3px;background:#7c8487;}QSlider::sub-page:horizontal{height:8px;border-radius:3px;background:#bdbdbd;}QSlider::handle:horizontal{width:12px;margin-top:-5px;margin-bottom:-4px;border-radius:6px;background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0.6 #7c8487,stop:0.8 #7c8487);}QSlider::groove:vertical,QSlider::sub-page:vertical{width:8px;border-radius:3px;background:#808080;}QSlider::add-page:vertical{width:8px;border-radius:3px;background:#808080;}QSlider::handle:vertical{height:12px;margin-left:-5px;margin-right:-4px;border-radius:6px;background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0.6 #7c8487,stop:0.8 #7c8487);}");
    m_opacitySlider->setFocusPolicy(Qt::NoFocus);
    m_opacitySlider->setFocusProxy(this);
    m_opacitySlider->setTracking(true);
    m_opacitySlider->setFixedWidth(80);
    m_opacitySlider->setRange(60, 100);
    m_opacitySlider->setSingleStep(1);
    m_opacitySlider->blockSignals(true);
    m_opacitySlider->setValue(m_preferences->m_opacity);//static_cast<int>(m_preferences->m_opacity * 100);
    m_opacitySlider->blockSignals(false);

    QHBoxLayout *m_opacityLayout = new QHBoxLayout;
    m_opacityLayout->setSpacing(5);
    m_opacityLayout->addWidget(opacityLabel);
    m_opacityLayout->addWidget(m_opacitySlider);

    QVBoxLayout *m_variableLayout = new QVBoxLayout;
    m_variableLayout->setContentsMargins(GROUP_BOX_MARGIN, 10, GROUP_BOX_MARGIN, 10);
    m_variableLayout->setSpacing(5);
    m_variableLayout->addLayout(m_freqLayout);
    m_variableLayout->addLayout(m_opacityLayout);
    m_variableGroup->setLayout(m_variableLayout);
//    QHBoxLayout *m_variableLayout = new QHBoxLayout;
//    m_variableLayout->setContentsMargins(GROUP_BOX_MARGIN, 10, GROUP_BOX_MARGIN, 10);
//    m_variableLayout->setSpacing(5);
//    m_variableLayout->addWidget(updateFreqLabel);
//    m_variableLayout->addWidget(m_spinBox);
//    m_variableGroup->setLayout(m_variableLayout);

    m_fixedGroup = new QGroupBox(m_systemWidget);
    m_fixedGroup->setStyleSheet("QGroupBox{border:1px solid #e0e0e0;border-radius:2px;margin-top:15px;font-size:14px;}QGroupBox:title{subcontrol-origin: margin;subcontrol-position: top left;padding: 6px 3px;color:#00609a;font-weight:bold;}");
    m_fixedGroup->setTitle(tr("Fixed configuration"));
    m_fixedGroup->setFixedWidth(m_variableGroup->width());

    QLabel *tempLabel = new QLabel;
    tempLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    tempLabel->setStyleSheet("QLabel{color:#808080;font-size:14px;text-align:left;}");
    tempLabel->setText(tr("Temperature standard") + ": " +  "˚C");
    QLabel *serviceLabel = new QLabel;
    serviceLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    serviceLabel->setStyleSheet("QLabel{color:#808080;font-size:14px;text-align:left;}");
    serviceLabel->setText(tr("Source of weather: heweather"));

    QVBoxLayout *m_fixedLayout = new QVBoxLayout;
    m_fixedLayout->setContentsMargins(GROUP_BOX_MARGIN, 10, GROUP_BOX_MARGIN, 10);
    m_fixedLayout->setSpacing(10);
    m_fixedLayout->addWidget(tempLabel);
    m_fixedLayout->addWidget(serviceLabel);
    m_fixedGroup->setLayout(m_fixedLayout);

    QVBoxLayout *m_systemLayout = new QVBoxLayout(m_systemWidget);
    m_systemLayout->setContentsMargins(10, 10, 10, 10);
    m_systemLayout->setSpacing(20);
    m_systemLayout->addWidget(m_variableGroup, 0, Qt::AlignLeft | Qt::AlignTop);
    m_systemLayout->addWidget(m_fixedGroup, 0, Qt::AlignLeft);
    m_systemLayout->addStretch();

    m_stackedWidget->addWidget(m_cityWidget);
    m_stackedWidget->addWidget(m_systemWidget);
    m_stackedWidget->setCurrentWidget(m_cityWidget);

    m_settingLayout->addWidget(m_settingTitleBar, 0, Qt::AlignTop);
    m_settingLayout->addWidget(m_stackedWidget, 1, Qt::AlignVCenter);
//    m_settingLayout->addWidget(m_okBtn, 0, Qt::AlignRight);
    m_settingLayout->addStretch();

    connect(m_settingTitleBar, &SettingTitleBar::requestCloseDialog, this, [=] {
        this->accept();
    });
    connect(m_cityWidget, &CityWidget::requestAddCity, this, [=] {
        this->setWindowTitle(tr("Kylin Weather - Search"));
        animationFromBottomToTop(m_settingFrame, m_searchFrame);
    });

    connect(m_cityWidget, &CityWidget::requestRefreshCityMenu, this, &SettingDialog::requestRefreshCityMenu);
    //connect(m_cityWidget, &CityWidget::requestSetDefaultCity, this, &SettingDialog::requestSetDefaultCity);
    connect(m_cityWidget, &CityWidget::requestRefreshWeatherById, this, &SettingDialog::requestRefreshWeatherById);
    /*connect(m_addCityBtn, &QPushButton::clicked, this, [=] {
        SearchDialog dlg;
        connect(&dlg, &SearchDialog::requestAddCityToMenu, this, [this] (const LocationData &data) {
            qDebug() << "set city's id=" << data.id;
            CitySettingData info;
            info.active = false;
            info.id = data.id;
            info.name = data.city;
            info.icon = ":/res/weather_icons/lightgrey/100.png";
            m_cityListWidget->loadItem(info);
            emit this->requestAddCityToMenu(data);
        });
        dlg.exec();
    });*/

    /*connect(m_okBtn, &QPushButton::clicked, this, [=] {
        this->accept();
    });*/
    connect(m_settingTitleBar, &SettingTitleBar::requestSwitchPage, this, [=] (bool b) {
        if (b) {
            m_stackedWidget->setCurrentWidget(m_cityWidget);
//            m_cityListWidget->resetData();
        }
        else {
            m_stackedWidget->setCurrentWidget(m_systemWidget);
        }
    });

    connect(m_spinBox, static_cast<void(SpinBox::*)(int)>(&SpinBox::valueChanged), this, [=] (int value) {
        //qDebug() << "SpinBox Value:"  << value << ", Text:"  << m_spinBox->text();
        m_preferences->m_updateFrequency = value;
    });

    connect(m_opacitySlider, &QSlider::valueChanged, [=] (int value) {
        m_preferences->m_opacity = value;
        emit requestChangeOpacity(value);
    });
}

void SettingDialog::initSearch()
{
    m_searchFrame = new QFrame;
    m_searchFrame->setStyleSheet("QFrame{background:rgba(255, 255, 255, 0.7);}");
    m_searchFrame->setFixedSize(this->size());
    m_searchFrame->setContentsMargins(0, 0, 0, 0);

    m_searchTitleBar = new SearchTitleBar(m_searchFrame);
    m_searchTitleBar->setFixedHeight(150);
    m_displayWidget = new QWidget(m_searchFrame);
//    m_displayWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    m_displayWidget->setContentsMargins(0, m_searchTitleBar->height(), 0, 0);

    m_searchModel = new SearchModel;
    m_searchView = new SearchView;
    m_searchDelegate = new SearchDelegate;
    m_searchView->setFixedWidth(m_searchFrame->width() - 2*CONTENT_MARGIN);
    m_searchView->setItemDelegate(m_searchDelegate);
    m_searchView->setModel(m_searchModel);
    m_searchView->setVisible(false);

    m_noResultLabel = new QLabel(m_searchFrame);
    m_noResultLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_noResultLabel->setAlignment(Qt::AlignCenter);
    m_noResultLabel->setText(tr("No Result"));
    m_noResultLabel->setVisible(false);
    QFont font = m_noResultLabel->font();
    const QFontMetrics fm(font);
    m_noResultLabel->setFixedWidth(fm.width(m_noResultLabel->text()));

    QVBoxLayout *layout = new QVBoxLayout(m_searchFrame);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);
    layout->addWidget(m_searchTitleBar);
    layout->addWidget(m_displayWidget);

    QHBoxLayout *contentLayout = new QHBoxLayout(m_displayWidget);
    contentLayout->setContentsMargins(CONTENT_MARGIN, 0, CONTENT_MARGIN, 0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(m_searchView, 1, Qt::AlignHCenter);
    contentLayout->addWidget(m_noResultLabel, 1, Qt::AlignCenter);

    connect(m_searchView, &SearchView::clicked, this, [this](const QModelIndex &index) {
        QVariant data = index.data(Qt::UserRole);//SearchModel::data Qt::UserRole
        QString selectCityId = data.value<QString>();
        for (const LocationData &line : m_searchModel->locationList()) {//it must exits.
            if (line.id == selectCityId) {
                //emit this->requestAddCityToMenu(line);
                if (m_preferences->isCitiesCountOverMax()) {
                    m_settingTitleBar->showWarnInfo(tr("Only 10 cities can be added at most!"));//最多只能添加10个城市
                    break;
                }
                if (m_preferences->isCityIdExist(line.id)) {
                    m_settingTitleBar->showWarnInfo(tr("The city already exists!"));//该城市已存在
                    break;
                }

                CitySettingData info;
                info.active = false;
                info.id = line.id;
                info.name = line.city;
                info.icon = ":/res/weather_icons/darkgrey/100.png";
                m_cityWidget->addCityItem(info);

                City city;
                city.id = line.id;
                city.name = line.city;

                m_preferences->addCityInfoToPref(city);

                emit this->requestRefreshCityMenu(info.active);

                this->setWindowTitle(tr("Kylin Weather - Setting"));
                animationFromTopToBottom(m_searchFrame, m_settingFrame);
                break;
            }
        }
    });

    connect(m_searchTitleBar, &SearchTitleBar::requestCloseDialog, this, [=] {
        this->setWindowTitle(tr("Kylin Weather - Setting"));
        animationFromTopToBottom(m_searchFrame, m_settingFrame);
    });

    connect(m_searchTitleBar, &SearchTitleBar::requestShowNoResultLabel, this, [=] {
        m_noResultLabel->setVisible(true);
    });
    connect(m_searchTitleBar, &SearchTitleBar::requestResetViewAndModel, this, [=] {
        m_searchView->setVisible(false);
        m_noResultLabel->setVisible(false);
        m_searchModel->setLocationData(QList<LocationData>());
    });

    connect(m_searchTitleBar, &SearchTitleBar::sendSearchResults, this, [=] (QList<LocationData> results) {
        m_searchModel->setLocationData(results);
        m_searchView->setVisible(true);
        m_noResultLabel->setVisible(false);
    });
}

void SettingDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = true;
        m_dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

void SettingDialog::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePressed = false;
}

void SettingDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mousePressed) {
        QPoint move_pos = event->globalPos();
        move(move_pos - m_dragPosition);
        event->accept();
    }
}

void SettingDialog::setData()
{

}

void SettingDialog::refreshCityList(const QString &id)
{
    if (m_cityWidget) {
        m_cityWidget->refreshCityList(id);
    }
}

void SettingDialog::addCityItem(const CitySettingData &info)
{
    m_cityWidget->addCityItem(info);
}

void SettingDialog::accept()
{
    hide();
    setResult(QDialog::Accepted);
    emit applied();
}

void SettingDialog::reject()
{
    hide();
    setResult(QDialog::Rejected);
    setResult(QDialog::Accepted);
}

void SettingDialog::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect borderRect;
    borderRect.setRect(this->rect().x(), this->rect().y(), this->rect().width(), this->rect().height());
    QPainterPath inBorderPath;
    inBorderPath.addRoundedRect(borderRect, 2, 2);
    painter.setClipPath(inBorderPath);

    painter.fillRect(0, 0, width(), height(), QBrush(QColor("#000000")));
}
