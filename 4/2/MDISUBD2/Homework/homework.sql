/////
KR1
/////
1.1 select * from employee where hiredate > (select hireDate from employee where name like 'Petrov');
1.2 select job from employee where salary = (select Avg(salary) from employee);
1.3 select name from pr where prId = (select pr from employee where sex like 'f' and age < 30);
1.4 select name, ssn from employee where salary < (select Avg(salary) from employee) and age > (select Avg(age) from employee);
1.5 select * from employee where salary != (select salary from employee where job like 'manager');
1.6 select name, ssn from employee where hireDate = (select hireDate from employee where job like 'manager') and salary = (select salary from employee where job like 'manager');
1.7 select name, age, sex, hireDate from employee where salary > (select Avg(salary) from employee);
/////
IPR1
/////

1.1
DECLARE
BEGIN
END;

1.2
BEGIN
dbms_output.put_line('My first message');
END;

1.3
DECLARE
just_a_number integer := 10;
message constant varchar2(20) := 'Hello world!';
BEGIN
dbms_output.put_line(message || just_a_number);
END;

1.4
DECLARE
employee_name employee.name%type;
other_employee_name employee.name%type;
employee_hire_date employee.hire%type;
other_employee_hire_date employee.hire%type;
employee_rec employee%rowtype;
BEGIN
;
END;

1.5
DECLARE
employee_name employee.name%type;
other_employee_name employee.name%type;
employee_hire_date employee.hire%type;
other_employee_hire_date employee.hire%type;
employee_rec employee%rowtype;
BEGIN
	select name,hire into employee_name, employee_hire_date from employee where employeeid = 1;
	select name,hire into other_employee_name, other_employee_hire_date from employee where employeeid = 3;
	select * into employee_rec from employee where employeeid = 2;
END;

1.6
DECLARE
employee_name employee.name%type;
other_employee_name employee.name%type;
employee_hire_date employee.hire%type;
other_employee_hire_date employee.hire%type;
employee_rec employee%rowtype;
BEGIN
	select name,hire into employee_name, employee_hire_date from employee where employeeid = 1;
	select name,hire into other_employee_name, other_employee_hire_date from employee where employeeid = 3;
    dbms_output.put_line('Diff in hire terms of ' || employee_name || ' and ' || other_employee_name || ' is ' || (employee_hire_date - other_employee_hire_date ) || ' days ');
END;

1.7
select name from employee where employeeid = 1;
select name from employee where employeeid = 3;
select hire - (select hire from employee where employeeid = 3) from employee where employeeid = 1 ;

1.8
DECLARE
minsalary int := 100;
BEGIN
delete from employee where salary<minsalary;
END;

1.9
select * from employee;
insert into employee (employeeid,salary) values (10,90);
insert into employee (employeeid,salary) values (11,80);
select * from employee;
DECLARE
opcount int := 0;
minsalary int := 100;
cursor low_wage_employees is
select employeeid from employee where salary<minsalary;

BEGIN
for n in low_wage_employees loop
    opcount := opcount + 1;
    delete from employee where employeeid = n.employeeid;
end loop;
dbms_output.put_line('removed ' || opcount || ' entries');
END;
////
IPR2
1.1
select * from employee;
DECLARE
employee_id employee.employeeid%type := 1;
work_exp_arg constant int := 10;
employee_work_exp int;
employee_hire_date employee.hire%type;
employee_name employee.name%type;
current_d date:=CURRENT_TIMESTAMP();
BEGIN
	select name,hire into employee_name, employee_hire_date from employee where employeeid = employee_id;
	employee_work_exp := current_d-employee_hire_date;
	IF( employee_work_exp < work_exp_arg ) THEN
		dbms_output.put_line(employee_name || ' young employee');
	ELSE
		dbms_output.put_line(employee_name || ' old employee');
	END IF;
	dbms_output.put_line('DAYS:' || employee_work_exp || 'MONTHS: ' || employee_work_exp/31 || 'YEARS: ' || employee_work_exp/365);
END;

1.2
select * from employee;
DECLARE
employee_age employee.age%type := 22;
cursor employeeofspecificage is select name,age from employee where age = employee_age;
e_name employee.name%type;
e_age employee.age%type;
BEGIN
	open employeeofspecificage;
	FETCH employeeofspecificage INTO e_name, e_age;
	dbms_output.put_line('EMP:' || e_name || ' age: ' || e_age);
	FETCH employeeofspecificage INTO e_name, e_age;
	dbms_output.put_line('EMP:' || e_name || ' age: ' || e_age);
	close employeeofspecificage;
END;

1.3
select * from employee;
DECLARE
employee_age employee.age%type := 22;
cursor employeeofspecificage is select * from employee where age = employee_age;
BEGIN
for n in employeeofspecificage loop
	dbms_output.put_line('EMP:' || n.name || ' age: ' || n.age);
end loop;
END;
/////
