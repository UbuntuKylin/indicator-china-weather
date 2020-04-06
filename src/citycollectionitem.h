#ifndef CITYCOLLECTIONITEM_H
#define CITYCOLLECTIONITEM_H

#include <QWidget>

namespace Ui {
class citycollectionitem;
}

class citycollectionitem : public QWidget
{
    Q_OBJECT

public:
    explicit citycollectionitem(QWidget *parent = nullptr);
    ~citycollectionitem();

    void setItemWidgetState(bool isShowNormal);

private slots:
    void on_btnAddCity_clicked();

private:
    Ui::citycollectionitem *ui;

signals:
    void showCityAddWiget();
};

#endif // CITYCOLLECTIONITEM_H
