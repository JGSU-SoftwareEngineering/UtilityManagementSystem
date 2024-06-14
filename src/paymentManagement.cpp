#include "paymentManagement.h"
#include "ui_paymentManagement.h"

#include "database.hpp"

#include <QMessageBox>

paymentManagement::paymentManagement(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::paymentManagement)
{
    initalWidget();
}

paymentManagement::~paymentManagement()
{
    delete ui;
}

void paymentManagement::setCurrentIndex(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

bool paymentManagement::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void paymentManagement::refreshHandleRecord()
{
    DataBase database;
    auto db=database.getInstance();

    const auto& paymentInfo=db->select("payment","isSolved=0");

    ui->recordOfHandle->setRowCount(paymentInfo.size());

    for(int i=0;i<paymentInfo.size();i++)
    {
        for(int j=0;j<columnOfRepairFields;j++)
        {
            ui->recordOfHandle->setItem(i,j,new QTableWidgetItem(paymentInfo[i][j].toString()));
        }
    }
}

void paymentManagement::refreshRepairRecord()
{
    DataBase database;
    auto db=database.getInstance();

    const auto& paymentInfo=db->select("payment");

    ui->recordOfRepair->setRowCount(paymentInfo.size());

    for(int i=0;i<paymentInfo.size();i++)
    {
        for(int j=0;j<columnOfRepairFields;j++)
        {
            ui->recordOfRepair->setItem(i,j,new QTableWidgetItem(paymentInfo[i][j].toString()));
        }
    }
}

void paymentManagement::setId(const QString& str)
{
    m_Id=str;
}

void paymentManagement::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    ui->recordOfHandle->setColumnCount(columnOfRepairFields);
    ui->recordOfHandle->setHorizontalHeaderLabels(QStringList()<<"报修ID"<<"寝室号"<<"报修详情"<<"是否解决");
    ui->recordOfHandle->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->recordOfRepair->setColumnCount(columnOfRepairFields);
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

        bool isSuccess=db->query("insert into payment values(NULL,"+dormitoryInfo[0][0].toString()+",'"+str+"',0)");

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
            db->update("payment","isSolved=1","r_id="+QString::number(i));
        }
        
        QMessageBox::about(this,"处理报修","处理成功");

        refreshHandleRecord();
    });

    connect(ui->btnOfRefresh,&QPushButton::clicked,this,&paymentManagement::refreshRepairRecord);
    connect(ui->btnOfRefreshForHandle,&QPushButton::clicked,this,&paymentManagement::refreshHandleRecord);
}
