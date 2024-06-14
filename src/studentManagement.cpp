#include "studentManagement.h"
#include "ui_studentManagement.h"

#include "database.hpp"
#include "excelReader.h"

#include <QMessageBox>
#include <QFileDialog>

studentManagement::studentManagement(QWidget * parent)
    : QWidget(parent)
    , ui(new Ui::studentManagement)
    , isStudent(false)
    , reader(nullptr)
{
    initalWidget();
}

studentManagement::~studentManagement()
{
    delete ui;

    if(reader!=nullptr)
        delete reader;
}

void studentManagement::setCurrentIndex(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

bool studentManagement::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void studentManagement::setId(const QString &str)
{
    if(str!="")
        isStudent=true;
    else
        isStudent=false;
    
    ui->editOfId->setText(str);
    ui->idOfNeedToSearch->setText(str);

    ui->editOfId->setEnabled(!isStudent);
    ui->idOfNeedToSearch->setEnabled(!isStudent);
}

void studentManagement::setEdit(bool isEdit)
{
    ui->editOfName->setEnabled(isEdit);
    ui->editOfAge->setEnabled(isEdit);
    ui->editOfGender->setEnabled(isEdit);
    ui->editOfTelephone->setEnabled(isEdit);
}

void studentManagement::initalWidget()
{
    ui->setupUi(this);

    /* 设置账号输入框只能为数字，且最长只能为 10 位 */
    QRegularExpressionValidator* reg = new QRegularExpressionValidator(this);
    reg->setRegularExpression(QRegularExpression(QString("\\d{1,10}")));

    ui->inputOfId->setValidator(reg);
    ui->gender->addItems(QStringList()<<"男"<<"女");
    ui->editOfGender->addItems(QStringList()<<"男"<<"女");

    ui->idOfNeedToDelete->setValidator(reg);

    ui->editOfId->setValidator(reg);
    ui->editOfId->setPlaceholderText("请输入所需编辑学生的学号，再点击确认按钮即可编辑");
    setEdit(false);

    ui->idOfNeedToSearch->setValidator(reg);

    ui->infoOfSearch->setColumnCount(4);
    ui->infoOfSearch->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"性别"<<"年龄"<<"电话");
    ui->infoOfSearch->setEditTriggers(QAbstractItemView::NoEditTriggers);

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

        db->insert("student_account",QList<QVariant>()<<
            ui->inputOfId->text()<<
            ui->inputOfId->text()
        );
    });

    connect(ui->btnOfBulkAdd,&QPushButton::clicked,this,[=]()
    {
        /* 获取用户选择 excel 文件的路径 */
        const QString& filepath=QFileDialog::getOpenFileName(this, QStringLiteral("select excel file"), "",QStringLiteral("Excel file(*.xls *.xlsx)"));

        if(filepath.isEmpty())
        {
            QMessageBox::warning(this,"批量添加","未选择任何 excel 文件进行批量添加");
            return;
        }

        if(reader==nullptr)
            reader=new excelReader;

        reader->readExcel(filepath);

        auto data=reader->getData();

        if(data.isEmpty())
        {
            QMessageBox::warning(this,"批量添加","导入数据为空");
            return;
        }

        data.removeFirst();
        qDebug()<<data;

        if(data[0].size()<columnOfStudentFields)
        {
            QMessageBox::warning(this,"批量添加","导入数据的格式有误");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        int countOfAdd=0;
        int countOfAlter=0;

        for(const auto& i : data)
        {
            bool isSuccess=false;
            const auto& studentInfo=db->select("student","id='"+i[0]+"'");
            if(!studentInfo.isEmpty())
            {
                // 进行修改数据
                db->remove("student","id="+i[0]);
                isSuccess=db->insert("student",QList<QVariant>()<<
                    i[0]<<
                    i[1]<<
                    i[2]<<
                    i[3].toInt()<<
                    i[4]
                );

                if(isSuccess)
                    countOfAlter++;
            }
            else
            {
                isSuccess=db->insert("student",QList<QVariant>()<<
                    i[0]<<
                    i[1]<<
                    i[2]<<
                    i[3].toInt()<<
                    i[4]
                );

                if(isSuccess)
                    countOfAdd++;

                db->insert("student_account",QList<QVariant>()<<
                    i[0]<<
                    i[0]
                );
            }
        }

        QMessageBox::about(this,"批量添加","成功添加"+QString::number(countOfAdd)+"个学生,"+"同时修改了"+QString::number(countOfAlter)+"个学生");
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

    connect(ui->btnOfEdit,&QPushButton::clicked,this,[=]()
    {
        static bool isEdit=false;

        QString str=ui->editOfId->text();

        if(str.isEmpty())
        {
            QMessageBox::warning(this,"编辑学生","执行失败，学号不能为空");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        if(!isEdit)
        {
            const auto& list=db->select("student","id="+str);
            if(list.isEmpty())
            {
                QMessageBox::warning(this,"编辑学生","编辑失败,学号不存在，请重新输入学号");
                ui->editOfId->setText("");
                ui->editOfName->setText("");
                ui->editOfGender->setCurrentText("男");
                ui->editOfAge->setText("");
                ui->editOfTelephone->setText("");
                return;
            }

            ui->editOfName->setText(list[0][1].toString());
            ui->editOfGender->setCurrentText(list[0][2].toString());
            ui->editOfAge->setText(list[0][3].toString());
            ui->editOfTelephone->setText(list[0][4].toString());

            ui->editOfId->setEnabled(isEdit);
            
            setEdit(!isEdit);
            isEdit=true;
        }
        else
        {
            if(ui->editOfName->text().isEmpty()||ui->editOfAge->text().isEmpty())
            {
                QMessageBox::warning(this,"编辑学生","编辑失败,还有未填写的信息，请重试");
                return;
            }

            db->remove("student","id="+ui->editOfId->text());
            db->insert("student",QList<QVariant>()<<
                ui->editOfId->text()<<
                ui->editOfName->text()<<
                ui->editOfGender->currentText()<<
                ui->editOfAge->text().toInt()<<
                ui->editOfTelephone->text()
            );

            QMessageBox::about(this,"编辑学生","编辑成功");
            if(!isStudent)
                ui->editOfId->setEnabled(isEdit);
            setEdit(!isEdit);
            isEdit=false;
        }
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