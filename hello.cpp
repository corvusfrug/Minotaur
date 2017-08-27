#include "hello.h"

char** field;
int n,m;
bool end_game_flag;
bool loose_game_flag;
int used_flag;
int player_x,player_y;
int minotavr_x, minotavr_y;
static int escape_x,escape_y;  //Координаты выходя для BFS на очереди с приоритетом
int score;
int** used_p;
int** used_m;
koord** q_prior;
koord** q_prior_m;
int uroven;
static int q_prior_size;        // Размер Очереди с приоритетом
static int move_count;          // Задержка при переправе игрока через воду
static int move_count_m;        // Задержка при проламывании минотавром леса

void input_field()
{
    ifstream readF("input.txt");
    readF >> n >> m;
    field = new char* [n];
    for(int i=0;i<n;i++)
        field[i] = new char[m];
    used_p = new int* [n];
    if((uroven==5)||(uroven==6)||(uroven==7)||(uroven==9)) used_m = new int* [n];
    if((uroven==8)||(uroven==9)) {q_prior = new koord* [n]; q_prior_m = new koord* [n];};
    if((uroven==8)||(uroven==9)) {move_count=0;move_count_m=0;};
    for(int i=0;i<n;i++)
    {
        used_p[i] = new int[m];
        if((uroven==5)||(uroven==6)||(uroven==7)||(uroven==9)) used_m[i] = new int[m];
        if((uroven==8)||(uroven==9))  q_prior[i] = new koord[m];
        if((uroven==9))  q_prior_m[i] = new koord[m];
    };

    string temp;
    getline(readF,temp);
    for(int i=0;i<n;i++)
    {
       getline(readF,temp);
       for(int j=0;j<m;j++)
       {
           field[i][j] = temp[j];
           if((temp[j]=='Y')||(temp[j]=='y'))
           {
                player_x=j;
                player_y=i;
           };
           if((temp[j]=='M')||(temp[j]=='m'))
           {
                minotavr_x=j;
                minotavr_y=i;
                if((uroven!=5)&&(uroven!=6)&&(uroven!=7)&&(uroven!=9))
                {
                    minotavr_x=-1;
                    minotavr_y=-1;
                    field[i][j]=' ';
                };
           };
           if((temp[j]=='Q')||(temp[j]=='q'))
           {
                escape_x=j;
                escape_y=i;
           };

       };

    };
    readF.close();

};

void output_field()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // Для цвету
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
            if(field[i][j]=='W')
            {
                SetConsoleTextAttribute(hStdOut, 1);
                if(((uroven==8)||(uroven==9))&&(used_p[i][j]==-8)) cout << "."; else
                    cout << field[i][j];
                SetConsoleTextAttribute(hStdOut, 7);
            } else
            if(field[i][j]=='U')
            {
                SetConsoleTextAttribute(hStdOut, 1);
                cout << field[i][j];
                SetConsoleTextAttribute(hStdOut, 7);
            } else
            if(field[i][j]=='Q')
            {
                SetConsoleTextAttribute(hStdOut, 4);
                cout << field[i][j];
                SetConsoleTextAttribute(hStdOut, 7);
            } else
            if(field[i][j]=='T')
            {
                SetConsoleTextAttribute(hStdOut, 2);
                if((uroven==9)&&(used_m[i][j]==-6)) cout << "."; else
                    cout << field[i][j];
                SetConsoleTextAttribute(hStdOut, 7);
            } else
            if(field[i][j]=='Y')
            {
                SetConsoleTextAttribute(hStdOut, 6);
                cout << field[i][j];
                SetConsoleTextAttribute(hStdOut, 7);

            } else
            if(field[i][j]=='M')
            {

                SetConsoleTextAttribute(hStdOut, 11);
                cout << field[i][j];
                SetConsoleTextAttribute(hStdOut, 7);
            } else
            if(used_p[i][j]<=-7)
            {

                SetConsoleTextAttribute(hStdOut, 12);
                cout << '.';
                SetConsoleTextAttribute(hStdOut, 7);
            } else
            if(((uroven==5)||(uroven==6)||(uroven==7)||(uroven==9))&&(used_m[i][j]==-6))
            {
                SetConsoleTextAttribute(hStdOut, 11);
                cout << '.';
                SetConsoleTextAttribute(hStdOut, 7);
            } else
            cout << field[i][j];
        cout << endl;
    };
    //system("pause");
};

void moove(int comand)
{
    system("cls");
    if((uroven==8)||(uroven==9))
        if(move_count==1)
        {
            move_count--;
            cout << "FLOATS!" << endl;
            if(comand==224) comand = _getch();
            //output_field();
            return;
        };
    if(comand==27) {end_game_flag=true; score = 999999; return;}; // Escape

    switch(comand)
    {
        case 224:
                 comand = _getch();
                 switch(comand)
                 {
                      case 72: player_moove('U'); cout << "UP! SCORE: " << score << endl; break;
                      case 80: player_moove('D'); cout << "DOWN! SCORE: " << score << endl; break;
                      case 75: player_moove('L'); cout << "LEFT! SCORE: " << score << endl; break;
                      case 77: player_moove('R'); cout << "RIGHT! SCORE: " << score << endl; break;
                 };
                 break;
        case 230: player_moove('U'); cout << "UP! SCORE: " << score << endl; break;
        case 119: player_moove('U'); cout << "UP! SCORE: " << score << endl; break;
        case 87: player_moove('U'); cout << "UP! SCORE: " << score << endl; break;
        case 150: player_moove('U'); cout << "UP! SCORE: " << score << endl; break;


        case 235:  player_moove('D'); cout << "DOWN! SCORE: " << score << endl; break;
        case 115:  player_moove('D'); cout << "DOWN! SCORE: " << score << endl; break;
        case 83:  player_moove('D'); cout << "DOWN! SCORE: " << score << endl; break;
        case 155:  player_moove('D'); cout << "DOWN! SCORE: " << score << endl; break;


        case 228: player_moove('L'); cout << "LEFT! SCORE: " << score << endl; break;
        case 97: player_moove('L'); cout << "LEFT! SCORE: " << score << endl; break;
        case 65: player_moove('L'); cout << "LEFT! SCORE: " << score << endl; break;
        case 148: player_moove('L'); cout << "LEFT! SCORE: " << score << endl; break;


        case 162: player_moove('R'); cout << "RIGHT! SCORE: " << score << endl; break;
        case 100: player_moove('R'); cout << "RIGHT! SCORE: " << score << endl; break;
        case 68: player_moove('R'); cout << "RIGHT! SCORE: " << score << endl; break;
        case 130: player_moove('R'); cout << "RIGHT! SCORE: " << score << endl; break;

        default:
            cout << " ERROR!" << endl;
            break;
    };
};

