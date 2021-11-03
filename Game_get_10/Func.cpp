#include "framework.h"
#include "Func.h"
#include <time.h>
#include <stdlib.h>
#define N 5
int size = 50;
int Field[N][N];
bool check = false;
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
int is_empty_file(FILE* fp) {
	int c = getc(fp);
	if (c == EOF)
		return 1;
	ungetc(c, fp);
	return 0;
}
int loadFile() {
	FILE* fin = fopen("save.txt", "rt");
	if (fin == NULL) {
		printf("File %s is not opened", "save.txt");
		return 0;
	}
	if (is_empty_file(fin)) {
		return 0;
	}
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			fscanf_s(fin, "%d", &Field[i][j]);
		}
	}
	fscanf_s(fin, "%d", &score);
	fclose(fin);
	return 1;
}
int saveFile() {
	FILE* fin = fopen("save.txt", "wt");
	if (fin == NULL) {
		printf("File %s is not opened", "save.txt");
		return 0;
	}
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			fprintf(fin, "%d ", Field[i][j]);
		}
		fprintf(fin, "\n");
	}
	fprintf(fin, "%d", score);
	fclose(fin);
	return 1;
}
void fillField() {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (Field[i][j] == -1) {
				Field[i][j] = rand() % 3 + 1;
			}
		}
	}
}
void drawBeginWindow(HDC hdc) {
	HFONT hFont;
	hFont = CreateFont(20,
		0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET,
		0, 0, 0, 0,
		L"Courier New"
	);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(148, 0, 211));
	TCHAR  string1[] = _T("Вы хотите усложнить игру?");
	TCHAR  string2[] = _T("Да!");
	TCHAR  string3[] = _T("Нет");
	TextOut(hdc, 500, 200, (LPCWSTR)string1, _tcslen(string1));
	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	Rectangle(hdc, 350, 300, 550, 350);
	TextOut(hdc, 425, 310, (LPCWSTR)string2, _tcslen(string2));
	Rectangle(hdc, 700, 300, 900, 350);
	TextOut(hdc, 775, 310, (LPCWSTR)string3, _tcslen(string3));
	DeleteObject(hPen);
	DeleteObject(hFont);
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
		check = true;
	}
	if (pos_x - 1 >= 0 && Field[pos_x - 1][pos_y] == num_of_box) {
		Field[pos_x - 1][pos_y] = -1;
		checkPlus(pos_x - 1, pos_y, num_of_box);
		check = true;
	}
	if (pos_y - 1 >= 0 && Field[pos_x][pos_y - 1] == num_of_box) {
		Field[pos_x][pos_y - 1] = -1;
		checkPlus(pos_x, pos_y - 1, num_of_box);
		check = true;
	}
	if (pos_y + 1 < N && Field[pos_x][pos_y + 1] == num_of_box) {
		Field[pos_x][pos_y + 1] = -1;
		checkPlus(pos_x, pos_y + 1, num_of_box);
		check = true;
	}
}
void checkMouse(int x, int y) {
	if ((x >= 200 && x <= 200 + 50 * N) && (y >= 200 && y <= 200 + 50 * N)) {
		score++;
		int num_x = int((x - movementFromWall) / 50);
		int num_y = int((y - movementFromWall) / 50);
		int num_in_box = Field[num_x][num_y];
		checkPlus(num_x, num_y, num_in_box);
		if (check) {
			Field[num_x][num_y] = num_in_box + 1;
			fillField();
			check = false;
		}
	}
}

