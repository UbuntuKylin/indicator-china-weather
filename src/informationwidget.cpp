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

#include "informationwidget.h"
#include "ui_informationwidget.h"
#include "weatherworker.h"

Information::Information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);

    this->setStyleSheet("QWidget{border:none;border-radius:4px;background:transparent;}");

    this->editQssString();
    this->initControlQss();
    //this->initControlData();

    m_weatherworker = new WeatherWorker(this);
    //connect(m_weatherworker, SIGNAL(requestSetForecastWeather(ForecastWeather)), this, SLOT(onSetForecastWeather(ForecastWeather)) );

    //connect(m_weatherworker, &WeatherWorker::requestSetForecastWeather, this, [=] (ForecastWeather forecastweather) {
    //    this->onSetForecastWeather(forecastweather);
    //});
}

Information::~Information()
{
    delete ui;
}


void Information::editQssString()
{
    lbDayQss = "QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,1);line-height:20px;}";
    lbDateQss = "QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,0.5);line-height:20px;}";
    lbIconQss = "QLabel{border:none;background:transparent;}";
    lbTmpQss = "QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,1);line-height:20px;}";
    lbWeaQss = "QLabel{border:none;background:transparent;font-size:12px;color:rgba(255,255,255,1);line-height:20px;}";

    lbLifeStyleQss = "QLabel{border:none;background:transparent;font-size:18px;color:rgba(255,255,255,1);line-height:36px;}";

    lbIndexIconQss = "QLabel{border:none;background:transparent;}";
    lbIndexQss = "QLabel{border:none;background:transparent;font-size:12px;color:rgba(255,255,255,0.5);line-height:18px;}";
    lbIndexBrfQss = "QLabel{border:none;background:transparent;font-size:16px;font-weight:400;color:rgba(255,255,255,1);line-height:18px;}";

    lineQss = "border:none;background-color:rgba(255,255,255,0.1);";
    lineLinearGradientQss = "border:none;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 255, 0), stop:0.5 rgba(255, 255, 255, 0.7), stop:1 rgba(255, 255, 255, 0));";
}

