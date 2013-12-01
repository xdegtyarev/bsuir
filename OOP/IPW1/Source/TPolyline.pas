unit TPolyline;

interface

uses TPoint,IDrawable,SysUtils,Crt;

type
	Polyline = class(TInterfacedObject,Drawable)
	private
		points: array of Point;
	public
		constructor Create(Args: array of Point);
		destructor Destroy; override;
		procedure Draw;
		class function DisplayCreationMenu():Drawable;
	end;


implementation
	
	constructor Polyline.Create(Args: array of Point);
	var i:Integer;
	begin
		SetLength(points,Length(Args));

		for i := 0 to Length(Args) do
		begin
			points[i]:=Args[i];
		end;
		
	end;

	destructor Polyline.Destroy();
	begin
		points:=nil;
	end;

	procedure Polyline.Draw();
	var i:Integer;
	begin
		WriteLn('PolyLine');
		if Length(points) > 0 then
		begin
			WriteLn('StartPoint');
			WriteLn(points[0].ToString());
			for i := 1 to Length(points)-2 do
			begin
				WriteLn('Point' + IntToStr(i));
				WriteLn(points[i].ToString());
			end;
			WriteLn('EndPoint:');
			WriteLn(points[Length(points)-1].ToString());
		end;
	end;	

	class function Polyline.DisplayCreationMenu():Drawable;
	var count,i: Integer; p: array of Point; line:Polyline;
	begin
		ClrScr();
		WriteLn('Polyline Creation Menu');
		WriteLn('Points Count');
		ReadLn(count);
		WriteLn('Points:' + IntToStr(count));
		SetLength(p,count);
		for i := 0 to count-1 do
		begin
			ClrScr();
			WriteLn('Point:' + IntToStr(i));
			p[i]:=Point.CreatePoint();
		end;
		line := PolyLine.Create(p);
		ClrScr();
		WriteLn('Polyline Created');
		line.Draw;
		WriteLn('Continue');
		ReadLn();
		result := line;
	end;

end.