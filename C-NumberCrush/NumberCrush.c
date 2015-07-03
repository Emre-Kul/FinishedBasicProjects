/* 
Coder Name : EMRE KUL
App Name : NUMBER CRUSH
Controls : W,A,S,D,ENTER,ESC
*/ 
#include <time.h>
#include <stdio.h>
#include <windows.h>

#define HEART 5
#define MENU_SIZE 5
#define BOARD_MAX_SIZE 25
#define STRUCTURE_I 0
#define STRUCTURE_T 1
#define STRUCTURE_L 2
#define STRUCTURE_F 3
#define STRUCTURE_C 4
#define STRUCTURE_U 5
#define STRUCTURE_E 6
#define STRUCTURE_O 7
                         //STRUCTS//
//I,T,L,F,C,U,E,O
//MYS = MyStructs kısaltması
struct MYS_Score{
       long int score,heart;
       char name[256];
};
struct MYS_Structure{
       int probability,type[9],id,point;     
       char name;  
       };
struct MYS_Settings{ 
       int matrixSize,range[2],numberOfStructures;
       struct MYS_Structure structures[8];
       struct MYS_Score player;
};
struct MYS_Button{
       int posx,posy,clicked; 
       char text[32]; 
       };
struct MYS_Menu{
       struct MYS_Button buttons[MENU_SIZE];
       int focusedButtonIndex,buttonSize,left,top,textSize;
       };
struct MYS_GameArea{
       char Board[BOARD_MAX_SIZE*BOARD_MAX_SIZE],picked;
       int focusedIndex[2];
       struct MYS_Settings settings;
       };                      
                          //Struct Func
void MYS_Score_SetScore(struct MYS_Score*,long int,char[],int);
void MYS_Menu_SetMenu(struct MYS_Menu*,int,int,int);
void MYS_Menu_AddToMenu(struct MYS_Menu*,char[]);
void MYS_Menu_SetClick(struct MYS_Menu*,int);
void MYS_Button_SetButton(struct MYS_Button*,char[],int,int,int);
void MYS_Settings_CreateStructures(struct MYS_Settings*,int*,int);
                         //Menu Func
void menuInput(struct MYS_Menu*,char);
                        //Drawer Func
void gotoxy(int,int);
void textColor(int);
void drawMenu(struct MYS_Menu * );
void drawGameArea();
                        //Game Func
void gameInit();
void gameRun();
int searchForStructures();
int controlStructure(char*,int,int);
void setStructuresArray(int*,int,int,int,int,int,int,int,int,int);
void clearStructure(char*,int,int);
void slideBoard();
                        //File Func
void SaveScore();
void ReadScore();


void Init(); 
                               
const char MY_FILENAME[32] =  "numberCrush.OPT\0";
char Message[2048];
struct MYS_GameArea Board;
struct MYS_Menu MainMenu,SettingsMenu;
struct MYS_Score HighScore;