void Information::initControlQss()
{
    /******************天气预报***************************/

    //设置星期
    ui->lbDay_1->setStyleSheet(lbDayQss);
    ui->lbDay_1->setAlignment(Qt::AlignCenter);
    ui->lbDay_2->setStyleSheet(lbDayQss);
    ui->lbDay_2->setAlignment(Qt::AlignCenter);
    ui->lbDay_3->setStyleSheet(lbDayQss);
    ui->lbDay_3->setAlignment(Qt::AlignCenter);
    ui->lbDay_4->setStyleSheet(lbDayQss);
    ui->lbDay_4->setAlignment(Qt::AlignCenter);
    ui->lbDay_5->setStyleSheet(lbDayQss);
    ui->lbDay_5->setAlignment(Qt::AlignCenter);
    ui->lbDay_6->setStyleSheet(lbDayQss);
    ui->lbDay_6->setAlignment(Qt::AlignCenter);
    ui->lbDay_7->setStyleSheet(lbDayQss);
    ui->lbDay_7->setAlignment(Qt::AlignCenter);

    //设置日期
    ui->lbDate_1->setStyleSheet(lbDateQss);
    ui->lbDate_1->setAlignment(Qt::AlignCenter);
    ui->lbDate_2->setStyleSheet(lbDateQss);
    ui->lbDate_2->setAlignment(Qt::AlignCenter);
    ui->lbDate_3->setStyleSheet(lbDateQss);
    ui->lbDate_3->setAlignment(Qt::AlignCenter);
    ui->lbDate_4->setStyleSheet(lbDateQss);
    ui->lbDate_4->setAlignment(Qt::AlignCenter);
    ui->lbDate_5->setStyleSheet(lbDateQss);
    ui->lbDate_5->setAlignment(Qt::AlignCenter);
    ui->lbDate_6->setStyleSheet(lbDateQss);
    ui->lbDate_6->setAlignment(Qt::AlignCenter);
    ui->lbDate_7->setStyleSheet(lbDateQss);
    ui->lbDate_7->setAlignment(Qt::AlignCenter);

    //设置图标控件
    ui->lbIcon_1->setStyleSheet(lbIconQss);
    ui->lbIcon_2->setStyleSheet(lbIconQss);
    ui->lbIcon_3->setStyleSheet(lbIconQss);
    ui->lbIcon_4->setStyleSheet(lbIconQss);
    ui->lbIcon_5->setStyleSheet(lbIconQss);
    ui->lbIcon_6->setStyleSheet(lbIconQss);
    ui->lbIcon_7->setStyleSheet(lbIconQss);

    //设置显示温度控件
    ui->lbTmp_1->setStyleSheet(lbTmpQss);
    ui->lbTmp_1->setAlignment(Qt::AlignCenter);
    ui->lbTmp_2->setStyleSheet(lbTmpQss);
    ui->lbTmp_2->setAlignment(Qt::AlignCenter);
    ui->lbTmp_3->setStyleSheet(lbTmpQss);
    ui->lbTmp_3->setAlignment(Qt::AlignCenter);
    ui->lbTmp_4->setStyleSheet(lbTmpQss);
    ui->lbTmp_4->setAlignment(Qt::AlignCenter);
    ui->lbTmp_5->setStyleSheet(lbTmpQss);
    ui->lbTmp_5->setAlignment(Qt::AlignCenter);
    ui->lbTmp_6->setStyleSheet(lbTmpQss);
    ui->lbTmp_6->setAlignment(Qt::AlignCenter);
    ui->lbTmp_7->setStyleSheet(lbTmpQss);
    ui->lbTmp_7->setAlignment(Qt::AlignCenter);

    //设置显示天气描述控件
    ui->lbWea_1->setStyleSheet(lbWeaQss);
    ui->lbWea_1->setAlignment(Qt::AlignCenter);
    ui->lbWea_2->setStyleSheet(lbWeaQss);
    ui->lbWea_2->setAlignment(Qt::AlignCenter);
    ui->lbWea_3->setStyleSheet(lbWeaQss);
    ui->lbWea_3->setAlignment(Qt::AlignCenter);
    ui->lbWea_4->setStyleSheet(lbWeaQss);
    ui->lbWea_4->setAlignment(Qt::AlignCenter);
    ui->lbWea_5->setStyleSheet(lbWeaQss);
    ui->lbWea_5->setAlignment(Qt::AlignCenter);
    ui->lbWea_6->setStyleSheet(lbWeaQss);
    ui->lbWea_6->setAlignment(Qt::AlignCenter);
    ui->lbWea_7->setStyleSheet(lbWeaQss);
    ui->lbWea_7->setAlignment(Qt::AlignCenter);

    //分割线设置
    ui->line_1->setStyleSheet(lineLinearGradientQss);
    ui->line_2->setStyleSheet(lineLinearGradientQss);
    ui->line_3->setStyleSheet(lineLinearGradientQss);
    ui->line_4->setStyleSheet(lineLinearGradientQss);
    ui->line_5->setStyleSheet(lineLinearGradientQss);
    ui->line_6->setStyleSheet(lineLinearGradientQss);
    ui->line_1->show();
    ui->line_2->show();
    ui->line_3->show();
    ui->line_4->show();
    ui->line_5->show();
    ui->line_6->show();


    /******************生活指数***************************/

    //生活指数
    ui->lbLifeStyle->setStyleSheet(lbLifeStyleQss);
    ui->lbLifeStyle->setText("生活指数");

    //穿衣指数
    ui->lb_drsg->setStyleSheet(lbIndexQss);
    ui->lb_drsg_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_uv_brf->move(96,316);
    ui->lb_drsg_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_drsg_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_clothe.png);}");
    ui->lb_drsg->setText("穿衣指数");

    //感冒指数
    ui->lb_flu->setStyleSheet(lbIndexQss);
    ui->lb_flu_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_uv_brf->move(299,316);
    ui->lb_flu_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_flu_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_flu.png);}");
    ui->lb_flu->setText("感冒指数");

    //紫外线指数
    ui->lb_uv->setStyleSheet(lbIndexQss);
    ui->lb_uv_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_uv_brf->move(504,316);
    ui->lb_uv_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_uv_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_uv.png);}");
    ui->lb_uv->setText("紫外线指数");

    //洗车指数
    ui->lb_cw->setStyleSheet(lbIndexQss);
    ui->lb_cw_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_cw_brf->move(707,316);
    ui->lb_cw_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_cw_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_cash_wash.png);}");
    ui->lb_cw->setText("洗车指数");

    //空气指数
    ui->lb_air->setStyleSheet(lbIndexQss);
    ui->lb_air_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_air_brf->move(96,395);
    ui->lb_air_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_air_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_air.png);}");
    ui->lb_air->setText("空气指数");

    //运动指数
    ui->lb_sport->setStyleSheet(lbIndexQss);
    ui->lb_sport_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_sport_brf->move(299,395);
    ui->lb_sport_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_sport_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_sport.png);}");
    ui->lb_sport->setText("运动指数");

    //分割线
    ui->lineH->setStyleSheet(lineQss);
    ui->lineV_1->setStyleSheet(lineQss);
    ui->lineV_2->setStyleSheet(lineQss);
    ui->lineV_3->setStyleSheet(lineQss);
    ui->lineH->show();
    ui->lineV_1->show();
    ui->lineV_2->show();
    ui->lineV_3->show();
}

