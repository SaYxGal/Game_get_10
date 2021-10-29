#include "framework.h"
#include "Func.h"
#include <time.h>
#include <stdlib.h>
#define N 5
int size = 50;
int Field[N][N];
extern int score;
int movementFromWall = 200;
void generateField() {
	srand(time(NULL));
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			Field[i][j] = rand() % 3 + 1;
		}
	}
}
void drawField(HDC hdc) {
	HPEN hpen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	HBRUSH hbrush_1 = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hbrush_2 = CreateSolidBrush(RGB(0, 255, 0));
	HBRUSH hbrush_3 = CreateSolidBrush(RGB(0, 0, 255));
	SelectObject(hdc, hpen);
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			RECT rect = { i * size + movementFromWall, j * size + movementFromWall, (i + 1) * size + movementFromWall, (j + 1) * size + movementFromWall };
			switch (Field[i][j]) {
			case(1):
				FillRect(hdc, &rect, hbrush_1);
				break;
			case(2):
				FillRect(hdc, &rect, hbrush_2);
				break;
			case(3):
				FillRect(hdc, &rect, hbrush_3);
				break;
			}
		}
	}
}
void checkPlus(int pos_x, int pos_y, int num_of_box) {
	if (pos_x + 1 < N && Field[pos_x + 1][pos_y] == num_of_box) {
		Field[pos_x + 1][pos_y] = -1;
		checkPlus(pos_x + 1, pos_y, num_of_box);
	}
	if (pos_x - 1 >= 0 && Field[pos_x - 1][pos_y] == num_of_box) {
		Field[pos_x - 1][pos_y] = -1;
		checkPlus(pos_x - 1, pos_y, num_of_box);
	}
	if (pos_y - 1 >= 0 && Field[pos_x][pos_y - 1] == num_of_box) {
		Field[pos_x][pos_y - 1] = -1;
		checkPlus(pos_x, pos_y - 1, num_of_box);
	}
	if (pos_y + 1 < N && Field[pos_x][pos_y + 1] == num_of_box) {
		Field[pos_x][pos_y + 1] = -1;
		checkPlus(pos_x, pos_y + 1, num_of_box);
	}
}
void checkMouse(int x, int y) {
	if ((x >= 200 && x <= 200 + 50 * N) && (y >= 200 && y <= 200 + 50 * N)) {
		score++;
		int num_x = int((x - movementFromWall) / 50);
		int num_y = int((y - movementFromWall) / 50);
		int num_in_box = Field[num_x][num_y];
		checkPlus(num_x, num_y, num_in_box);
		Field[num_x][num_y] = num_in_box + 1;
	}
}

