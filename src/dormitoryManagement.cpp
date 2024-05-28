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

    QString str="未分配宿舍的学号：\n";
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

    str+=list.join("\n");

    ui->labelOfAllocationInfo->setText(str);
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

        bool isSuccess=db->insert("dormitory",QList<QVariant>()<<
            ui->didOfAllocation->text()<<
            ui->idOfAllocation->text()<<
            dormitoryCapacity
        );

        if(isSuccess)
            QMessageBox::about(this,"宿舍分配","分配成功");
        else
            QMessageBox::warning(this,"宿舍分配","分配失败，未知原因");

        updateAllocation();
    });

    connect(ui->btnOfSearch,&QPushButton::clicked,this,[=]()
    {
        ui->labelOfSearchInfo->setText("");

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
        QString str="该学号所在寝室为："+dormitoryInfo[0][0].toString()+"\n其中学生学号有：\n";

        for(const auto& i : sameDormitoryInfo)
        {
            roommate<<i[1].toString();
        }

        str+=roommate.join("\n");

        ui->labelOfSearchInfo->setText(str);
    });
}
