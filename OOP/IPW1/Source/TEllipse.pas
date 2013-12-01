unit TEllipse;

interface

uses TPoint,IDrawable,Crt;

type
	Ellipse = class(TInterfacedObject,Drawable)
	private
		center: Point;
		Axis: Point;
	public
		constructor Create(c: Point; a: Double; b:Double);
		destructor Destroy; override;
		procedure Draw();
		class function DisplayCreationMenu():Drawable;
	end;

implementation
	
	constructor Ellipse.Create(c: Point; a: Double; b:Double);
	begin
		center := c;
		Axis := Point.Create(a,b);
	end;

	destructor Ellipse.Destroy();
	begin
	end;

	procedure Ellipse.Draw();
	begin
		WriteLn('Ellipse');
		WriteLn('CenterPoint');
		WriteLn(center.ToString());
		WriteLn('Axis:');
		WriteLn(Axis.ToString());
	end;	

	class function Ellipse.DisplayCreationMenu():Drawable;
	var c,a:Point; e:Ellipse;
	begin
		ClrScr();
		WriteLn('Ellipse Creation Menu');
		WriteLn('CenterPoint');
		c:=Point.CreatePoint();
		WriteLn('Axis');
		a:=Point.CreatePoint();		
		e := Ellipse.Create(c,a.x,a.y);
		ClrScr();
		WriteLn('EllipseCreated');
		e.Draw();
		WriteLn('Continue');
		ReadLn();
		result := e;
	end;
end.