#ifndef CITYADDITION_H
#define CITYADDITION_H

#include "cityaddsearchbox.h"
#include "cityaddsearchview.h"
#include "cityaddsearchdelegate.h"
#include "locationworker.h"

#include <QWidget>
#include <QButtonGroup>
#include <QObjectUserData>
#include <QSortFilterProxyModel>

namespace Ui {
class cityaddition;
}

class CityAddition : public QWidget
{
    Q_OBJECT

public:
    explicit CityAddition(QWidget *parent = nullptr);
    ~CityAddition();

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

    CityAddSearchView *m_cityaddsearchview = nullptr;
    CityAddSearchDelegate *m_cityaddsearchdelegate = nullptr;
    QSortFilterProxyModel* m_proxyModel = nullptr;
    QStandardItemModel *m_model = nullptr;

    LocationWorker *m_locationWorker = nullptr;

    // search and show cities in list from file china-city-list.csv
    void searchCityName();

    // handle when search box edited,
    void onSearchBoxEdited();

signals:
    void hideCityAddWiget(); //send this signal to hide collected city add widget
    void requestChangeWidgetState(); // send this signal to collected both add collect city widget and show city list widget
    void requestAddNewCity(QString cityId); // send this signal to add new collected city in collected city list
};

#endif // CITYADDITION_H