void player_moove(char comand)
{
     switch(comand)
     {
         case 'U':
              if((field[player_y-1][player_x]==' ')||
                 (field[player_y-1][player_x]=='Q')||
                 (field[player_y-1][player_x]=='*')||
                 (field[player_y-1][player_x]=='M')||
                 (field[player_y-1][player_x]=='.')||
                 (((uroven==8)||(uroven==9))&&(field[player_y-1][player_x]=='W')))
              {
                  if(field[player_y-1][player_x]=='Q') end_game_flag=true;
                  if(field[player_y-1][player_x]=='M') loose_game_flag=true;
                  if(((uroven==8)||(uroven==9))&&(field[player_y][player_x]=='U')) field[player_y][player_x]='W'; else field[player_y][player_x]='*';
                  if(((uroven==8)||(uroven==9))&&(field[player_y-1][player_x]=='W'))
                    {field[--player_y][player_x]='U';move_count++;} else
                        field[--player_y][player_x]='Y';
                  score++;
              } else cout << "ROAD BLOCK!";
              break;

         case 'L':
              if((field[player_y][player_x-1]==' ')||
                 (field[player_y][player_x-1]=='Q')||
                 (field[player_y][player_x-1]=='*')||
                 (field[player_y][player_x-1]=='M')||
                 (field[player_y][player_x-1]=='.')||
                 (((uroven==8)||(uroven==9))&&(field[player_y][player_x-1]=='W')))
              {
                  if(field[player_y][player_x-1]=='Q') end_game_flag=true;
                  if(field[player_y][player_x-1]=='M') loose_game_flag=true;
                  if(((uroven==8)||(uroven==9))&&(field[player_y][player_x]=='U')) field[player_y][player_x]='W'; else field[player_y][player_x]='*';
                  if(((uroven==8)||(uroven==9))&&(field[player_y][player_x-1]=='W'))
                    {field[player_y][--player_x]='U';move_count++;} else
                        field[player_y][--player_x]='Y';
                  score++;
              } else cout << "ROAD BLOCK!";
              break;

         case 'D':
              if((field[player_y+1][player_x]==' ')||
                 (field[player_y+1][player_x]=='Q')||
                 (field[player_y+1][player_x]=='*')||
                 (field[player_y+1][player_x]=='M')||
                 (field[player_y+1][player_x]=='.')||
                 (((uroven==8)||(uroven==9))&&(field[player_y+1][player_x]=='W')))
              {
                  if(field[player_y+1][player_x]=='Q') end_game_flag=true;
                  if(field[player_y+1][player_x]=='M') loose_game_flag=true;
                  if(((uroven==8)||(uroven==9))&&(field[player_y][player_x]=='U')) field[player_y][player_x]='W'; else field[player_y][player_x]='*';
                  if(((uroven==8)||(uroven==9))&&(field[player_y+1][player_x]=='W'))
                    {field[++player_y][player_x]='U';move_count++;} else
                        field[++player_y][player_x]='Y';
                  score++;
              } else cout << "ROAD BLOCK!";
              break;

         case 'R':
              if((field[player_y][player_x+1]==' ')||
                 (field[player_y][player_x+1]=='Q')||
                 (field[player_y][player_x+1]=='*')||
                 (field[player_y][player_x+1]=='M')||
                 (field[player_y][player_x+1]=='.')||
                 (((uroven==8)||(uroven==9))&&(field[player_y][player_x+1]=='W')))
              {
                  if(field[player_y][player_x+1]=='Q') end_game_flag=true;
                  if(field[player_y][player_x+1]=='M') loose_game_flag=true;
                  if(((uroven==8)||(uroven==9))&&(field[player_y][player_x]=='U')) field[player_y][player_x]='W'; else field[player_y][player_x]='*';
                  if(((uroven==8)||(uroven==9))&&(field[player_y][player_x+1]=='W'))
                    {field[player_y][++player_x]='U';move_count++;} else
                        field[player_y][++player_x]='Y';
                  score++;
              } else cout << "ROAD BLOCK!";
              break;
     };
     if(end_game_flag) {cout << "DID YOU GET OUT! CONGRATULATIONS!\n";};
     if(loose_game_flag) {end_game_flag=true; cout << "YOU ATE, YOU DIED!!\n"; };
};

void moove_minotavr()
{
     int perem=0;
     bool flag=false;

     if((field[minotavr_y-1][minotavr_x]=='Y')) {perem=1;flag=true;} //up
     if((field[minotavr_y][minotavr_x-1]=='Y')) {perem=2;flag=true;} //left
     if((field[minotavr_y+1][minotavr_x]=='Y')) {perem=3;flag=true;} //down
     if((field[minotavr_y][minotavr_x+1]=='Y')) {perem=4;flag=true;} // right

     if((!flag)&&(used_m[minotavr_y-1][minotavr_x]==-6)) perem=1; //up
     if((!flag)&&(used_m[minotavr_y][minotavr_x-1]==-6)) perem=2; //left
     if((!flag)&&(used_m[minotavr_y+1][minotavr_x]==-6)) perem=3; //down
     if((!flag)&&(used_m[minotavr_y][minotavr_x+1]==-6)) perem=4; // right

     if(perem==1)  //up
     {
         field[minotavr_y--][minotavr_x] = ' ';
         field[minotavr_y][minotavr_x] = 'M';
         if((minotavr_y==player_y)&&(minotavr_x==player_x))
         {
             cout << "YOU ATE, YOU DIED!!\n";
             loose_game_flag=true;
             end_game_flag=true;
         };
     } else
         if(perem==2)  //left
         {
             field[minotavr_y][minotavr_x--] = ' ';
             field[minotavr_y][minotavr_x] = 'M';
             if((minotavr_y==player_y)&&(minotavr_x==player_x))
             {
                 cout << "YOU ATE, YOU DIED!!\n";
                 loose_game_flag=true;
                 end_game_flag=true;
             };
         } else
             if(perem==3) //down
             {
                 field[minotavr_y++][minotavr_x] = ' ';
                 field[minotavr_y][minotavr_x] = 'M';
                 if((minotavr_y==player_y)&&(minotavr_x==player_x))
                 {
                     cout << "YOU ATE, YOU DIED!!\n";
                     loose_game_flag=true;
                     end_game_flag=true;
                 };
             } else
                 if(perem==4) // right
                 {
                     field[minotavr_y][minotavr_x++] = ' ';
                     field[minotavr_y][minotavr_x] = 'M';
                     if((minotavr_y==player_y)&&(minotavr_x==player_x))
                     {
                         cout << "YOU ATE, YOU DIED!!\n";
                         loose_game_flag=true;
                         end_game_flag=true;
                     };
                 };
};

