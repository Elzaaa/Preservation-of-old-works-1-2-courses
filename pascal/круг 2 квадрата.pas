Program dfach;
uses GraphABC;
  begin
   var jMoveYDown : integer =0;// переменная для движения вниз
   var iMoveX : integer;// переменная для движения вперед 
  
    //цикл движеня вперед
   for iMoveX:=300 to 1000 do begin
     jMoveYDown := jMoveYDown+1;
     ClearWindow();
     Circle(0+iMoveX*2,600,100);
     FloodFill(20+iMoveX*2,600,clred);
   
       ClearWindow();
       Rectangle(400,jMoveYDown,600,jMoveYDown+200);
       FloodFill(450,jMoveYDown+50,clMoneyGreen);  
         Rectangle(800,jMoveYDown,1000,jMoveYDown+200);
         FloodFill(850,jMoveYDown+50,clMoneyGreen); 
         if jMoveYDown=700 then break
      end;
end.