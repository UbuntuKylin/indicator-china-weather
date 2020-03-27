#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwindow.h"
#include "leftupcitybtn.h"
#include "leftupsearchbox.h"
#include "leftupsearchview.h"
#include "leftupsearchdelegate.h"
#include "informationwidget.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <QMainWindow>
#include <QStandardPaths>
#include <QMouseEvent>
#include <QDebug>
#include <QButtonGroup>
#include <QObjectUserData>
#include <QSortFilterProxyModel>
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:

    void on_btnMinimize_clicked();

    void on_btnCancel_clicked();

private:
    Ui::MainWindow *ui;
    QScrollArea *m_scrollarea = nullptr;
    QWidget *m_scrollwidget = nullptr;

    LeftUpCityBtn *m_leftupcitybtn = nullptr;
    LeftUpSearchBox *m_leftupsearchbox = nullptr;

    LeftUpSearchView *m_searchView = nullptr;
    LeftUpSearchDelegate *m_delegate;
    QSortFilterProxyModel* m_proxyModel;
    QStandardItemModel *m_model;

    int totalNum;
    int redNum;
    int blueNum;
    int yellowNum;

    void checkSingle();

    void initData();

    void initControlQss();

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // MAINWINDOW_H
