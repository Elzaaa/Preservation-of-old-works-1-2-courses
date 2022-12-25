Program Ural;
uses GraphABC;
var x,y : integer;// variables
procedure KeyDown (key : integer);

begin
case key of// motion control 
  VK_Up : y := y-3;//movement up
  VK_Down : y := y+3;//movement down
  VK_Right : x := x+3;//movement right
  VK_Left: x := x-3;//movement left
  end;
  Window.Clear;
  end;
  begin
  lockDrawing;
  x := 300;
  y := 300;
  repeat
    redraw;
    sleep (1);
    circle (x,y,50);
    onKeyDown:=KeyDown;
  until(false);
end.