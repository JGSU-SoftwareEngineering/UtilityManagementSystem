#include "subwidget.h"
#include "ui_subwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QIntValidator>

#include "clicklabel.h"

subWidget::subWidget(QWidget * parent)
    : QWidget(parent)
    , ui(new Ui::subWidget)
    , m_Background(nullptr)
    , m_Pic(nullptr)
{
    initalWidget();
}

subWidget::~subWidget()
{
    if(m_Background!=nullptr)
        delete m_Background;
    
    if(m_Pic!=nullptr)
        delete m_Pic;
}

void subWidget::setIcon(const QString &path)
{
    if(m_Pic!=nullptr)
        delete m_Pic;
    m_Pic=new QPixmap(path);
    ui->icon->setPixmap(*m_Pic);
}

void subWidget::setBackground(const QString &path)
{
    if(m_Background!=nullptr)
        delete m_Background;
    m_Background=new QPixmap(path);
}

void subWidget::setTitle(const QString &title)
{
    ui->labelOfTitle->setText(title);
}

void subWidget::setContent(const QString &content)
{
    ui->labelOfContent->setText(content);
}

void subWidget::setTextOfBottom(const QString &left, const QString &mid, const QString &right)
{
    ui->btnOfYes->setText(left);
    
    if(mid.isEmpty())
    {
        ui->btnOfInfo->setText(mid);
        ui->btnOfInfo->hide();
    }
    else
    {
        ui->btnOfInfo->setText(mid);
        ui->btnOfInfo->show();
    }
    
    ui->btnOfNo->setText(right);
}

void subWidget::setFuncOfBtn(std::function<void(void)> left, std::function<void(void)> right)
{
    connect(ui->btnOfYes,&QPushButton::clicked,this,left);
    ui->btnOfInfo->hide();
    connect(ui->btnOfNo,&QPushButton::clicked,this,right);
}

void subWidget::setFuncOfBtn(std::function<void(void)> left, std::function<void(void)> mid, std::function<void(void)> right)
{
    connect(ui->btnOfYes,&QPushButton::clicked,this,left);
    connect(ui->btnOfInfo,&QPushButton::clicked,this,mid);
    connect(ui->btnOfNo,&QPushButton::clicked,this,right);
}

void subWidget::showContent()
{
    ui->widgetsOfCenter->setCurrentIndex(0);
}

void subWidget::showInput()
{
    ui->widgetsOfCenter->setCurrentIndex(1);
}

QString subWidget::getInput()
{
    return ui->input->text();
}

bool subWidget::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void subWidget::initalWidget()
{
    ui->setupUi(this);

    ui->labelOfContent->setText("这是一段文本，是窗口的主要内容");

    QFont font;
    
    font.setPointSize(8);
    ui->labelOfContent->setFont(font);
    
    font.setPointSize(10);
    font.setBold(true);

    ui->labelOfTitle->setFont(font);
    ui->input->setValidator(new QIntValidator(1,65535,this));

    connect(ui->btnOfYes,&QPushButton::clicked,this,&subWidget::close);
    connect(ui->btnOfNo,&QPushButton::clicked,this,&subWidget::close);
}