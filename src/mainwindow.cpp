/*
 * Copyright (C) 2013 ~ 2020 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
 *
 * Authors:
 *  Kobe Lee    lixiang@kylinos.cn/kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dataitem.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    checkSingle();

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFocusPolicy(Qt::StrongFocus);//this->setFocusPolicy(Qt::NoFocus);
    this->setWindowTitle(tr("Kylin Weather"));
    this->setWindowIcon(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/control_icons/indicator-china-weather.png")) );

    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    this->setStyleSheet("QWidget{border:none;border-radius:6px;}");

    this->initControlQss();

    ui->widget_normal->show();

    m_leftupcitybtn = new LeftUpCityBtn(ui->widget_normal);
    m_leftupcitybtn->move(20, 22);
    m_leftupcitybtn->show();


    m_leftupsearchbox = new LeftUpSearchBox(ui->widget_normal);
    m_leftupsearchbox->move(116, 17);
    m_leftupsearchbox->show();


    initData();
    m_searchView = new LeftUpSearchView(ui->widget_normal);
    m_delegate = new LeftUpSearchDelegate(m_searchView);
    m_proxyModel = new QSortFilterProxyModel(m_searchView);
    m_searchView->move(116, 49);
    m_searchView->resize(181,260);
    m_searchView->hide();
    m_searchView->setItemDelegate(m_delegate);       //为视图设置委托
    m_searchView->setSpacing(1);                   //为视图设置控件间距
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterRole(Qt::UserRole);
    m_proxyModel->setDynamicSortFilter(true);
    m_searchView->setModel(m_proxyModel);                  //为委托设置模型
    m_searchView->setViewMode(QListView::IconMode); //设置Item图标显示
    m_searchView->setDragEnabled(false);            //控件不允许拖动


    connect(m_leftupsearchbox, &LeftUpSearchBox::textEdited, this, [this] () {
        if (m_leftupsearchbox->text().size() == 0){
            m_searchView->hide();
        }else{
            m_searchView->show();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkSingle()
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString lockPath = homePath.at(0) + "/.config/china-weather-lock";
    int fd = open(lockPath.toUtf8().data(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fd < 0) { exit(1); }

    if (lockf(fd, F_TLOCK, 0)) {
        qDebug()<<"Can't lock single file, indicator-china-weather is already running!";
        exit(0);
    }
}

void MainWindow::initControlQss()
{
    ui->centralwidget->setStyleSheet("#centralwidget{border:1px solid rgba(255,255,255,0.05);border-radius:6px;background:rgba(19,19,20,0);}");
    ui->centralwidget->setStyleSheet("#centralwidget{color:white;background-image:url(':/res/background/weather-clear.png');background-repeat:no-repeat;}");

    ui->btnMinimize->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/min_normal_btn.png);}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/min_hover_btn.png);}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/min_pressed_btn.png);}");

    ui->btnCancel->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/close_normal_btn.png);}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/close_hover_btn.png);}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:transparent;background-image:url(:/res/control_icons/close_pressed_btn.png);}");

    ui->lbCurrTmp->setStyleSheet("QLabel{border:none;background:transparent;font-size:120px;color:rgba(255,255,255,1);line-height:80px;}");
    ui->lbCurrTmp->setText("12");
    ui->lbCurrTmpUnit->setStyleSheet("QLabel{border:none;background:transparent;font-size:20px;color:rgba(255,255,255,1);line-height:14px;}");
    ui->lbCurrTmpUnit->setText("℃");
    ui->lbCurrWea->setStyleSheet("QLabel{border:none;background:transparent;font-size:20px;color:rgba(255,255,255,1);line-height:14px;}");
    ui->lbCurrWea->setText("多云");
    ui->lbCurrHum->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,1);line-height:14px;}");
    ui->lbCurrHum->setAlignment(Qt::AlignCenter);
    ui->lbCurrHum->setText("湿度 98%   东南风 1级");


    m_scrollarea = new QScrollArea(ui->centralwidget);
    m_scrollarea->setFixedSize(858, 220);
    m_scrollarea->move(4, 290);
    m_scrollarea->setStyleSheet("QScrollArea{border:none;border-radius:4px;background:transparent;color:rgba(255,255,255,1);}");
    m_scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_scrollarea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 2px 0px 2px;width:10px;background:rgba(255,255,255,0);border-radius:6px;}"
                                                     "QScrollBar::up-arrow:vertical{height:0px;}"
                                                     "QScrollBar::sub-line:vertical{border:0px solid;height:0px}"
                                                     "QScrollBar::sub-page:vertical{background:transparent;}"
                                                     "QScrollBar::handle:vertical{width:6px;background:rgba(255,255,255,0.2);border-radius:3px;}"
                                                     "QScrollBar::handle:vertical:hover{width:6px;background:rgba(255,255,255,0.2);border-radius:3px;}"
                                                     "QScrollBar::handle:vertical:pressed{width:6px;background:rgba(255,255,255,0.2);border-radius:3px;}"
                                                     "QScrollBar::add-page:vertical{background:transparent;}"
                                                     "QScrollBar::add-line:vertical{border:0px solid;height:0px}"
                                                     "QScrollBar::down-arrow:vertical{height:0px;}");

    m_scrollwidget = new QWidget(m_scrollarea);
    m_scrollwidget->resize(858, 450);
    m_scrollwidget->setStyleSheet("QWidget{border:none;border-radius:4px;background:transparent;color:rgba(255,255,255,1);}");
    m_scrollarea->setWidget(m_scrollwidget);
    m_scrollwidget->move(0, 0);

    Information *m_information = new Information(m_scrollwidget);
    m_information->move(0,0);
}

void MainWindow::initData()
{
    totalNum = 5;
    redNum = 0;
    blueNum = 0;
    yellowNum = 0;

    m_model = new QStandardItemModel();

    for (int i = 0; i < totalNum; ++i) {
        QStandardItem *Item = new QStandardItem;

        ItemData itemData;

        itemData.name = QString("Name %1").arg(i);
        itemData.tel = QString("TEL:1331234567%1").arg(i);
        int randNum = rand()% 3;
        ItemStatus itemStatus;
        switch (randNum) {
        case 0:
            itemStatus = S_RED;
            redNum++;
            break;
        case 1:
            itemStatus = S_BLUE;
            blueNum++;
            break;
        case 2:
            itemStatus = S_YELLOW;
            yellowNum++;
            break;
        }
        Item->setData(itemStatus,Qt::UserRole);  // 单一存取
        Item->setData(QVariant::fromValue(itemData),Qt::UserRole+1);//整体存取

        m_model->appendRow(Item);      //追加Item
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
}

void MainWindow::on_btnMinimize_clicked()
{
    QWidget::showMinimized();
}

void MainWindow::on_btnCancel_clicked()
{
    QApplication* app;
    app->exit(0);
}
