#ifndef CITYADDITION_H
#define CITYADDITION_H
#define FITTHEMEWINDOW "org.ukui.style"

#include "cityaddsearchbox.h"
#include "cityaddsearchview.h"
#include "cityaddsearchdelegate.h"
#include "darkcityaddsearchdelegate.h"
#include "locationworker.h"
#include "cityaddhotview.h"

#include <QPainterPath>
#include <QWidget>
#include <QButtonGroup>
#include <QObjectUserData>
#include <QSortFilterProxyModel>
#include <QGSettings>
namespace Ui {
class cityaddition;
}

class CityAddition : public QWidget
{
    Q_OBJECT

public:
    explicit CityAddition(QWidget *parent = nullptr);
    ~CityAddition();
void getStr(QString str);



private slots:
    // click to close window.
    void on_btnCancel_clicked();

    // line edit stay original state after selecting a new collected city.
    void onRequestClearLineEdit();

    // click to return collected cities window.
    void on_btnReturn_clicked();

private:
    Ui::cityaddition *ui;

    CityAddSearchBox *m_cityaddsearchbox = nullptr;
    CityAddHotView *m_cityaddhotview = nullptr;
    CityAddSearchView *m_cityaddsearchview = nullptr;
    CityAddSearchDelegate *m_cityaddsearchdelegate = nullptr;
    DarkCityAddSearchDelegate *m_darkcityaddsearchdelegate = nullptr;
    QSortFilterProxyModel* m_proxyModel = nullptr;
    QStandardItemModel *m_model = nullptr;

    LocationWorker *m_locationWorker = nullptr;
    QString ThemeStyle;


    // search and show cities in list from file china-city-list.csv
    void searchCityName();

    // handle when search box edited,
    void onSearchBoxEdited();
    bool event(QEvent *event);

signals:
    void setHotCity(QString);
    void hideCityAddWiget(); //send this signal to hide collected city add widget
    void requestChangeWidgetState(); // send this signal to collected both add collect city widget and show city list widget
    void requestAddNewCity(QString cityId); // send this signal to add new collected city in collected city list
};

#endif // CITYADDITION_H
