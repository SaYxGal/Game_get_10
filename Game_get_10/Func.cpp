#include "framework.h"
#include "Func.h"
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#define N 5
#define MAX_NUM_RECORDS 10
int size = 50;
int Field[N][N];
int quest = 1;
int maxx = 1;
bool check = false;
extern int score;
extern int time_s;
extern bool isEnd;
extern int flag;
struct Record {
	char name[20];
	int max_box;
	int points;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
};
// Таблица рекордов
struct Record records[MAX_NUM_RECORDS + 1];
// текущее количество рекордов в таблице
int numRecords = 0;

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
void LoadRecords() {
	// Открываем файл с рекордами на чтение
	FILE* fout = fopen("records.txt", "rt");
	if (fout == NULL || is_empty_file(fout)) {
		// выходим, не загрузив рекорды из файла
		return;
	}

	fscanf(fout, "%d", &numRecords); // количество рекордов в таблице

	int i;
	for (i = 0; i < numRecords; i++) {
		// загружаем из файла каждое поле каждого рекорда
		fscanf(fout, "%s%d%d%d%d%d%d%d%d\n",
			records[i].name,
			&records[i].max_box,
			&records[i].points,
			&records[i].year,
			&records[i].month,
			&records[i].day,
			&records[i].hour,
			&records[i].minute,
			&records[i].second
		);
	}
	// закрываем файл
	fclose(fout);
}
void SaveRecords() {
	// Запись в выходной файл
	FILE* fout = fopen("records.txt", "wt");
	if (fout == NULL) {
		// выходим, не сохранив результаты в файл
		return;
	}

	fprintf(fout, "%d\n", numRecords);

	int i;
	for (i = 0; i < numRecords; i++) {
		// сохраняем в файле каждое поле каждого рекорда
		fprintf(fout, "%s %d %d %d %d %d %d %d %d\n",
			records[i].name,
			records[i].max_box,
			records[i].points,
			records[i].year,
			records[i].month,
			records[i].day,
			records[i].hour,
			records[i].minute,
			records[i].second
		);
	}
	// закрываем файл
	fclose(fout);
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
				for (int k = i; k >= 1; --k) {
					Field[k][j] = Field[k-1][j];
				}
				Field[0][j] = -1;
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
	HBRUSH hbrush_4 = CreateSolidBrush(RGB(48, 213, 200));
	HBRUSH hbrush_5 = CreateSolidBrush(RGB(155, 45, 48));
	HBRUSH hbrush_6 = CreateSolidBrush(RGB(80, 200, 120));
	HBRUSH hbrush_7 = CreateSolidBrush(RGB(255, 165, 0));
	HBRUSH hbrush_8 = CreateSolidBrush(RGB(172, 183, 142));
	HBRUSH hbrush_9 = CreateSolidBrush(RGB(255, 127, 80));
	HBRUSH hbrush_10 = CreateHatchBrush(5, RGB(255, 0, 0));
	SelectObject(hdc, hpen);
	TCHAR nums[] = _T("1");
	TCHAR nums1[] = _T("2");
	TCHAR nums2[] = _T("3");
	TCHAR nums3[] = _T("4");
	TCHAR nums4[] = _T("5");
	TCHAR nums5[] = _T("6");
	TCHAR nums6[] = _T("7");
	TCHAR nums7[] = _T("8");
	TCHAR nums8[] = _T("9");
	TCHAR nums9[] = _T("10");
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
			case(7):
				FillRect(hdc, &rect, hbrush_7);
				TextOut(hdc, j * size + size / 2 + movementFromWall - 3, i * size + size / 2 + movementFromWall - 10, (LPCWSTR)nums6, _tcslen(nums6));
				break;
			case(8):
				FillRect(hdc, &rect, hbrush_8);
				TextOut(hdc, j * size + size / 2 + movementFromWall - 3, i * size + size / 2 + movementFromWall - 10, (LPCWSTR)nums7, _tcslen(nums7));
				break;
			case(9):
				FillRect(hdc, &rect, hbrush_9);
				TextOut(hdc, j * size + size / 2 + movementFromWall - 3, i * size + size / 2 + movementFromWall - 10, (LPCWSTR)nums8, _tcslen(nums8));
				break;
			case(10):
				FillRect(hdc, &rect, hbrush_10);
				TextOut(hdc, j * size + size / 2 + movementFromWall - 3, i * size + size / 2 + movementFromWall - 10, (LPCWSTR)nums9, _tcslen(nums9));
				break;
			}
		}
	}
}
int clearFile() {
	FILE* fin = fopen("save.txt", "wt");
	if (fin == NULL) {
		printf("File %s is not opened", "save.txt");
		return 0;
	}
	fclose(fin);
	return 1;
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
bool isLose() {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (i + 1 < N && Field[i + 1][j] == Field[i][j]) {
				return false;
			}
			if (i - 1 >=0 && Field[i - 1][j] == Field[i][j]) {
				return false;
			}
			if (j - 1 >=0 && Field[i][j - 1] == Field[i][j]) {
				return false;
			}
			if (j + 1 < N && Field[i][j + 1] == Field[i][j]) {
				return false;
			}
		}
	}
	return true;
}
void createWriteRecord(HWND* hBtn, HWND* hEdt1, HINSTANCE hInst, HWND hWnd, LPARAM lParam) {
	flag = 2;
	// Создаем и показываем поле редактирования - для ввода имени рекордсмена
	*(hEdt1) = CreateWindowW(_T("edit"), _T("Noname"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 800, 300, 160, 20,
		hWnd, 0, hInst, NULL);
	ShowWindow(*(hEdt1), SW_SHOWNORMAL);
	// Создаем и показываем кнопку
	*(hBtn) = CreateWindowW(_T("button"), _T("Запомнить!"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		800, 350, 160, 20, hWnd, 0, hInst, NULL);
	ShowWindow(*(hBtn), SW_SHOWNORMAL);
}
void DrawRecords(HDC hdc) {
	HFONT hFont;
	hFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"Courier New"
	);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(0, 64, 64));

	TCHAR  string1[] = _T("! No ! Дата       ! Время    ! Имя                  ! Макс. знач. ! Очков !");
	TextOut(hdc, 10, 50, (LPCWSTR)string1, _tcslen(string1));

	int i;
	for (i = 0; i < numRecords; i++) {
		TCHAR  string2[80];
		char str[80];
		sprintf(str, "! %2d ! %02d.%02d.%04d ! %02d:%02d:%02d ! %-20s ! %4d       ! %5d !",
			i + 1,
			records[i].day, records[i].month, records[i].year,
			records[i].hour, records[i].minute, records[i].second,
			records[i].name, records[i].max_box, records[i].points
		);
		OemToChar(str, string2);
		TextOut(hdc, 10, 24 * (i + 1) + 50, (LPCWSTR)string2, _tcslen(string2));
	}
	DeleteObject(hFont);
}
int CompareRecords(int index1, int index2)
{
	if (records[index1].max_box < records[index2].max_box)
		return -1;
	if (records[index1].max_box > records[index2].max_box)
		return +1;


	if (records[index1].points < records[index2].points)
		return -1;
	if (records[index1].points > records[index2].points)
		return +1;
	return 0;
}
void InsertRecord(char name[])
{
	strcpy(records[numRecords].name, name);
	records[numRecords].max_box = maxx;
	records[numRecords].points = score;

	SYSTEMTIME st;
	// Получаем текущее время
	GetLocalTime(&st);

	// и разбрасываем его по полям в таблицу рекордов
	records[numRecords].year = st.wYear;
	records[numRecords].month = st.wMonth;
	records[numRecords].day = st.wDay;

	records[numRecords].hour = st.wHour;
	records[numRecords].minute = st.wMinute;
	records[numRecords].second = st.wSecond;
	int i = numRecords;
	while (i > 0) {
		if (CompareRecords(i - 1, i) < 0) {
			struct Record temp = records[i];
			records[i] = records[i - 1];
			records[i - 1] = temp;
		}
		i--;
	}
	// Если таблица заполнена не полностью
	if (numRecords < MAX_NUM_RECORDS) {
		numRecords++;
	}
}
void checkMouse(int x, int y) {
	if ((x >= 200 && x <= 200 + 50 * N) && (y >= 200 && y <= 200 + 50 * N)) {
		int num_x = int((y - movementFromWall) / 50);//important
		int num_y = int((x - movementFromWall) / 50);
		int num_in_box = Field[num_x][num_y];
		checkPlus(num_x, num_y, num_in_box);
		if (check) {
			Field[num_x][num_y] = num_in_box + 1;
			if (Field[num_x][num_y] > maxx) {
				maxx = Field[num_x][num_y];
			}
			score+= 10 * Field[num_x][num_y];
			lowerBoxes();
			fillField();
			check = false;
			time_s = 10;
			if (Field[num_x][num_y] == 10) isEnd = true;
		}
	}
}

