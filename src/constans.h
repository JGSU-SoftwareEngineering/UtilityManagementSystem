#ifndef CONSTANTS_H
#define CONSTANTS_H

const QString passwd="123456";

/* DataBase */
const QString DatabaseName="User.db";
const QString UserName="azh";
const QString UserPassword=passwd;

const int dormitoryCapacity=4;

const QStringList Student_Fields={"id","name","gender","age","telephone"};
const QStringList Dormitory_Fields={"d_id","id","capacity","gender"};
const QStringList Student_Account_Fields={"id","passwd"};
const QStringList Admin_Account_Fields={"id","passwd"};
const QStringList Repair_Fields={"r_id","d_id","info","isSolved"};
const QStringList Announcement_Fields={"id","info"};

const QList<QPair<QString,QStringList>> relationOfFields=
{
    QPair<QString,QStringList>("student",Student_Fields),
    QPair<QString,QStringList>("dormitory",Dormitory_Fields),
    QPair<QString,QStringList>("student_account",Student_Account_Fields),
    QPair<QString,QStringList>("admin_account",Admin_Account_Fields),
    QPair<QString,QStringList>("repair",Repair_Fields),
    QPair<QString,QStringList>("announcement",Announcement_Fields)
};

#endif /* CONSTANTS_H */