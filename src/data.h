#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QMetaType>

struct ItemData
{
    QString cityId;
    QString cityName;
    QString cityProvince;
};

struct City
{
    QString id;
    QString name;
};


struct AqiAir
{
    QString id;
    QString aqi;
    QString qlty;
    QString main;
    QString pm25;
    QString pm10;
    QString no2;
    QString so2;
    QString co;
    QString o3;
};

struct ObserveWeather
{
    QString id;
    QString city;
    QString updatetime;
    QString air;//空气质量
    QString cloud;//云量
    QString cond_code;//实况天气状况代码 	100
    QString cond_txt;//实况天气状况描述 晴
    QString fl;//体感温度，默认单位：摄氏度 	23
    QString hum;//相对湿度 	40
    QString pcpn;//降水量(毫米 mm) 	0
    QString pres;//大气压强(百帕 hPa) 	1020
    QString tmp;//温度，默认单位：摄氏度℃ 	21
    QString vis;//能见度，默认单位：公里 km 	10
    QString wind_deg;//风向360角度 	305
    QString wind_dir;//风向 	西北
    QString wind_sc;//风力 	3 (0:静风  1:1级风)
    QString wind_spd;//风速，公里/小时 km/h 	15
    QString serveTime;//天气信息的更新时间
};

struct ForecastWeather
{
    QString cond_code_d;//白天天气状况代码 	100
    QString cond_code_n;//晚间天气状况代码 	100
    QString cond_txt_d;//白天天气状况描述 	晴
    QString cond_txt_n;//晚间天气状况描述 	晴
    QString forcast_date;//预报日期 	2013-12-30
    QString hum;//相对湿度 	37
    //QString mr_ms;//月升时间 	04:47   月落时间 	14:59
    QString mr;//月升时间 	04:47
    QString ms;//月落时间 	14:59
    QString pcpn;//降水量 	0
    QString pop;//降水概率 	0
    QString pres;//大气压强 	1018
    //QString sr_ss;//日出时间 	07:36   日落时间 	16:58
    QString sr;//日出时间 	07:36   日落时间 	16:58
    QString ss;//日出时间 	07:36   日落时间 	16:58
    QString tmp_max;//最高温度 	4
    QString tmp_min;//最低温度 	-5
    QString uv_index;//紫外线强度指数 	3
    QString vis;//能见度，单位：公里 	10
    QString wind_deg;//风向360角度 	310
    QString wind_dir;//风向 	西北风
    QString wind_sc;//风力 	1-2 (级风)
    QString wind_spd;//风速，公里/小时 	14
    QString date;//日期
    QString dateTime;//当前城市信息更新日期
};

struct LifeStyle
{
    QString air_brf;
    QString air_txt;
    QString comf_brf;
    QString comf_txt;
    QString cw_brf;
    QString cw_txt;
    QString drsg_brf;
    QString drsg_txt;
    QString flu_brf;
    QString flu_txt;
    QString sport_brf;
    QString sport_txt;
    QString trav_brf;
    QString trav_txt;
    QString uv_brf;
    QString uv_txt;
};

struct LocationData {
    QString id;
    QString city_en;
    QString city;
    QString country_en;
    QString country;
    QString province_en;
    QString province;
    QString admin_district_en;
    QString admin_district;
    QString shorthand;
};

class CitySettingData
{
public:
    bool active;

    QString id;
    QString name;
    QString temperature;
    QString icon;

public:
    void setActive(bool b) { active = b; }
};

inline bool operator ==(const City &data, const City &other) {
    return data.id == other.id;
}

inline bool operator ==(const CitySettingData &data, const CitySettingData &other) {
    return data.id == other.id;
}

inline bool operator ==(const LocationData &data, const LocationData &that)
{
    return data.id == that.id || (data.admin_district == that.admin_district && data.city == that.city);
}

Q_DECLARE_METATYPE(ItemData)
Q_DECLARE_METATYPE(City)
Q_DECLARE_METATYPE(ObserveWeather)
Q_DECLARE_METATYPE(ForecastWeather)
Q_DECLARE_METATYPE(LifeStyle)
Q_DECLARE_METATYPE(LocationData)
Q_DECLARE_METATYPE(CitySettingData)

#endif // DATA_H
