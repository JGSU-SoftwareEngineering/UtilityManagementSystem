#include "repairManagement.h"
#include "ui_repairManagement.h"

repairManagement::repairManagement(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::repairManagement)
{
    initalWidget();
}

repairManagement::~repairManagement()
{
    delete ui;
}

void repairManagement::setCurrentIndex(int i)
{
}

bool repairManagement::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void repairManagement::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    
}
