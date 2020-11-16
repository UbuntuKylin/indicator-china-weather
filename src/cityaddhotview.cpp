#include "cityaddhotview.h"
#include "citycollectionwidget.h"


CityAddHotView::CityAddHotView(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(504, 130);
    this->setMouseTracking(true);
    m_addCityhot = new QLabel(this);
    m_addCityhot->setText("热门城市");
    m_addCityhot->setStyleSheet("QLabel{color:#444444;font-size:14px;}");
    m_addCityhot->move(0,0);

    m_addcity11 = new HotCity(this,"北京","101010100");
    connect(m_addcity11,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity11->move(0,36);

    m_addcity12 = new HotCity(this,"上海","101020100");
    connect(m_addcity12,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity12->move(68,36);

    m_addcity13 = new HotCity(this,"天津","101030100");
    connect(m_addcity13,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity13->move(136,36);

    m_addcity14 = new HotCity(this,"重庆","101040100");
    connect(m_addcity14,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity14->move(204,36);

    m_addcity15 = new HotCity(this,"大连","101070201");
    connect(m_addcity15,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity15->move(272,36);

    m_addcity16 = new HotCity(this,"青岛","101120201");
    connect(m_addcity16,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity16->move(340,36);

    m_addcity17 = new HotCity(this,"西安","101110101");
    connect(m_addcity17,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity17->move(408,36);

    m_addcity18 = new HotCity(this,"南京","101190101");
    connect(m_addcity18,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity18->move(474,36);

    m_addcity21 = new HotCity(this,"苏州","101190401");
    connect(m_addcity21,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity21->move(0,72);

    m_addcity22 = new HotCity(this,"杭州","101210101");
    connect(m_addcity22,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity22->move(68,72);

    m_addcity23 = new HotCity(this,"厦门","101230201");
    connect(m_addcity23,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity23->move(136,72);

    m_addcity24 = new HotCity(this,"成都","101270101");
    connect(m_addcity24,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity24->move(204,72);

    m_addcity25 = new HotCity(this,"深圳","101280601");
    connect(m_addcity25,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity25->move(272,72);

    m_addcity26 = new HotCity(this,"广州","101280101");
    connect(m_addcity26,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity26->move(340,72);

    m_addcity27 = new HotCity(this,"三亚","101310201");
    connect(m_addcity27,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity27->move(408,72);

    m_addcity28 = new HotCity(this,"台北","101340101");
    connect(m_addcity28,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity28->move(474,72);

    m_addcity31 = new HotCity(this,"香港","101320101");
    connect(m_addcity31,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity31->move(0,108);

    m_addcity32 = new HotCity(this,"济南","101120101");
    connect(m_addcity32,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity32->move(68,108);

    m_addcity33 = new HotCity(this,"宁波","101210401");
    connect(m_addcity33,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity33->move(136,108);

    m_addcity34 = new HotCity(this,"沈阳","101070101");
    connect(m_addcity34,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity34->move(204,108);

    m_addcity35 = new HotCity(this,"武汉","101200101");
    connect(m_addcity35,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity35->move(272,108);

    m_addcity36 = new HotCity(this,"郑州","101180101");
    connect(m_addcity36,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity36->move(340,108);

    m_addcity37 = new HotCity(this,"长沙","101250101");
    connect(m_addcity37,&HotCity::hotCityClick,this,&CityAddHotView::setHotCity);
    m_addcity37->move(408,108);

//    m_citylayout = new QHBoxLayout(this);

}
//主题适配
void CityAddHotView::ThemeCityHotView(QString str)
{
    m_addcity11->onThemeChanged(str);
    m_addcity12->onThemeChanged(str);
    m_addcity13->onThemeChanged(str);
    m_addcity14->onThemeChanged(str);
    m_addcity15->onThemeChanged(str);
    m_addcity16->onThemeChanged(str);
    m_addcity17->onThemeChanged(str);
    m_addcity18->onThemeChanged(str);

    m_addcity21->onThemeChanged(str);
    m_addcity22->onThemeChanged(str);
    m_addcity23->onThemeChanged(str);
    m_addcity24->onThemeChanged(str);
    m_addcity25->onThemeChanged(str);
    m_addcity26->onThemeChanged(str);
    m_addcity27->onThemeChanged(str);
    m_addcity28->onThemeChanged(str);

    m_addcity31->onThemeChanged(str);
    m_addcity32->onThemeChanged(str);
    m_addcity33->onThemeChanged(str);
    m_addcity34->onThemeChanged(str);
    m_addcity35->onThemeChanged(str);
    m_addcity36->onThemeChanged(str);
    m_addcity37->onThemeChanged(str);

    if("ukui-dark" == str || "ukui-black" == str)
    {
       m_addCityhot->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(255,255,255,1);}");
}
    else if("ukui-default" == str || "ukui-white" == str || "ukui-light" == str)
    {
       m_addCityhot->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(68,68,68,1);}");
    }
}