void Information::onSetForecastWeather(ForecastWeather forecastweather)
{
    if (m_day == 8){
        m_day = 1; //重新设置第一天
    }

    //获取星期
    QDateTime current_date_time = QDateTime::fromString(forecastweather.dateTime,"yyyy-MM-dd");//将从服务器上获取到的日期处理成特定格式
    QString current_week_1 = "今天";//current_date_time.toString("ddd");
    QString current_week_2 = current_date_time.addDays(+1).toString("ddd");
    QString current_week_3 = current_date_time.addDays(+2).toString("ddd");
    QString current_week_4 = current_date_time.addDays(+3).toString("ddd");
    QString current_week_5 = current_date_time.addDays(+4).toString("ddd");
    QString current_week_6 = current_date_time.addDays(+5).toString("ddd");
    QString current_week_7 = current_date_time.addDays(+6).toString("ddd");

    //获取日期
    QDateTime time = QDateTime::fromString(forecastweather.dateTime,"yyyy-MM-dd");//将从服务器上获取到的日期处理成特定格式
    QString date_1 = time.toString("M月d日"); //设置显示格式
    QString date_2 = time.addDays(+1).toString("M月d日");
    QString date_3 = time.addDays(+2).toString("M月d日");
    QString date_4 = time.addDays(+3).toString("M月d日");
    QString date_5 = time.addDays(+4).toString("M月d日");
    QString date_6 = time.addDays(+5).toString("M月d日");
    QString date_7 = time.addDays(+6).toString("M月d日");

    //获取天气图片
    int code;
    QTime current_time = QTime::currentTime();
    int hour = current_time.hour();
    if (hour>=6 && hour<= 18){
        code  = forecastweather.cond_code_d.toInt();
    } else {
        code  = forecastweather.cond_code_n.toInt();
    }
    QString picStr = convertCodeToBackgroud(code);
    QString pic = "QLabel{background-image:url(" + picStr + ");}";

    //获取气温
    QString tmp = forecastweather.tmp_min + "℃~" + forecastweather.tmp_max + "℃";

    //获取天气及风力
    QString wea;
    if (forecastweather.cond_code_d == forecastweather.cond_code_n){
        wea = forecastweather.cond_txt_d + "\n" + forecastweather.wind_dir + forecastweather.wind_sc + "级";
    }else {
        wea = forecastweather.cond_txt_d + "转" + forecastweather.cond_txt_n + "\n" + forecastweather.wind_dir + forecastweather.wind_sc + "级";
    }

    switch (m_day) {
    case 1:
        ui->lbDay_1->setText(current_week_1);
        ui->lbDate_1->setText(date_1);
        ui->lbIcon_1->setStyleSheet(pic);
        ui->lbTmp_1->setText(tmp);
        ui->lbWea_1->setText(wea);
        break;
    case 2:
        ui->lbDay_2->setText(current_week_2);
        ui->lbDate_2->setText(date_2);
        ui->lbIcon_2->setStyleSheet(pic);
        ui->lbTmp_2->setText(tmp);
        ui->lbWea_2->setText(wea);
        break;
    case 3:
        ui->lbDay_3->setText(current_week_3);
        ui->lbDate_3->setText(date_3);
        ui->lbIcon_3->setStyleSheet(pic);
        ui->lbTmp_3->setText(tmp);
        ui->lbWea_3->setText(wea);
        break;
    case 4:
        ui->lbDay_4->setText(current_week_4);
        ui->lbDate_4->setText(date_4);
        ui->lbIcon_4->setStyleSheet(pic);
        ui->lbTmp_4->setText(tmp);
        ui->lbWea_4->setText(wea);
        break;
    case 5:
        ui->lbDay_5->setText(current_week_5);
        ui->lbDate_5->setText(date_5);
        ui->lbIcon_5->setStyleSheet(pic);
        ui->lbTmp_5->setText(tmp);
        ui->lbWea_5->setText(wea);
        break;
    case 6:
        ui->lbDay_6->setText(current_week_6);
        ui->lbDate_6->setText(date_6);
        ui->lbIcon_6->setStyleSheet(pic);
        ui->lbTmp_6->setText(tmp);
        ui->lbWea_6->setText(wea);
        break;
    case 7:
        ui->lbDay_7->setText(current_week_7);
        ui->lbDate_7->setText(date_7);
        ui->lbIcon_7->setStyleSheet(pic);
        ui->lbTmp_7->setText(tmp);
        ui->lbWea_7->setText(wea);
        break;
    default:
        break;
    }

    m_day += 1;
}

