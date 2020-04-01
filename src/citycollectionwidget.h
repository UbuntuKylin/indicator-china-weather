#ifndef CITYCOLLECTION_H
#define CITYCOLLECTION_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class citycollectionwidget;
}

class CityCollectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CityCollectionWidget(QWidget *parent = nullptr);
    ~CityCollectionWidget();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_btnCancel_clicked();

private:
    Ui::citycollectionwidget *ui;

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // CITYCOLLECTION_H
