#include "framework.h"
#include "Func.h"
#include <time.h>
#include <stdlib.h>
#define N 5
int size = 50;
int Field[N][N];
int quest = 1;
bool check = false;
extern int score;
extern int time_s;
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
void lowerBoxes() {
	for (int j = 0; j < N; ++j) {
		for (int i = 0; i < N; ++i) {
			if (Field[i][j] == -1) {
				for (int k = i; k > 1; --k) {
					Field[k][j] = Field[k-1][j];
				}
				Field[0][j] = -1;
				break;
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
	TCHAR  string1[] = _T("�� ������ ��������� ����?");
	TCHAR  string2[] = _T("��!");
	TCHAR  string3[] = _T("���");
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
	HBRUSH hbrush_4 = CreateSolidBrush(RGB(48, 213, 200));
	HBRUSH hbrush_5 = CreateSolidBrush(RGB(155, 45, 48));
	HBRUSH hbrush_6 = CreateSolidBrush(RGB(80, 200, 120));
	SelectObject(hdc, hpen);
	TCHAR nums[] = _T("1");
	TCHAR nums1[] = _T("2");
	TCHAR nums2[] = _T("3");
	TCHAR nums3[] = _T("4");
	TCHAR nums4[] = _T("5");
	TCHAR nums5[] = _T("6");
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			RECT rect;
			rect = { j * size + movementFromWall, i * size + movementFromWall, (j + 1) * size + movementFromWall, (i + 1) * size + movementFromWall };//important
			switch (Field[i][j]) {
			case(1):
				FillRect(hdc, &rect, hbrush_1);
				TextOut(hdc, j * size + size / 2 + movementFromWall - 3, i * size + size / 2 + movementFromWall - 10, (LPCWSTR)nums, _tcslen(nums));
				break;
			case(2):
				FillRect(hdc, &rect, hbrush_2);
				TextOut(hdc, j * size + size / 2 + movementFromWall - 3, i * size + size / 2 + movementFromWall - 10, (LPCWSTR)nums1, _tcslen(nums1));
				break;
			case(3):
				FillRect(hdc, &rect, hbrush_3);
				TextOut(hdc, j * size + size / 2 + movementFromWall - 3, i * size + size / 2 + movementFromWall - 10, (LPCWSTR)nums2, _tcslen(nums2));
				break;
			case(4):
				FillRect(hdc, &rect, hbrush_4);
				TextOut(hdc, j * size + size / 2 + movementFromWall - 3, i * size + size / 2 + movementFromWall - 10, (LPCWSTR)nums3, _tcslen(nums3));
				break;
			case(5):
				FillRect(hdc, &rect, hbrush_5);
				TextOut(hdc, j * size + size / 2 + movementFromWall - 3, i * size + size / 2 + movementFromWall - 10, (LPCWSTR)nums4, _tcslen(nums4));
				break;
			case(6):
				FillRect(hdc, &rect, hbrush_6);
				TextOut(hdc, j * size + size / 2 + movementFromWall - 3, i * size + size / 2 + movementFromWall - 10, (LPCWSTR)nums5, _tcslen(nums5));
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
		int num_x = int((y - movementFromWall) / 50);//important
		int num_y = int((x - movementFromWall) / 50);
		int num_in_box = Field[num_x][num_y];
		checkPlus(num_x, num_y, num_in_box);
		if (check) {
			Field[num_x][num_y] = num_in_box + 1;
			lowerBoxes();
			fillField();
			check = false;
			time_s = 10;
		}
	}
}

