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

void widget::funcOfStudentManagement(const clickLabel *label)
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
    ui->student->setCurrentIndex(i);
}

void widget::funcOfDormitoryManagement(const clickLabel *label)
{
    int i=0;

    if(label==m_Icons[8])
    {
        i=0;
        ui->pageOfDormitory->updateAllocation();
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
    ui->pageOfDormitory->setCurrentIndex(i);
}

void widget::funcOfRepairManagement(const clickLabel *label)
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
    ui->pageOfRepair->setCurrentIndex(i);
}

void widget::funcOfAnnouncementManagement(const clickLabel *label)
{
    int i=0;

    if(label==m_Icons[14])
    {
        i=0;
    }
    else if(label==m_Icons[15])
    {
        i=1;
        updateAnnouncement();
    }

    ui->widgetsOfCenter->setCurrentIndex(4);
    ui->announcement->setCurrentIndex(i);
}

void widget::reset()
{
    if(m_UserType==loginType::Student)
    {
        ui->iconOfAddStudent->hide();
        ui->labelOfAddStudent->hide();
        ui->iconOfDeleteStudent->hide();
        ui->labelOfDeleteStudent->hide();

        ui->iconOfDormitoryAllocation->hide();
        ui->labelOfDormitoryAllocation->hide();

        ui->iconOfHandleRepair->hide();
        ui->labelOfHandleRepair->hide();
        ui->iconOfRaiseRepair->show();
        ui->labelOfRaiseRepair->show();

        ui->iconOfPublicAnnouncement->hide();
        ui->labelOfPublicAnnouncement->hide();
    }
    else
    {
        ui->iconOfAddStudent->show();
        ui->labelOfAddStudent->show();
        ui->iconOfDeleteStudent->show();
        ui->labelOfDeleteStudent->show();

        ui->iconOfDormitoryAllocation->show();
        ui->labelOfDormitoryAllocation->show();

        ui->iconOfHandleRepair->show();
        ui->labelOfHandleRepair->show();
        ui->iconOfRaiseRepair->hide();
        ui->labelOfRaiseRepair->hide();

        ui->iconOfPublicAnnouncement->show();
        ui->labelOfPublicAnnouncement->show();
    }

    ui->widgetsOfSubModule->setCurrentIndex(0);
        
    if(ui->widgetsOfCenter->currentIndex()!=0)
        ui->widgetsOfCenter->setCurrentIndex(0);
}

void widget::updateAnnouncement()
{
    DataBase database;
    auto db=database.getInstance();

    const auto& list=db->select("announcement");

    if(!list.isEmpty())
        ui->labelOfAnnouncement->setText(list[0][1].toString());
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
        ui->iconOfStudentManagement,ui->iconOfDormitoryManagement,ui->iconOfRepairManagement,ui->iconOfAnnouncementManagement,
        ui->iconOfAddStudent,ui->iconOfDeleteStudent,ui->iconOfEditStudent,ui->iconOfSearchStudent,
        ui->iconOfDormitoryAllocation,ui->iconOfDormitoryAdjustment,ui->iconOfDormitorySearch,
        ui->iconOfRaiseRepair,ui->iconOfHandleRepair,ui->iconOfSearchRepair,
        ui->iconOfPublicAnnouncement,ui->iconOfSearchAnnouncement
    };

    m_Icons=icons;

    QList<QString> pathOfIcons=
    {
        ":/img/main/student_management",
        ":/img/main/dormitory_management",
        ":/img/main/warranty_management",
        ":/img/main/announcement_management",
        ":/img/student/add",
        ":/img/student/delete",
        ":/img/student/edit",
        ":/img/student/search",
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
        &widget::funcOfStudentManagement,&widget::funcOfStudentManagement,&widget::funcOfStudentManagement,&widget::funcOfStudentManagement,
        &widget::funcOfDormitoryManagement,&widget::funcOfDormitoryManagement,&widget::funcOfDormitoryManagement,
        &widget::funcOfRepairManagement,&widget::funcOfRepairManagement,&widget::funcOfRepairManagement,
        &widget::funcOfAnnouncementManagement,&widget::funcOfAnnouncementManagement
    };

    QSizePolicy sp = ui->iconOfFront->sizePolicy();
    sp.setRetainSizeWhenHidden(true);
    ui->iconOfFront->setSizePolicy(sp);
    ui->iconOfFront->hide();

    ui->iconOfDormitoryAdjustment->hide();
    ui->labelOfDormitoryAdjustment->hide();

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
    initalAnnouncement();
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

        ui->student->setId(id);
        ui->pageOfDormitory->setId(id);
        ui->pageOfRepair->setId(id);
    });
}

void widget::initalAnnouncement()
{
    connect(ui->btnOfSubmitAnnouncement,&QPushButton::clicked,this,[=]()
    {
        if(ui->editOfAnnouncement->toPlainText()=="")
        {
            QMessageBox::warning(this,"公告管理","发布内容为空，请重新输入");
            return;
        }

        const QString& content=ui->editOfAnnouncement->toPlainText();

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