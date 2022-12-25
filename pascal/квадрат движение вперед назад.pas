Program kvadrat;
uses GraphABC;
begin
  var iMoveX : integer;//переменная движения вперед
  var iMoveXBack : integer;//счетчик движения назад
  var iNumIter : integer;//счетчик для повтора
  //цикл повтора
  for iNumIter:=0 to 7 do begin
    //цикл движеня вперед
    for iMoveX:=100 to 400 do begin
      ClearWindow();
      Rectangle(iMoveX,100,iMoveX+100,200);
      FloodFill(iMoveX+50,150,clBlue);
    end;
    //цикл движения назад
    for iMoveXBack:=0 to 300 do begin
      ClearWindow();
      Rectangle(iMoveX-iMoveXBack,100,iMoveX+100-iMoveXBack,200);
      FloodFill(iMoveX+50-iMoveXBack,150,clBlue);
    end;
   end;
end.