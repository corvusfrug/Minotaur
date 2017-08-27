#ifndef HELLO_H_INCLUDED
#define HELLO_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <conio.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::ifstream;
using std::ofstream;
using std::swap;

struct koord
{
       int x,y;
       int prior;  // для уровней < 8 не используется
       int time;   // для уровней < 8 не используется
};

void intro();                 // Приветствие, режим, сложность
void fin();                   // Конец благоприятного для игрока исхода, вывод результатов лучших
void loose();                 // Конец НЕблагоприятного для игрока исхода
void input_field();           // Ввод карты мира, инициализация памяти
void output_field();          // Вывод карты мира, маршрутов(DFS/BFS)
void moove(int);              // Анализ, команд на движение(с клавиатуры)
void moove_minotavr();        // Перемещение минотавра
void player_moove(char);      // Перемещение игрока
void player_dfs(int,int);     // Поиск любого пути игрока до выхода
void minotavr_dfs(int,int);   // Поиск любого пути минотавра до игрока
void zero_used();             // Обнуление матриц смежности бля DFS-ов
void zero_used_bfs();         // Обнуление матриц смежности бля BFS-ов
void player_bfs();            // Поиск кратчайшего пути игрока до выхода
void minotavr_bfs();          // Поиск кратчайшего пути минотавра до игрока
void minotavr_bfs_smart();    // Поиск кратчайшего пути минотавра до кратчайшего пути игрока а затем до игрока
void moove_minotavr_tree();   // Перемещение минотавра (может сносить леса)

// prior queue
void player_bfs_priority();   // Алгоритм работы с очередью с приоритетами для игрока (минимальный путь взвешанного графа)
void minotavr_bfs_priority(); // Алгоритм работы с очередью с приоритетами для минотавра (минимальный путь взвешанного графа)
koord q_prior_min();          // Дает из очереди поле (вершину графа) минимального веса для игрока
koord q_prior_min_minotavr(); // Дает из очереди поле (вершину графа) минимального веса для минотавра
// prior queue

extern char** field;                // Карта мира    все extern в hello.cpp
extern int n,m;                     // Размеры мира
extern bool end_game_flag;          // Конец игры
extern bool loose_game_flag;        // Проигрыш
extern int used_flag;               // Для DFS служебный
extern int player_x,player_y;       // Координаты игрока
extern int minotavr_x, minotavr_y;  // Координаты минотавра
extern int score;                   // Очки игрока (меньше == лучше)
extern int** used_p;                // Матрица смежности для BFS-ов игрока
extern int** used_m;                // Матрица смежности для BFS-ов минотавра
extern int uroven;                  // Режим игры

#endif // HELLO_H_INCLUDED
