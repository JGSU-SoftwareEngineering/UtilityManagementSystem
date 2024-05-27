#ifndef UTILS_H
#define UTILS_H

#include <QFile>

/* 读取指定路径下的文件，并将其中内容返回 */
QString readFile(const QString &filePath)
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

QString toString(const QList<QVariant>& fields,const QString& sep=" ")
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

QString toString(const QList<QList<QVariant>>& list,const QString& sep1=" ",const QString& sep2="\n")
{
    QString str;

    for(int i=0;i<list.size();i++)
    {
        str+=toString(list[i],sep1);
        if(i!=list.size()-1)
            str+=sep2;
    }

    return str;
}

#endif /* UTILS_H */