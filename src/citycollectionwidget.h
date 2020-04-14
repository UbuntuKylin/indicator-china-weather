#ifndef CITYCOLLECTION_H
#define CITYCOLLECTION_H

#include <QWidget>
#include <QMouseEvent>
#include <QRect>
#include <QScreen>
#include <QStandardPaths>
#include <QDebug>
#include <QObject>

#include <unistd.h>

class CityAddition;
class citycollectionitem;

namespace Ui {
class citycollectionwidget;
}

class CityCollectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CityCollectionWidget(QWidget *parent = nullptr);
    ~CityCollectionWidget();

public slots:
    void onRequestAddNewCity(QString cityId);
    void onRequestDeleteCity(QString cityId);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_btnCancel_clicked();

    void onShowCityAddWiget();
    void onHideCityAddWiget();

private:
    Ui::citycollectionwidget *ui;

    CityAddition *m_cityaddition;

    void writeCollectedCity(QString cityId);
    QString readCollectedCity();

    void setCurrentCity();
    void setCollectCity();
    void showCollectCity(int x, int y, bool isShowNormal, QString cityId);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // CITYCOLLECTION_H
