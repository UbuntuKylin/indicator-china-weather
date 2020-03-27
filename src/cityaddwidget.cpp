#include "cityaddwidget.h"
#include "ui_cityaddwidget.h"
#include "citycollectionwidget.h"
#include "dataitem.h"

CityAddition::CityAddition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cityaddition)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QWidget{border:1px solid rgba(255,255,255,0.05);border-radius:6px;background:rgba(255,255,255,0.1);}");

    m_cityaddsearchbox = new CityAddSearchBox(this);
    m_cityaddsearchbox->move(35, 93);
    m_cityaddsearchbox->show();

    initData();
    m_cityaddsearchview = new CityAddSearchView(this);
    m_cityaddsearchdelegate = new CityAddSearchDelegate(m_cityaddsearchview);
    m_proxyModel = new QSortFilterProxyModel(m_cityaddsearchview);
    m_cityaddsearchview->move(35, 125);
    m_cityaddsearchview->resize(470,220);
    m_cityaddsearchview->hide();
    m_cityaddsearchview->setItemDelegate(m_cityaddsearchdelegate);       //为视图设置委托
    m_cityaddsearchview->setSpacing(1);                   //为视图设置控件间距
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterRole(Qt::UserRole);
    m_proxyModel->setDynamicSortFilter(true);
    m_cityaddsearchview->setModel(m_proxyModel);                  //为委托设置模型
    m_cityaddsearchview->setViewMode(QListView::IconMode); //设置Item图标显示
    m_cityaddsearchview->setDragEnabled(false);            //控件不允许拖动


    connect(m_cityaddsearchbox, &CityAddSearchBox::textEdited, this, [this] () {
        if (m_cityaddsearchbox->text().size() == 0){
            m_cityaddsearchview->hide();
        }else{
            m_cityaddsearchview->show();
        }
    });
}

CityAddition::~CityAddition()
{
    delete ui;
}

void CityAddition::initData()
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

void CityAddition::on_btnCancel_clicked()
{
    this->hide();
    CityCollectionWidget *m_cityaddition = new CityCollectionWidget();
    m_cityaddition->show();
}