void clearStructure(char *tmpBoard,int tmpIndex,int structureIndex){
int i,j,m,size,swap;
char tmpText[256];
size = Board.settings.matrixSize;
j = tmpIndex;
j -= (size+4)*2;
j -= (j/(size+4)*2) + ((j/(size+4)+1)*2);

sprintf(tmpText,"You Hit The %c Structure and Won %d point Index : %d\n",Board.settings.structures[structureIndex].name,Board.settings.structures[structureIndex].point,j);//62
if(strlen(Message) + strlen(tmpText) < 2048)strcat(Message,tmpText);
else strcpy(Message,tmpText);
for(i = 0;i < 9;i++){
	if(Board.settings.structures[structureIndex].type[i])tmpBoard[tmpIndex] = 0;
	if(i%3 == 2)tmpIndex += (size+2);
	else tmpIndex++;
}
tmpIndex = j;

for(i = 0;i < 9;i++){
	if(Board.settings.structures[structureIndex].type[i])Board.Board[j] = 0;
	if(i%3 == 2)j += (size-2);
	else j++;
}
}
int main(int argc, char *argv[]){ 
  char esc,tmp,strInputs[8] = {'I','T','L','F','C','U','E','O'};
  int i,j,activeStructures[8];
  Init();
  while(1){
           char c,c2;
           if(MainMenu.buttons[0].clicked){//PLAY button
                                           gameInit();
                                           gameRun();
                                           MainMenu.buttons[0].clicked = 0;
                                           }
           else if(MainMenu.buttons[1].clicked){//BEST SCORE button
                printf("Name : %s   Best Score : %d",HighScore.name,HighScore.score);
                getch();
                MainMenu.buttons[1].clicked = 0;
                }
           else if(MainMenu.buttons[2].clicked){//SETTINGS button
                if(SettingsMenu.buttons[0].clicked){//MATRIXSIZE button
                     printf("Current Matrix Size : %d",Board.settings.matrixSize);
                     printf("\nPress Enter To Change...");
                     c2 = getch();
                     if(c2 == 13) {
                           printf("\nNew Size : ");
                           scanf("%d",&Board.settings.matrixSize);
						   if(Board.settings.matrixSize > BOARD_MAX_SIZE)Board.settings.matrixSize = BOARD_MAX_SIZE;
						   else if(Board.settings.matrixSize < 5)Board.settings.matrixSize = 5;
                           printf("Current Matrix Size : %d",Board.settings.matrixSize);
                           getch();
                     }
                     SettingsMenu.buttons[0].clicked = 0;
                     }
                else if(SettingsMenu.buttons[1].clicked){//RANGE button
                     printf("Number Range : %d , %d",Board.settings.range[0],Board.settings.range[1]);
                     printf("\n(Difference must be greater then 2 <2,10>)\nPress Enter To Change...");
                     c2 = getch();
                     if(c2 == 13) {
                           printf("\nRange Begin : ");
                           scanf("%d",&Board.settings.range[0]);
						   if(Board.settings.range[0] < 2)Board.settings.range[0] = 2;
                           printf("Range End : ");
                           scanf("%d",&Board.settings.range[1]);
						   if(Board.settings.range[1] > 10)Board.settings.range[1] = 10;
						   if(Board.settings.range[0] > Board.settings.range[1] - 2){
						   Board.settings.range[0] = 2;
						   Board.settings.range[1] = 10;
						   }
                           printf("\nNew Number Range : %d , %d",Board.settings.range[0],Board.settings.range[1]);
                           getch();
                      }
                     SettingsMenu.buttons[1].clicked = 0;
                     }
                else if(SettingsMenu.buttons[2].clicked){//STRUCTURE button

				     printf("Active Structures : ");
				     for(i = 0;i < Board.settings.numberOfStructures;i++)printf(" %c ",Board.settings.structures[i].name);
                     printf("\nAvaliable Structures : I T L C E F O U");
                     printf("\nPress Enter To Change...");
                     c2 = getch();
                     if(c2 == 13) {
					       printf("\n\nThe characters you used will be saved up till you use different ones\n\n");//ingilizceye çevir
					       i = 0;
						   esc = 0;
						   while(i < 8 && !esc){
								j = 0;
								printf("Enter Structure : ");
								scanf(" %c",&tmp);
								while(j < 8 && strInputs[j] != tmp && strInputs[j] != (tmp-'a'+'A') )j++;
								if(j < 8)activeStructures[i] = j;
								else esc = 1;
								i++;
						   }
						   if(i > 1){
								if(!esc)MYS_Settings_CreateStructures(&Board.settings,activeStructures,i);
								else MYS_Settings_CreateStructures(&Board.settings,activeStructures,i-1);
						   }
						   printf("Active Structures : ");
						   for(i = 0;i < Board.settings.numberOfStructures;i++)printf(" %c ",Board.settings.structures[i].name);
						   getch();
					 }
                     SettingsMenu.buttons[2].clicked = 0;
                     }
                else if(SettingsMenu.buttons[3].clicked){//PLAYER NAME button
					 char name[32];
					 printf("Current Name : %s",Board.settings.player.name);
                     printf("\n(Name Length must be greater then 2)\nPress Enter To Change...");
                     c2 = getch();
                     if(c2 == 13) {
                           printf("\nNew Name : ");
                           gets(name);
						   if(strlen(name) > 2)
                           strcpy(Board.settings.player.name,name);
						   printf("Current Name : %s",Board.settings.player.name);
						   SaveScore();
                           getch();
                     }
                     SettingsMenu.buttons[3].clicked = 0;
				
				}
				else if(SettingsMenu.buttons[4].clicked){//BACK button
                     SettingsMenu.buttons[4].clicked = 0;
                     MainMenu.buttons[2].clicked = 0;
                     MainMenu.focusedButtonIndex = 0;
                     }
                else{
                     drawMenu(&SettingsMenu);
                     c = getch();
                     if(c == 27){//BACK button
                          SettingsMenu.buttons[4].clicked = 0;
                          MainMenu.buttons[2].clicked = 0;
                          MainMenu.focusedButtonIndex = 0;
                          }
                     else menuInput(&SettingsMenu,c);
                     }
                }
           else if(MainMenu.buttons[3].clicked){//EXIT button
                printf("CODED BY EMRE KUL");
                getch();
                return;
                }
           else {//w s enter key
                drawMenu(&MainMenu);
                c = getch();
                menuInput(&MainMenu,c);
                } 
           system("cls");    
   }
  return 0;
}
void Init(){ 

  int activeStructures[8] = {7,6,2,3,4,5,1,0};
  
  srand(time(0));
  MYS_Menu_SetMenu(&MainMenu,35,3,10);
  MYS_Menu_SetMenu(&SettingsMenu,35,3,10);
  
  MYS_Menu_AddToMenu(&MainMenu,"PLAY\0");
  MYS_Menu_AddToMenu(&MainMenu,"BEST SCORE\0");
  MYS_Menu_AddToMenu(&MainMenu,"SETTINGS\0"); 
  MYS_Menu_AddToMenu(&MainMenu,"EXIT\0");
  
  
  MYS_Menu_AddToMenu(&SettingsMenu,"MATRIX SIZE\0");
  MYS_Menu_AddToMenu(&SettingsMenu,"NUMBER RANGE\0");
  MYS_Menu_AddToMenu(&SettingsMenu,"STRUCTRES\0"); 
  MYS_Menu_AddToMenu(&SettingsMenu,"PLAYER NAME\0");
  MYS_Menu_AddToMenu(&SettingsMenu,"BACK\0");
  
  MYS_Score_SetScore(&Board.settings.player,0,"NO NAME\0",HEART);
  MYS_Score_SetScore(&HighScore,0,"NO NAME\0",HEART);
  
  ReadScore(); 
  SaveScore();
  
  Board.settings.matrixSize = 5;
  Board.settings.range[0] = 2;
  Board.settings.range[1] = 10;
  
  MYS_Settings_CreateStructures(&Board.settings,activeStructures,8);
  }
