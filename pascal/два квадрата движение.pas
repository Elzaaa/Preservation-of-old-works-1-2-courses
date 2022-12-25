Program kvadrat;
uses GraphABC;
begin
  var iMoveX : integer;
  for iMoveX:=0 to 300 do begin
    //очистка экрана
    ClearWindow();
    
    //одна фигура
    Rectangle(100+iMoveX,100,200+iMoveX,200);
    FloodFill(150+iMoveX,150,clBlue);
    
    //вторая фигура
    Rectangle(0+iMoveX*2,300,100+iMoveX*2,400);
    FloodFill(50+iMoveX*2,350,clRed); 
  end
end.