#include "stateManagement.h"
#include "ui_stateManagement.h"

#include <QMessageBox>
#include <QDateTime>
#include <qcustomplot.h>
#include <QCollator>
#include <algorithm>

#include "database_utils.hpp"

stateManagement::stateManagement(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::stateManagement)
    , m_Id("")
{
    initalWidget();
}

stateManagement::~stateManagement()
{
    delete ui;
}

void stateManagement::setCurrentIndex(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

bool stateManagement::eventFilter(QObject *obj, QEvent *e)
{
    return false;
}

void stateManagement::setId(const QString& str)
{
    m_Id=str;
}


void stateManagement::updateCombox()
{
    DataBase database;
    auto db=database.getInstance();

    if(m_Id.isEmpty())
    {
        ui->dormitoryOfMonth->setEnabled(true);
        ui->dormitoryOfYear->setEnabled(true);

        ui->dormitoryOfMonth->blockSignals(true);
        ui->dormitoryOfYear->blockSignals(true);

        QStringList list=getDormitoryIds();

        #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
            list=list.toSet().toList();
        #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
            QSet<QString> set(list.begin(),list.end());
            list=QList::QList<QString>(set.begin(),set.end());
        #endif

        QCollator collator;
        collator.setNumericMode(true);
        std::sort(list.begin(), list.end(), collator);

        ui->dormitoryOfMonth->clear();
        ui->dormitoryOfYear->clear();

        ui->dormitoryOfMonth->addItems(list);
        ui->dormitoryOfYear->addItems(list);

        ui->dormitoryOfMonth->blockSignals(false);
        ui->dormitoryOfYear->blockSignals(false);
    }
    else
    {
        ui->dormitoryOfMonth->setEnabled(false);
        ui->dormitoryOfYear->setEnabled(false);

        ui->dormitoryOfMonth->setCurrentText(getDormitoryIdById(m_Id));
        ui->dormitoryOfYear->setCurrentText(getDormitoryIdById(m_Id));
    }

    updateRecord();
    updateChart();
}

void stateManagement::updateRecord()
{
    const QString& d_id=ui->dormitoryOfMonth->currentText();
    const QString& month=ui->monthOfMonth->currentText();
    const QString& year=ui->yearOfMonth->currentText();

    QDate date(year.toInt(),month.toInt(),1);

    QList<QVariantList> info;

    QVariantList list;
    list<<getDataByDIdAndDate(d_id,date,"water")<<getDataByDIdAndDate(d_id,date,"electricity");

    info<<list;

    QStringList listOfMonth=QStringList()<<"月用水量(t)"<<"月用电量(kW·h)";
    
    ui->recordOfMonth->clear();
    ui->recordOfMonth->setColumnCount(listOfMonth.size());
    ui->recordOfMonth->setHorizontalHeaderLabels(listOfMonth);
    
    addToRecordTable(ui->recordOfMonth,info);
}

void stateManagement::updateChart()
{
    const QString& d_id=ui->dormitoryOfYear->currentText();
    const QString& year=ui->yearOfYear->currentText();
    ui->chartOfYear->clearGraphs();

    QCPGraph* graphOfWater=ui->chartOfYear->addGraph();
    QCPGraph* graphOfElectricity=ui->chartOfYear->addGraph();

    QVector<qreal> xData;
    for(int i=1;i<=12;i++)
        xData.push_back(i);

    QString field;
    QString currText;

    if(ui->currDataOfYear->currentText()=="水用量")
    {
        field="water";
        currText="水用量/t";
    }
    else if(ui->currDataOfYear->currentText()=="电用量")
    {
        field="electricity";
        currText="电用量/kW·h";
    }

    QVector<qreal> yData=getDataByDIdAndYear(d_id,year,field);

    graphOfWater->setData(xData,yData);

    ui->chartOfYear->xAxis->setLabel("月份/月");
    ui->chartOfYear->yAxis->setLabel(currText);
    ui->chartOfYear->xAxis->setRange(0,12);
    ui->chartOfYear->xAxis->ticker()->setTickCount(11);
    ui->chartOfYear->yAxis->setRange(0,200);

    ui->chartOfYear->replot();
}

void stateManagement::initalWidget()
{
    ui->setupUi(this);
    installEventFilter(this);

    QStringList info;
    info<<"宿舍编号"<<"当月水用量(t)"<<"当月电用量(kW·h)"<<"创建日期";

    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&stateManagement::updateCombox);

    QStringList months;
    for(int i=1;i<=12;i++)
    {
        months<<QString::number(i);
    }
    ui->monthOfMonth->addItems(months);

    QStringList years;
    for(int i=2000;i<=2050;i++)
    {
        years<<QString::number(i);
    }
    ui->yearOfMonth->addItems(years);
    ui->yearOfYear->addItems(years);

    ui->currDataOfYear->addItems(QStringList()<<"水用量"<<"电用量");

    connect(ui->dormitoryOfMonth,&QComboBox::currentTextChanged,this,&stateManagement::updateRecord);
    connect(ui->monthOfMonth,&QComboBox::currentTextChanged,this,&stateManagement::updateRecord);
    connect(ui->yearOfMonth,&QComboBox::currentTextChanged,this,&stateManagement::updateRecord);
    connect(ui->dormitoryOfMonth,&QComboBox::currentTextChanged,this,&stateManagement::updateChart);
    connect(ui->yearOfYear,&QComboBox::currentTextChanged,this,&stateManagement::updateChart);
    connect(ui->currDataOfYear,&QComboBox::currentTextChanged,this,&stateManagement::updateChart);

    updateRecord();
    updateChart();

    timer->start(10000);

    QDate date=QDateTime::currentDateTime().date();
    
    ui->monthOfMonth->setCurrentText(QString::number(date.month()));
    ui->yearOfMonth->setCurrentText(QString::number(date.year()));

    ui->yearOfYear->setCurrentText(QString::number(date.year()));
}