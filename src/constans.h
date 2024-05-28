#ifndef CONSTANTS_H
#define CONSTANTS_H

const QString passwd="123456";

/* DataBase */
const QString DatabaseName="User.db";
const QString UserName="azh";
const QString UserPassword=passwd;

const QStringList Student_Fields={"id","name","gender","age","telephone"};
const QStringList Dromitory_Fields={"d_id","id","capacity"};
const QStringList Student_Account_Fields={"id","passwd"};
const QStringList Admin_Account_Fields={"id","passwd"};

const QList<QPair<QString,QStringList>> relationOfFields=
{
    QPair<QString,QStringList>("student",Student_Fields),
    QPair<QString,QStringList>("dromitory",Dromitory_Fields),
    QPair<QString,QStringList>("student_account",Student_Account_Fields),
    QPair<QString,QStringList>("admin_account",Admin_Account_Fields)
};

#endif /* CONSTANTS_H */