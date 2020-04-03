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
    void onSetForecastWeather(ForecastWeather forecastweather);
    void onSetLifeStyle(LifeStyle lifestyle);

private:
    int m_day = 1;

    Ui::information *ui;

    void editQssString();
    void initControlQss();
    void initControlData();

    QString convertCodeToBackgroud(int code);

    QString lbDayQss, lbDateQss, lbIconQss, lbTmpQss, lbWeaQss;
    QString lbLifeStyleQss;
    QString lbIndexIconQss, lbIndexQss, lbIndexBrfQss, lineQss, lineLinearGradientQss;

    WeatherWorker *m_weatherworker = nullptr;

};

#endif // INFORMATION_H
