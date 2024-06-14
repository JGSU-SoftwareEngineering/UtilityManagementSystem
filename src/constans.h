#ifndef CONSTANTS_H
#define CONSTANTS_H

/* 若使用 mysql，请开启这个宏，需要设置 DatabaseName 为数据库名、Host 为远程数据库 ip、UserName 为用户名、UserPassword 为密码 */
// #define USE_MYSQL

/* DataBase */

#ifndef USE_MYSQL

const QString DataBaseType="QSQLITE";
const QString DatabaseName="User.db";

#else

const QString DataBaseType="QMYSQL";
const QString DatabaseName="dormitory";

#endif

const QString Host="127.0.0.1";
const QString UserName="debian-sys-maint";
const QString UserPassword="7go0NB6JVsEW4WpP";

const int dormitoryCapacity=4;

const int columnOfStudentFields=5;
const int columnOfRepairFields=4;

#endif /* CONSTANTS_H */