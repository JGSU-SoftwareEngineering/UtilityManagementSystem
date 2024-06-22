#include "tenantManagement.h"
#include "ui_tenantManagement.h"

#include "database.hpp"
#include "excelReader.h"

#include <QMessageBox>
#include <QFileDialog>

#include "database_utils.hpp"

tenantManagement::tenantManagement(QWidget * parent)
    : QWidget(parent)
    , ui(new Ui::tenantManagement)
{
    initalWidget();
}

tenantManagement::~tenantManagement()
{
    delete ui;
}

void tenantManagement::setCurrentIndex(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

bool tenantManagement::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void tenantManagement::setEdit(bool isEdit)
{
    ui->editOfName->setEnabled(isEdit);
    ui->editOfAge->setEnabled(isEdit);
    ui->editOfGender->setEnabled(isEdit);
    ui->editOfTelephone->setEnabled(isEdit);
    ui->editOfDormitory->setEnabled(isEdit);
}

void tenantManagement::initalWidget()
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
    ui->editOfId->setPlaceholderText("请输入所需编辑租客的编号，再点击确认按钮即可编辑");
    setEdit(false);

    ui->idOfNeedToSearch->setValidator(reg);

    QStringList info;
    info<<"租客编号"<<"姓名"<<"性别"<<"年龄"<<"电话"<<"宿舍编号";

    ui->infoOfSearch->setColumnCount(info.size());
    ui->infoOfSearch->setHorizontalHeaderLabels(info);
    ui->infoOfSearch->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->btnOfAdd,&QPushButton::clicked,this,[=]()
    {
        if(ui->inputOfId->text().isEmpty()||ui->inputOfName->text().isEmpty()
            ||ui->inputOfAge->text().isEmpty()||ui->inputOfTelephone->text().isEmpty()
            ||ui->inputOfDormitory->text().isEmpty()
        )
        {
            QMessageBox::warning(this,"添加租客","添加失败,还有未填写的信息");
            return;
        }

        if(!validateChinesePhoneNumber(ui->inputOfTelephone->text()))
        {
            QMessageBox::warning(this,"添加租客","添加失败,电话输入非法");
            return;
        }

        DataBase database;
        auto db=database.getInstance();
        bool isSuccess=db->insert("tenant",QList<QVariant>()<<
            ui->inputOfId->text()<<
            ui->inputOfName->text()<<
            ui->gender->currentText()<<
            ui->inputOfAge->text().toInt()<<
            ui->inputOfTelephone->text()<<
            ui->inputOfDormitory->text()<<
            ui->inputOfId->text() // 密码默认为租客编号
        );

        if(isSuccess)
            QMessageBox::about(this,"添加租客","添加成功");
        else
            QMessageBox::warning(this,"添加租客","添加失败，该租客可能已存在");
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

        if(data[0].size()<columnOfTenantFields)
        {
            QMessageBox::warning(this,"批量添加","导入数据的格式有误");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        int countOfAdd=0;
        int countOfAlter=0;
        int countOfError=0;

        for(const auto& i : data)
        {
            bool isSuccess=false;

            if(dormitoryIsFull(i[5]))
            {
                countOfError++;
                continue;
            }

            if(validateId("tenant",i[0]))
            {
                // 进行修改数据
                db->remove("tenant","id="+i[0]);
                isSuccess=db->insert("tenant",QList<QVariant>()<<
                    i[0]<<
                    i[1]<<
                    i[2]<<
                    i[3].toInt()<<
                    i[4]<<
                    i[5]<<
                    i[6]
                );

                if(isSuccess)
                    countOfAlter++;
            }
            else
            {
                isSuccess=db->insert("tenant",QList<QVariant>()<<
                    i[0]<<
                    i[1]<<
                    i[2]<<
                    i[3].toInt()<<
                    i[4]<<
                    i[5]<<
                    i[6]
                );

                if(isSuccess)
                    countOfAdd++;
            }
        }

        QMessageBox::about(this,"批量添加","成功添加"+QString::number(countOfAdd)+"个租客,"+"同时修改了"+QString::number(countOfAlter)+"个租客,失败了"+QString::number(countOfError)+"次");
    });

    connect(ui->btnOfDelete,&QPushButton::clicked,this,[=]()
    {
        QString str=ui->idOfNeedToDelete->text();

        if(str.isEmpty())
        {
            QMessageBox::warning(this,"删除租客","删除失败，编号不能为空");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        bool isSuccess=db->remove("tenant","id="+str);
        
        if(isSuccess)
            QMessageBox::about(this,"删除租客","删除成功");
        else
            QMessageBox::warning(this,"删除租客","删除失败，该编号可能不存在");
    });

    connect(ui->btnOfEdit,&QPushButton::clicked,this,[=]()
    {
        static bool isEdit=false;

        QString str=ui->editOfId->text();

        if(str.isEmpty())
        {
            QMessageBox::warning(this,"编辑租客","执行失败，编号不能为空");
            return;
        }

        DataBase database;
        auto db=database.getInstance();

        if(!isEdit)
        {
            const auto& list=db->select("tenant","id="+str);

            if(list.isEmpty())
            {
                QMessageBox::warning(this,"编辑租客","编辑失败,编号不存在，请重新输入编号");
                ui->editOfId->setText("");
                ui->editOfName->setText("");
                ui->editOfGender->setCurrentText("男");
                ui->editOfAge->setText("");
                ui->editOfTelephone->setText("");
                ui->editOfDormitory->setText("");
                return;
            }

            ui->editOfName->setText(list[0][1].toString());
            ui->editOfGender->setCurrentText(list[0][2].toString());
            ui->editOfAge->setText(list[0][3].toString());
            ui->editOfTelephone->setText(list[0][4].toString());
            ui->editOfDormitory->setText(list[0][5].toString());

            ui->editOfId->setEnabled(isEdit);
            
            setEdit(!isEdit);
            isEdit=true;
        }
        else
        {
            if(ui->editOfName->text().isEmpty()||ui->editOfAge->text().isEmpty())
            {
                QMessageBox::warning(this,"编辑租客","编辑失败,还有未填写的信息，请重试");
                return;
            }

            if(validateChinesePhoneNumber(ui->editOfTelephone->text()))
            {
                QMessageBox::warning(this,"编辑租客","编辑失败,电话输入非法，请重试");
                return;
            }

            db->remove("tenant","id="+ui->editOfId->text());
            db->insert("tenant",QList<QVariant>()<<
                ui->editOfId->text()<<
                ui->editOfName->text()<<
                ui->editOfGender->currentText()<<
                ui->editOfAge->text().toInt()<<
                ui->editOfTelephone->text()<<
                getPasswd("tenant","id='"+ui->editOfId->text()+"'")
            );

            QMessageBox::about(this,"编辑租客","编辑成功");

            setEdit(!isEdit);
            isEdit=false;
        }
    });

    connect(ui->btnOfSearch,&QPushButton::clicked,this,[=]()
    {
        QString str=ui->idOfNeedToSearch->text();

        DataBase database;
        auto db=database.getInstance();

        const auto& list=(!str.isEmpty())?(db->select("tenant","id="+str)):(db->select("tenant"));

        ui->infoOfSearch->setRowCount(list.size());

        for(int i=0;i<list.size();i++)
        {
            for(int j=0;j<list[i].size();j++)
            {
                ui->infoOfSearch->setItem(i,j,new QTableWidgetItem(list[i][j].toString()));
            }
        }
    });
}