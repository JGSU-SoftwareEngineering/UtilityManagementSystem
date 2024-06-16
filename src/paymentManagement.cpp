#include "paymentManagement.h"
#include "ui_paymentManagement.h"

#include "database_utils.hpp"

#include <QMessageBox>
#include <QTImer>

paymentManagement::paymentManagement(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::paymentManagement)
{
    initalWidget();
    initalTimer();
}

paymentManagement::~paymentManagement()
{
    delete ui;
}

void paymentManagement::setCurrentIndex(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

bool paymentManagement::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void paymentManagement::refreshRaiseRecord()
{
    DataBase database;
    auto db=database.getInstance();

    const QString& d_id=getDormitoryIdById(m_Id);

    QStringList listOfUtilityIds;
    
    if(!d_id.isEmpty())
        listOfUtilityIds=getUtilityIdsByDId(d_id);

    QStringList listOfRaise=QStringList()<<"账单ID"<<"缴费金额"<<"创建时间"<<"是否支付";

    QList<QVariantList> info;

    for(int i=0;i<listOfUtilityIds.size();i++)
    {
        const auto& bills=getBillNotPayByUId(listOfUtilityIds[i]);

        if(bills.isEmpty())
        {
            continue;
        }

        QVariantList tmp;
        
        for(const auto& j : bills)
        {
            tmp.push_back(j);
        }
        
        // tmp[3]=tmp[3].toInt()==1?"是":"否";
        tmp.removeAt(1);

        info.push_back(tmp);
    }

    ui->recordOfRaise->clear();
    ui->recordOfRaise->setColumnCount(listOfRaise.size());
    ui->recordOfRaise->setHorizontalHeaderLabels(listOfRaise);
    
    addToRecordTable(ui->recordOfRaise,info);
}

void paymentManagement::refreshBillRecord()
{
    DataBase database;
    auto db=database.getInstance();

    QList<QVariantList> info;

    if(m_Id.isEmpty())
    {
        info=db->select("utility");
    }
    else
    {
        const QString& d_id=getDormitoryIdById(m_Id);
        if(!d_id.isEmpty())
            info=db->select("utility","d_id='"+d_id+"'");
    }

    for(int i=0;i<info.size();i++)
    {
        const QStringList& list=getBillByUId(info[i][0].toString());
        if(list.isEmpty())
            continue;
        const QString& b_id=list[0];
        info[i][0]=b_id;
        info[i][4]=info[i][4].toString().left(7);
        info[i].push_back(getMoneyByBId(b_id));
    }

    QStringList listOfBill=QStringList()<<"账单ID"<<"宿舍号"<<"当月水用量"<<"当月电用量"<<"所属月份"<<"总金额";
    
    ui->recordOfBill->clear();
    ui->recordOfBill->setColumnCount(listOfBill.size());
    ui->recordOfBill->setHorizontalHeaderLabels(listOfBill);

    addToRecordTable(ui->recordOfBill,info);
}

void paymentManagement::refreshPaymentRecord()
{
    DataBase database;
    auto db=database.getInstance();

    QStringList listOfPayment=QStringList()<<"账单ID"<<"寝室号"<<"支付金额"<<"支付时间"<<"所属月份";

    QStringList bills;

    QList<QVariantList> list;

    if(m_Id.isEmpty())
    {
        bills=getBillIds();
    }
    else
    {
        const auto& u_ids=getUtilityIdsByDId(getDormitoryIdById(m_Id));

        for(const auto& u_id : u_ids)
        {
            bills.push_back(getBillIdByUId(u_id));
        }
    }

    for(const auto& i : bills)
    {
        const auto& billInfo=db->select("bill","id="+i);
        const auto& utilityInfo=db->select("utility","id="+billInfo[0][1].toString());
        if(utilityInfo.isEmpty()||billInfo.isEmpty())
            continue;

        QVariantList tmp;
        tmp<<i<<getDormitoryIdByBId(i)<<billInfo[0][2]<<billInfo[0][3]<<utilityInfo[0][4].toString().left(7);
        list<<tmp;
    }

    ui->recordOfPayment->clear();
    ui->recordOfPayment->setColumnCount(listOfPayment.size());
    ui->recordOfPayment->setHorizontalHeaderLabels(listOfPayment);

    addToRecordTable(ui->recordOfPayment,list);
}

void paymentManagement::initalTimer()
{
    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=]()
    {
        if(!m_Id.isEmpty())
            refreshRaiseRecord();
        refreshBillRecord();
        refreshPaymentRecord();
    });

    timer->start(5000);
}

void paymentManagement::setId(const QString& str)
{
    m_Id=str;
}

void paymentManagement::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    ui->recordOfRaise->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->recordOfBill->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->recordOfPayment->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->btnOfRaise,&QPushButton::clicked,this,[=]()
    {
        DataBase database;
        auto db=database.getInstance();

        const auto& list=getSelectRowsByTable(*ui->recordOfRaise);

        if(list.isEmpty())
        {
            QMessageBox::about(this,"提交缴费","未选择任何账单进行缴费");
            return;
        }

        for(const auto& i : list)
        {
            db->update("bill","isPay=1","id="+i);
            db->query("insert into payment values(" \
            +i+",'" \
            +getMoneyByBId(i)+"','" \
            +QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
            +"')");
        }

        QMessageBox::about(this,"提交缴费","缴费成功");
        refreshRaiseRecord();
    });
}