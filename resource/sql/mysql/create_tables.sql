CREATE TABLE if not exists tenant
(
    id CHAR(10) NOT NULL,
    name TEXT NOT NULL,
    gender CHAR(2) NOT NULL,
    age INT NOT NULL,
    telephone CHAR(11),
    d_id CHAR(10) NOT NULL,
    passwd CHAR(20) NOT NULL,
    PRIMARY KEY(id,d_id)
);

CREATE TABLE if not exists admin
(
    id CHAR(10) NOT NULL,
    name CHAR(10) NOT NULL,
    gender CHAR(2) NOT NULL,
    telephone CHAR(11),
    passwd TEXT NOT NULL,
    primary key(id)
);

CREATE TABLE if not exists utility
(
    id INTEGER PRIMARY KEY auto_increment,
    d_id CHAR(10) NOT NULL,
    water CHAR(10) NOT NULL,
    electricity CHAR(10) NOT NULL,
    create_time DATE NOT NULL
);

CREATE TABLE if not exists bill
(
    id INTEGER PRIMARY KEY auto_increment,
    u_id CHAR(10) NOT NULL,
    money CHAR(20) NOT NULL,
    create_time DATETIME NOT NULL,
    isPay INTEGER NOT NULL
);

CREATE TABLE if not exists payment
(
    id INTEGER PRIMARY KEY,
    money CHAR(20) NOT NULL,
    foreign key(id) references bill(id) on delete cascade on update cascade
);

INSERT INTO admin VALUES('admin','admin','','18279692373','admin')