void gameInit(){
     int i,j,size,turn;
   
	 size = Board.settings.matrixSize;
     for(i = 0;i < size;i++) for(j = 0;j < size;j++)Board.Board[size*i+j] = rand()%(Board.settings.range[1]-Board.settings.range[0])+Board.settings.range[0];
     
	 if(size%2 == 1)Board.focusedIndex[0] = size*size/2;
     else Board.focusedIndex[0] = size*size/2 + size/2;
     
	 
	 
	 turn = 0;
	 while(searchForStructures() && turn < 100){
	 for(i = 0;i < size*size;i++){
		if(!Board.Board[i]) Board.Board[i] = rand()%(Board.settings.range[1]-Board.settings.range[0])+Board.settings.range[0];
	 }
	 turn ++; 
	 }//buldu
	 for(i = 0;i < size*size;i++){
		if(!Board.Board[i]) Board.Board[i] = rand()%(Board.settings.range[1]-Board.settings.range[0])+Board.settings.range[0];
	 }
    //Başlangıçta structure bulursa bunu bozuyor çok iyi bir algoritma değil uç noktalarda structureların bazılarını atmıyor
	
	 Board.focusedIndex[1] = Board.focusedIndex[0];
     Board.picked = 0; 
     Board.settings.player.score = 0;
     Board.settings.player.heart = HEART;	 
     sprintf(Message," ");
	 //sleep(3000);
     }
