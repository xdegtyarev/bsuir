unit TLine;

interface

uses TPoint,IDrawable,Crt;

type
	Line = class(TInterfacedObject,Drawable)
	private
		startPoint: Point;
		endPoint: Point;
	public
		constructor Create(s,e:Point);
		destructor Destroy; override;
		procedure Draw();
		class function DisplayCreationMenu():Drawable;
	end;

implementation
	constructor Line.Create(s,e:Point);
	begin
		startPoint := s;
		endPoint := e;
	end;

	destructor Line.Destroy;
	begin
		
	end;


	procedure Line.Draw();
	begin
		WriteLn('Line');
		WriteLn('StartPoint');
		WriteLn(startPoint.ToString());
		WriteLn('EndPoint:');
		WriteLn(endPoint.ToString());
	end;	

	class function Line.DisplayCreationMenu():Drawable;
	var s,e:Point; l:Line;
	begin
		ClrScr();
		WriteLn('Line Creation Menu');
		WriteLn('StartPoint');
		s:=Point.CreatePoint();
		WriteLn('EndPoint');
		e:=Point.CreatePoint();
		l:=Line.Create(s,e);
		ClrScr();
		WriteLn('LineCreated');
		l.Draw();
		WriteLn('Continue');
		ReadLn();
		result := l;
	end;
end.