void moove_minotavr_smart()
{
     int comand=0, napr=0;
     if((field[minotavr_y-1][minotavr_x]=='Y')||((used_p[minotavr_y-1][minotavr_x]<=-7)&&(used_p[minotavr_y-1][minotavr_x]<comand))) //up
         {comand = used_p[minotavr_y-1][minotavr_x];napr=1;};
     if((field[minotavr_y][minotavr_x-1]=='Y')||((used_p[minotavr_y][minotavr_x-1]<=-7)&&(used_p[minotavr_y][minotavr_x-1]<comand))) //left
         {comand = used_p[minotavr_y][minotavr_x-1];napr=2;};
     if((field[minotavr_y+1][minotavr_x]=='Y')||((used_p[minotavr_y+1][minotavr_x]<=-7)&&(used_p[minotavr_y+1][minotavr_x]<comand))) //down
         {comand = used_p[minotavr_y+1][minotavr_x];napr=3;};
     if((field[minotavr_y][minotavr_x+1]=='Y')||((used_p[minotavr_y][minotavr_x+1]<=-7)&&(used_p[minotavr_y][minotavr_x+1]<comand))) //right
         {comand = used_p[minotavr_y][minotavr_x+1];napr=4;};



     if(napr==1)  //up
     {
         field[minotavr_y--][minotavr_x] = ' ';
         field[minotavr_y][minotavr_x] = 'M';
         if((minotavr_y==player_y)&&(minotavr_x==player_x))
         {
             cout << "YOU ATE, YOU DIED!!\n";
             loose_game_flag=true;
             end_game_flag=true;
         };
     } else
         if(napr==2)  //left
         {
             field[minotavr_y][minotavr_x--] = ' ';
             field[minotavr_y][minotavr_x] = 'M';
             if((minotavr_y==player_y)&&(minotavr_x==player_x))
             {
                 cout << "YOU ATE, YOU DIED!!\n";
                 loose_game_flag=true;
                 end_game_flag=true;
             };
         } else
             if(napr==3) //down
             {
                 field[minotavr_y++][minotavr_x] = ' ';
                 field[minotavr_y][minotavr_x] = 'M';
                 if((minotavr_y==player_y)&&(minotavr_x==player_x))
                 {
                     cout << "YOU ATE, YOU DIED!!\n";
                     loose_game_flag=true;
                     end_game_flag=true;
                 };
             } else
                 if(napr==4) // right
                 {
                     field[minotavr_y][minotavr_x++] = ' ';
                     field[minotavr_y][minotavr_x] = 'M';
                     if((minotavr_y==player_y)&&(minotavr_x==player_x))
                     {
                         cout << "YOU ATE, YOU DIED!!\n";
                         loose_game_flag=true;
                         end_game_flag=true;
                     };
                 };
};

void fin()
{
    ifstream in("finisher.txt");
    int w1,w2,w3;
    in >> w1 >> w2 >> w3;
    in.close();
    system("cls");
    output_field();
    cout << "\n\n";
    if(score==999999) cout << "YOU DIDN'T GET OUT\n"; else
        cout << "YOUR SCORE: " << score << endl;
    cout << "BEST OF THE BEST LIST:\n";
    if(score<w1)
    {
        cout << "1)  " << score << "  -  YOU" << endl;
        cout << "2)  " << w2 << endl;
        cout << "3)  " << w3 << endl;
        w1=score;
    } else
    if(score<w2)
    {
        cout << "1)  " << w1 << endl;
        cout << "2)  " << score << "  -  YOU" << endl;
        cout << "3)  " << w3 << endl;
        w2=score;
    } else
    if(score<w3)
    {
        cout << "1)  " << w1 << endl;
        cout << "2)  " << w2 << endl;
        cout << "3)  " << score << "  -  YOU" << endl;
        w3=score;
    } else
    {
        cout << "1)  " << w1 << endl;
        cout << "2)  " << w2 << endl;
        cout << "3)  " << w3 << endl;
    }


    ofstream out("finisher.txt");
    out << w1 << " " << w2 << " " << w3;
    out.close();
    system("pause");

    system("cls");
    cout << "\n\n\n\n\n\n\n\n\n\n";
    cout << "\t\t\t THE END!!!" << endl;
    cout << "\t\t      CONGRATULATIONS!" << endl;
    cout << "\n\n\n\n\n\n\n\n\n\n";
    system("pause");
};

void loose()
{
    system("pause");
    ifstream in("finisher.txt");
    int w1,w2,w3;
    in >> w1 >> w2 >> w3;
    in.close();
    system("cls");
    output_field();
    cout << "\n\n";
    cout << "YOU'RE DEAD!\n";
   // cout << "YOUR SCORE: " << score << endl;
    cout << "BEST OF THE BEST LIST:\n";
    cout << "1)  " << w1 << endl;
    cout << "2)  " << w2 << endl;
    cout << "3)  " << w3 << endl;
    system("pause");

    system("cls");
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
    cout << "\t\t\t THE END!!!" << endl;
    cout << "\n\n\n\n\n\n\n\n\n\n";
    system("pause");
};

void zero_used()
{
    for(int i=0;i<n;i++)
         for(int j=0;j<m;j++)
             {
                 used_p[i][j]=0;
                 //if((uroven==8)||(uroven==9)) {used_p[i][j]=-1;}
                 if((uroven==5)||(uroven==6)||(uroven==7)) used_m[i][j]=0;
             };
    used_flag=false;
};
void player_dfs(int x, int y)
{
     if(used_p[y][x]!=0) return;
     used_p[y][x] = 1;
     if(field[y][x]=='Q') {used_flag=true; return;};

     if(((y-1)>0)&&((field[y-1][x]==' ')||(field[y-1][x]=='Q'))) player_dfs(x,y-1);  // UP;
     if((used_flag)&&(field[y][x]!='Y')) { used_p[y][x] = -7; /*field[y][x]='.';*/ return;};
     if(((x-1)>0)&&((field[y][x-1]==' ')||(field[y][x-1]=='Q'))) player_dfs(x-1,y);  // LEFT;
     if((used_flag)&&(field[y][x]!='Y')) { used_p[y][x] = -7; /*field[y][x]='.'*/; return;};
     if(((y+1)<n-1)&&((field[y+1][x]==' ')||(field[y+1][x]=='Q'))) player_dfs(x,y+1);  // DOWN;
     if((used_flag)&&(field[y][x]!='Y')) { used_p[y][x] = -7; /*field[y][x]='.';*/ return;};
     if(((x+1)<m-1)&&((field[y][x+1]==' ')||(field[y][x+1]=='Q'))) player_dfs(x+1,y);  // RIGHT;
     if((used_flag)&&(field[y][x]!='Y')) { used_p[y][x] = -7; /*field[y][x]='.';*/ return;};
};

void minotavr_dfs(int x, int y)
{
     if(used_m[y][x]!=0) return;
     used_m[y][x] = 1;
     if(field[y][x]=='Y') {used_flag=true; return;};

     if(((y-1)>0)&&((field[y-1][x]==' ')||(field[y-1][x]=='Y')||(field[y-1][x]=='*'))) minotavr_dfs(x,y-1);  // UP;
     if((used_flag)&&(field[y][x]!='M')) { used_m[y][x] = -6; /*field[y][x]='.';*/ return;};
     if(((x-1)>0)&&((field[y][x-1]==' ')||(field[y][x-1]=='Y')||(field[y][x-1]=='*'))) minotavr_dfs(x-1,y);  // LEFT;
     if((used_flag)&&(field[y][x]!='M')) { used_m[y][x] = -6; /*field[y][x]='.'*/; return;};
     if(((y+1)<n-1)&&((field[y+1][x]==' ')||(field[y+1][x]=='Y')||(field[y+1][x]=='*'))) minotavr_dfs(x,y+1);  // DOWN;
     if((used_flag)&&(field[y][x]!='M')) { used_m[y][x] = -6; /*field[y][x]='.';*/ return;};
     if(((x+1)<m-1)&&((field[y][x+1]==' ')||(field[y][x+1]=='Y')||(field[y][x+1]=='*'))) minotavr_dfs(x+1,y);  // RIGHT;
     if((used_flag)&&(field[y][x]!='M')) { used_m[y][x] = -6; /*field[y][x]='.';*/ return;};
};

