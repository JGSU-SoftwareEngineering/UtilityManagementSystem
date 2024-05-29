#include "dormitoryManagement.h"
#include "ui_dormitoryManagement.h"

#include "database.hpp"

#include <QMessageBox>

dormitoryManagement::dormitoryManagement(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::dormitoryManagement)
    , isStudent(false)
{
    initalWidget();
}

dormitoryManagement::~dormitoryManagement()
{
    delete ui;
}

void dormitoryManagement::setCurrentIndex(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

bool dormitoryManagement::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void dormitoryManagement::updateAllocation()
{
    DataBase database;
    auto db=database.getInstance();

    const auto& idOfDormitory=db->select("dormitory");
    const auto& idOfStudent=db->select("student");

    ui->infoOfAllocation->clear();
    ui->infoOfAllocation->setColumnCount(1);
    ui->infoOfAllocation->setColumnWidth(1,4);
    ui->infoOfAllocation->setHorizontalHeaderLabels(QStringList()<<"未分配宿舍的学号");
    
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

void dormitoryManagement::setId(const QString& str)
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

void dormitoryManagement::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    ui->infoOfAllocation->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->infoOfSearch->setColumnCount(3);
    ui->infoOfSearch->setHorizontalHeaderLabels(QStringList()<<"宿舍号"<<"学号"<<"男（女）寝");
    ui->infoOfSearch->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    connect(ui->btnOfAllocate,&QPushButton::clicked,this,[=]()
    {
        updateAllocation();

        if(ui->idOfAllocation->text().isEmpty()||ui->didOfAllocation->text().isEmpty())
        {
            QMessageBox::warning(this,"宿舍分配","分配失败,学号或宿舍号为空");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        const auto& studentInfo=db->select("student","id='"+ui->idOfAllocation->text()+"'");

        if(studentInfo.isEmpty())
        {
            QMessageBox::warning(this,"宿舍分配","分配失败,该学号不存在");
            return;
        }

        const auto& dormitoryInfo=db->select("dormitory","id='"+ui->idOfAllocation->text()+"'");

        if(!dormitoryInfo.isEmpty())
        {
            QMessageBox::warning(this,"宿舍分配","分配失败,该学号已在宿舍："+dormitoryInfo[0][0].toString()+" 中");
            return;
        }

        const auto& listOfDormitory=db->select("dormitory","d_id='"+ui->didOfAllocation->text()+"'");

        if(listOfDormitory.size()>=dormitoryCapacity)
        {
            QMessageBox::warning(this,"宿舍分配","分配失败,该宿舍已满");
            return;
        }
        else if(listOfDormitory.size()>=1)
        {
            if(studentInfo[0][2].toString()!=listOfDormitory[0][3].toString())
            {
                QMessageBox::warning(this,"宿舍分配","该学生性别为"+studentInfo[0][2].toString()+"，无法入住"+listOfDormitory[0][3].toString()+"寝，请选择其他寝室");
                return;
            }
        }

        bool isSuccess=db->insert("dormitory",QList<QVariant>()<<
            ui->didOfAllocation->text()<<
            ui->idOfAllocation->text()<<
            dormitoryCapacity<<
            studentInfo[0][2].toString()
        );

        if(isSuccess)
            QMessageBox::about(this,"宿舍分配","分配成功");
        else
            QMessageBox::warning(this,"宿舍分配","分配失败，未知原因");

        updateAllocation();
    });

    connect(ui->btnOfSearch,&QPushButton::clicked,this,[=]()
    {
        ui->infoOfSearch->clear();
        ui->infoOfSearch->setHorizontalHeaderLabels(QStringList()<<"宿舍号"<<"学号"<<"男（女）寝");

        if(!isStudent)
        {
            if(ui->idOfNeedToSearch->text().isEmpty())
            {
                QMessageBox::warning(this,"信息查询","查询失败,学号不能为空");
                return;
            }
        }

        DataBase database;
        auto db=database.getInstance();

        if(!isStudent)
        {
            const auto& studentInfo=db->select("student","id='"+ui->idOfNeedToSearch->text()+"'");

            if(studentInfo.isEmpty())
            {
                QMessageBox::warning(this,"信息查询","查询失败,该学号不存在");
                return;
            }
        }

        const auto& dormitoryInfo=db->select("dormitory","id='"+ui->idOfNeedToSearch->text()+"'");

        if(dormitoryInfo.isEmpty())
        {
            QMessageBox::warning(this,"信息查询","查询失败,该学生未入住宿舍");
            return;
        }

        const auto& sameDormitoryInfo=db->select("dormitory","d_id='"+dormitoryInfo[0][0].toString()+"'");

        QStringList roommate;

        for(const auto& i : sameDormitoryInfo)
        {
            roommate<<i[1].toString();
        }

        ui->infoOfSearch->setRowCount(roommate.size());

        for(int i=0;i<sameDormitoryInfo.size();i++)
        {
            ui->infoOfSearch->setItem(i,0,new QTableWidgetItem(dormitoryInfo[0][0].toString()));
            ui->infoOfSearch->setItem(i,1,new QTableWidgetItem(roommate[i]));
            ui->infoOfSearch->setItem(i,2,new QTableWidgetItem(sameDormitoryInfo[0][3].toString()));
        }
    });
}
