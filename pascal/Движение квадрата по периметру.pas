Program perimrtr;
uses GraphABC;
begin 
var iMoveXBack : integer;//счетчик движения назад
var iMoveX : integer;
var iNumIter : integer;//счетчик для повтора
 for iNumIter:=0 to 3 do begin
    for iMoveX:=0 to 1600 do begin
    ClearWindow();
    Rectangle(100+iMoveX,100,200+iMoveX,200);
    FloodFill(150+iMoveX,150,clBlue);
    end;
       for iMoveX:=100 to 800 do begin
       ClearWindow();
       Rectangle(1700,iMoveX,1800,100+iMoveX);
       FloodFill(1750,50+iMoveX,clBlue);
       end;
          for iMoveXBack:=0 to 1600 do begin
          ClearWindow();
          Rectangle(iMoveX+900-iMoveXBack,800,iMoveX+1000-iMoveXBack,900);
          FloodFill(iMoveX+950-iMoveXBack,850,clBlue);
          end;  
             for iMoveX:=800 to 1500 do begin//цикл движения вверх
             ClearWindow();
             Rectangle(100,iMoveXBack-iMoveX,200,iMoveXBack+100-iMoveX);
             FloodFill(150,iMoveXBack+50-iMoveX,clBlue);
             end; 
  end;
  end.