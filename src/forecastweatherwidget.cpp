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

#include "forecastweatherwidget.h"
#include "forecastitemwidget.h"
#include "indexitemwidget.h"
#include "separator.h"

#include <QEvent>
#include <QDebug>

namespace {

const QStringList LIFESTYLE = {QString(QObject::tr("comf")), QString(QObject::tr("drsg")), QString(QObject::tr("flu")), QString(QObject::tr("sport")), QString(QObject::tr("trav")), QString(QObject::tr("uv")), QString(QObject::tr("cw")), QString(QObject::tr("air"))};
const QStringList LIFESTYLEICON = {":/res/cash_wash_index.png", ":/res/clothe_index.png", ":/res/cash_wash_index.png", ":/res/cash_wash_index.png", ":/res/cash_wash_index.png", ":/res/ultraviolet_rays.png", ":/res/cash_wash_index.png", ":/res/cash_wash_index.png"};

}

ForecastWeatherWidget::ForecastWeatherWidget(WeatherWorker *weatherWorker, QFrame *parent) :
    QFrame(parent)
    , m_weatherWorker(weatherWorker)
{
    this->setFixedSize(355, 340);
//    this->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:rgba(0,0,0,0.2)}");

    m_lifeIndexList = LIFESTYLE;
    m_lifeIndexIconList = LIFESTYLEICON;

    this->initWidgets();



    /*m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    m_firstDay = new ForecastItemWidget(this);
    m_secondDay = new ForecastItemWidget(this);
    m_thirdDay = new ForecastItemWidget(this);

    m_forecastlayout = new QHBoxLayout;
    m_forecastlayout->setContentsMargins(0, 0, 0, 0);
    m_forecastlayout->setSpacing(20);
    m_forecastlayout->addWidget(m_firstDay, 0, Qt::AlignVCenter);
    m_forecastlayout->addWidget(m_secondDay, 0, Qt::AlignVCenter);
    m_forecastlayout->addWidget(m_thirdDay, 0, Qt::AlignVCenter);

    m_layout->addLayout(m_forecastlayout);
    m_layout->addStretch();*/


//    m_label = new QLabel;
//    m_label->setFixedSize(this->width(), this->height());
//    m_label->setText(tr("Weather Forecast Category Page"));
//    m_label->setAlignment(Qt::AlignCenter/* | Qt::AlignTop*/);
//    m_label->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:rgba(0,0,0,0.2);}");

//    m_layout->addWidget(m_label, 0, Qt::AlignHCenter);
}

ForecastWeatherWidget::~ForecastWeatherWidget()
{
    for(int i=0; i<pl.count(); i++) {
        IndexItemWidget *item = pl.at(i);
        delete item;
        item = NULL;
    }
    pl.clear();

    while (QLayoutItem *item = m_indexGridLayout->takeAt(0)) {
        item->widget()->deleteLater();
        delete item;
    }
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void ForecastWeatherWidget::initWidgets()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    this->setLayout(m_layout);

    this->initForecastWidget();
    this->initIndexTitleWidget();
    this->initIndexWidget();
    m_layout->addStretch();
}

void ForecastWeatherWidget::initForecastWidget()
{
    QWidget *w = new QWidget;
    w->setFixedSize(this->width(), 180);
    //test background
    /*w->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::gray));
    w->setPalette(palette);
    //w->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:rgba(0,0,0,0.2);}");
    */
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);//w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_forecastlayout = new QHBoxLayout(w);
    m_forecastlayout->setContentsMargins(0, 10, 0, 10);
    m_forecastlayout->setSpacing(20);

    m_layout->addWidget(w, 0, Qt::AlignTop);

    m_firstDay = new ForecastItemWidget(this);
    m_secondDay = new ForecastItemWidget(this);
    m_thirdDay = new ForecastItemWidget(this);
//    m_thirdDay->setTextData();

    m_forecastlayout->addWidget(m_firstDay, 0, Qt::AlignVCenter);
    m_forecastlayout->addWidget(new VSeparator/*, 0, Qt::AlignVCenter*/);
    m_forecastlayout->addWidget(m_secondDay, 0, Qt::AlignVCenter);
    m_forecastlayout->addWidget(new VSeparator/*, 0, Qt::AlignVCenter*/);
    m_forecastlayout->addWidget(m_thirdDay, 0, Qt::AlignVCenter);
}

void ForecastWeatherWidget::initIndexTitleWidget()
{
    QWidget *w = new QWidget;
    w->setFixedSize(this->width(), 34);

    //test background
    /*w->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::red));
    w->setPalette(palette);
    //w->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:rgba(0,0,0,0.2);}");
    */
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);//w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_indexTitlelayout = new QVBoxLayout(w);
    m_indexTitlelayout->setContentsMargins(0, 0, 0, 0);
    m_indexTitlelayout->setSpacing(5);

