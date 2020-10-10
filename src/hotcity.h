#ifndef HOTCITY_H
#define HOTCITY_H
#include <QPushButton>

class HotCity : public QPushButton
{
    Q_OBJECT

public:
    HotCity(QWidget *parent,QString name, QString code);
    void onThemeChanged(QString str);
signals:
    void hotCityClick(QString);

public slots:
    void onHotCityClick();

private:
    QString cityCode="";
};

#endif // HOTCITY_H
