#include "citycollectionitem.h"
#include "ui_citycollectionitem.h"

citycollectionitem::citycollectionitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::citycollectionitem)
{
    ui->setupUi(this);
}

citycollectionitem::~citycollectionitem()
{
    delete ui;
}
