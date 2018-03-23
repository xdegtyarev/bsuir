Test Cases

R1001 Save with empty fields
R1001 Test whether it is contained withing 1 file
R1001.1 Launch simultanious apps

R1002 Interface looks like sketch1
R1002 Default tab is General
R1002.1 Buttons are available on all tabs
R1002.1 Save is disabled when until @new@ clicked
R1003 database created on new click
R1004 database opens on open button click
R1004.1 save propose on open new, while editing
R1005 database saves on save clicked
R1005.1 saved name is displated in top of app screen
R1005.2 save button not displayed if opened db is not modified
R1005.3 alert message on overwrite

Husband
R2001 Husband tab is clickable opens screen as Sketch 2
R2001.1 Data validation (First name, Middle Name, LastName(br1003), Birth Day(br1001))
R2001.2 Test that Age is not editable
R2001.2 Test that Age is calculated by BirthDay

Wife
R3001 Wife tab is clickable opens screen as Sketch 2
R3001.1 Data validation (First name, Middle Name, LastName(br1003), Birth Day(br1001))
R3001.2 Test that Age is not editable
R3001.2 Test that Age is calculated by BirthDay

Children
R4001 Children tab is clickable on Childre tab and looks like Sketch 3
R4002 Table is empty with a new DB
R4002 Table collumns are named Name Height BirthDate
R4002.1 Cells are not editable
R4003 Insert as many children as you can
R4003 Scoling should be displayed when table gets bigger
R4004 Children tab has 3 hinted buttons (Add, Del, Edit)
R4004.1 Del,Edit are disabled when there is no records
R4004.2 Add is always enabled
R4005 on add child new screen appears with (Name Height Birth)
R4005 fields has validation as 1003, 1004, 1001
R4005.1 on ok click new record appears and filled with field attributes
R4005.2 on cancel writes no changes
R4006 on select and del child button record deletes
R4006 on remove click confirmation message appears
R4006 on ok click - record removes
R4006 on cancel click - record stays
R4007 on edit click screen with 3 attributes appears
R4007 on ok click - changes save
R4007 on cancel click - changes don't save

Br1002 DescField input is 50char Max, alphanumeric+special, can be blank
Br1003 NameFields input is 50char Max, alphanumeric+special, can be blank
Br1004 HeightField alphanumeric, less than 300, has cm label, manually or CalcPop-ip, can be blank