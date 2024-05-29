#include "repairManagement.h"
#include "ui_repairManagement.h"

#include "database.hpp"

#include <QMessageBox>

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
    ui->stackedWidget->setCurrentIndex(i);
}

bool repairManagement::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void repairManagement::refreshHandleRecord()
{
    DataBase database;
    auto db=database.getInstance();

    const auto& repairInfo=db->select("repair","isSolved=0");

    ui->recordOfHandle->setRowCount(repairInfo.size());

    for(int i=0;i<repairInfo.size();i++)
    {
        for(int j=0;j<Repair_Fields.size();j++)
        {
            ui->recordOfHandle->setItem(i,j,new QTableWidgetItem(repairInfo[i][j].toString()));
        }
    }
}

void repairManagement::refreshRepairRecord()
{
    DataBase database;
    auto db=database.getInstance();

    const auto& repairInfo=db->select("repair");

    ui->recordOfRepair->setRowCount(repairInfo.size());

    for(int i=0;i<repairInfo.size();i++)
    {
        for(int j=0;j<Repair_Fields.size();j++)
        {
            ui->recordOfRepair->setItem(i,j,new QTableWidgetItem(repairInfo[i][j].toString()));
        }
    }
}

void repairManagement::setId(const QString& str)
{
    m_Id=str;
}

void repairManagement::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    ui->recordOfHandle->setColumnCount(Repair_Fields.size());
    ui->recordOfHandle->setHorizontalHeaderLabels(QStringList()<<"报修ID"<<"寝室号"<<"报修详情"<<"是否解决");
    ui->recordOfHandle->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->recordOfRepair->setColumnCount(Repair_Fields.size());
    ui->recordOfRepair->setHorizontalHeaderLabels(QStringList()<<"报修ID"<<"寝室号"<<"报修详情"<<"是否解决");
    ui->recordOfRepair->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->btnOfRaise,&QPushButton::clicked,this,[=]()
    {
        QString str=ui->contentOfRaise->toPlainText();

        if(str.isEmpty())
        {
            QMessageBox::warning(this,"申请报修","报修内容不能为空");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        const auto& dormitoryInfo=db->select("dormitory","id='"+m_Id+"'");

        if(dormitoryInfo.isEmpty())
        {
            QMessageBox::warning(this,"申请报修","该学生未入住，无法报修");
            return;
        }

        bool isSuccess=db->query("insert into repair values(NULL,"+dormitoryInfo[0][0].toString()+",'"+str+"',0)");

        if(isSuccess)
        {
            QMessageBox::about(this,"申请报修","报修成功");
        }
        else
        {
            QMessageBox::warning(this,"申请报修","报修失败");
        }
    });

    connect(ui->btnOfHandle,&QPushButton::clicked,this,[=]()
    {
        QString str=ui->contentOfRaise->toPlainText();

        const auto& list=ui->recordOfHandle->selectedItems();

        QList<int> selectedRepairId;

        for(int i=0;i<list.count();i++)
        {
            selectedRepairId.push_back(ui->recordOfHandle->item(list[i]->row(),0)->text().toInt());
        }

        selectedRepairId=selectedRepairId.toSet().toList();

        if(selectedRepairId.isEmpty())
        {
            QMessageBox::about(this,"处理报修","暂无报修处理");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        for(const auto& i : selectedRepairId)
        {
            db->update("repair","isSolved=1","r_id="+QString::number(i));
        }
        
        QMessageBox::about(this,"处理报修","处理成功");

        refreshHandleRecord();
    });

    connect(ui->btnOfRefresh,&QPushButton::clicked,this,&repairManagement::refreshRepairRecord);
    connect(ui->btnOfRefreshForHandle,&QPushButton::clicked,this,&repairManagement::refreshHandleRecord);
}
