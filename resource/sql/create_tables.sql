CREATE TABLE if not exists tenant
(
    id CHAR(10) PRIMARY KEY NOT NULL,
    name TEXT NOT NULL,
    gender CHAR(2) NOT NULL,
    age INT NOT NULL,
    telephone CHAR(11),
    d_id CHAR(10) NOT NULL,
    passwd CHAR(20) NOT NULL
);

CREATE TABLE if not exists admin_account
(
    id CHAR(10) NOT NULL,
    passwd TEXT NOT NULL,
    primary key(id)
);

CREATE TABLE if not exists utility
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    d_id CHAR(10) NOT NULL,
    water CHAR(10) NOT NULL,
    electricity CHAR(10) NOT NULL,
    create_time DATA NOT NULL,
    foreign key(d_id) references tenant(d_id) on delete cascade on update cascade
);

CREATE TABLE if not exists payment
(
    r_id INTEGER PRIMARY KEY AUTOINCREMENT,
    d_id CHAR(10) NOT NULL,
    info TEXT NOT NULL,
    isSolved INTEGER NOT NULL,
    foreign key(d_id) references utility(d_id) on delete cascade on update cascade
);

CREATE TABLE if not exists stat
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    info TEXT NOT NULL
);

INSERT INTO admin_account VALUES('admin','admin');