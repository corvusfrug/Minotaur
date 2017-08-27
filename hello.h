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
       int prior;  // ��� ������� < 8 �� ������������
       int time;   // ��� ������� < 8 �� ������������
};

void intro();                 // �����������, �����, ���������
void fin();                   // ����� �������������� ��� ������ ������, ����� ����������� ������
void loose();                 // ����� ���������������� ��� ������ ������
void input_field();           // ���� ����� ����, ������������� ������
void output_field();          // ����� ����� ����, ���������(DFS/BFS)
void moove(int);              // ������, ������ �� ��������(� ����������)
void moove_minotavr();        // ����������� ���������
void player_moove(char);      // ����������� ������
void player_dfs(int,int);     // ����� ������ ���� ������ �� ������
void minotavr_dfs(int,int);   // ����� ������ ���� ��������� �� ������
void zero_used();             // ��������� ������ ��������� ��� DFS-��
void zero_used_bfs();         // ��������� ������ ��������� ��� BFS-��
void player_bfs();            // ����� ����������� ���� ������ �� ������
void minotavr_bfs();          // ����� ����������� ���� ��������� �� ������
void minotavr_bfs_smart();    // ����� ����������� ���� ��������� �� ����������� ���� ������ � ����� �� ������
void moove_minotavr_tree();   // ����������� ��������� (����� ������� ����)

// prior queue
void player_bfs_priority();   // �������� ������ � �������� � ������������ ��� ������ (����������� ���� ����������� �����)
void minotavr_bfs_priority(); // �������� ������ � �������� � ������������ ��� ��������� (����������� ���� ����������� �����)
koord q_prior_min();          // ���� �� ������� ���� (������� �����) ������������ ���� ��� ������
koord q_prior_min_minotavr(); // ���� �� ������� ���� (������� �����) ������������ ���� ��� ���������
// prior queue

extern char** field;                // ����� ����    ��� extern � hello.cpp
extern int n,m;                     // ������� ����
extern bool end_game_flag;          // ����� ����
extern bool loose_game_flag;        // ��������
extern int used_flag;               // ��� DFS ���������
extern int player_x,player_y;       // ���������� ������
extern int minotavr_x, minotavr_y;  // ���������� ���������
extern int score;                   // ���� ������ (������ == �����)
extern int** used_p;                // ������� ��������� ��� BFS-�� ������
extern int** used_m;                // ������� ��������� ��� BFS-�� ���������
extern int uroven;                  // ����� ����

#endif // HELLO_H_INCLUDED
