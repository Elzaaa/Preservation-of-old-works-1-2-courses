program shahmati_plus;
uses GraphABC;
 var x,y: integer;
 var v: integer;//проверка на нечетность
 var z: integer;//переменная чисел от 1 до 8
 var с: integer;//переменная отвечающая за ось Y в цикле для цисел

 begin//начало цикла
    rectangle (200,200,600,600);//рамка
 begin//цикл посторения цифр слева
  for z:=1 to 8 do begin
   с:=с+50;
    TextOut(185,с+166,chr(48+z));// цифры слева
  end;
 end;
 begin//цикл посторения цифр справа
  for z:=1 to 8 do begin
   с:=с+50;
    TextOut(607,с-233,chr(48+z));// цифры справа
  end;
 end;
 begin//цикл букв сверху
  for z:=1 to 8 do begin
   y:=y+50;
    TextOut(y+170,179,chr(64+z));// буквы сверху
  end;
 end;
 begin//цикл букв снизу
  for z:=1 to 8 do begin
   y:=y+50;
    TextOut(y-230,604,chr(64+z));// буквы снизу
  end;
 end;
//цикл сетки 8*8 квадратов
     for x:=0 to 7 do begin
            for y:=0 to 7 do begin
            v := x+y;
              if v mod 2=0 then begin setbrushcolor(clBlack); //если число нечетное, то цвет заливки черный
              rectangle (x*50+200,y*50+200,x*50+50+200,y*50+50+200);//количество растояния между пикселями 
              end
              else begin setbrushcolor(clwhite);//если число четное, то закрашиваем белым
               rectangle (x*50+200,y*50+200,x*50+50+200,y*50+50+200);//количество растояния между пикселями 
              end;
            end;
     end;
end.