void gameRun(){
     char c1,c2;
     int swap,matrixSize,focus0,focus1,finded,clsScreen;    
	 clsScreen = 1;
     while(Board.settings.player.heart > 0){
						  
                          matrixSize = Board.settings.matrixSize;
                          focus0 = Board.focusedIndex[0];
                          focus1 = Board.focusedIndex[1];
                          
                          if(clsScreen){
										system("cls");
										drawGameArea();
										clsScreen = 0;	
						  }
						  c1 = getch();
                          if(c1 == 27) {
                               system("cls");
                               printf("Go Back Main Menu?(ENTER KEY)");
                               c2 = getch();
                               if(c2 == 13)Board.settings.player.heart = 0;       
                               clsScreen = 1;
							   }
                           else if(c1 == 'd'){
                                if(Board.picked){
                                                 if(focus0 == focus1 && focus1/matrixSize == (focus1+1)/matrixSize){
														   clsScreen = 1;
														   Board.focusedIndex[1] = focus0+1;
														   }
                                                 else if(focus1 == focus0-1){
                                                           Board.focusedIndex[1] = focus0;
														   clsScreen = 1;
														   }
                                                 }
                                else if(focus0/matrixSize == (focus0+1)/matrixSize) {Board.focusedIndex[0]++;clsScreen = 1;}
                                 }
                           else if(c1 == 'a'){
                               if(Board.picked){
                                                 if(focus0 == focus1 && focus1/matrixSize == (focus1-1)/matrixSize && focus0 > 0){
														   clsScreen = 1;
                                                           Board.focusedIndex[1] = focus0-1;
														   }
                                                 else if(focus1 == focus0+1){
                                                           Board.focusedIndex[1] = focus0;
														   clsScreen = 1;
														   }
                                                 }
                                else if(focus0/matrixSize == (focus0-1)/matrixSize && focus0 > 0) {Board.focusedIndex[0]--;clsScreen = 1;}
                                 
                                 }
                           else if(c1 == 'w'){
                                  if(Board.picked){
                                                 if(focus0 == focus1 && focus1 - matrixSize >=0){
                                                           Board.focusedIndex[1] = focus0-matrixSize;
														   clsScreen = 1;
														   }
                                                 else if(focus1 == focus0+matrixSize){
														   clsScreen = 1;
                                                           Board.focusedIndex[1] = focus0;
														   }
                                                 }
                                else if(focus0-matrixSize >= 0){ Board.focusedIndex[0]-=matrixSize;clsScreen = 1;}
                                  
                                 }
                           else if(c1 == 's'){
                               if(Board.picked){
                                                if(focus0 == focus1 && focus1 + matrixSize <matrixSize*matrixSize){
														   clsScreen = 1;
                                                           Board.focusedIndex[1] = focus0+matrixSize;
														   }
                                                 else if(focus1 == focus0-matrixSize){
                                                           Board.focusedIndex[1] = focus0;
														   clsScreen = 1;
														   }
                                                 }
                                else if(focus0+matrixSize < matrixSize*matrixSize) {Board.focusedIndex[0]+=matrixSize;clsScreen = 1;}
                                  
                                 }
                           else if(c1 == 13){
								
                                if(Board.picked){					 
												 finded = 0;
                                                 Board.picked = 0;
                                                 swap = Board.Board[Board.focusedIndex[0]];                                   
                                                 Board.Board[Board.focusedIndex[0]] = Board.Board[Board.focusedIndex[1]];
                                                 Board.Board[Board.focusedIndex[1]] = swap;
												 Message[0] = 0;
												 while(searchForStructures()){
												 Board.focusedIndex[0] = Board.focusedIndex[1];
												 finded = 1;
												 slideBoard();
												 clsScreen = 1;
												 }
												 if(!finded){
                                                                             swap = Board.Board[Board.focusedIndex[0]];                                   
                                                                             Board.Board[Board.focusedIndex[0]] = Board.Board[Board.focusedIndex[1]];
                                                                             Board.Board[Board.focusedIndex[1]] = swap;   
																			 if(Board.focusedIndex[0] != Board.focusedIndex[1])Board.settings.player.heart --;
																			 clsScreen = 1;
                                                                            }
                       
												 							 
                                                }
                                else {
                                     Board.picked = 1;
                                     Board.focusedIndex[1] = Board.focusedIndex[0] ; 
                                     }
                                }
                            
     }
	 printf("\n\nFinal Score : %d ",Board.settings.player.score);
	 printf("\n\nPress Any Key For Go to Menu...");
	 getch();
}
void SaveScore(){   
     char tmp[32];
     FILE *ncfile;
     ncfile = fopen(MY_FILENAME,"w");     
     fputs(Board.settings.player.name,ncfile);fputc(1,ncfile);
	 fputs(HighScore.name,ncfile);fputc(2,ncfile);
	 sprintf(tmp,"%d",HighScore.score);
     fputs(tmp,ncfile);
     fputc(3,ncfile);
     fclose(ncfile);
     }
