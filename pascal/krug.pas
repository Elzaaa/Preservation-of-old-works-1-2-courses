uses graphABC;
var N, K, H, X1, Y1, X2, Y2, X3, Y3, X4, Y4: integer; 
begin 
  
N:=450; 
K:=350; 
H:=220; 
X1:=1500; 
Y1:=700; 
X2:=X1-N; 
Y2:=Y1; 
X3:=X2-(K-N)div 2; 
Y3:=Y1+H; 
X4:=X1+(K-N)div 2; 
Y4:=Y3; 
SetWindowSize(400,250); 
Line (X1, Y1, X2, Y2); 
Line (X2, Y2, X3, Y3); 
Line (X3, Y3, X4, Y4); 
Line (X4, Y4, X1, Y1);

 begin
 Rectangle(0,700,400,1200);
 FloodFill(200,800,clGreen)
 end;
     begin
     Rectangle(400,921,1500,2000);
      FloodFill(1000,1010,clSkyBlue)
     end;
       begin
       Rectangle(1500,700,3000,1200);
        FloodFill(1600,800,clYellow)
       end;
         begin
         Circle(200,600,100);
         FloodFill(190,580,clred);
         end;
end.