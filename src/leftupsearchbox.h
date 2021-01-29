#ifndef LEFTUPSEARCHBOX_H
#define LEFTUPSEARCHBOX_H

#include <QLineEdit>
#include <QMouseEvent>

class LeftUpSearchBox : public QLineEdit
{
    Q_OBJECT

public:
    explicit LeftUpSearchBox(QWidget* parent = 0);
    void ThemeLeftUpSearchBox(QString str);
signals:
    void lineEditKeyEvent(QString str);
protected:
    // set the display style of the searchbox.
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    QString m_searchText;
    QPixmap m_searchPixmap;
    bool isFocus;
};

#endif // LEFTUPSEARCHBOX_H
