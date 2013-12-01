unit TRenderQueue;

interface

uses IDrawable, SysUtils;

type
	Renderer = class(TObject)
	private
		RenderQueue: array of Drawable;
		queueLength: Integer;
	public
		constructor Create;
		destructor Destroy; override;
		procedure Enqueue(drawable: Drawable);
		procedure DrawFrame;
	end;

implementation
	
	constructor Renderer.Create();	
	begin
		queueLength:=0;
		SetLength(RenderQueue,0);
	end;

	destructor Renderer.Destroy();
	begin
		SetLength(RenderQueue,0);
	end;

	procedure Renderer.Enqueue(drawable: Drawable);
	begin
		queueLength+=1;
		SetLength(RenderQueue,queueLength);
		RenderQueue[queueLength-1]:=drawable;
	end;

	procedure Renderer.DrawFrame();
	var 
	i: Integer;	obj: Drawable;
	begin
		if(queueLength>0)then
		begin
		for i := 0 to queueLength-1 do
		begin
			WriteLn('Pass' + IntToStr(queueLength));
			obj := RenderQueue[i];
			obj.Draw();
		end;		
		end;
		WriteLn('Done Rendering');
		WriteLn('Continue');
		ReadLn();
	end;
end.

