#ifndef CITYADDITION_H
#define CITYADDITION_H

#include "cityaddsearchbox.h"
#include "cityaddsearchview.h"
#include "cityaddsearchdelegate.h"

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

private:
    Ui::cityaddition *ui;

    CityAddSearchBox *m_cityaddsearchbox = nullptr;

    CityAddSearchView *m_cityaddsearchview = nullptr;
    CityAddSearchDelegate *m_cityaddsearchdelegate;
    QSortFilterProxyModel* m_proxyModel;
    QStandardItemModel *m_model;

    int totalNum;
    int redNum;
    int blueNum;
    int yellowNum;

    void initData();
};

#endif // CITYADDITION_H