void ReadScore(){
     char tmpHighName[256],tmpScore[32],tmp;
     long int scr;
     int i,j,m,notLast;     
     FILE *ncfile;
     ncfile = fopen(MY_FILENAME,"r");
     if(!ncfile)return;
     i = 0;
     j = 0;
	 m = 0;
     notLast = 1;
     while(i < 256 && notLast){
             tmp = fgetc(ncfile);
             if(tmp == 1)notLast = 0;
             else Board.settings.player.name[i] = tmp;
             i++; 
             }
	 Board.settings.player.name[i-1] = 0;
	 notLast = 1;
	 i = 0;
     while(i < 256 && notLast){
             tmp = fgetc(ncfile);
             if(tmp == 2)notLast = 0;
             else tmpHighName[i] = tmp;
             i++; 
             }
	 tmpHighName[i-1] = 0;
     notLast = 1;
	 i = 0;
     while(i < 32 && notLast){
             tmp = fgetc(ncfile);
             if(tmp == 3)notLast = 0;
             else tmpScore[i] = tmp;
             i++; 
             }
     tmpScore[i-1] = 0;
     scr = atoi(tmpScore);
	 
     MYS_Score_SetScore(&HighScore,scr,tmpHighName,HEART);
     fclose(ncfile);
     }
void setStructuresArray(int *array,int a,int b,int c,int d,int e,int f, int g,int h,int i){
                                array[0] = a; array[1] = b;array[2] = c;
                                array[3] = d; array[4] = e;array[5] = f;
                                array[6] = g; array[7] = h;array[8] = i;
                                    }  
int controlStructure(char *tempBoard,int index,int structureId){
    int i,j,num,finded,size;
    j = index;
    num = 0;
    finded = 1;
    i = 0;
	
	size = Board.settings.matrixSize + 4;
	
    while(i < 9 && finded){
          if(Board.settings.structures[structureId].type[i] == 1 && tempBoard[j] == 0)finded = 0;
          if(Board.settings.structures[structureId].type[i] == 1 && num == 0)num = tempBoard[j];
          if(Board.settings.structures[structureId].type[i] == 1 && num != 0 && tempBoard[j] != num)finded = 0; 
 
          if(i % 3 == 2) j += (size-2);
          else j++;
          i++;
          }
    return finded;
    }
int searchForStructures(){
     char tempBoard[ (BOARD_MAX_SIZE+4) * (BOARD_MAX_SIZE+4)]; 
     int i,j,finded,matrixSize,rtr;
     finded = 1;j = 0;rtr = 0;
     matrixSize = Board.settings.matrixSize;
     for(i = 0;i < (matrixSize+4) * (matrixSize+4);i++){
             if(i % (matrixSize+4) < 2 || i %(matrixSize+4) >= matrixSize+2 || 
                i / (matrixSize+4) < 2 || i/(matrixSize+4) >= matrixSize+2)tempBoard[i] = 0;
             else {
                  tempBoard[i] = Board.Board[j];
                  j++;
                  }
     }
	 while(finded){
     finded = 0;
	 i = 0;
     while(i < Board.settings.numberOfStructures && !finded){ 
             j = 0;
             while(j < (matrixSize+2)*(matrixSize+2) && !finded ){
                     if(controlStructure(tempBoard,j,i)){//verilen index ile structure ı karşılaştırıyor
                                  finded = 1;
								  rtr = 1;
								  Board.settings.player.score += Board.settings.structures[i].point;
								  clearStructure(tempBoard,j,i);
                                  if(Board.settings.player.score > HighScore.score){
								  MYS_Score_SetScore(&HighScore,Board.settings.player.score,Board.settings.player.name,3);
								  SaveScore();
								  }														   
					 }
                     if( j % (matrixSize+4) == matrixSize+2)j += 2;
                     else j++;
                     }
             i++;
             } 
    }
                     //sleep(30000);
             return rtr;
     }
