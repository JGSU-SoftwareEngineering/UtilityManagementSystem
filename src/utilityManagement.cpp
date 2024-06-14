#include "utilityManagement.h"
#include "ui_utilityManagement.h"

#include "database.hpp"

#include <QMessageBox>

utilityManagement::utilityManagement(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::utilityManagement)
    , isStudent(false)
{
    initalWidget();
}

utilityManagement::~utilityManagement()
{
    delete ui;
}

void utilityManagement::setCurrentIndex(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

bool utilityManagement::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void utilityManagement::updateAllocation()
{
    DataBase database;
    auto db=database.getInstance();

    const auto& idOfDormitory=db->select("utility");
    const auto& idOfStudent=db->select("tenant");

    ui->infoOfAllocation->clear();
    ui->infoOfAllocation->setColumnCount(1);
    ui->infoOfAllocation->setColumnWidth(1,4);
    ui->infoOfAllocation->setHorizontalHeaderLabels(QStringList()<<"未分配宿舍的编号");
    
    QStringList list;

    if(!idOfStudent.isEmpty())
    {
        for(const auto& i : idOfStudent)
        {
            bool inDormitory=false;
            for(const auto& j : idOfDormitory)
            {
                if(i[0].toString()==j[1].toString())
                    inDormitory=true;
            }

            if(!inDormitory)
                list<<i[0].toString();
        }
    }

    ui->infoOfAllocation->setRowCount(list.size());

    for(int i=0;i<list.size();i++)
    {
        ui->infoOfAllocation->setItem(i,0,new QTableWidgetItem(list[i]));
    }
}

void utilityManagement::setId(const QString& str)
{
    if(str!="")
        isStudent=true;
    else
        isStudent=false;

    ui->idOfNeedToSearch->setText(str);

    if(isStudent)
        ui->idOfNeedToSearch->setEnabled(false);
    else
        ui->idOfNeedToSearch->setEnabled(true);
}

void utilityManagement::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    
}
