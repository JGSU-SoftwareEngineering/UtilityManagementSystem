#ifndef CONSTANTS_H
#define CONSTANTS_H

const QString passwd="123456";

/* DataBase */
const QString DatabaseName="User.db";
const QString UserName="azh";
const QString UserPassword=passwd;

const QStringList Student_Fields={"id","name","gender","age","telephone"};
const QList<QPair<QString,QStringList>> relationOfFields={QPair<QString,QStringList>("student",Student_Fields)};

#endif /* CONSTANTS_H */