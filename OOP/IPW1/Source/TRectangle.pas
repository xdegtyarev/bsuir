Unit TRectangle;
interface
uses TPoint,IDrawable,SysUtils,Crt;

type
	Rectangle = class(TInterfacedObject,Drawable)
	private
		lowerLeft, upperRight: Point;
	public
		constructor Create(ll,ur:Point);
		destructor Destroy; override;
		procedure Draw;
		class function DisplayCreationMenu():Drawable;
	end;

implementation
	
	constructor Rectangle.Create(ll,ur:Point);
	begin
		lowerLeft := ll;
		upperRight := ur;
	end;

	destructor Rectangle.Destroy();
	begin
	end;


	procedure Rectangle.Draw();
	begin
		WriteLn('Rectangle');
		WriteLn('LowerLeft:' + lowerLeft.ToString());
		WriteLn('UpperRight:' + upperRight.ToString());
	end;	


	class function Rectangle.DisplayCreationMenu():Drawable;
	var ll,ur:Point; rect:Rectangle;
	begin
		ClrScr();
		WriteLn('Rectangle Creation Menu');
		WriteLn('Lower Left Point');
		ll:=Point.CreatePoint();
		WriteLn('Upper Right');
		ur:=Point.CreatePoint();
		rect := Rectangle.Create(ll,ur);
		ClrScr();
		WriteLn('Rectangle created');
		rect.Draw();
		WriteLn('Continue');
		ReadLn();
		result := rect;
	end;
end.

