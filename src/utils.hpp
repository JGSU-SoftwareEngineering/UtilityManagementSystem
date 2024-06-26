#ifndef UTILS_H
#define UTILS_H

#include <QFile>
#include <QDate>
#include <QTableWidget>

/* 读取指定路径下的文件，并将其中内容返回 */
inline QString readFile(const QString &filePath)
{
    QString str;
    if(filePath.isEmpty())
        return str;
    
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly|QFile::Text))
        return str;
    QTextStream stream(&file);
    str=stream.readAll();
    file.close();
    return str;
}

inline QString toString(const QList<QVariant>& fields,const QString& sep=" ")
{
    QString str;

    for(int i=0;i<fields.size();i++)
    {
        switch (fields[i].type())
        {
        case QVariant::String:
            str+="'"+fields[i].toString()+"'";
            break;

        case QVariant::Int:
            str+=QString::number(fields[i].toInt());
        
        default:
            break;
        }
        if(i!=fields.size()-1)
            str+=sep;
    }

    return str;
}

inline QString toString(const QList<QList<QVariant>>& list,const QString& sep1=" ",const QString& sep2="\n")
{
    QString str;

    for(int i=0;i<list.size();i++)
    {
        for(int j=0;j<list[i].size();j++)
        {
            str+=list[i][j].toString();
            
            if(j!=list[i].size()-1)
                str+=sep1;
        }
        if(i!=list.size()-1)
            str+=sep2;
    }

    return str;
}

inline bool validateChinesePhoneNumber(const QString &phoneNumber)
{
    // 定义正则表达式用于匹配手机号码和固定电话
    QRegularExpression mobileRegex("^1[3-9][0-9]{9}$");
    QRegularExpression landlineRegex("^0[0-9]{2,3}-?[0-9]{7,8}$");

    // 检查是否匹配手机号码或固定电话
    return mobileRegex.match(phoneNumber).hasMatch() || landlineRegex.match(phoneNumber).hasMatch();
}

inline bool isDecimal(const QString& str)
{
    QRegularExpression reg("^[+]?\\d*\\.?\\d+$"); // 正则表达式匹配小数，允许带正号，小数点前后可以有数字
    QRegularExpressionValidator v(reg,0);

    int pos=0;
    QString tmp(str);
    QValidator::State result=v.validate(tmp,pos);

    if(result==QValidator::State::Acceptable)
    {
        return true;
    }
   
    return false;
}

inline bool isSameMonth(const QString &dateStr1, const QString &dateStr2, const QString &format="yyyy-MM-dd")
{
    QDate date1 = QDate::fromString(dateStr1, format);
    QDate date2 = QDate::fromString(dateStr2, format);

    if (date1.isNull() || date2.isNull())
    {
        // 无效的日期字符串
        return false;
    }

    return (date1.year() == date2.year()) && (date1.month() == date2.month());
}

inline QList<QString> getSelectRowsByTable(const QTableWidget& record,int column=0)
{
    const auto& list=record.selectedItems();

    QList<QString> selected;

    for(int i=0;i<list.count();i++)
    {
        selected.push_back(record.item(list[i]->row(),column)->text());
    }

    #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
        selected=selected.toSet().toList();
    #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
        QSet<QString> set(selected.begin(),selected.end());
        selected=QList::QList<QString>(set.begin(),set.end());
    #endif

    return selected;
}

inline QList<int> toInt(const QList<QString>& list)
{
    QList<int> data;

    for(const auto& i : list)
    {
        data.push_back(i.toInt());
    }

    return data;
}

inline void addToRecordTable(QTableWidget *record, const QList<QVariantList> &info)
{
    record->setRowCount(info.size());

    for(int i=0;i<info.size();i++)
    {
        for(int j=0;j<info[i].size();j++)
        {
            record->setItem(i,j,new QTableWidgetItem(info[i][j].toString()));
        }
    }
}

#endif /* UTILS_H */