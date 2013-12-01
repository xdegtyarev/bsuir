Unit TPoint;
interface
uses SysUtils,Crt;
type
	Point = class(TObject)
		public
		x: Double;
		y: Double;
		constructor Create(_x,_y:Double);
		function ToString(): String;
		class function CreatePoint(): Point;
	end;

implementation
	constructor Point.Create(_x,_y:Double);
		begin
			x := _x;
			y := _y;
		end;

	function Point.ToString(): String;
	begin
		result := 'y:' + FloatToStr(x) + ' x:' + FloatToStr(y)
	end;

	class function Point.CreatePoint(): Point;
	var _x,_y:Double; p:Point;
		begin
			WriteLn('X:');
			ReadLn(_x);
			WriteLn('Y:');
			ReadLn(_y);
			p := Point.Create(_x,_y);
			// ClrScr();
			// WriteLn('PointCreated');
			// WriteLn(p.ToString);
			// WriteLn('Continue');
			// ReadLn();
			result := p;
		end;	
end.