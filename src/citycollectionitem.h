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

private:
    Ui::citycollectionitem *ui;
};

#endif // CITYCOLLECTIONITEM_H
