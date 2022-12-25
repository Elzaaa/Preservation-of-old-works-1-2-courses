program shahmati;
uses GraphABC;
var z,y: integer;
begin
 for z:=1 to 8 do begin
   y:=y+50;
    TextOut(y,50,chr(64+z));// цифры слева
end;
writeln ('1');
end.