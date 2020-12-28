#include "addcityaction.h"


AddCityAction::AddCityAction(QWidget *parent) : QAction(parent)
{
    emit cityNameLabelSignal("N/A");//需要//此处为加载时N/A
    connect(this,&AddCityAction::triggered,this,&AddCityAction::addCityClick);
//    connect(this, &AddCityAction::requestSetCityName, this, [=] (QString cityName) {
//        emit cityNameLabelSignal(cityName);
//        qDebug()<<"城市名称:"<<cityName;
//    });
}
void AddCityAction::addCityClick()
{
    qDebug()<<"点击了我!";
    if (!is_open_city_collect_widget) {
        m_citycollectionwidget = new CityCollectionWidget();
        //接收来自收藏城市窗口发来的信号，再发送一个信号到主窗口
        connect(m_citycollectionwidget, &CityCollectionWidget::sendCurrentCityId, this, &AddCityAction::sendCurrentCityId);

        //给收藏城市窗口发送信号进行更新
        connect(this,&AddCityAction::updatecity,m_citycollectionwidget,&CityCollectionWidget::updatecity);

        //requestShowCollCityWeather信号会一直传到weathermanager.cpp，用于获取收藏列表中城市的实时天气
        connect(m_citycollectionwidget, &CityCollectionWidget::requestShowCollCityWeather, this, &AddCityAction::requestShowCollCityWeather);

        //接收来自主窗口的天气数据，再发送到收藏城市列表窗口
        connect(this, SIGNAL(requestSetCityWeather(QString)), m_citycollectionwidget, SLOT(onRequestSetCityWeather(QString)));

        //收到关闭窗口的消息
        connect(m_citycollectionwidget, &CityCollectionWidget::requestChangeWidgetState, this, [=] () {
            is_open_city_collect_widget = false;
            m_citycollectionwidget->deleteLater(); //销毁窗口
        });
        emit requestShowCollCityWeather(); //发送该信号，显示收藏城市窗口各城市的事实天气
        is_open_city_collect_widget = true; //为真时无法打开新的收藏窗口

        //! \brief 模态对话框会阻塞，发信号和设置is_open_city_collect_widget都要在exec之前
        m_citycollectionwidget->exec();
    }
    //! \note CityCollectionWidget打开后会阻塞主窗口，是不是可以把这个判断删掉了
    else{
        if(nullptr != m_citycollectionwidget){
            m_citycollectionwidget->show();
            m_citycollectionwidget->activateWindow();
            return;
        }
    }
}
