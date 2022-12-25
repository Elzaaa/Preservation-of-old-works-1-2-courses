uses graphAbc;
var
  xc,yc,xr,yr:integer;
begin
  xc:=10;yc:=20;
  xr:=Window.Width-90;yr:=40;
  for i:integer:=0 to Window.Width do begin
    SetPenColor(clBlue);
    Rectangle(xr-i,yr,xr+50-i,yr+40);
    circle(xc+i,yc,10);
    UnlockDrawing;
    Sleep(10);
    LockDrawing;
    ClearWindow;
  end;  
end.