void intro()
{
     cout << "AVAILABLE MODES: \n" ;
     cout << "1) STANDART\n2) DFS\n3) BFS\n4) ESCAPE THE MINOTAUR\n5) PRIORITY QUEUE\n\n";
     cout << "ENTER THE NUMBER CORRESPONDING TO THE MODE: " ;
     cin >> uroven;
     if((uroven!=2)&&(uroven!=3)&&(uroven!=1)&&(uroven!=4)&&(uroven!=5))
     {
         cout << "\nDEFAULT MODE: STANDART \n" ;
         uroven=1;
         system("pause");
     };
     if(uroven==4)
     {
          int i=0;
          cout << "\n\tSET LEVEL: \n" ;
          cout << "\t1) EASY-CRAZY(DFS)\n\t2) NORMAL(BFS)\n\t3) SMART-HARD(BFS++)\n";
          cout << "\tENTER THE NUMBER CORRESPONDING TO THE LEVEL: " ;
          cin >> i;
          if(i==1) uroven+=i; else    // easy
              if(i==3) uroven+=i;/*hard*/ else uroven=6;  //normal

          if((i!=1)&&(i!=2)&&(i!=3))
          {
              cout << "\n\tDEFAULT MODE: NORMAL(BFS) \n" ;
              uroven=6;
              system("pause");
          };
          system("cls");
          return;
     };
     if(uroven==5)
     {
          int i=0;
          cout << "\n\tAVAILABLE SUBMODES: \n" ;
          cout << "\t1) BFS 2 (PASSABLE WATER)\n\t2) ESCAPE THE MINOTAUR 2\n";
          cout << "\tENTER THE NUMBER CORRESPONDING TO THE SUBMODE:  " ;
          cin >> i;
          if(i==1) uroven+=i+2; else    // 8 BFS 2
              if(i==2) uroven+=i+2; // 9  MINOTAUR 2

          if((i!=1)&&(i!=2))
          {
              cout << "\n\tDEFAULT SUBMODE: BFS 2 (PASSABLE WATER) \n" ;
              uroven=8;
              system("pause");
          };
     };

     system("cls");
};

void zero_used_bfs()
{

     for(int i=0;i<n;i++)
         for(int j=0;j<m;j++)
             {
                 used_p[i][j]=-1;
                 if((uroven==5)||(uroven==6)||(uroven==7)||(uroven==9)) used_m[i][j]=-1;
                 if((uroven==8)||(uroven==9))
                 {
                     q_prior[i][j].x=j;
                     q_prior[i][j].y=i;
                     q_prior[i][j].prior=999999999;
                     q_prior[i][j].time=0;
                     if ((j==player_x)&&(i==player_y))
                         q_prior[i][j].prior=0;
                 };
                 if((uroven==9))
                 {
                     q_prior_m[i][j].x=j;
                     q_prior_m[i][j].y=i;
                     q_prior_m[i][j].prior=999999999;
                     q_prior_m[i][j].time=0;
                     if ((j==minotavr_x)&&(i==minotavr_y))
                         q_prior_m[i][j].prior=0;
                };
             };
     if((uroven==8)||(uroven==9))
         q_prior_size=n*m;
     if((uroven==5)||(uroven==6)||(uroven==7)||(uroven==9)) used_m[minotavr_y][minotavr_x]=0;
     //cout << "ERROR zzz5" << endl;
};

void player_bfs()
{
     zero_used_bfs();

     koord queue[(n-1)*(m-1)*(n-1)*(m-1)];
     int queue_size=0;

     queue[queue_size].x=player_x;
     queue[queue_size++].y=player_y;
     //cout << "ERROR 1" << endl;

     /*cout << endl;
     for(int i=0;i<n;i++)
     {
        for(int j=0;j<m;j++)
         if(used_p[i][j]==-1) cout << '*' << " ";
           else cout << used_p[i][j] << " ";
        cout << endl;
     }; */

     int i=0;
     while(i<queue_size)
     {
           if(field[queue[i].y][queue[i].x]=='Q') break;
                              //cout << "ERROR 2" << endl;
           if((queue[i].y-1>=0)&&(used_p[queue[i].y-1][queue[i].x]==-1)&&((field[queue[i].y-1][queue[i].x]==' ')||(field[queue[i].y-1][queue[i].x]=='*')||(field[queue[i].y-1][queue[i].x]=='Q')||(field[queue[i].y-1][queue[i].x]=='M')))
               {           //UP
               //cout << "ERROR 3" << endl;
                   used_p[queue[i].y-1][queue[i].x] = used_p[queue[i].y][queue[i].x]+1;
                   queue[queue_size].x=queue[i].x;
                   queue[queue_size++].y=queue[i].y-1;
                   //cout << "ERROR 4" << endl;
               };
           if((queue[i].x-1>=0)&&(used_p[queue[i].y][queue[i].x-1]==-1)&&((field[queue[i].y][queue[i].x-1]==' ')||(field[queue[i].y][queue[i].x-1]=='*')||(field[queue[i].y][queue[i].x-1]=='Q')||(field[queue[i].y][queue[i].x-1]=='M')))
               {           //LEFT
                   used_p[queue[i].y][queue[i].x-1] = used_p[queue[i].y][queue[i].x]+1;
                   queue[queue_size].x=queue[i].x-1;
                   queue[queue_size++].y=queue[i].y;
               };
           if((queue[i].y+1<n)&&(used_p[queue[i].y+1][queue[i].x]==-1)&&((field[queue[i].y+1][queue[i].x]==' ')||(field[queue[i].y+1][queue[i].x]=='*')||(field[queue[i].y+1][queue[i].x]=='Q')||(field[queue[i].y+1][queue[i].x]=='M')))
               {           //DOWN
                   used_p[queue[i].y+1][queue[i].x] = used_p[queue[i].y][queue[i].x]+1;
                   queue[queue_size].x=queue[i].x;
                   queue[queue_size++].y=queue[i].y+1;
               };
           if((queue[i].x+1<m)&&(used_p[queue[i].y][queue[i].x+1]==-1)&&((field[queue[i].y][queue[i].x+1]==' ')||(field[queue[i].y][queue[i].x+1]=='*')||(field[queue[i].y][queue[i].x+1]=='Q')||(field[queue[i].y][queue[i].x+1]=='M')))
               {           //RIGHT
                   used_p[queue[i].y][queue[i].x+1] = used_p[queue[i].y][queue[i].x]+1;
                   queue[queue_size].x=queue[i].x+1;
                   queue[queue_size++].y=queue[i].y;
               };
               i++;
               //cout << i << " ERROR 5" << endl;
     };
      //cout << i << " ERROR 6" << endl;
      //for(int j=0;j<100;j++) cout << used_p[queue[j].y][queue[j].x] << endl;
      //system("pause");
     // find putb
     int temp_x=queue[i].x,temp_y=queue[i].y;
     static int metka=-7;
     while(used_p[temp_y][temp_x]>0)
     {
             //cout << i << " ERROR 7" << endl;
             if(used_p[temp_y-1][temp_x]+1==used_p[temp_y][temp_x])  //UP
             {
                 used_p[temp_y][temp_x]=metka--;
                 temp_y--;
             };
             if(used_p[temp_y][temp_x-1]+1==used_p[temp_y][temp_x])  //LEFT
             {
                 used_p[temp_y][temp_x]=metka--;
                 temp_x--;
             };
             if(used_p[temp_y+1][temp_x]+1==used_p[temp_y][temp_x])  //DOWN
             {
                 used_p[temp_y][temp_x]=metka--;
                 temp_y++;
             };
             if(used_p[temp_y][temp_x+1]+1==used_p[temp_y][temp_x])  //RIGHT
             {
                 used_p[temp_y][temp_x]=metka--;
                 temp_x++;
             };

             //cout << temp_y <<"|" << temp_x << " / " << used_p[temp_y][temp_x] << endl;
     };

     /*cout << endl;
     for(int i=0;i<n;i++)
     {
        for(int j=0;j<m;j++)
         if(used_p[i][j]==-1) cout << '*' << " ";
         else if(used_p[i][j]==-5) cout << "@" << " ";
           else cout << "$" << " ";
        cout << endl;
     };*/

};

