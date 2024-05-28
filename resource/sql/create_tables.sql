CREATE TABLE if not exists student
(
    id CHAR(10) PRIMARY KEY NOT NULL,
    name CHAR(10) NOT NULL,
    gender CHAR(2) NOT NULL,
    age INT NOT NULL,
    telephone CHAR(11)
);

CREATE TABLE if not exists student_account
(
    id CHAR(10) NOT NULL,
    passwd CHAR(20) NOT NULL,
    primary key(id),
    foreign key(id) references student(id) on delete cascade on update cascade
);

CREATE TABLE if not exists admin_account
(
    id CHAR(10) NOT NULL,
    passwd CHAR(20) NOT NULL,
    primary key(id)
);

CREATE TABLE if not exists dormitory
(
    d_id CHAR(10) NOT NULL,
    id CHAR(10) NOT NULL,
    capacity INT NOT NULL,
    primary key(d_id),
    foreign key(id) references student(id) on delete cascade on update cascade
);

INSERT INTO admin_account VALUES('admin','admin')