#pragma once
#include "framework.h"
void generateField();
int is_empty_file(FILE* fp);
int loadFile();
int saveFile();
void fillField();
void drawField(HDC hdc);
void checkPlus(int pos_x, int pos_y, int num_of_box);
void checkMouse(int x, int y);