void minotavr_bfs()
{
     zero_used_bfs();

     koord queue[(n-1)*(m-1)*(n-1)*(m-1)];
     int queue_size=0;
     queue[queue_size].x=minotavr_x;
     queue[queue_size++].y=minotavr_y;

     //cout << "ERROR 1" << endl;
     int i=0;
     while(i<queue_size)
     {

           if(field[queue[i].y][queue[i].x]=='Y') break;
                              //cout << "ERROR 2" << endl;
           if((queue[i].y-1>=0)&&(used_m[queue[i].y-1][queue[i].x]==-1)&&((field[queue[i].y-1][queue[i].x]==' ')||(field[queue[i].y-1][queue[i].x]=='*')||(field[queue[i].y-1][queue[i].x]=='Q')||(field[queue[i].y-1][queue[i].x]=='Y')||(field[queue[i].y-1][queue[i].x]=='.')))
               {           //UP
               //cout << "ERROR 3" << endl;
                   used_m[queue[i].y-1][queue[i].x] = used_m[queue[i].y][queue[i].x]+1;
                   queue[queue_size].x=queue[i].x;
                   queue[queue_size++].y=queue[i].y-1;
                  // cout << "ERROR 4" << endl;
               };
           if((queue[i].x-1>=0)&&(used_m[queue[i].y][queue[i].x-1]==-1)&&((field[queue[i].y][queue[i].x-1]==' ')||(field[queue[i].y][queue[i].x-1]=='*')||(field[queue[i].y][queue[i].x-1]=='Q')||(field[queue[i].y][queue[i].x-1]=='Y')||(field[queue[i].y][queue[i].x-1]=='.')))
               {           //LEFT
                   used_m[queue[i].y][queue[i].x-1] = used_m[queue[i].y][queue[i].x]+1;
                   queue[queue_size].x=queue[i].x-1;
                   queue[queue_size++].y=queue[i].y;
               };
           if((queue[i].y+1<n)&&(used_m[queue[i].y+1][queue[i].x]==-1)&&((field[queue[i].y+1][queue[i].x]==' ')||(field[queue[i].y+1][queue[i].x]=='*')||(field[queue[i].y+1][queue[i].x]=='Q')||(field[queue[i].y+1][queue[i].x]=='Y')||(field[queue[i].y+1][queue[i].x]=='.')))
               {           //DOWN
                   used_m[queue[i].y+1][queue[i].x] = used_m[queue[i].y][queue[i].x]+1;
                   queue[queue_size].x=queue[i].x;
                   queue[queue_size++].y=queue[i].y+1;
               };
           if((queue[i].x+1<m)&&(used_m[queue[i].y][queue[i].x+1]==-1)&&((field[queue[i].y][queue[i].x+1]==' ')||(field[queue[i].y][queue[i].x+1]=='*')||(field[queue[i].y][queue[i].x+1]=='Q')||(field[queue[i].y][queue[i].x+1]=='Y')||(field[queue[i].y][queue[i].x+1]=='.')))
               {           //RIGHT
                   used_m[queue[i].y][queue[i].x+1] = used_m[queue[i].y][queue[i].x]+1;
                   queue[queue_size].x=queue[i].x+1;
                   queue[queue_size++].y=queue[i].y;
               };
               i++;
               //cout << i << " ERROR 5" << endl;
     };
      //cout << i << " ERROR 6" << endl;
      //for(int j=0;j<100;j++) cout << used_m[queue[j].y][queue[j].x] << endl;
      //system("pause");
     // find putb
     int temp_x=queue[i].x,temp_y=queue[i].y;
     while(used_m[temp_y][temp_x]>0)
     {
             //cout << i << " ERROR 7" << endl;
             if(used_m[temp_y-1][temp_x]+1==used_m[temp_y][temp_x])  //UP
             {
                 used_m[temp_y][temp_x]=-6;
                 temp_y--;
             };
             if(used_m[temp_y][temp_x-1]+1==used_m[temp_y][temp_x])  //LEFT
             {
                 used_m[temp_y][temp_x]=-6;
                 temp_x--;
             };
             if(used_m[temp_y+1][temp_x]+1==used_m[temp_y][temp_x])  //DOWN
             {
                 used_m[temp_y][temp_x]=-6;
                 temp_y++;
             };
             if(used_m[temp_y][temp_x+1]+1==used_m[temp_y][temp_x])  //RIGHT
             {
                 used_m[temp_y][temp_x]=-6;
                 temp_x++;
             };

             //cout << temp_y <<"|" << temp_x << " / " << used_p[temp_y][temp_x] << endl;
     };

     /*cout << endl;
     for(int i=0;i<n;i++)
     {
        for(int j=0;j<m;j++)
         if(used_p[i][j]==-1) cout << '*' << " ";
         else if(used_p[i][j]==-5) cout << "@" << " ";
           else cout << "$" << " ";
        cout << endl;
     }; */

};

