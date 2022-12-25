Program kvadrat;
uses GraphABC;
begin
  var iMoveX : integer;
  for iMoveX:=0 to 300 do begin
    ClearWindow();
    Rectangle(100+iMoveX,100,400+iMoveX,400);
    FloodFill(200+iMoveX,200,clBlue);
  end
end.