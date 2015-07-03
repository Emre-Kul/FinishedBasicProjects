program GAME_2048; {$mode objfpc}
uses crt,sysutils;
var
    game,undo : array [1..16] of integer;
    temp : array [1..4] of integer;
    m,i,j,zero,point,rand,highscore,undopoint,undozero,randc:integer;
    moved,finished,undoned,updated,win: boolean;
    key : char;
Procedure writeScoreTofile();
var
   dbfile:textfile;
Begin
  AssignFile(dbfile, '2048DB.db'); {$I+}
  try
    Rewrite(dbfile);
    write(dbfile,highscore);
    CloseFile(dbfile);
  except
    on E: EInOutError do
  end;
End;
Procedure readScoreFromFile();
var
   dbfile:textfile;
Begin
  AssignFile(dbfile, '2048DB.db');{$I+}
  try
    Reset(dbfile);
    Readln(dbfile,highscore);
    CloseFile(dbfile);
  except
    on E: EInOutError do Begin
    writeScoreTofile();
    End;
  end;
End;
Procedure createUndo();
var k:integer;
Begin
    if undoned = false then Begin
       for k:=1 to 16 Do undo[k] := game[k];
       undoned:= true;
       undopoint := point;
       undozero := zero;
    End;
End;
Procedure generateRandom();
var  randfor4 : integer;
Begin
    randc := -1;
    if moved then Begin
        zero := zero - 1;
        moved := false;
        rand := Random(zero)+1;
        randfor4 := Random(10);
        undoned := false;
     End else rand := -1;
     for i:=1 to 16 Do Begin
             if game[i] = 0 then Begin
                  rand := rand - 1;
                  if rand = 0 then Begin
                    randc := i;
                    if randfor4 = 0 then game[i] := 4 {%10 ihtimal 4}
                    else game[i] := 2; {%90 ihtimal 2}
                  End;
             End;
      End;
End;
Procedure initGame();
begin
   for i := 1 to 16 Do Begin
       game[i] := 0;
       undo[i] := 0;
   End;
   m := 0; point := 0; rand := 0;
   zero := 16;
   rand := Random(zero)+1;
   game[rand] := 2;
   moved := true; finished := false; undoned := false; updated := false; win := false;
   createundo();
   readscorefromfile();
   if(highscore > 999999) then  highscore := 0;
End;
Procedure isgameFinish();
var
   m,n:integer;
   fGame : array  [0..3,0..3] of integer;
Begin
    finished := true;
    for m:= 1 to 16 Do fgame[(m-1) div 4][(m-1)mod 4] := game[m];
    for m:= 0 to 2 Do Begin
    n:= 0;
        while(n < 4) and (finished) Do Begin
             if(fgame[m][n] = fgame[m+1][n]) or (fgame[m+1][n] = 0) or (fgame[m][n] = 0) then finished := false;
             n := n + 1;
        End;
    End;
    if (finished) then Begin
       for n:= 0 to 2 Do Begin
       m:= 0;
           while(m < 4) and (finished) Do Begin
                     if(fgame[m][n] = fgame[m][n+1]) or (fgame[m][n] = 0) or (fgame[m][n+1] = 0) then finished := false;
                     m := m + 1;
           End;
       End;
    End;
End;
Procedure drawScreen(scr:integer);
Var
   T : String;
Begin
  T := 'Score : ' + inttostr(point) + '   High Score : ' + inttostr(highscore);
  clrscr();
  TextColor(White);
  if scr = 0 then Begin {Quit Screen }
      gotoXY(64,1);Write('Coded By Emre Kul');
      gotoXY(30,2);
      Write('Good Bye...');
      Delay(1000);
  End;
  if scr = 1 then Begin {Game Screen}
  gotoxy(25,2);
  Write(T);
  writeln();
  writeln();
  for i:=0 to 4 Do Begin
  gotoxy(15,i*3+5);
  write('______________________ | ______________________');
  End;
      writeln();
     for i:=1 to 16 Do Begin
             if (game[i] > 0) or (undo[i] > 0) then Begin
                if (randc = i) and (game[i] = 2) then textcolor(12)
                else if (randc = i) and (game[i] = 4) then textcolor(BLINK)
                else if (game[i] = 2) then textcolor(15)
                else if(game[i] = 4) then textcolor(14)
                else if(game[i] = 8) then textcolor(2)
                else if(game[i] = 16) then textcolor(3)
                else if(game[i] = 32) then textcolor(4)
                else if(game[i] = 64) then textcolor(5)
                else if(game[i] = 128) then textcolor(6)
                else if(game[i] = 256) then textcolor(9)
                else if(game[i] = 512) then textcolor(10)
                else if(game[i] = 1024) then textcolor(1)
                else textcolor(12); {colors}
                if game[i] > 0 then Begin
                   gotoxy(40 + ((i-1) mod 4)*6,((i-1) div 4)*3 + 7);
                   write(game[i],' ');
                End;
                if undo[i] > 0 then  Begin
                   textcolor(WHITE);
                   gotoxy(15 + ((i-1) mod 4)*6,((i-1) div 4)*3 + 7);
                   write(undo[i],' ');
                End;
                if(i mod 4) = 0 then writeln();
             End;
   End;
   textColor(15);
   gotoxy(23,23);
   writeln('New Game : Enter        Quit : ESC     ');
   writeln();writeln();
   textColor(15);write('***Controls are ');textcolor(2); writeln('W,A,S,D or Arrow Keys');
   textColor(15);write('***Press');textcolor(12);write(' BACKSPACE ');textColor(15);writeln('Key to Undo');
   textColor(15);writeln('***There is %10 chance of spawning 4 and %90 chance of spawning 2.');
   write('***When random number spawns they look like ');
   textColor(BLINK);write('4');textcolor(15);write(' and ');textcolor(12);write('2');
   if finished then Begin
        gotoxy(35,21);
        textColor(12);
        write('Cant Move...');
    End;
    if win then Begin
        gotoxy(35,19);
        textColor(2);
        write('You Won...');
    End;
    gotoxy(1,1);
  End;
