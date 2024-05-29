CREATE TABLE if not exists student
(
    id CHAR(10) PRIMARY KEY NOT NULL,
    name TEXT NOT NULL,
    gender CHAR(2) NOT NULL,
    age INT NOT NULL,
    telephone CHAR(11)
);

CREATE TABLE if not exists student_account
(
    id CHAR(10) NOT NULL,
    passwd TEXT NOT NULL,
    primary key(id),
    foreign key(id) references student(id) on delete cascade on update cascade
);

CREATE TABLE if not exists admin_account
(
    id CHAR(10) NOT NULL,
    passwd TEXT NOT NULL,
    primary key(id)
);

CREATE TABLE if not exists dormitory
(
    d_id CHAR(10) NOT NULL,
    id CHAR(10) NOT NULL,
    capacity INTEGER NOT NULL,
    gender CHAR(2) NOT NULL,
    primary key(id),
    foreign key(id) references student(id) on delete cascade on update cascade
);

CREATE TABLE if not exists repair
(
    r_id INTEGER PRIMARY KEY AUTOINCREMENT,
    d_id CHAR(10) NOT NULL,
    info TEXT NOT NULL,
    isSolved INTEGER NOT NULL,
    foreign key(d_id) references dormitory(d_id) on delete cascade on update cascade
);

CREATE TABLE if not exists announcement
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    info TEXT NOT NULL
);

INSERT INTO admin_account VALUES('admin','admin');
INSERT INTO student VALUES('1','123','1',21,'18231329131');
INSERT INTO student_account VALUES('1','1');
INSERT INTO dormitory VALUES('630','1',4)