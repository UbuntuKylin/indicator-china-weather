#ifndef INFORMATION_H
#define INFORMATION_H

#include <QWidget>

namespace Ui {
class information;
}

class Information : public QWidget
{
    Q_OBJECT

public:
    explicit Information(QWidget *parent = nullptr);
    ~Information();

private:
    Ui::information *ui;

    void editQssString();
    void initControlQss();

    QString lbDayQss, lbDateQss, lbIconQss, lbTmpQss, lbWeaQss;
    QString lbLifeStyleQss;
    QString lbIndexIconQss, lbIndexQss, lbIndexBrfQss, lineQss, lineLinearGradientQss;
};

#endif // INFORMATION_H
