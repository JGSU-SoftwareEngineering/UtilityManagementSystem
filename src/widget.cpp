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
    initalLoginBox();
}

widget::~widget()
{
    /* 释放图片标签 */
    if(!m_Pic.empty())
        for(auto i : m_Pic)
            delete i;
    delete ui;
    delete m_Login;

    if(reader!=nullptr)
        delete reader;
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
    ui->utility->setCurrentIndex(i);
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
    ui->payment->setCurrentIndex(i);
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
    }

    ui->widgetsOfCenter->setCurrentIndex(4);
    ui->state->setCurrentIndex(i);
}

void widget::reset()
{
    if(m_UserType==loginType::Tenant)
    {
        ui->iconOfTenantManagement->hide();
        ui->labelOfTenantManagement->hide();

        ui->iconOfUtilityAllocation->hide();
        ui->labelOfUtilityAllocation->hide();

        ui->iconOfRaisePayment->show();
        ui->labelOfRaisePayment->show();
    }
    else
    {
        ui->iconOfTenantManagement->show();
        ui->labelOfTenantManagement->show();

        ui->iconOfUtilityAllocation->show();
        ui->labelOfUtilityAllocation->show();

        ui->iconOfRaisePayment->hide();
        ui->labelOfRaisePayment->hide();
    }

    ui->widgetsOfSubModule->setCurrentIndex(0);
        
    if(ui->widgetsOfCenter->currentIndex()!=0)
        ui->widgetsOfCenter->setCurrentIndex(0);
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
        ui->iconOfRaisePayment,ui->iconOfSearchBill,ui->iconOfSearchPayment,
        ui->iconOfMonthStat,ui->iconOfYearStat
    };

    m_Icons=icons;

    QList<QString> pathOfIcons=
    {
        ":/img/main/tenant_management",
        ":/img/main/utility_management",
        ":/img/main/payment_management",
        ":/img/main/stat_management",
        ":/img/tenant/add",
        ":/img/tenant/delete",
        ":/img/tenant/edit",
        ":/img/tenant/search",
        ":/img/utility/update",
        ":/img/utility/adjust",
        ":/img/utility/search",
        ":/img/payment/raise",
        ":/img/payment/handle",
        ":/img/payment/search",
        ":/img/stat/month",
        ":/img/stat/year"
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

        ui->utility->setId(id);
        ui->payment->setId(id);
    });

    connect(m_Login,&loginBox::logout,this,[=]()
    {
        if(m_Login->isHidden())
        {
            reset();
            m_Login->show();
        }
    });
}

void widget::moveToCenter(QWidget& widget)
{
    QPoint globalPos = this->mapToGlobal(QPoint(0,0));
    int x = globalPos.x() + (this->width() - widget.width()) / 2;
    int y = globalPos.y() + (this->height() - widget.height()) / 2;
    widget.move(x, y);
}