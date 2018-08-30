#include "forecastitemwidget.h"
#include "tooltip.h"

#include <QDebug>
#include <QEvent>
#include <QCursor>

ForecastItemWidget::ForecastItemWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(100, 160);
    this->setStyleSheet("QWidget{border-radius: 0px;color:rgb(250,250,250);background-color:rgba(0,0,0,0.2)}");
//    setAttribute(Qt::WA_TransparentForMouseEvents);

    m_weekLabel = new QLabel(this);
    m_dateLabel = new QLabel(this);
    m_weatherLabel = new QLabel(this);
    m_iconLabel = new QLabel(this);
    m_tempLabel = new QLabel(this);

    m_weekLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_dateLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_weatherLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_iconLabel->setStyleSheet("QLabel{border:none;background-color:transparent;}");
    m_tempLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");

    m_iconLabel->setFixedSize(48, 48);

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(5);
    m_layout->addWidget(m_weekLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    m_layout->addWidget(m_dateLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_weatherLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_iconLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_tempLabel, 0, Qt::AlignBottom | Qt::AlignHCenter);

    m_toolTip = new ToolTip();

    this->setDefaultData();
}

void ForecastItemWidget::resetForecastData()
{

}

void ForecastItemWidget::setTextData()
{
    m_weekLabel->setText("星期五");
}

void ForecastItemWidget::setDefaultData()
{
    m_weekLabel->setText("今天");
    m_dateLabel->setText("20180821");
    m_weatherLabel->setText("晴");
    QPixmap pixmap = QPixmap(":/res/weather_icons/lightgrey/100.png");
    pixmap = pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_iconLabel->setPixmap(pixmap);
    m_tempLabel->setText("28°C-36°C");
}

void ForecastItemWidget::setLabelText(const QString &name, const QString &desc)
{
//    QFont ft;
//    QFontMetrics fm(ft);
//    QString elided_text = fm.elidedText(name, Qt::ElideRight, 260);
//    name_label->setText(elided_text);
//    elided_text = fm.elidedText(desc, Qt::ElideRight, 260);
//    description_label->setText(elided_text);
//    if(elided_text.endsWith("…"))
//        description_label->setToolTip(desc);
}

bool ForecastItemWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        m_toolTip->resetData();
        m_toolTip->popupTip(QCursor::pos());
    }
    else if (event->type() == QEvent::Leave) {
        m_toolTip->hide();
    }

    return QWidget::event(event);
}