void drawMenu(struct MYS_Menu *menu){
     int i,j;
	 textColor(7);
     for(i = 0;i < menu->buttonSize;i++){
           gotoxy(menu->buttons[i].posx,menu->buttons[i].posy);
           printf("%s",menu->buttons[i].text);
           if(menu->focusedButtonIndex == i){
              gotoxy(menu->buttons[i].posx,menu->buttons[i].posy+1);
              for(j = 0;j < strlen(menu->buttons[i].text);j++)printf("-");
           }
     }    
     printf("\n");
}
void drawGameArea(){
     int i,j,m,size,len;
	 size =  Board.settings.matrixSize;
	 printf("                        Name : %s   Score : %d  Heart : %d",Board.settings.player.name,Board.settings.player.score,Board.settings.player.heart);
	 printf("\n\n\n");
	 len = (80 - (Board.settings.matrixSize * 3))/2;//başta konulacak boşluğu belirliyor
	 
     for(i = 0;i <size;i++){
	       for(m = 0;m < len;m++)printf(" ");
           for(j = 0;j < size;j++){
		                textColor(Board.Board[i*size+j]);
                        if(Board.focusedIndex[0] == i*size+j || (Board.picked && Board.focusedIndex[1] == i*size+j) && Board.Board[i*size+j] != 0)printf("|%d|",Board.Board[i*size+j]);
                        else if(Board.Board[i*size+j] != 0 && Board.Board[i*size+j] != 0 ) printf(" %d ",Board.Board[i*size+j]);
                        else printf("   ");
           }
           printf("\n\n");       
           }
     textColor(7);
     printf("\n%s",Message);
	 }
void menuInput(struct MYS_Menu *menu,char input){


    if(input == 's' || input == 'S' )   
                      menu->focusedButtonIndex++;
    else if(input == 'w' || input == 'W' )
                      menu->focusedButtonIndex--;
    else if(input == 13)
         MYS_Menu_SetClick(menu,menu->focusedButtonIndex);
    if(menu->focusedButtonIndex >= menu->buttonSize)menu->focusedButtonIndex = 0;
    else if( menu->focusedButtonIndex < 0)menu->focusedButtonIndex = menu->buttonSize-1;
     }
