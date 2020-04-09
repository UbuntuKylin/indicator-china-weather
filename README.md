# indicator-china-weather

The weather data are from the heweather API s6 version.

![](./doc/weather_zh_CN.png)

### Author's Home Page
 
[Eight Plus &rarr;](https://eightplus.github.io/)



### v1.0 test url

+ http://service.ubuntukylin.com:8001/weather/api/1.0/observe/101250101/

+ http://service.ubuntukylin.com:8001/weather/api/1.0/heweather_forecast/101250101/

+ http://service.ubuntukylin.com:8001/weather/pingnetwork/

+ http://service.ubuntukylin.com:8001/weather/pinginformation/


### v2.0 test url

+ http://service.ubuntukylin.com:8001/weather/api/2.0/heweather_observe_s6/101250101

+ http://service.ubuntukylin.com:8001/weather/api/2.0/heweather_forecast_s6/101250101


### Internationalization

1. lupdate indicator-china-weather.pro
2. linguist translation/indicator-china-weather_zh_CN.ts
3. lrelease indicator-china-weather.pro


### Lintian

lintian -i -EvIL +pedantic --verbose ../indicator-china-weather_3.0.0_amd64.changes
