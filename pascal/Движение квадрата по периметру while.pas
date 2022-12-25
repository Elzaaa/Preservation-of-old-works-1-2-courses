Program perimrtr;
uses GraphABC;
begin 
var iMoveX : integer;//счетчик движения x
var iMoveY : integer;//счетчик движения y
    iMoveX :=0;
    while iMoveX<=1600 do begin//цикл движения вперед
        iMoveX := iMoveX+5;
        ClearWindow();
        Rectangle(100+iMoveX,100,200+iMoveX,200);
        FloodFill(150+iMoveX,150,clBlue);
    end;
    iMoveY :=100;
    while iMoveY<= 800 do begin//цикл движения вниз
       iMoveY := iMoveY+5;
       ClearWindow();
       Rectangle(1700,iMoveY,1800,iMoveY+100);
       FloodFill(1750,iMoveY+50,clBlue);
     end;
    iMoveX:=1900; 
    while iMoveX >= 300 do begin//цикл движения назад
        iMoveX := iMoveX-5;
        ClearWindow();
        Rectangle(iMoveX-100,800,iMoveX-200,900);
        FloodFill(iMoveX-150,850,clBlue);
     end;
    iMoveY := 1000;;
    while iMoveY>=300 do begin//цикл движения вверх
    iMoveY := iMoveY-5;
        ClearWindow();
        Rectangle(100,iMoveY-100,200,iMoveY-200);
        FloodFill(150,iMoveY-150,clBlue);
    end;   
  end.