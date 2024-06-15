#include "utilityManagement.h"
#include "ui_utilityManagement.h"

#include <QMessageBox>
#include <QDateTime>

#include "database_utils.hpp"

utilityManagement::utilityManagement(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::utilityManagement)
    , isTenant(false)
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

void utilityManagement::setId(const QString& str)
{
    if(str!="")
        isTenant=true;
    else
        isTenant=false;

    ui->didOfNeedToSearch->setText(getDormitoryIdById(str));

    if(isTenant)
        ui->didOfNeedToSearch->setEnabled(false);
    else
        ui->didOfNeedToSearch->setEnabled(true);
}

void utilityManagement::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    QStringList info;
    info<<"宿舍编号"<<"当月水用量"<<"当月电用量"<<"创建日期";

    ui->infoOfSearch->setColumnCount(info.size());
    ui->infoOfSearch->setHorizontalHeaderLabels(info);
    ui->infoOfSearch->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->btnOfUpdate,&QPushButton::clicked,this,[=]()
    {
        if(ui->inputOfDormitory->text().isEmpty()||ui->inputOfWater->text().isEmpty()
            ||ui->inputOfElectricity->text().isEmpty()
        )
        {
            QMessageBox::warning(this,"更新水电","更新失败,还有未填写的信息");
            return;
        }

        if(!isDecimal(ui->inputOfWater->text())||!isDecimal(ui->inputOfElectricity->text()))
        {
            QMessageBox::warning(this,"更新水电","更新失败,水电输入格式有误");
            return;
        }

        if(!validateDormitoryId(ui->inputOfDormitory->text()))
        {
            QMessageBox::warning(this,"更新水电","更新失败,该宿舍不存在");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        const auto& list=getUtilityDateList(ui->inputOfDormitory->text());
        QString currDate=QDateTime::currentDateTime().toString("yyyy-MM-dd");
        bool isSame=false;

        for(const auto& i : list)
            if(isSameMonth(i,currDate))
                isSame=true;

        if(isSame)
        {
            QMessageBox::warning(this,"更新水电","更新失败,该宿舍当月已更新水电");
            return;
        }

        bool isSuccess=db->query("insert into utility values(NULL," \
            +ui->inputOfDormitory->text()+"," \
            +ui->inputOfWater->text()+"," \
            +ui->inputOfElectricity->text()+"," \
            +currDate+")");

        if(isSuccess)
            QMessageBox::about(this,"更新水电","更新成功");
        else
            QMessageBox::warning(this,"更新租客","更新失败，该月水电可能已更新");
    });

    connect(ui->btnOfSearch,&QPushButton::clicked,this,[=]()
    {
        QString str=ui->didOfNeedToSearch->text();

        DataBase database;
        auto db=database.getInstance();

        const auto& list=(!str.isEmpty())?(db->select("utility","d_id="+str)):(db->select("utility"));

        ui->infoOfSearch->setRowCount(list.size());

        for(int i=0;i<list.size();i++)
        {
            for(int j=1;j<list[i].size();j++)
            {
                ui->infoOfSearch->setItem(i,j-1,new QTableWidgetItem(list[i][j].toString()));
            }
        }
    });
}
