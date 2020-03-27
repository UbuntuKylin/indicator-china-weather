#include "informationwidget.h"
#include "ui_informationwidget.h"

Information::Information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{border:none;border-radius:4px;background:transparent;}");

    this->editQssString();
    this->initControlQss();
}

Information::~Information()
{
    delete ui;
}


void Information::editQssString()
{
    lbDayQss = "QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,1);line-height:20px;}";
    lbDateQss = "QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,0.5);line-height:20px;}";
    lbIconQss = "QLabel{border:none;background:transparent;}";
    lbTmpQss = "QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,1);line-height:20px;}";
    lbWeaQss = "QLabel{border:none;background:transparent;font-size:14px;color:rgba(255,255,255,1);line-height:20px;}";

    lbLifeStyleQss = "QLabel{border:none;background:transparent;font-size:18px;color:rgba(255,255,255,1);line-height:36px;}";

    lbIndexIconQss = "QLabel{border:none;background:transparent;}";
    lbIndexQss = "QLabel{border:none;background:transparent;font-size:12px;color:rgba(255,255,255,0.5);line-height:18px;}";
    lbIndexBrfQss = "QLabel{border:none;background:transparent;font-size:16px;color:rgba(255,255,255,1);line-height:18px;}";

    lineQss = "border:none;background-color:rgba(255,255,255,0.1);";
    lineLinearGradientQss = "border:none;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 255, 0), stop:0.5 rgba(255, 255, 255, 0.7), stop:1 rgba(255, 255, 255, 0));";
}

