PRAGMA foreign_keys=on;
drop table if exists Regions;
drop table if exists Cities;
drop table if exists Users;
drop table if exists Links;
drop table if exists Info;
drop table if exists Report;

CREATE TABLE Regions(
	id_o	integer primary key autoincrement,
	Federal_District 	varchar(100)
);

CREATE TABLE Cities(
	id_t	integer primary key autoincrement,
	id_o 	integer,
	Town 	varchar(100) unique not null,
	foreign key (id_o) references Regions (id_o)
);

CREATE TABLE Users(
	id_user		integer primary key autoincrement,
	email 	varchar(100),
	pass 	varchar(100)
);

CREATE TABLE Links(
	id_user integer,
	id_t integer,
	foreign key (id_user) references Users (id_user),
	foreign key (id_t) references Cities (id_t)
);

CREATE TABLE Info(
	id_user integer primary key,
	name varchar(100),
	sname varchar(100),
	tel text,
	foreign key (id_user) references Users (id_user)
);

CREATE TABLE Report(
	row integer primary key autoincrement,
	id_t integer,
	date text,
	temp real,
	temp_min real,
	temp_max real,
	avg_day real,
	sky real,
	wind_power real,
	pressure real,	
	foreign key (id_t) references Cities (id_t)
);
















	