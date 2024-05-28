#include "loginBox.h"
#include "ui_loginBox.h"

#include <QRegularExpressionValidator>
#include <QMessageBox>

#include "database.hpp"

loginBox::loginBox(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginBox)
    , m_Logged(false)
{
    initalWidget();
}

loginBox::~loginBox()
{
    delete ui;
}

bool loginBox::isLogged()
{
    return m_Logged;
}

bool loginBox::eventFilter(QObject *obj, QEvent *e)
{
    if(obj==this&&e->type()==QEvent::Close)
    {
        emit closed();
    }

    return false;
}

void loginBox::reset()
{
    ui->account->setText("");
    ui->passwd->setText("");
    ui->type->setCurrentText("学生");
    m_Logged=false;
}

void loginBox::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    /* 设置登录框为模态 */
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);

    ui->type->addItems(QStringList()<<"学生"<<"管理员");

    connect(ui->btnOfLogin,&QPushButton::clicked,this,[=]()
    {
        DataBase database;
        auto db=database.getInstance();
    
        QString table_name;
        loginType type;

        if(ui->type->currentText()=="学生")
        {
            table_name="student_account";
            type=loginType::Student;
        }
        else
        {
            table_name="admin_account";
            type=loginType::Admin;
        }

        const auto& list=db->select(table_name,"id='"+ui->account->text()+"'");
        if(list.isEmpty())
        {
            QMessageBox::warning(this,"登录","登录失败，账号错误");
            reset();
            return;
        }
        else
        {
            if(list[0][1].toString()!=ui->passwd->text())
            {
                QMessageBox::warning(this,"登录","登录失败，密码错误");
                reset();
                return;
            }
        }

        m_Logged=true;
        QMessageBox::about(this,"登录","登录成功");
        close();
        emit logged(type,(type==loginType::Student?ui->account->text():""));
    });
}
