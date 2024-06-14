#include "widget.h"
#include "ui_widget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QPushButton>
#include <QTimer>
#include <QComboBox>

#include "constans.h"
#include "database.hpp"

widget::widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget)
    , m_Login(new loginBox)
{
    /* 初始化界面 */
    initalWidget();
}

widget::~widget()
{
    /* 释放图片标签 */
    if(!m_Pic.empty())
        for(auto i : m_Pic)
            delete i;
    delete ui;
    delete m_Login;
}

bool widget::eventFilter(QObject *obj, QEvent *e)
{
    if(obj==this&&e->type()==QEvent::Close)
    {
        if(ui->allWidgets->currentIndex()!=0)
        {
            ui->allWidgets->setCurrentIndex(0);
            ui->widgetsOfCenter->setCurrentIndex(0);
        }

        if(m_Login->isLogged())
        {
            e->ignore();
            m_Login->reset();
            m_Login->show();

            return true;
        }
    }
    else if(obj==this&&e->type()==QEvent::Resize)
    {
        static bool isInital=false;
        /* 只执行一次 */
        if(!isInital)
        {
            m_Login->show();
            isInital=true;
        }
    }

    return false;
}

void widget::funcOfTenantManagement(const clickLabel *label)
{
    int i=0;
    if(label==m_Icons[4])
    {
        i=0;
    }
    else if(label==m_Icons[5])
    {
        i=1;
    }
    else if(label==m_Icons[6])
    {
        i=2;
    }
    else if(label==m_Icons[7])
    {
        i=3;
    }

    ui->widgetsOfCenter->setCurrentIndex(1);
    ui->tenant->setCurrentIndex(i);
}

void widget::funcOfUtilityManagement(const clickLabel *label)
{
    int i=0;

    if(label==m_Icons[8])
    {
        i=0;
    }
    else if(label==m_Icons[9])
    {
        i=1;
    }
    else if(label==m_Icons[10])
    {
        i=2;
    }

    ui->widgetsOfCenter->setCurrentIndex(2);
    ui->pageOfUtility->setCurrentIndex(i);
}

void widget::funcOfPaymentManagement(const clickLabel *label)
{
    int i=0;

    if(label==m_Icons[11])
    {
        i=0;
    }
    else if(label==m_Icons[12])
    {
        i=1;
    }
    else if(label==m_Icons[13])
    {
        i=2;
    }

    ui->widgetsOfCenter->setCurrentIndex(3);
    ui->pageOfPayment->setCurrentIndex(i);
}

void widget::funcOfStatManagement(const clickLabel *label)
{
    int i=0;

    if(label==m_Icons[14])
    {
        i=0;
    }
    else if(label==m_Icons[15])
    {
        i=1;
        updateStat();
    }

    ui->widgetsOfCenter->setCurrentIndex(4);
    ui->announcement->setCurrentIndex(i);
}

void widget::reset()
{
    if(m_UserType==loginType::Tenant)
    {
        ui->iconOfAddTenant->hide();
        ui->labelOfAddTenant->hide();
        ui->iconOfDeleteTenant->hide();
        ui->labelOfDeleteTenant->hide();

        ui->iconOfUtilityAllocation->hide();
        ui->labelOfUtilityAllocation->hide();

        ui->iconOfHandlePayment->hide();
        ui->labelOfHandlePayment->hide();
        ui->iconOfRaisePayment->show();
        ui->labelOfRaisePayment->show();

        ui->iconOfPublicStat->hide();
        ui->labelOfPublicStat->hide();
    }
    else
    {
        ui->iconOfAddTenant->show();
        ui->labelOfAddTenant->show();
        ui->iconOfDeleteTenant->show();
        ui->labelOfDeleteTenant->show();

        ui->iconOfUtilityAllocation->show();
        ui->labelOfUtilityAllocation->show();

        ui->iconOfHandlePayment->show();
        ui->labelOfHandlePayment->show();
        ui->iconOfRaisePayment->hide();
        ui->labelOfRaisePayment->hide();

        ui->iconOfPublicStat->show();
        ui->labelOfPublicStat->show();
    }

    ui->widgetsOfSubModule->setCurrentIndex(0);
        
    if(ui->widgetsOfCenter->currentIndex()!=0)
        ui->widgetsOfCenter->setCurrentIndex(0);
}

void widget::updateStat()
{
    DataBase database;
    auto db=database.getInstance();

    const auto& list=db->select("announcement");

    if(!list.isEmpty())
        ui->labelOfStat->setText(list[0][1].toString());
}

