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
    void on_btnCancel_clicked();

    void onRequestClearLineEdit();

private:
    Ui::cityaddition *ui;

    CityAddSearchBox *m_cityaddsearchbox = nullptr;

    CityAddSearchView *m_cityaddsearchview = nullptr;
    CityAddSearchDelegate *m_cityaddsearchdelegate = nullptr;
    QSortFilterProxyModel* m_proxyModel = nullptr;
    QStandardItemModel *m_model = nullptr;

    LocationWorker *m_locationWorker = nullptr;

    void searchCityName();

    void onSearchBoxEdited();

signals:
    void hideCityAddWiget();
    void requestAddNewCity(QString cityId);
};

#endif // CITYADDITION_H
