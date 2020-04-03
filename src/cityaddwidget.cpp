#include "cityaddwidget.h"
#include "ui_cityaddwidget.h"
#include "citycollectionwidget.h"
#include "data.h"

#include <QDebug>

CityAddition::CityAddition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cityaddition)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    this->setStyleSheet("QWidget{border:none;border-radius:6px;}");

    ui->backwidget->setStyleSheet("QWidget{border:1px solid rgba(255,255,255,0.05);border-radius:6px;background:rgba(255,255,255,1);}");

    ui->lbLeftUpIcon->setStyleSheet("QLabel{border:none;background:transparent;background-image:url(':/res/control_icons/logo.png');}");

    ui->lbLeftUpTitle->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbLeftUpTitle->setText("麒麟天气");

    ui->btnCancel->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(:/res/control_icons/close_black.png);}");

    m_cityaddsearchbox = new CityAddSearchBox(this);
    m_cityaddsearchbox->move(35, 93);
    m_cityaddsearchbox->show();

    m_cityaddsearchview = new CityAddSearchView(ui->backwidget);
    m_cityaddsearchdelegate = new CityAddSearchDelegate(m_cityaddsearchview);
    m_proxyModel = new QSortFilterProxyModel(m_cityaddsearchview);
    m_model = new QStandardItemModel();
    m_cityaddsearchview->move(35, 125);
    m_cityaddsearchview->resize(470,220);
    m_cityaddsearchview->hide();


    connect(m_cityaddsearchbox, &CityAddSearchBox::textChanged, this, [this] () {
        if (m_cityaddsearchbox->text().size() == 0){
            m_cityaddsearchview->hide();
        }else{
            m_cityaddsearchview->show();
            onSearchBoxEdited();
        }
    });

    m_locationWorker = new LocationWorker();
}

CityAddition::~CityAddition()
{
    delete ui;
}

void CityAddition::onSearchBoxEdited()
{
    searchCityName();

    m_cityaddsearchview->setItemDelegate(m_cityaddsearchdelegate);       //为视图设置委托
    m_cityaddsearchview->setSpacing(1);                   //为视图设置控件间距
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterRole(Qt::UserRole);
    m_proxyModel->setDynamicSortFilter(true);
    m_cityaddsearchview->setModel(m_proxyModel);                  //为委托设置模型
    m_cityaddsearchview->setViewMode(QListView::IconMode); //设置Item图标显示
    m_cityaddsearchview->setDragEnabled(false);            //控件不允许拖动
}

void CityAddition::searchCityName()
{
    const QString inputText = m_cityaddsearchbox->text().trimmed();
    if (inputText.isEmpty())
        return;

    QList<LocationData> searchResultList;
    searchResultList = m_locationWorker->exactMatchCity(inputText);

    if (searchResultList.isEmpty()) {
        qDebug()<<"fail to search city information";
    }
    else {
        delete m_model;
        m_model = new QStandardItemModel();

        foreach(LocationData m_locationdata, searchResultList){
            QStandardItem *Item = new QStandardItem;

            ItemData itemData;

            itemData.cityId = QString(m_locationdata.id);
            itemData.cityName = QString(m_locationdata.city);
            itemData.cityProvince = QString(m_locationdata.province);
            Item->setData(QVariant::fromValue(itemData),Qt::UserRole); //整体存取

            m_model->appendRow(Item); //追加Item
        }
    }
}

void CityAddition::on_btnCancel_clicked()
{
    this->hide();
    CityCollectionWidget *m_cityaddition = new CityCollectionWidget();
    m_cityaddition->show();
}