void Information::initControlQss()
{
    ui->lbDay_1->setStyleSheet(lbDayQss);
    ui->lbDay_1->setText("星期一");
    ui->lbDay_2->setStyleSheet(lbDayQss);
    ui->lbDay_2->setText("星期一");
    ui->lbDay_3->setStyleSheet(lbDayQss);
    ui->lbDay_3->setText("星期一");
    ui->lbDay_4->setStyleSheet(lbDayQss);
    ui->lbDay_4->setText("星期一");
    ui->lbDay_5->setStyleSheet(lbDayQss);
    ui->lbDay_5->setText("星期一");
    ui->lbDay_6->setStyleSheet(lbDayQss);
    ui->lbDay_6->setText("星期一");
    ui->lbDay_7->setStyleSheet(lbDayQss);
    ui->lbDay_7->setText("星期一");

    ui->lbDate_1->setStyleSheet(lbDateQss);
    ui->lbDate_1->setText("3月26日");
    ui->lbDate_2->setStyleSheet(lbDateQss);
    ui->lbDate_2->setText("3月26日");
    ui->lbDate_3->setStyleSheet(lbDateQss);
    ui->lbDate_3->setText("3月26日");
    ui->lbDate_4->setStyleSheet(lbDateQss);
    ui->lbDate_4->setText("3月26日");
    ui->lbDate_5->setStyleSheet(lbDateQss);
    ui->lbDate_5->setText("3月26日");
    ui->lbDate_6->setStyleSheet(lbDateQss);
    ui->lbDate_6->setText("3月26日");
    ui->lbDate_7->setStyleSheet(lbDateQss);
    ui->lbDate_7->setText("3月26日");

    ui->lbIcon_1->setStyleSheet(lbIconQss);
    ui->lbIcon_1->setStyleSheet("QLabel{background-image:url(:/res/control_icons/weather-clear.png);}");
    ui->lbIcon_2->setStyleSheet(lbIconQss);
    ui->lbIcon_2->setStyleSheet("QLabel{background-image:url(:/res/control_icons/weather-clear.png);}");
    ui->lbIcon_3->setStyleSheet(lbIconQss);
    ui->lbIcon_3->setStyleSheet("QLabel{background-image:url(:/res/control_icons/weather-clear.png);}");
    ui->lbIcon_4->setStyleSheet(lbIconQss);
    ui->lbIcon_4->setStyleSheet("QLabel{background-image:url(:/res/control_icons/weather-clear.png);}");
    ui->lbIcon_5->setStyleSheet(lbIconQss);
    ui->lbIcon_5->setStyleSheet("QLabel{background-image:url(:/res/control_icons/weather-clear.png);}");
    ui->lbIcon_6->setStyleSheet(lbIconQss);
    ui->lbIcon_6->setStyleSheet("QLabel{background-image:url(:/res/control_icons/weather-clear.png);}");
    ui->lbIcon_7->setStyleSheet(lbIconQss);
    ui->lbIcon_7->setStyleSheet("QLabel{background-image:url(:/res/control_icons/weather-clear.png);}");

    ui->lbTmp_1->setStyleSheet(lbTmpQss);
    ui->lbTmp_1->setText("13℃~23℃");
    ui->lbTmp_2->setStyleSheet(lbTmpQss);
    ui->lbTmp_2->setText("13℃~23℃");
    ui->lbTmp_3->setStyleSheet(lbTmpQss);
    ui->lbTmp_3->setText("13℃~23℃");
    ui->lbTmp_4->setStyleSheet(lbTmpQss);
    ui->lbTmp_4->setText("13℃~23℃");
    ui->lbTmp_5->setStyleSheet(lbTmpQss);
    ui->lbTmp_5->setText("13℃~23℃");
    ui->lbTmp_6->setStyleSheet(lbTmpQss);
    ui->lbTmp_6->setText("13℃~23℃");
    ui->lbTmp_7->setStyleSheet(lbTmpQss);
    ui->lbTmp_7->setText("13℃~23℃");


    ui->lbWea_1->setStyleSheet(lbWeaQss);
    ui->lbWea_1->setText("阵雨转中雨\n东南风微风");
    ui->lbWea_2->setStyleSheet(lbWeaQss);
    ui->lbWea_2->setText("阵雨转中雨\n东南风微风");
    ui->lbWea_3->setStyleSheet(lbWeaQss);
    ui->lbWea_3->setText("阵雨转中雨\n东南风微风");
    ui->lbWea_4->setStyleSheet(lbWeaQss);
    ui->lbWea_4->setText("阵雨转中雨\n东南风微风");
    ui->lbWea_5->setStyleSheet(lbWeaQss);
    ui->lbWea_5->setText("阵雨转中雨\n东南风微风");
    ui->lbWea_6->setStyleSheet(lbWeaQss);
    ui->lbWea_6->setText("阵雨转中雨\n东南风微风");
    ui->lbWea_7->setStyleSheet(lbWeaQss);
    ui->lbWea_7->setText("阵雨转中雨\n东南风微风");

    ui->line_1->setMaximumWidth(1);
    ui->line_2->show();
    ui->line_3->show();
    ui->line_4->show();
    ui->line_5->show();
    ui->line_6->show();
    ui->line_1->setStyleSheet(lineLinearGradientQss);
    ui->line_2->setStyleSheet(lineLinearGradientQss);
    ui->line_3->setStyleSheet(lineLinearGradientQss);
    ui->line_4->setStyleSheet(lineLinearGradientQss);
    ui->line_5->setStyleSheet(lineLinearGradientQss);
    ui->line_6->setStyleSheet(lineLinearGradientQss);
    ui->line_1->show();
    ui->line_2->show();
    ui->line_3->show();
    ui->line_4->show();
    ui->line_5->show();
    ui->line_6->show();



    ui->lbLifeStyle->setStyleSheet(lbLifeStyleQss);
    ui->lbLifeStyle->setText("生活指数");

    ui->lb_drsg->setStyleSheet(lbIndexQss);
    ui->lb_drsg->setText("穿衣指数");
    ui->lb_drsg_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_drsg_brf->setText("温凉");
    ui->lb_drsg_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_drsg_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_clothe.png);}");

    ui->lb_flu->setStyleSheet(lbIndexQss);
    ui->lb_flu->setText("感冒指数");
    ui->lb_flu_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_flu_brf->setText("温凉");
    ui->lb_flu_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_flu_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_flu.png);}");

    ui->lb_uv->setStyleSheet(lbIndexQss);
    ui->lb_uv->setText("紫外线指数");
    ui->lb_uv_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_uv_brf->setText("温凉");
    ui->lb_uv_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_uv_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_uv.png);}");

    ui->lb_cw->setStyleSheet(lbIndexQss);
    ui->lb_cw->setText("洗车指数");
    ui->lb_cw_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_cw_brf->setText("温凉");
    ui->lb_cw_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_cw_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_cash_wash.png);}");

    ui->lb_air->setStyleSheet(lbIndexQss);
    ui->lb_air->setText("空气指数");
    ui->lb_air_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_air_brf->setText("温凉");
    ui->lb_air_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_air_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_air.png);}");

    ui->lb_sport->setStyleSheet(lbIndexQss);
    ui->lb_sport->setText("运动指数");
    ui->lb_sport_brf->setStyleSheet(lbIndexBrfQss);
    ui->lb_sport_brf->setText("温凉");
    ui->lb_sport_icon->setStyleSheet(lbIndexIconQss);
    ui->lb_sport_icon->setStyleSheet("QLabel{background-image:url(:/res/control_icons/index_sport.png);}");

    ui->lineH->setStyleSheet(lineQss);
    ui->lineV_1->setStyleSheet(lineQss);
    ui->lineV_2->setStyleSheet(lineQss);
    ui->lineV_3->setStyleSheet(lineQss);
    ui->lineH->show();
    ui->lineV_1->show();
    ui->lineV_2->show();
    ui->lineV_3->show();

}