void minotavr_bfs_smart()
{
       if(used_p[minotavr_y][minotavr_x]<=-7)  moove_minotavr_smart();
       else
       {
           koord queue[(n-1)*(m-1)*(n-1)*(m-1)];
           int queue_size=0;
           queue[queue_size].x=minotavr_x;
           queue[queue_size++].y=minotavr_y;

           //cout << "ERROR 1" << endl;
           int i=0;
           while(i<queue_size)
           {

                 if((used_p[queue[i].y][queue[i].x]<-7)||(field[queue[i].y][queue[i].x]=='Y')) break;
                                    //cout << "ERROR 2" << endl;
                 if((queue[i].y-1>=0)&&(used_m[queue[i].y-1][queue[i].x]==-1)&&((field[queue[i].y-1][queue[i].x]==' ')||(field[queue[i].y-1][queue[i].x]=='*')||(field[queue[i].y-1][queue[i].x]=='Q')||(field[queue[i].y-1][queue[i].x]=='Y')||(field[queue[i].y-1][queue[i].x]=='.')))
                     {           //UP
                     //cout << "ERROR 3" << endl;
                         used_m[queue[i].y-1][queue[i].x] = used_m[queue[i].y][queue[i].x]+1;
                         queue[queue_size].x=queue[i].x;
                         queue[queue_size++].y=queue[i].y-1;
                        // cout << "ERROR 4" << endl;
                     };
                 if((queue[i].x-1>=0)&&(used_m[queue[i].y][queue[i].x-1]==-1)&&((field[queue[i].y][queue[i].x-1]==' ')||(field[queue[i].y][queue[i].x-1]=='*')||(field[queue[i].y][queue[i].x-1]=='Q')||(field[queue[i].y][queue[i].x-1]=='Y')||(field[queue[i].y][queue[i].x-1]=='.')))
                     {           //LEFT
                         used_m[queue[i].y][queue[i].x-1] = used_m[queue[i].y][queue[i].x]+1;
                         queue[queue_size].x=queue[i].x-1;
                         queue[queue_size++].y=queue[i].y;
                     };
                 if((queue[i].y+1<n)&&(used_m[queue[i].y+1][queue[i].x]==-1)&&((field[queue[i].y+1][queue[i].x]==' ')||(field[queue[i].y+1][queue[i].x]=='*')||(field[queue[i].y+1][queue[i].x]=='Q')||(field[queue[i].y+1][queue[i].x]=='Y')||(field[queue[i].y+1][queue[i].x]=='.')))
                     {           //DOWN
                         used_m[queue[i].y+1][queue[i].x] = used_m[queue[i].y][queue[i].x]+1;
                         queue[queue_size].x=queue[i].x;
                         queue[queue_size++].y=queue[i].y+1;
                     };
                 if((queue[i].x+1<m)&&(used_m[queue[i].y][queue[i].x+1]==-1)&&((field[queue[i].y][queue[i].x+1]==' ')||(field[queue[i].y][queue[i].x+1]=='*')||(field[queue[i].y][queue[i].x+1]=='Q')||(field[queue[i].y][queue[i].x+1]=='Y')||(field[queue[i].y][queue[i].x+1]=='.')))
                     {           //RIGHT
                         used_m[queue[i].y][queue[i].x+1] = used_m[queue[i].y][queue[i].x]+1;
                         queue[queue_size].x=queue[i].x+1;
                         queue[queue_size++].y=queue[i].y;
                     };
                     i++;
                     //cout << i << " ERROR 5" << endl;
           };
            //cout << i << " ERROR 6" << endl;
            //for(int j=0;j<100;j++) cout << used_m[queue[j].y][queue[j].x] << endl;
            //system("pause");
           // find putb
           int temp_x=queue[i].x,temp_y=queue[i].y;
           while(used_m[temp_y][temp_x]>0)
           {
                   //cout << i << " ERROR 7" << endl;
                   if(used_m[temp_y-1][temp_x]+1==used_m[temp_y][temp_x])  //UP
                   {
                       used_m[temp_y][temp_x]=-6;
                       temp_y--;
                   };
                   if(used_m[temp_y][temp_x-1]+1==used_m[temp_y][temp_x])  //LEFT
                   {
                       used_m[temp_y][temp_x]=-6;
                       temp_x--;
                   };
                   if(used_m[temp_y+1][temp_x]+1==used_m[temp_y][temp_x])  //DOWN
                   {
                       used_m[temp_y][temp_x]=-6;
                       temp_y++;
                   };
                   if(used_m[temp_y][temp_x+1]+1==used_m[temp_y][temp_x])  //RIGHT
                   {
                       used_m[temp_y][temp_x]=-6;
                       temp_x++;
                   };

                   //cout << temp_y <<"|" << temp_x << " / " << used_p[temp_y][temp_x] << endl;
           };

           /*cout << endl;
           for(int i=0;i<n;i++)
           {
              for(int j=0;j<m;j++)
               if(used_p[i][j]==-1) cout << '*' << " ";
               else if(used_p[i][j]==-5) cout << "@" << " ";
                 else cout << "$" << " ";
              cout << endl;
           }; */
       moove_minotavr();
       };
};

void player_bfs_priority()
{
    // init
    //cout << "ERROR 1"<<endl;
    zero_used_bfs();

    // dijkstra
    koord v;
    int i=0;
    while(q_prior_size>0)
    {
        //cout << q_prior_size <<" ERROR 2"<<endl;
        v = q_prior_min();
        if(v.prior==999999999) break;
        //cout << "(x:" << v.x << " y:" << v.y << ") prior: " << v.prior << " time: " << v.time;
        if((field[v.y-1][v.x]==' ')||
           (field[v.y-1][v.x]=='M')||
           (field[v.y-1][v.x]=='W')||
           (field[v.y-1][v.x]=='Q')||
           (field[v.y-1][v.x]=='*')) //up
        {
              if((field[v.y-1][v.x]=='W')&&(q_prior[v.y-1][v.x].prior>q_prior[v.y][v.x].prior+2))
              {
                  q_prior[v.y-1][v.x].prior=q_prior[v.y][v.x].prior+2;
                  q_prior[v.y-1][v.x].time=i++;
              };
              if((field[v.y-1][v.x]==' ')&&(q_prior[v.y-1][v.x].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y-1][v.x].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y-1][v.x].time=i++;
              };
              if((field[v.y-1][v.x]=='*')&&(q_prior[v.y-1][v.x].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y-1][v.x].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y-1][v.x].time=i++;
              };
              if((field[v.y-1][v.x]=='M')&&(q_prior[v.y-1][v.x].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y-1][v.x].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y-1][v.x].time=i++;
              };
              if((field[v.y-1][v.x]=='Q')&&(q_prior[v.y-1][v.x].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y-1][v.x].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y-1][v.x].time=i++;
              };
        };
        if((field[v.y][v.x-1]==' ')||
           (field[v.y][v.x-1]=='M')||
           (field[v.y][v.x-1]=='W')||
           (field[v.y][v.x-1]=='Q')||
           (field[v.y][v.x-1]=='*')) //left
        {
              if((field[v.y][v.x-1]=='W')&&(q_prior[v.y][v.x-1].prior>q_prior[v.y][v.x].prior+2))
              {
                  q_prior[v.y][v.x-1].prior=q_prior[v.y][v.x].prior+2;
                  q_prior[v.y][v.x-1].time=i++;
              };
              if((field[v.y][v.x-1]==' ')&&(q_prior[v.y][v.x-1].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y][v.x-1].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y][v.x-1].time=i++;
              };
              if((field[v.y][v.x-1]=='*')&&(q_prior[v.y][v.x-1].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y][v.x-1].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y][v.x-1].time=i++;
              };
              if((field[v.y][v.x-1]=='M')&&(q_prior[v.y][v.x-1].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y][v.x-1].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y][v.x-1].time=i++;
              };
              if((field[v.y][v.x-1]=='Q')&&(q_prior[v.y][v.x-1].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y][v.x-1].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y][v.x-1].time=i++;
              };
        };
        if((field[v.y+1][v.x]==' ')||
           (field[v.y+1][v.x]=='M')||
           (field[v.y+1][v.x]=='W')||
           (field[v.y+1][v.x]=='Q')||
           (field[v.y+1][v.x]=='*')) //down
        {
              if((field[v.y+1][v.x]=='W')&&(q_prior[v.y+1][v.x].prior>q_prior[v.y][v.x].prior+2))
              {
                  q_prior[v.y+1][v.x].prior=q_prior[v.y][v.x].prior+2;
                  q_prior[v.y+1][v.x].time=i++;
              };
              if((field[v.y+1][v.x]==' ')&&(q_prior[v.y+1][v.x].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y+1][v.x].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y+1][v.x].time=i++;
              };
              if((field[v.y+1][v.x]=='*')&&(q_prior[v.y+1][v.x].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y+1][v.x].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y+1][v.x].time=i++;
              };
              if((field[v.y+1][v.x]=='M')&&(q_prior[v.y+1][v.x].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y+1][v.x].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y+1][v.x].time=i++;
              };
              if((field[v.y+1][v.x]=='Q')&&(q_prior[v.y+1][v.x].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y+1][v.x].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y+1][v.x].time=i++;
              };
        };
        if((field[v.y][v.x+1]==' ')||
           (field[v.y][v.x+1]=='M')||
           (field[v.y][v.x+1]=='W')||
           (field[v.y][v.x+1]=='Q')||
           (field[v.y][v.x+1]=='*')) //right
        {
              if((field[v.y][v.x+1]=='W')&&(q_prior[v.y][v.x+1].prior>q_prior[v.y][v.x].prior+2))
              {
                  q_prior[v.y][v.x+1].prior=q_prior[v.y][v.x].prior+2;
                  q_prior[v.y][v.x+1].time=i++;
              };
              if((field[v.y][v.x+1]==' ')&&(q_prior[v.y][v.x+1].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y][v.x+1].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y][v.x+1].time=i++;
              };
              if((field[v.y][v.x+1]=='*')&&(q_prior[v.y][v.x+1].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y][v.x+1].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y][v.x+1].time=i++;
              };
              if((field[v.y][v.x+1]=='M')&&(q_prior[v.y][v.x+1].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y][v.x+1].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y][v.x+1].time=i++;
              };
              if((field[v.y][v.x+1]=='Q')&&(q_prior[v.y][v.x+1].prior>q_prior[v.y][v.x].prior+1))
              {
                  q_prior[v.y][v.x+1].prior=q_prior[v.y][v.x].prior+1;
                  q_prior[v.y][v.x+1].time=i++;
              };
        };
        //system("pause");
    };
    //cout << "ERROR 3"<<endl;
    v=q_prior[escape_y][escape_x];
    koord temp=v;
    while(v.prior!=0)
    {
        used_p[v.y][v.x]=-8;
        if(q_prior[v.y-1][v.x].prior<v.prior) temp=q_prior[v.y-1][v.x]; // up
        if(q_prior[v.y][v.x-1].prior<v.prior) temp=q_prior[v.y][v.x-1]; // left
        if(q_prior[v.y+1][v.x].prior<v.prior) temp=q_prior[v.y+1][v.x]; // down
        if(q_prior[v.y][v.x+1].prior<v.prior) temp=q_prior[v.y][v.x+1]; // right
        v=temp;
    };
    used_p[temp.y][temp.x]=-8;
};

