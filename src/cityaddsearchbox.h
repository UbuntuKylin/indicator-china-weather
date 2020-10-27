#ifndef CITYADDSEARCHBOX_H
#define CITYADDSEARCHBOX_H

#include <QLineEdit>

class CityAddSearchBox : public QLineEdit
{
    Q_OBJECT

public:
    explicit CityAddSearchBox(QWidget* parent = 0);
    void ThemeCitySearchBox(QString str);

protected:
    // set the display style of the searchbox.
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
private:
    QString m_searchText;
    QPixmap m_searchPixmap;
    QString m_ThemeStyle;

    void drawbackground();
};

#endif // CITYADDSEARCHBOX_H
