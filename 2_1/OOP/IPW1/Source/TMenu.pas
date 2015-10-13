unit TMenu;
interface
uses Crt, TRenderQueue, IDrawable, SysUtils, TLine, TPolygon, TPolyline, TRectangle, TEllipse;
type
	Menu = class
	public 
	queue:Renderer;
	createdItems: array of Drawable;
	createdItemsCount: Integer;
	constructor Create();
	destructor Destroy;	
	procedure MainMenu;
	procedure GeometryCreationMenu;
	procedure GeometrySelectionMenu;
end;

implementation
	constructor Menu.Create();
	begin
		queue:=Renderer.Create();
		createdItemsCount:=0;
		SetLength(createdItems,createdItemsCount);
	end;

	destructor Menu.Destroy;
	begin
		
	end;

	procedure Menu.MainMenu();
	var input: Integer; validity,quit: Boolean; 
	begin
		repeat
			ClrScr();
			WriteLn('Geometry App');
			WriteLn('Menu:');
			WriteLn('1.Create Geometry Object');
			WriteLn('2.Enque Geometry Object');
			WriteLn('3.Draw frame');
			WriteLn('4.Quit');
			repeat
				ReadLn(input);
				case input of
					1: begin GeometryCreationMenu(); validity := true; end;
					2: begin GeometrySelectionMenu(); validity := true; end;
					3: begin queue.DrawFrame(); validity := true; end;
					4: begin validity := true; quit := true; end;
					else validity := false;	WriteLn('WrongChar'); end;
			until validity = true;
		until quit = true;
	end;


	procedure Menu.GeometrySelectionMenu();
	var SelectedIndex,input: Integer;validity: Boolean;
	begin
		ClrScr();
		WriteLn('SelectGeometry');
		WriteLn('Items:' + IntToStr(Length(createdItems)));
		repeat
			ReadLn(SelectedIndex);
		until (SelectedIndex >= 0) AND (SelectedIndex < Length(createdItems));
		WriteLn('Item Selected');
		createdItems[SelectedIndex].Draw();
		WriteLn('1.Enqueue Geometry Object');
		WriteLn('2.Back To Menu');
		repeat
			ReadLn(input);
			case input of
				1: begin queue.Enqueue(createdItems[SelectedIndex]); validity := true; end;
				2: begin MainMenu(); validity := true; end;
				else 
					validity := false;
					WriteLn('WrongChar');
			end;
		until validity = true;
	end;

	procedure Menu.GeometryCreationMenu();
	var input: Integer; validity: Boolean;
	begin
		ClrScr();
		WriteLn('CreateGeometry');
		WriteLn('1.Line');
		WriteLn('2.PolyLine');
		WriteLn('3.Rectangle');
		WriteLn('4.Polygon');
		WriteLn('5.Ellipse');
		WriteLn('6.Back To Menu');
		repeat
		ReadLn(input);
		case input of
			1: begin createdItemsCount += 1; SetLength(createdItems,createdItemsCount); createdItems[createdItemsCount-1] := Line.DisplayCreationMenu(); validity:=true; end;
			2: begin createdItemsCount += 1; SetLength(createdItems,createdItemsCount); createdItems[createdItemsCount-1] := Polyline.DisplayCreationMenu();  validity:=true;  end;
			3: begin createdItemsCount += 1; SetLength(createdItems,createdItemsCount); createdItems[createdItemsCount-1] := Rectangle.DisplayCreationMenu();  validity:=true; end;
			4: begin createdItemsCount += 1; SetLength(createdItems,createdItemsCount); createdItems[createdItemsCount-1] := Polygon.DisplayCreationMenu();  validity:=true; end;
			5: begin createdItemsCount += 1; SetLength(createdItems,createdItemsCount); createdItems[createdItemsCount-1] := Ellipse.DisplayCreationMenu();  validity:=true;  end;
			6: begin MainMenu(); validity := true; end;
			else validity := false; WriteLn('WrongChar');end;
		until validity = true;
	end;
end.