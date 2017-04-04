/////
KR1.2
/////
1.2.1 select * from employee;
1.2.2 select name, age, sex from employee;
1.2.3 select name, sex, age+10 from employee;
1.2.4 select e.name, e.age, e.sex from employee as e;
1.2.5 select name, "is", age, "years old" from employee;
1.2.6 select distinct name from employee;
1.2.7 select distinct name, ssn from employee;
1.2.8 select name, age, sex from employee order by name, age asc
/////
KR2.1
/////
2.1.1
insert into employee (ssn,employeeId,name,age,sex,pr,address,salary)
values (1,null,'Evgen',22,'m',1,'Brovki',6);
insert into employee (ssn,employeeId,name,age,sex,pr,address,salary)
values (120,2,'Marina',20,'f',2,'Borvki',12);
insert into employee (ssn,employeeId,name,age,sex,pr,address,salary)
values (13,3,'Alex',null,'m',1,'Brovki',6);
2.1.2 update employee set address='Gikalo' where pr=2;
2.1.3 update employee set address='Gikalo' where name='Evgen';
2.1.4 =/= begin transaction commit
2.1.5 delete from employee where name='Alex';
2.1.6 rollback
2.1.7 select * from employee where pr=1;
2.1.8 =/=
/////
KR2.2
/////
2.2.1 create table new_employee(
	ssn int,
	id int primary key,
	name text not null,
	age int,
	sex text not null,
	pr int,
	foreign key(pr) references pr(id));
2.2.2 (no specifications for size)
	create table new_chief(
	airportId int,
	name text,
	experience int,
	age int,
	ssn int)
2.2.3 (not supported in sqlite3)
	alter table new_chief
	alter column name char;
2.2.4 =/=
2.2.5
create table employee2 as select * from employee;
alter table employee2 add column address2;
2.2.6 drop table employee;
2.2.7 alter table employee2 rename to employee;
2.2.8 airportId int primary key,
2.2.9 foreign key(pr) references pr(id));
2.2.10 =/=
2.2.11 alter table employee add column address2 text;
//////
IPR1.1
//////
1.1.1 select name, age, ssn from employee where phone like '55512__'
1.1.2 select name, age, sex from employee where age>30
1.1.3 select ssn, name, age from employee where address glob '*Masherova*'
1.1.4 select name, ssn, phone from employee where address glob '*Gikalo' and age between 30 and 50;
1.1.5 select name from employee cross join pr;
1.1.6 select name, address, count(address) from employee group by address having (count(address)>1);
1.1.7 select aircompany.name, aircompany.city from aircompany join pr on aircompany.name = pr.name;
1.1.8 select distinct aircompany.name, aircompany.city from aircompany join pr on aircompany.name != pr.name;
1.1.9 select pr.* from aircompany inner join pr on aircompany.aircompanyId = pr.aircompany;
1.1.10 select aircompany.name from aircompany inner join pr on pr.name like '%Flights%';
1.1.11.7 select name, city from aircompany where name in (select name from pr);
1.1.11.8 select name, city from aircompany where name not in (select name from pr);
1.1.11.10 select name from aircompany where aircompanyId in (select aircompany from pr where name like '%Flights%');
//////
IPR1.2
//////
SQL*Plus only;
1.2.1 var vage int; exec :vage :=1; select name, sex, age from employee where age=:vage;
1.2.2 var comp; var ord; exec :comp := 'age<30'; exec :ord := 'name asc' select name, sex, age from employee where comp order by ord;
1.2.3 var col; exec :col := 'name'; select *,:col from employee order by :col asc;
1.2.4 var cols; exec :cols := 'name,age,sex'; select :cols from employee
sqlite specific:
	.print EmployeeReport
	.width 15 5 5
	column format not available in sqlite
//////
IPR2.1
//////
2.1.1 select ssn from employee where name like 'A_______________';
2.1.2 select ssn, name from employee where ((ssn % length(name)) = 1);
2.1.3 select name, phone, strftime('%d','now') - strftime('%d',hireDate) from employee;
2.1.4 select name, phone, strftime('%m','now') - strftime('%m',hireDate) from employee;
2.1.5 select name, phone, date(hireDate,'utc') from employee;
2.1.6 select name, ifnull(ssn,'unknown') from employee;
2.1.7 select replace(replace(replace(replace(name,'Evgen','Yauheni'),'Alex','Alexander'),'Slaval','Vyacheslav'),'Dima','Dmitry') from employee;
2.1.8 select rpad('age','*',age/5), name from employee; //not available in sqlite3
//////
IPR2.2
//////
2.2.1 select * from employee where ssn is not null order by age asc;
		select * from employee where ssn is not null order by age desc;
2.2.2 select count(ssn) from employee where ssn is not null;
2.2.3 select count(distinct job) from employee;
2.2.4 select avg(ifnull(age,0)) from employee;
2.2.5 select airport.name, count(pr.prId) from airport cross join pr group by airport.airportId having airport.airportId = pr.airport;
2.2.6 select city, count(airportId) from airport group by city;
2.2.7 select count(employeeId) from employee group by (age-20)/10;
2.2.8 select job from employee group by job having avg(salary)>100;
2.2.9 select job from employee group by job having avg(salary)>200 AND job like '%wait%';
2.2.10 select job,avg(salary) from employee group by job order by avg(salary);