void widget::initalWidget()
{
    /* 设置 ui */
    ui->setupUi(this);

    /* 为此窗口安装事件过滤器 */
    installEventFilter(this);

    /* 窗口透明 */
    // setWindowFlags(Qt::FramelessWindowHint);
    // setAttribute(Qt::WA_TranslucentBackground);

    QStackedWidget* stackWidget=ui->widgetsOfSubModule;

    QList<clickLabel*> icons=
    {
        ui->iconOfTenantManagement,ui->iconOfUtilityManagement,ui->iconOfPaymentManagement,ui->iconOfStatManagement,
        ui->iconOfAddTenant,ui->iconOfDeleteTenant,ui->iconOfEditTenant,ui->iconOfSearchTenant,
        ui->iconOfUtilityAllocation,ui->iconOfUtilityAdjustment,ui->iconOfUtilitySearch,
        ui->iconOfRaisePayment,ui->iconOfHandlePayment,ui->iconOfSearchPayment,
        ui->iconOfPublicStat,ui->iconOfSearchStat
    };

    m_Icons=icons;

    QList<QString> pathOfIcons=
    {
        ":/img/main/tenant_management",
        ":/img/main/dormitory_management",
        ":/img/main/warranty_management",
        ":/img/main/announcement_management",
        ":/img/tenant/add",
        ":/img/tenant/delete",
        ":/img/tenant/edit",
        ":/img/tenant/search",
        ":/img/dormitory/allocate",
        ":/img/dormitory/adjust",
        ":/img/dormitory/search",
        ":/img/repair/raise",
        ":/img/repair/handle",
        ":/img/repair/search",
        ":/img/announcement/publish",
        ":/img/announcement/search"
    };

    QList<void(widget::*)(const clickLabel*)> functions=
    {
        nullptr,nullptr,nullptr,nullptr,
        &widget::funcOfTenantManagement,&widget::funcOfTenantManagement,&widget::funcOfTenantManagement,&widget::funcOfTenantManagement,
        &widget::funcOfUtilityManagement,&widget::funcOfUtilityManagement,&widget::funcOfUtilityManagement,
        &widget::funcOfPaymentManagement,&widget::funcOfPaymentManagement,&widget::funcOfPaymentManagement,
        &widget::funcOfStatManagement,&widget::funcOfStatManagement
    };

    QSizePolicy sp = ui->iconOfFront->sizePolicy();
    sp.setRetainSizeWhenHidden(true);
    ui->iconOfFront->setSizePolicy(sp);
    ui->iconOfFront->hide();

    ui->iconOfUtilityAdjustment->hide();
    ui->labelOfUtilityAdjustment->hide();

    connect(stackWidget,&QStackedWidget::currentChanged,this,[=](int i)
    {
        if(i==0)
            ui->iconOfFront->hide();
        else
            ui->iconOfFront->show();
    });

    connect(ui->iconOfFront,&clickLabel::clicked,this,[=]()
    {
        stackWidget->setCurrentIndex(0);
        
        if(ui->widgetsOfCenter->currentIndex()!=0)
            ui->widgetsOfCenter->setCurrentIndex(0);
    });

    /* 初始化图标标签 */
    for(int i=0;i<pathOfIcons.length();i++)
    {
        /* 加载图片 */
        m_Pic.push_back(new QPixmap(pathOfIcons[i]));

        /* 设置标签的图标及连接标签的点击信号到槽函数 */
        icons[i]->setPixmap(*m_Pic[i]);

        if(functions[i]!=nullptr)
            connect(icons[i],&clickLabel::clicked,this,functions[i]);
        if(i<4)
        {
            connect(icons[i],&clickLabel::clicked,this,[=]()
            {
                stackWidget->setCurrentIndex(i+1);
            });
        }
    }

    initalLoginBox();
    initalStat();
}

void widget::initalLoginBox()
{
    connect(m_Login,&loginBox::closed,this,[=]()
    {
        if(!m_Login->isLogged())
        {
            close();
        }
    });

    connect(m_Login,&loginBox::logged,this,[=](const loginType& type,const QString& id)
    {
        this->m_UserType=type;
        reset();

        ui->pageOfUtility->setId(id);
        ui->pageOfPayment->setId(id);
    });
}

void widget::initalStat()
{
    connect(ui->btnOfSubmitStat,&QPushButton::clicked,this,[=]()
    {
        if(ui->editOfStat->toPlainText()=="")
        {
            QMessageBox::warning(this,"公告管理","发布内容为空，请重新输入");
            return;
        }

        const QString& content=ui->editOfStat->toPlainText();

        DataBase database;
        auto db=database.getInstance();

        const auto& list=db->select("announcement");

        if(list.isEmpty())
        {
            db->query("insert into announcement values(NULL,'"+content+"')");
        }
        else
        {
            db->query("update announcement set info='"+content+"' where id="+list[0][0].toString());
        }

        QMessageBox::about(this,"公告管理","发布成功");
    });
}

void widget::moveToCenter(QWidget& widget)
{
    QPoint globalPos = this->mapToGlobal(QPoint(0,0));
    int x = globalPos.x() + (this->width() - widget.width()) / 2;
    int y = globalPos.y() + (this->height() - widget.height()) / 2;
    widget.move(x, y);
}