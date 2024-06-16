# UtilityManagementSystem

**简介**

水电收费管理系统，基于 Qt、Mysql（Sqlite）开发，只是应付课设。

**数据库**

1. 可选择 sqlite、mysql（默认）
2. 使用 mysql，需要修改 src/constants.h 中关于数据库的变量（为了让 qt 程序正常连接 mysql 数据库，还需要再可执行文件路径下放置 Qt Sql 的驱动，默认是使用 Sqlite，不需要任何驱动），及取消第五行的注释，即可使用 mysql。
3. 占时抛弃 sqlite，后续再适配