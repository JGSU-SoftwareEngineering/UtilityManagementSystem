# DormitoryManagementSystem

**简介**

宿舍管理系统，基于 Qt、Mysql（Sqlite）开发，只是应付作业。

**数据库**

1. 可选择 sqlite（默认）、mysql
2. 若需要使用 mysql，需要修改 src/constants.h 中关于数据库的变量（为了让 qt 程序正常连接 mysql 数据库，还需要再可执行文件路径下放置 Qt Sql 的驱动，默认是使用 Sqlite，不需要任何驱动），及取消第五行的注释，即可使用 mysql。

**登录界面**

<img src="https://github.com/azh-1415926/DormitoryManagementSystem/blob/main/img/login.png" alt="login.png" width="50%" height="50%">

**主界面**

<img src="https://github.com/azh-1415926/DormitoryManagementSystem/blob/main/img/mainPage.png" alt="mainPage.png" width="50%" height="50%">

**学生管理**

<img src="https://github.com/azh-1415926/DormitoryManagementSystem/blob/main/img/student.png" alt="student.png" width="50%" height="50%">

**宿舍管理**

<img src="https://github.com/azh-1415926/DormitoryManagementSystem/blob/main/img/dormitory.png" alt="dormitory.png" width="50%" height="50%">

**报修管理**

<img src="https://github.com/azh-1415926/DormitoryManagementSystem/blob/main/img/repair.png" alt="repair.png" width="50%" height="50%">

处理报修

<img src="https://github.com/azh-1415926/DormitoryManagementSystem/blob/main/img/handle_repair.png" alt="handle_repair.png" width="50%" height="50%">


**公告管理**

<img src="https://github.com/azh-1415926/DormitoryManagementSystem/blob/main/img/announce.png" alt="announce.png" width="50%" height="50%">