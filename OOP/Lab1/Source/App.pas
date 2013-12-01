program Lab1;
uses
	Pizza;
var
	p:TPizza;
begin
	WriteLn('Creating Pizza');
	p:= TPizza.Create;
	p.Deliver();
	ReadLn;
end.