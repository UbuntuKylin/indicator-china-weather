#include "citycollectionwidget.h"
#include "ui_citycollectionwidget.h"
#include "cityaddwidget.h"
#include "citycollectionitem.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QEventLoop>
#include <QFile>
#include <QApplication>
#include <QStringList>
#include <QUrl>
#include <QVariant>

CityCollectionWidget::CityCollectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::citycollectionwidget)
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
    this->setWindowIcon(QIcon::fromTheme("indicator-china-weather", QIcon(":/res/control_icons/indicator-china-weather.png")) );

    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    this->move((availableGeometry.width()-this->width())/2, (availableGeometry.height()-this->height())/2);

    ui->backwidget->setStyleSheet("QWidget{border:1px solid rgba(255,255,255,0.05);border-radius:6px;background:rgba(255,255,255,1);}");

    ui->lbLeftUpIcon->setStyleSheet("QLabel{border:none;background:transparent;background-image:url(':/res/control_icons/logo.png');}");

    ui->lbLeftUpTitle->setStyleSheet("QLabel{border:none;background:transparent;font-size:14px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbLeftUpTitle->setText("麒麟天气");

    ui->btnCancel->setStyleSheet("QPushButton{border:0px;background:transparent;background-image:url(:/res/control_icons/close_black.png);}");

    ui->lbCityCurrent->setStyleSheet("QLabel{border:none;background:transparent;font-size:18px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbCityCurrent->setText("当前城市");

    ui->lbCityCollect->setStyleSheet("QLabel{border:none;background:transparent;font-size:18px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbCityCollect->setText("收藏城市");

    ui->lbCityCount->setStyleSheet("QLabel{border:none;background:transparent;font-size:12px;font-weight:400;color:rgba(68,68,68,1);}");
    ui->lbCityCount->setText("1/8");

    m_cityaddition = new CityAddition(this);
    m_cityaddition->move(0, 0);
    m_cityaddition->hide();
    connect(m_cityaddition, SIGNAL(hideCityAddWiget()), this, SLOT(onHideCityAddWiget()) );
    connect(m_cityaddition, SIGNAL(requestAddNewCity(QString)), this, SLOT(onRequestAddNewCity(QString)) );

    m_networkManager = new QNetworkAccessManager(this);

    setCurrentCity(); //显示当前城市
    setCollectCity(); //显示收藏城市
}

CityCollectionWidget::~CityCollectionWidget()
{
    delete ui;
}

void CityCollectionWidget::setCurrentCity()
{
    QString strCurrCity = readCollectedCity();
    QStringList listCityId = strCurrCity.split(",");
    if (listCityId.size() >= 1){
        qDebug()<<"get current city string is: "<<listCityId.at(0);
    } else {
        QString cityId = "101010100"; //使用北京的ID
    }

    citycollectionitem *m_currentcity = new citycollectionitem(this);
    m_currentcity->move(35, 81);
    m_currentcity->setItemWidgetState(true);
    m_currentcity->show();
}

void CityCollectionWidget::setCollectCity()
{
    QString strSavedCity = readCollectedCity();
    qDebug()<<"get all collected city string is: "<<strSavedCity;

    QStringList listSavedCityId = strSavedCity.split(",");
    int cityNumber = listSavedCityId.size() - 1; //减掉一个含空字符的项

    int row = 0; //当前行
    int column = 0; //当前列
    if (cityNumber > 1){
        //先不考虑当前城市这个item
        for (int i=0;i<cityNumber-1;i++){
            showCollectCity(35 + column*170, 242 + row*100, true);
            column += 1;
            if (column == 3){
                column = 0;
                row += 1;
            }
        }
    }
    showCollectCity(35 + column*170, 242 + row*100, false);
}

void CityCollectionWidget::showCollectCity(int x, int y, bool isShowNormal)
{
    citycollectionitem *m_currentcity = new citycollectionitem(this);
    m_currentcity->move(x, y); //m_currentcity->move(35 + j*170, 242 + i*100);
    m_currentcity->setItemWidgetState(isShowNormal);
    m_currentcity->show();
    connect(m_currentcity, SIGNAL(showCityAddWiget()), this, SLOT(onShowCityAddWiget()) );
}

void CityCollectionWidget::onRequestAddNewCity(QString cityId)
{
    m_cityaddition->hide();

    QString strSavedCity = readCollectedCity();
    QStringList listSavedCityId = strSavedCity.split(",");
    if (listSavedCityId.size() == 10){
        listSavedCityId.replace(8, cityId);
    }else {
        listSavedCityId.append(cityId);
    }

    QString newStrCityId = "";
    foreach(QString str, listSavedCityId){
        if (str != ""){
            newStrCityId.append(str);
            newStrCityId.append(",");
        }
    }

    newStrCityId.trimmed();
    qDebug()<<"111111111111111"<<newStrCityId;
    writeCollectedCity(newStrCityId);
    setCollectCity();
}

void CityCollectionWidget::writeCollectedCity(QString cityId)
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString collectPath = homePath.at(0) + "/.config/china-weather-data";

    QFile file(collectPath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(cityId.toUtf8());
    file.close();
}

QString CityCollectionWidget::readCollectedCity()
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString collectPath = homePath.at(0) + "/.config/china-weather-data";

    QFile file(collectPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray cityId = file.readAll();
    QString readCityId = (QString(cityId));
    file.close();

    return readCityId;
}

void CityCollectionWidget::onWeatherDataRequest(const QString &cityId)
{
    qDebug()<<"cityId: "<<cityId;
    if (cityId.isEmpty()) {
        return;
    }

    QString forecastUrl = QString("http://service.ubuntukylin.com:8001/weather/api/3.0/heweather_data_s6/%1/").arg(cityId);
    QNetworkRequest request;
    request.setUrl(forecastUrl);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &CityCollectionWidget::onWeatherDataReply );
}

void CityCollectionWidget::onWeatherDataReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<"value of statusCode is: "<<statusCode;

    if(reply->error() != QNetworkReply::NoError) {
        qDebug() << "reply error!";
        return;
    }

    QByteArray ba = reply->readAll();
    //QString reply_content = QString::fromUtf8(ba);
    //qDebug() <<reply_content;
    reply->close();
    reply->deleteLater();

    QJsonParseError err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &err);
    if (err.error != QJsonParseError::NoError) {// Json type error
        qDebug() << "Json type error";
        return;
    }
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        qDebug() << "Json null or empty!";
        return;
    }

    QJsonObject jsonObject = jsonDocument.object();
    if (jsonObject.isEmpty() || jsonObject.size() == 0) {
        qDebug() << "Json object null or empty!";
        return;
    }
    if (jsonObject.contains("KylinWeather")) {
        QJsonObject mainObj = jsonObject.value("KylinWeather").toObject();
        if (mainObj.isEmpty() || mainObj.size() == 0) {
            return;
        }
        if (mainObj.contains("weather")) {
            QJsonObject weatherObj = mainObj.value("weather").toObject();
            if (!weatherObj.isEmpty() && weatherObj.size() > 0) {
                //处理实时天气
                QString now_msg = weatherObj.value("now").toString();
                if (now_msg != ""){
                    QStringList strList = now_msg.split(",");
                    QStringList strListSub;
                    foreach(QString str, strList){
                        if (str != ""){
                            strListSub.append(str.split("=").at(1));
                        }
                    }

                    ObserveWeather m_observeweather;
                    m_observeweather.tmp = strListSub.at(0);
                    m_observeweather.wind_sc = strListSub.at(1);
                    m_observeweather.cond_txt = strListSub.at(2);
                    m_observeweather.vis = strListSub.at(3);
                    m_observeweather.hum = strListSub.at(4);
                    m_observeweather.cond_code = strListSub.at(5);
                    m_observeweather.wind_deg = strListSub.at(6);
                    m_observeweather.pcpn = strListSub.at(7);
                    m_observeweather.pres = strListSub.at(8);
                    m_observeweather.wind_spd = strListSub.at(9);
                    m_observeweather.wind_dir = strListSub.at(10);
                    m_observeweather.fl = strListSub.at(11);
                    m_observeweather.cloud = strListSub.at(12);

                    emit this->needSetObserveWeather(m_observeweather);//用于设置主窗口

                }
            }
        }
    }
}

void CityCollectionWidget::onShowCityAddWiget()
{
    m_cityaddition->show();
    m_cityaddition->raise();
}

void CityCollectionWidget::onHideCityAddWiget()
{
    m_cityaddition->hide();
}

void CityCollectionWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
}

void CityCollectionWidget::mouseReleaseEvent(QMouseEvent *event){
    this->isPress = false;
}

void CityCollectionWidget::mouseMoveEvent(QMouseEvent *event){
    if(this->isPress){
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
}

void CityCollectionWidget::on_btnCancel_clicked()
{
    this->hide();
}