void slideBoard(){
int i,j,size,changed;
size = Board.settings.matrixSize;
system("cls");
drawGameArea();
changed = 0;
for(i = 0;i < size*size;i++){
	if(!Board.Board[i]){
	   changed = 1;
		
		j = i;
		while(j-size > 0){
			Board.Board[j] = Board.Board[j-size];
			j -= size;
		}
		Board.Board[j] = rand()%(Board.settings.range[1]-Board.settings.range[0])+Board.settings.range[0]; 
		
	}
	if(i%size == (size-1) && changed){
			sleep(500);
			system("cls");
			drawGameArea();
			changed = 0;
	}
}
}
void gotoxy(int x, int y){ //silincek
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void textColor(int color){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo; 
    SetConsoleTextAttribute(hConsole, color);
}

void MYS_Score_SetScore(struct MYS_Score *strscore,long int scr,char nm[],int hrt){
                         int i = 0;
             
                         strscore->score = scr;
						 i = 0;
						 while(i < 256 && nm[i] != 0){
						 strscore->name[i] = nm[i];
                         i++;
						 }
						 strscore->name[i] = 0;
                         strscore->heart = hrt;
                         }
void MYS_Menu_SetMenu(struct MYS_Menu *menu,int lft,int tp,int sz){
     menu->buttonSize = 0;
     menu->focusedButtonIndex = 0;   
     menu->left = lft;
     menu->top = tp;
     menu->textSize = sz;
     }
void MYS_Menu_AddToMenu(struct MYS_Menu *menu,char txt[]){
     int posx,posy;
     if(menu->buttonSize < MENU_SIZE){
                    posy = menu->top + (menu->top * menu->buttonSize);
                    posx = menu->left + (menu->textSize - strlen(txt))/2;
                    MYS_Button_SetButton(&menu->buttons[menu->buttonSize],txt,posx,posy,0);
                    menu->buttonSize ++;
                    }
     }
void MYS_Menu_SetClick(struct MYS_Menu *menu,int index){
     int i;
     for(i = 0;i < menu->buttonSize;i++)
           if(i != index)menu->buttons[i].clicked = 0;
     menu->buttons[index].clicked = 1;
           
     }
void MYS_Button_SetButton(struct MYS_Button *button,char txt[],int x,int y,int clck){
     button->posx = x;
     button->posy = y;
     button->clicked = clck;
     strcpy(button->text,txt);
     }
void MYS_Settings_CreateStructures(struct MYS_Settings *sets,int *structures,int size){
     int i,j,index,num;
     struct MYS_Structure swap;
     sets->numberOfStructures = size;
     for(i = 0;i < size;i++){
           //I,T,L,F,C,U,E,O
           switch(structures[i]){
                                 sets->structures[i].id = structures[i];
                                 case STRUCTURE_I:
                                      sets->structures[i].probability = 8;//en olası en yüksek değere sahip
                                      sets->structures[i].point = 5;
                                      sets->structures[i].name = 'I';
                                      setStructuresArray(sets->structures[i].type,0,1,0,0,1,0,0,1,0);
                                      break;
                                 case STRUCTURE_T:
                                      sets->structures[i].probability = 7;
                                      sets->structures[i].point = 15;
                                      sets->structures[i].name = 'T';
                                      setStructuresArray(sets->structures[i].type,1,1,1,0,1,0,0,1,0);
                                      break;
                                 case STRUCTURE_L:
                                      sets->structures[i].probability = 6;
                                      sets->structures[i].point = 25;
                                      sets->structures[i].name = 'L';
                                      setStructuresArray(sets->structures[i].type,1,0,0,1,0,0,1,1,1);
                                      break;
                                 case STRUCTURE_F:
                                      sets->structures[i].probability = 5;
                                      sets->structures[i].point = 35;
                                      sets->structures[i].name = 'F';
                                      setStructuresArray(sets->structures[i].type,1,1,1,1,1,0,1,0,0);
                                      break;
                                 case STRUCTURE_C:
                                      sets->structures[i].probability = 4;
                                      sets->structures[i].point = 45;
                                      sets->structures[i].name = 'C';
                                      setStructuresArray(sets->structures[i].type,1,1,1,1,0,0,1,1,1);
                                      break;
                                 case STRUCTURE_U:
                                      sets->structures[i].probability = 3;
                                      sets->structures[i].point = 55;
                                      sets->structures[i].name = 'U';
                                      setStructuresArray(sets->structures[i].type,1,0,1,1,0,1,1,1,1);
                                      break;
                                 case STRUCTURE_E: 
                                      sets->structures[i].probability = 2;
                                      sets->structures[i].point = 65;
                                      sets->structures[i].name = 'E';
                                      setStructuresArray(sets->structures[i].type,1,1,1,1,1,0,1,1,1);
                                      break;
                                 case STRUCTURE_O:
                                      sets->structures[i].probability = 1;
                                      sets->structures[i].point = 75;
                                      sets->structures[i].name = 'O';
                                      setStructuresArray(sets->structures[i].type,1,1,1,1,0,1,1,1,1);
                                      break;
                                 default:
                                      break;     
                                 }
                                 
           }
        
           for(i = 0;i < size;i++){
                 num = sets->structures[i].probability;index = i;
                 for(j = i;j < size;j++){
                         if(num > sets->structures[j].probability){
                                num = sets->structures[j].probability;
                                index = j;
                         }
                       }
                       if(num != i)  {
                         swap = sets->structures[index];
                         sets->structures[index] = sets->structures[i];
                         sets->structures[i] = swap;
                         }
                 }
     }