koord q_prior_min()
{
    koord result;
    result.x=player_x;
    result.y=player_y;
    result.prior=999999999;
    result.time=999999999;
    //int result_i;
    //int result_j;
    for(int i=0;i<n;i++)
         for(int j=0;j<m;j++)
            if((q_prior[i][j].time!=-1)&&((q_prior[i][j].prior<result.prior)||((q_prior[i][j].prior==result.prior)&&(q_prior[i][j].time<result.time))))
            {
                result=q_prior[i][j];
                //result_i=i;
                //result_j=j;
            };
    /*for(int i=result_i;i<q_prior.size()-1;i++)
        q_prior[i]=q_prior[i+1];
    q_prior.pop_back();*/
    q_prior[result.y][result.x].time=-1;
    q_prior_size--;
    return result;
};
koord q_prior_min_minotavr()
{
    koord result;
    result.x=minotavr_x;
    result.y=minotavr_y;
    result.prior=999999999;
    result.time=999999999;
    //int result_i;
    //int result_j;
    for(int i=0;i<n;i++)
         for(int j=0;j<m;j++)
            if(
               (q_prior_m[i][j].time!=-1)&&
               ((q_prior_m[i][j].prior<result.prior)||((q_prior_m[i][j].prior==result.prior)&&(q_prior_m[i][j].time<result.time))))
            {
                result=q_prior_m[i][j];
                //result_i=i;
                //result_j=j;
            };
    /*for(int i=result_i;i<q_prior.size()-1;i++)
        q_prior[i]=q_prior[i+1];
    q_prior.pop_back();*/
    q_prior_m[result.y][result.x].time=-1;
    q_prior_size--;
    return result;
};
void minotavr_bfs_priority()
{
    // init
    //cout << "ERROR 1"<<endl;
    //zero_used_bfs();

    // dijkstra
    koord v;
    int i=0;
    while(q_prior_size>0)
    {
        //cout << q_prior_size <<" ERROR 2"<<endl;
        v = q_prior_min_minotavr();
        if(v.prior==999999999) break;
        //cout << "(x:" << v.x << " y:" << v.y << ") prior: " << v.prior << " time: " << v.time;
        if((field[v.y-1][v.x]==' ')||
           (field[v.y-1][v.x]=='Y')||
           (field[v.y-1][v.x]=='T')||
           (field[v.y-1][v.x]=='*')) //up
        {
              if((field[v.y-1][v.x]=='T')&&(q_prior_m[v.y-1][v.x].prior>q_prior_m[v.y][v.x].prior+2))
              {
                  q_prior_m[v.y-1][v.x].prior=q_prior_m[v.y][v.x].prior+2;
                  q_prior_m[v.y-1][v.x].time=i++;
              };
              if((field[v.y-1][v.x]==' ')&&(q_prior_m[v.y-1][v.x].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y-1][v.x].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y-1][v.x].time=i++;
              };
              if((field[v.y-1][v.x]=='*')&&(q_prior_m[v.y-1][v.x].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y-1][v.x].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y-1][v.x].time=i++;
              };
              if((field[v.y-1][v.x]=='Y')&&(q_prior_m[v.y-1][v.x].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y-1][v.x].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y-1][v.x].time=i++;
              };
              if((field[v.y-1][v.x]=='Q')&&(q_prior_m[v.y-1][v.x].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y-1][v.x].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y-1][v.x].time=i++;
              };
        };
        if((field[v.y][v.x-1]==' ')||
           (field[v.y][v.x-1]=='Y')||
           (field[v.y][v.x-1]=='T')||
           (field[v.y][v.x-1]=='*')) //left
        {
              if((field[v.y][v.x-1]=='T')&&(q_prior_m[v.y][v.x-1].prior>q_prior_m[v.y][v.x].prior+2))
              {
                  q_prior_m[v.y][v.x-1].prior=q_prior_m[v.y][v.x].prior+2;
                  q_prior_m[v.y][v.x-1].time=i++;
              };
              if((field[v.y][v.x-1]==' ')&&(q_prior_m[v.y][v.x-1].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y][v.x-1].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y][v.x-1].time=i++;
              };
              if((field[v.y][v.x-1]=='*')&&(q_prior_m[v.y][v.x-1].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y][v.x-1].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y][v.x-1].time=i++;
              };
              if((field[v.y][v.x-1]=='Y')&&(q_prior_m[v.y][v.x-1].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y][v.x-1].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y][v.x-1].time=i++;
              };
              if((field[v.y][v.x-1]=='Q')&&(q_prior_m[v.y][v.x-1].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y][v.x-1].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y][v.x-1].time=i++;
              };
        };
        if((field[v.y+1][v.x]==' ')||
           (field[v.y+1][v.x]=='Y')||
           (field[v.y+1][v.x]=='T')||
           (field[v.y+1][v.x]=='*')) //down
        {
              if((field[v.y+1][v.x]=='T')&&(q_prior_m[v.y+1][v.x].prior>q_prior_m[v.y][v.x].prior+2))
              {
                  q_prior_m[v.y+1][v.x].prior=q_prior_m[v.y][v.x].prior+2;
                  q_prior_m[v.y+1][v.x].time=i++;
              };
              if((field[v.y+1][v.x]==' ')&&(q_prior_m[v.y+1][v.x].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y+1][v.x].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y+1][v.x].time=i++;
              };
              if((field[v.y+1][v.x]=='*')&&(q_prior_m[v.y+1][v.x].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y+1][v.x].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y+1][v.x].time=i++;
              };
              if((field[v.y+1][v.x]=='Y')&&(q_prior_m[v.y+1][v.x].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y+1][v.x].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y+1][v.x].time=i++;
              };
              if((field[v.y+1][v.x]=='Q')&&(q_prior_m[v.y+1][v.x].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y+1][v.x].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y+1][v.x].time=i++;
              };
        };
        if((field[v.y][v.x+1]==' ')||
           (field[v.y][v.x+1]=='Y')||
           (field[v.y][v.x+1]=='T')||
           (field[v.y][v.x+1]=='*')) //right
        {
              if((field[v.y][v.x+1]=='T')&&(q_prior_m[v.y][v.x+1].prior>q_prior_m[v.y][v.x].prior+2))
              {
                  q_prior_m[v.y][v.x+1].prior=q_prior_m[v.y][v.x].prior+2;
                  q_prior_m[v.y][v.x+1].time=i++;
              };
              if((field[v.y][v.x+1]==' ')&&(q_prior_m[v.y][v.x+1].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y][v.x+1].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y][v.x+1].time=i++;
              };
              if((field[v.y][v.x+1]=='*')&&(q_prior_m[v.y][v.x+1].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y][v.x+1].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y][v.x+1].time=i++;
              };
              if((field[v.y][v.x+1]=='Y')&&(q_prior_m[v.y][v.x+1].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y][v.x+1].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y][v.x+1].time=i++;
              };
              if((field[v.y][v.x+1]=='Q')&&(q_prior_m[v.y][v.x+1].prior>q_prior_m[v.y][v.x].prior+1))
              {
                  q_prior_m[v.y][v.x+1].prior=q_prior_m[v.y][v.x].prior+1;
                  q_prior_m[v.y][v.x+1].time=i++;
              };
        };
        //system("pause");
    };
    //cout << "ERROR 3"<<endl;
    v=q_prior_m[player_y][player_x];
    koord temp=v;
    while(v.prior!=0)
    {
        used_m[v.y][v.x]=-6;
        if((q_prior_m[v.y-1][v.x].prior<v.prior)&&(field[v.y-1][v.x]!='W')) temp=q_prior_m[v.y-1][v.x]; // up
        if((q_prior_m[v.y][v.x-1].prior<v.prior)&&(field[v.y][v.x-1]!='W')) temp=q_prior_m[v.y][v.x-1]; // left
        if((q_prior_m[v.y+1][v.x].prior<v.prior)&&(field[v.y+1][v.x]!='W')) temp=q_prior_m[v.y+1][v.x]; // down
        if((q_prior_m[v.y][v.x+1].prior<v.prior)&&(field[v.y][v.x+1]!='W')) temp=q_prior_m[v.y][v.x+1]; // right
        v=temp;
    };
    used_m[temp.y][temp.x]=-6;
};
void moove_minotavr_tree()
{
     if((uroven==9))
        if(move_count_m==1)
        {
            move_count_m--;
            //cout << "FLOATS!" << endl;
            //if(comand==224) comand = _getch();
            //output_field();
            return;
        };
     int perem=0;
     bool flag=false;

     if((field[minotavr_y-1][minotavr_x]=='Y')&&
        (field[minotavr_y-1][minotavr_x]!='W')&&
        (field[minotavr_y-1][minotavr_x]!='U')) {perem=1;flag=true;} //up
     if((field[minotavr_y][minotavr_x-1]=='Y')&&
        (field[minotavr_y][minotavr_x-1]!='W')&&
        (field[minotavr_y][minotavr_x-1]!='U')) {perem=2;flag=true;} //left
     if((field[minotavr_y+1][minotavr_x]=='Y')&&
        (field[minotavr_y+1][minotavr_x]!='W')&&
        (field[minotavr_y+1][minotavr_x]!='U')) {perem=3;flag=true;} //down
     if((field[minotavr_y][minotavr_x+1]=='Y')&&
        (field[minotavr_y][minotavr_x+1]!='W')&&
        (field[minotavr_y][minotavr_x+1]!='U')) {perem=4;flag=true;} // right

     if((!flag)&&(used_m[minotavr_y-1][minotavr_x]==-6)) perem=1; //up
     if((!flag)&&(used_m[minotavr_y][minotavr_x-1]==-6)) perem=2; //left
     if((!flag)&&(used_m[minotavr_y+1][minotavr_x]==-6)) perem=3; //down
     if((!flag)&&(used_m[minotavr_y][minotavr_x+1]==-6)) perem=4; // right

     if(perem==1)  //up
     {
         field[minotavr_y--][minotavr_x] = ' ';
         if((uroven==9)&&(field[minotavr_y][minotavr_x]=='T')) move_count_m++;
         field[minotavr_y][minotavr_x] = 'M';
         if((minotavr_y==player_y)&&(minotavr_x==player_x))
         {
             cout << "YOU ATE, YOU DIED!!\n";
             loose_game_flag=true;
             end_game_flag=true;
         };
     } else
         if(perem==2)  //left
         {
             field[minotavr_y][minotavr_x--] = ' ';
             if((uroven==9)&&(field[minotavr_y][minotavr_x]=='T')) move_count_m++;
             field[minotavr_y][minotavr_x] = 'M';
             if((minotavr_y==player_y)&&(minotavr_x==player_x))
             {
                 cout << "YOU ATE, YOU DIED!!\n";
                 loose_game_flag=true;
                 end_game_flag=true;
             };
         } else
             if(perem==3) //down
             {
                 field[minotavr_y++][minotavr_x] = ' ';
                 if((uroven==9)&&(field[minotavr_y][minotavr_x]=='T')) move_count_m++;
                 field[minotavr_y][minotavr_x] = 'M';
                 if((minotavr_y==player_y)&&(minotavr_x==player_x))
                 {
                     cout << "YOU ATE, YOU DIED!!\n";
                     loose_game_flag=true;
                     end_game_flag=true;
                 };
             } else
                 if(perem==4) // right
                 {
                     field[minotavr_y][minotavr_x++] = ' ';
                     if((uroven==9)&&(field[minotavr_y][minotavr_x]=='T')) move_count_m++;
                     field[minotavr_y][minotavr_x] = 'M';
                     if((minotavr_y==player_y)&&(minotavr_x==player_x))
                     {
                         cout << "YOU ATE, YOU DIED!!\n";
                         loose_game_flag=true;
                         end_game_flag=true;
                     };
                 };
};
