#ifndef LEFTUPSEARCHBOX_H
#define LEFTUPSEARCHBOX_H

#include <QLineEdit>

class LeftUpSearchBox : public QLineEdit
{
    Q_OBJECT

public:
    explicit LeftUpSearchBox(QWidget* parent = 0);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QString m_searchText;
    QPixmap m_searchPixmap;
};

#endif // LEFTUPSEARCHBOX_H