End;
Function listenInput():boolean;
var
   res: boolean;
begin
   res := false;
   key := readkey();
   if key = #27 then res := true;{quit}
   if key = #13 then initgame(); {enter}
   if (key = #08) and (finished = false) then Begin
              for i:= 1 to 16 Do game[i] := undo[i];
              point := undopoint;
              zero := undozero;
              drawscreen(1);
              End;
    listenInput := res;
End;
Procedure Move();
 var mi,mj:integer;
 var indexes : array [1..4] of  integer;
 Begin
	 for mi := 1 to 4 Do Begin
         indexes[mi] := temp[mi];
         temp[mi] := game[temp[mi]];
     End;
	 for mi := 1 to 4 Do Begin
		mj := mi;
		while (mj <= 4) and (temp[mj] = 0) Do mj := mj+1;
		if(mj > mi) and (mj <= 4) then Begin
            createundo();
			temp[mi] := temp[mj];
			temp[mj] := 0;
			if(temp[mi] > 0) then moved := true;	
		 End; 
		 if( mj = mi) and (mj <= 4) then mj := mj+1;
		 while(mj <= 4) and (temp[mj] = 0) Do mj := mj + 1;
		 if(mj <= 4) then
		 if(temp[mi] = temp[mj]) and (temp[mi] > 0) then Begin
                createundo();
				temp[mi] := 2*temp[mi];
				temp[mj]  := 0;
				zero := zero + 1;
				moved := true;
                if(temp[mi] >= 2048) then win := true;{2048 i görürse kazanır}
				point := point + temp[mi];
		 End;
	 End;
	 for mi := 1 to 4 Do game[indexes[mi]] := temp[mi];
End;
Procedure update();
 Begin
 if (key = 'w') or (key = 'W') or (key = #72) then Begin {1,5,9,13 - 2,6,10,14 - 3,7,11,15 - 4,8,12,16}
	for i := 1 to 4 Do Begin
	        m := 1;
			j := i;
			while(j <= i+12) Do Begin
			temp[m] := j;
			j := j+4; 
			m := m+1;
			End;
			Move();
	End;
 End
 else if (key = 's') or (key = 'S') or (key = #80)  then Begin {13,9,5,1 - 14,10,6,2 - 16,11,7,3 - 16,12,8,4}
	for i := 1 to 4 Do Begin
		m := 1;
        j := 12 + i;
		while(j >= i) Do Begin
		temp[m] := j;
		j := j - 4;
		m := m + 1;
		End;
		Move();
	End;
 End
 else if (key = 'd') or (key = 'D') or (key = #77) then Begin {4,3,2,1 - 8,7,6,5 - 12,11,10,9 - 16,15,14,13}
	for i := 1 to 4 Do Begin
	    m := 1;
        j := i*4;
		while j >= ( (i-1) * 4 + 1)  Do Begin
		temp[m] := j;
		j := j-1;
		m := m + 1;
		End;
		Move();
	End;
 End
 else if (key = 'a') or (key = 'A') or (key = #75)  then Begin {1,2,3,4 - 5,6,7,8 - 9,10,11,12 - 13,14,15,16}
	for i := 1 to 4 Do Begin
	        m := 1;
            j := (i-1)*4 + 1;
			while(j <= (i*4) ) Do Begin
			temp[m] := j;
			j := j+1;
			m := m + 1;
			End;
			Move();
	End;	
 End;
 if (zero <= 1) then isgamefinish();{0 kalmadıysa bitmişmi diye kontrol et}
 if (moved) or (finished) then
 Begin
 updated := true;
 generateRandom();
 End;
End;
begin
initGame();
repeat
  if(not finished) then update();
  if (updated) then Begin
   if(highscore <= point) then
      Begin
               writescoretofile();
               highscore := point;
      End;
  drawScreen(1);{game screen}
  updated := false;
  End;
  Delay(1);
until listenInput();
  drawScreen(0);{quit screen}
end.
