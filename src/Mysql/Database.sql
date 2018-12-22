drop database if exists Simple_chat_room;
create database Simple_chat_room;

use Simple_chat_room;

drop table if exists account;
create table account (
username varchar(30) BINARY not null,
password varchar(30) not null default '123456'
);
alter table account add primary key(username);

insert into account(username) values('Cyanic');
insert into account(username) values('Twofyw');
insert into account(username) values('YYYuna');
insert into account(username) values('novatez');

drop table if exists history;
create table history (
message_id bigint unsigned,
username_main varchar(30) BINARY not null,
username_sub varchar(30) BINARY not null,
message_info varchar(100) not null default 'error'
);

alter table history add primary key(message_id);