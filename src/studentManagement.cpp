#include "studentManagement.h"
#include "ui_studentManagement.h"

#include "database.hpp"

#include <QMessageBox>

studentManagement::studentManagement(QWidget * parent)
    : QWidget(parent)
    , ui(new Ui::studentManagement)
{
    initalWidget();
}

studentManagement::~studentManagement()
{
    delete ui;
}

void studentManagement::setCurrentIndex(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

bool studentManagement::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void studentManagement::initalWidget()
{
    ui->setupUi(this);

    ui->gender->addItems(QStringList()<<"男"<<"女");
    ui->editOfGender->addItems(QStringList()<<"男"<<"女");

    connect(ui->btnOfAdd,&QPushButton::clicked,this,[=]()
    {
        if(ui->inputOfId->text().isEmpty()||ui->inputOfName->text().isEmpty()||ui->inputOfAge->text().isEmpty())
        {
            QMessageBox::warning(this,"添加学生","添加失败,还有未填写的信息");
            return;
        }

        DataBase database;
        auto db=database.getInstance();
        bool isSuccess=db->insert("student",QList<QVariant>()<<
            ui->inputOfId->text()<<
            ui->inputOfName->text()<<
            ui->gender->currentText()<<
            ui->inputOfAge->text().toInt()<<
            ui->inputOfTelephone->text()
        );

        if(isSuccess)
            QMessageBox::about(this,"添加学生","添加成功");
        else
            QMessageBox::warning(this,"添加学生","添加失败，该学号可能已存在");
    });

    connect(ui->btnOfDelete,&QPushButton::clicked,this,[=]()
    {
        QString str=ui->idOfNeedToDelete->text();

        if(str.isEmpty())
        {
            QMessageBox::warning(this,"删除学生","删除失败，学号不能为空");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        bool isSuccess=db->remove("student","id="+str);
        
        if(isSuccess)
            QMessageBox::about(this,"删除学生","删除成功");
        else
            QMessageBox::warning(this,"删除学生","删除失败，该学号可能不存在");
    });

    connect(ui->btnOfSearch,&QPushButton::clicked,this,[=]()
    {
        QString str=ui->idOfNeedToSearch->text();

        if(str.isEmpty())
        {
            QMessageBox::warning(this,"查询学生","查询失败，学号不能为空");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        const auto& list=db->select("student","id="+str);
        ui->infoOfSearch->setText(toString(list));
    });
}