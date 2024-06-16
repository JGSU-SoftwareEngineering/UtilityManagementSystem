#ifndef DATABASE_UTILS
#define DATABASE_UTILS

#include "database.hpp"

inline QString getDormitoryIdById(const QString& id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& tenantInfo=db->select("tenant","id='"+id+"'");

    if(tenantInfo.isEmpty())
        return "";

    return tenantInfo[0][5].toString();
}

inline QString getDormitoryIdByBId(const QString& b_id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& u_id=db->select("bill","id='"+b_id+"'",QStringList()<<"u_id");

    if(u_id.isEmpty())
        return "";

    const auto& d_id=db->select("utility","id='"+u_id[0][0].toString()+"'",QStringList()<<"d_id");

    if(d_id.isEmpty())
        return "";

    return d_id[0][0].toString();
}

inline bool validateId(const QString& table_name,const QString& id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& info=db->select(table_name,"id='"+id+"'");

    return !info.isEmpty();
}

inline QString getPasswd(const QString& table_name,const QString& id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& info=db->select(table_name,"id='"+id+"'",QStringList()<<"passwd");

    return info[0][0].toString();
}

inline bool updatePasswd(const QString& table_name,const QString& id,const QString& passwd)
{
    DataBase database;
    auto db=database.getInstance();

    return db->update(table_name,"passwd='"+passwd+"'","id='"+id+"'");
}

inline bool dormitoryIsFull(const QString& d_id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& tenantInfo=db->select("tenant","d_id='"+d_id+"'");

    if(tenantInfo.size()>=dormitoryCapacity)
    {
        return true;
    }

    return false;
}

inline QList<QString> getUtilityDateList(const QString& d_id)
{
    DataBase database;
    auto db=database.getInstance();

    QList<QString> dates;

    const auto& utilityInfo=db->select("utility","d_id='"+d_id+"'",QStringList()<<"create_date");

    for(const auto& i : utilityInfo)
    {
        dates<<i[0].toString();
    }

    return dates;
}

inline bool validateDormitoryId(const QString& d_id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& info=db->select("tenant","d_id='"+d_id+"'");

    return !info.isEmpty();
}

inline QStringList getUtilityIdsByDId(const QString& d_id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& info=db->select("utility","d_id='"+d_id+"'",QStringList()<<"id");

    QStringList data;

    if(info.isEmpty())
        return data;

    for(const auto& i : info)
    {
        data<<i[0].toString();
    }

    return data;
}

inline QStringList getUtilityIds()
{
    DataBase database;
    auto db=database.getInstance();

    const auto& info=db->select("utility",QStringList()<<"id");

    QStringList data;

    if(info.isEmpty())
        return data;

    for(const auto& i : info)
    {
        data<<i[0].toString();
    }

    return data;
}

inline QStringList getBillByUId(const QString& u_id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& info=db->select("bill","u_id='"+u_id+"'");

    QStringList data;

    if(info.isEmpty())
        return data;

    for(const auto& i : info[0])
    {
        data<<i.toString();
    }

    return data;
}

inline QString getBillIdByUId(const QString& u_id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& info=db->select("bill","u_id='"+u_id+"'",QStringList()<<"id");

    if(info.isEmpty())
        return "";

    return info[0][0].toString();
}

inline QStringList getBillIds()
{
    DataBase database;
    auto db=database.getInstance();

    const auto& info=db->select("bill",QStringList()<<"id");

    QStringList data;

    if(info.isEmpty())
        return data;

    for(const auto& i : info)
    {
        data<<i[0].toString();
    }

    return data;
}

inline QStringList getBillNotPayByUId(const QString& u_id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& info=db->select("bill","u_id='"+u_id+"' and isPay=0");

    QStringList data;

    if(info.isEmpty())
        return data;

    for(const auto& i : info[0])
    {
        data<<i.toString();
    }

    return data;
}

inline QString caculateUtilityMoney(const QString& water,const QString& electricity)
{
    return QString::number(water.toDouble()*3+electricity.toDouble()*1);
}

inline QString getMoneyByBId(const QString& b_id)
{
    DataBase database;
    auto db=database.getInstance();

    const auto& info=db->select("bill","id="+b_id,QStringList()<<"money");

    if(info.isEmpty())
        return "";

    return info[0][0].toString();
}

#endif /* DATABASE_UTILS */