//    m_layout->addSpacing(5);
    m_layout->addWidget(w, 0, Qt::AlignLeft);

    QLabel *iconLabel = new QLabel;
    QLabel *textLabel = new QLabel;
    iconLabel->setFixedSize(20 ,20);
    iconLabel->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    iconLabel->setPixmap(QPixmap(":/res/life_index_d.png"));
    textLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    textLabel->setText(tr("Life index"));

    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->setContentsMargins(10, 0, 0, 0);
    h_layout->setSpacing(5);
    h_layout->addWidget(iconLabel, 0, Qt::AlignLeft);
    h_layout->addWidget(textLabel, 0, Qt::AlignVCenter | Qt::AlignLeft);
    h_layout->addStretch();

    m_indexTitlelayout->addWidget(new HSeparator);
    m_indexTitlelayout->addStretch();
    m_indexTitlelayout->addLayout(h_layout);
//    m_indexTitlelayout->addWidget(iconLabel, 0, Qt::AlignLeft);
//    m_indexTitlelayout->addWidget(textLabel, 0, Qt::AlignVCenter | Qt::AlignLeft);
    m_indexTitlelayout->addStretch();
    m_indexTitlelayout->addWidget(new HSeparator);
}

void ForecastWeatherWidget::initIndexWidget()
{
    QWidget *w = new QWidget;
    w->setFixedSize(this->width(), 126);
    //test background
    /*w->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::blue));
    w->setPalette(palette);
    //w->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:rgba(0,0,0,0.2);}");
    */
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);//w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_indexlayout = new QVBoxLayout(w);
    m_indexlayout->setContentsMargins(0, 0, 0, 0);
    m_indexlayout->setSpacing(1);

//    m_layout->addSpacing(8);
    m_layout->addWidget(w, 0, Qt::AlignHCenter);

    m_indexGridLayout = new QGridLayout;
    m_indexGridLayout->setSpacing(1);
    m_indexGridLayout->setMargin(0);
    m_indexlayout->addLayout(m_indexGridLayout);
    m_indexlayout->setSpacing(1);

    this->refershLifeIndexGridLayout();

    showLifeStyleIndex(QString());
}


void ForecastWeatherWidget::refershLifeIndexGridLayout()
{
    while (QLayoutItem *item = m_indexGridLayout->takeAt(0)) {
        item->widget()->deleteLater();
        delete item;
    }

    int index = 0;
    const int count = m_lifeIndexList.size();
    for (int i = 0; i != count; ++i, ++index) {
        IndexItemWidget *item = new IndexItemWidget(m_lifeIndexList[i], m_lifeIndexIconList[i]);
        connect(item, SIGNAL(requestShowMsg(QString)), this, SLOT(showLifeStyleIndex(QString)));
        m_indexGridLayout->addWidget(item, index / 3, index % 3);
        pl.append(item);
    }
}

void ForecastWeatherWidget::refreshForecastData(const ForecastWeather &data, int index)
{
    if (index == 0) {
        m_firstDay->resetForecastData(data, index);
    }
    else if (index == 1) {
        m_secondDay->resetForecastData(data, index);
    }
    if (index == 2) {
        m_thirdDay->resetForecastData(data, index);
    }
}

void ForecastWeatherWidget::refreshLifestyleData(const LifeStyle &data)
{
    int n = 0;
    if (pl.count() == 8) {
        //舒适度指数
        pl[n++]->refreshLifeStyle(data.comf_brf, data.comf_txt);
        //穿衣指数
        pl[n++]->refreshLifeStyle(data.drsg_brf, data.drsg_txt);
        //感冒指数
        pl[n++]->refreshLifeStyle(data.flu_brf, data.flu_txt);
        //运动指数
        pl[n++]->refreshLifeStyle(data.sport_brf, data.sport_txt);
        //旅游指数
        pl[n++]->refreshLifeStyle(data.trav_brf, data.trav_txt);
        //紫外线指数
        pl[n++]->refreshLifeStyle(data.uv_brf, data.uv_txt);
        //洗车指数
        pl[n++]->refreshLifeStyle(data.cw_brf, data.cw_txt);
        //空气污染扩散条件指数
        pl[n++]->refreshLifeStyle(data.air_brf, data.air_txt);
    }
}

void ForecastWeatherWidget::showLifeStyleIndex(const QString &name)
{
    //comf_brf  舒适度指数
    //drsg_brf 穿衣指数
    //flu_brf 感冒指数
    //sport_brf 运动指数
    //trav_brf 旅游指数
    //uv_brf 紫外线指数
    //cw_brf 洗车指数
    //air_brf 空气污染扩散条件指数

    static const char *index_strings[] = {
        QT_TR_NOOP("comf index"),
        QT_TR_NOOP("drsg index"),
        QT_TR_NOOP("flu index"),
        QT_TR_NOOP("sport index"),
        QT_TR_NOOP("trav index"),
        QT_TR_NOOP("uv index"),
        QT_TR_NOOP("cw index"),
        QT_TR_NOOP("air index")
    };

    if (!name.isEmpty()) {
        const int idx = LIFESTYLE.indexOf(name);
        qDebug() << tr(index_strings[idx]);
    }
}

void ForecastWeatherWidget::leaveEvent(QEvent *e)
{
    QWidget::leaveEvent(e);

    showLifeStyleIndex(QString());
}
