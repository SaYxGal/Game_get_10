#pragma once
#include "framework.h"
void generateField();
int is_empty_file(FILE* fp);
int loadFile();
int saveFile();
void lowerBoxes();
void fillField();
bool isLose();
int clearFile();
void drawBeginWindow(HDC hdc);
void drawField(HDC hdc);
void checkPlus(int pos_x, int pos_y, int num_of_box);
void DrawRecords(HDC hdc);
void LoadRecords();
void SaveRecords();
int CompareRecords(int index1, int index2);
void InsertRecord(char name[]);
void createWriteRecord(HWND* hBtn, HWND* hEdt1, HINSTANCE hInst, HWND hWnd, LPARAM lParam);
void checkMouse(int x, int y);