#ifndef INFORMATION_H
#define INFORMATION_H

#include "data.h"

#include <QWidget>

class WeatherWorker;

namespace Ui {
class information;
}

class Information : public QWidget
{
    Q_OBJECT

public:
    explicit Information(QWidget *parent = nullptr);
    ~Information();

public slots:
    //set forecast weather in mainwindow
    void onSetForecastWeather(ForecastWeather forecastweather);

    //set lifestyle index in mainwindow
    void onSetLifeStyle(LifeStyle lifestyle);

private:
    int m_day = 1;

    Ui::information *ui;

    void editQssString(); //set style of forecast control and lifestyle control
    void initControlQss(); //initial data of some control

    QString convertCodeToBackgroud(int code); //update background image according to weather code

    QString lbDayQss, lbDateQss, lbIconQss, lbTmpQss, lbWeaQss;
    QString lbLifeStyleQss;
    QString lbIndexIconQss, lbIndexQss, lbIndexBrfQss, lineQss, lineLinearGradientQss;

    WeatherWorker *m_weatherworker = nullptr;

};

#endif // INFORMATION_H
