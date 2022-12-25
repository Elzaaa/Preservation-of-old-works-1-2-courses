program shahmati;
uses GraphABC;
 var x,y: integer;
 var v: integer;//проверка на нечетность
 var z: integer;//проверка на нечетность
 var c: integer;//проверка на нечетность
 begin//начало цикла
 rectangle (200,200,600,600);//рамка
  textout (220,180,'a            b           c            d           e            f            g           h');//буквы сверху
  textout (220,605,'a            b           c            d           e            f            g           h');//буквы снизу
    textout (185,215,'1');// цифры слева
    textout (185,265,'2');
    textout (185,315,'3');
    textout (185,365,'4');
    textout (185,415,'5');
    textout (185,465,'6');
    textout (185,515,'7');
    textout (185,565,'8');
       textout (610,215,'1');// цифры справа
       textout (610,265,'2');
       textout (610,315,'3');
       textout (610,365,'4');
       textout (610,415,'5');
       textout (610,465,'6');
       textout (610,515,'7');
       textout (610,565,'8');
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