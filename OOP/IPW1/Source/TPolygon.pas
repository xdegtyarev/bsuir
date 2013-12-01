unit TPolygon;

interface

uses TPoint,IDrawable,SysUtils,Crt;

type
	Polygon = class(TInterfacedObject,Drawable)
	private
		points: array of Point;
	public
		constructor Create(Args: array of Point);
		destructor Destroy; override;
		function IsConvex: Boolean;
		procedure Draw;
		class function DisplayCreationMenu():Drawable;
	end;

implementation
	
	constructor Polygon.Create(Args: array of Point);
	var i:Integer;
	begin
		SetLength(points,Length(Args));

		for i := 0 to Length(Args) do
		begin
			points[i]:=Args[i];
		end;
		
	end;

	destructor Polygon.Destroy();
	begin
		points:=nil;
	end;

	function Polygon.IsConvex(): Boolean;
	begin
		result := false;
	end;


	procedure Polygon.Draw();
	var i:Integer;
	begin
		WriteLn('Polygon');
		if Length(points) > 0 then
		begin
			for i := 0 to Length(points) do
			begin
				WriteLn('Point' + IntToStr(i));
				WriteLn(points[i].ToString());
			end;
		end;
		if IsConvex() then
		begin
			WriteLn('IsConvex');
		end;
	end;	


	class function Polygon.DisplayCreationMenu():Drawable;
	var count,i: Integer; p: array of Point; poly: Polygon;
	begin
		ClrScr();
		WriteLn('Polygon Creation Menu');
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
		poly := Polygon.Create(p);
		ClrScr();
		WriteLn('Polygon Created');
		poly.Draw();
		WriteLn('Continue');
		ReadLn();
		result := poly;
	end;

end.