void Information::onSetLifeStyle(LifeStyle lifestyle)
{
    ui->lb_drsg_brf->setText(lifestyle.drsg_brf); //穿衣指数

    ui->lb_flu_brf->setText(lifestyle.flu_brf); //感冒指数

    ui->lb_uv_brf->setText(lifestyle.uv_brf); //紫外线指数

    ui->lb_cw_brf->setText(lifestyle.cw_brf); //洗车指数

    ui->lb_air_brf->setText(lifestyle.air_brf); //空气指数

    ui->lb_sport_brf->setText(lifestyle.sport_brf); //运动指数
}

QString Information::convertCodeToBackgroud(int code)
{
    if (code == 100 || code == 900) {
        return ":/res/control_icons/weather-clear.png";
    }
    else if (code <= 103 && code >= 101) {
        return ":/res/control_icons/weather-few-clouds.png";
    }
    else if (code == 104 || code == 901) {
        return ":/res/control_icons/weather-overcast.png";
    }
    else if (code <= 204 && code >= 200) {
        return ":/res/control_icons/weather-clear.png";
    }
    else if (code <= 213 && code >= 205) {
        return ":/res/control_icons/weather-overcast.png";
    }
    else if (code <= 399 && code >= 300) {
        return ":/res/control_icons/weather-rain.png";
    }
    else if (code <= 499 && code >= 400) {
        return ":/res/control_icons/weather-snow.png";
    }
    else if (code <= 502 && code >= 500) {
        return ":/res/control_icons/weather-fog.png";
    }
    else if (code <= 508 && code >= 503) {
        return ":/res/control_icons/weather-sandstorm.png";
    }
    else if (code <= 515 && code >= 509) {
        return ":/res/control_icons/weather-fog.png";
    }
    else if (code <= 999 && code >= 900) {
        return ":/res/control_icons/weather-none.png";
    }
    else {
        return ":/res/control_icons/weather-clear.png";
    }
}
