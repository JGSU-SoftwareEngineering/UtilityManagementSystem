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

    const auto& utilityInfo=db->select("utility","d_id='"+d_id+"'",QStringList()<<"create_time");

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

#endif /* DATABASE_UTILS */