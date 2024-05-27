CREATE TABLE if not exists student
(
    id CHAR(10) PRIMARY KEY NOT NULL,
    name CHAR(10) NOT NULL,
    gender CHAR(2) NOT NULL,
    age INT NOT NULL,
    telephone CHAR(11)
)