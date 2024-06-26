#include "loginBox.h"
#include "ui_loginBox.h"

#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QTimer>

#include "database_utils.hpp"

loginBox::loginBox(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginBox)
    , m_Logged(false)
    , m_Connected(false)
{
    initalWidget();
    initalConnectState();
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

void loginBox::updateConnectState()
{
    DataBase database;
    auto db=database.getInstance();

    db->open();

    static bool flag=false;

    bool isConnected=db->isOpened();
    QString str=!isConnected?"<html><head/><body><p><span style=\" color:#ff0000;\">未连接服务器</span></p></body></html>":"<html><head/><body><p><span style=\" color:#00aa00;\">已连接服务器</span></p></body></html>";
    
    ui->labelOfConnectState->setText(str);

    if(!isConnected&&!flag)
    {
        emit logout();
    }

    if(isConnected)
    {
        m_Connected=true;
        flag=false;
    }  
    else
    {
        m_Connected=false;
        flag=true;
    }
}

void loginBox::reset()
{
    ui->account->setText("");
    ui->passwd->setText("");
    ui->type->setCurrentText("租客");
    m_Logged=false;
}

void loginBox::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    /* 设置登录框为模态 */
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);

    ui->type->addItems(QStringList()<<"租客"<<"管理员");

    ui->labelOfReset->hide();
    ui->resetPasswd->hide();

    connect(ui->btnOfLogin,&QPushButton::clicked,this,[=]()
    {
        if(!m_Connected)
        {
            QMessageBox::warning(this,"登录","登录失败，未连接到远程数据库");
            return;
        }

        if(ui->account->text()==""||ui->passwd->text()=="")
        {
            QMessageBox::warning(this,"登录","登录失败，账号或密码不能为空");
            return;
        }
    
        QString table_name;
        loginType type;

        if(ui->type->currentText()=="租客")
        {
            table_name="tenant";
            type=loginType::Tenant;
        }
        else
        {
            table_name="admin";
            type=loginType::Admin;
        }

        if(!validateId(table_name,ui->account->text()))
        {
            QMessageBox::warning(this,"登录","登录失败，账号错误");
            reset();
            return;
        }

        if(getPasswd(table_name,ui->account->text())!=ui->passwd->text())
        {
            QMessageBox::warning(this,"登录","登录失败，密码错误");
            reset();
            return;
        }

        m_Logged=true;
        QMessageBox::about(this,"登录","登录成功");
        close();
        emit logged(type,(type==loginType::Tenant?ui->account->text():""));
    });

    connect(ui->btnOfResetPassword,&QPushButton::clicked,this,[=]()
    {
        if(!m_Connected)
        {
            QMessageBox::warning(this,"登录","登录失败，未连接到远程数据库");
            return;
        }

        static bool isResetting=false;

        if(!isResetting)
        {
            if(ui->account->text()==""||ui->passwd->text()=="")
            {
                QMessageBox::warning(this,"修改密码","修改密码前需要输入正确的账号密码，账号或密码不能为空");
                return;
            }
        }
    
        QString table_name;
        loginType type;

        if(ui->type->currentText()=="租客")
        {
            table_name="tenant_account";
            type=loginType::Tenant;
        }
        else
        {
            table_name="admin_account";
            type=loginType::Admin;
        }

        if(!isResetting)
        {
            if(!validateId(table_name,ui->account->text()))
            {
                QMessageBox::warning(this,"修改密码","账号错误");
                reset();
                return;
            }

            if(getPasswd(table_name,ui->account->text())!=ui->passwd->text())
            {
                QMessageBox::warning(this,"修改密码","密码错误");
                reset();
                return;
            }

            ui->labelOfReset->show();
            ui->resetPasswd->show();
            ui->btnOfLogin->hide();
            ui->account->setEnabled(false);
            ui->passwd->setEnabled(false);
            isResetting=true;
        }
        else
        {
            if(ui->resetPasswd->text()=="")
            {
                QMessageBox::warning(this,"修改密码","新密码不能为空");
                return;
            }

            bool isSuccess=updatePasswd(table_name,ui->account->text(),ui->resetPasswd->text());

            if(isSuccess)
            {
                QMessageBox::about(this,"修改密码","修改成功");
            }
            else
            {
                QMessageBox::warning(this,"修改密码","修改失败，未知原因");
            }

            ui->labelOfReset->hide();
            ui->resetPasswd->hide();
            ui->btnOfLogin->show();
            ui->account->setEnabled(true);
            ui->passwd->setEnabled(true);
            isResetting=false;
            ui->account->clear();
            ui->passwd->clear();
        }
    });
}

void loginBox::initalConnectState()
{
    updateConnectState();
    
    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&loginBox::updateConnectState);

    timer->start(10000);
}