#pragma once
#include <iostream>
#include <conio.h>
#include <string>
#include <string.h>
#include <vector>
#include <windows.h>


using namespace std;



#define PERR(bSuccess, api){if(!(bSuccess)) printf("%s:Error %d from %s \ on line %d\n", __FILE__, GetLastError(), api, __LINE__);}

wchar_t* chartoLPWSTR(char* context)
{
	int size = strlen(context) + 1;
	wchar_t* wtext = new wchar_t[size];
	mbstowcs(wtext, context, size);//Plus null
	return wtext;

}

void cls(HANDLE hConsole)
{
	COORD coordScreen = { 0, 0 };    /* here's where we'll home the
										cursor */
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
	DWORD dwConSize;                 /* number of character cells in
										the current buffer */

										/* get the number of character cells in the current buffer */

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	PERR(bSuccess, "GetConsoleScreenBufferInfo");
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	/* fill the entire screen with blanks */

	bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR) ' ',
		dwConSize, coordScreen, &cCharsWritten);
	PERR(bSuccess, "FillConsoleOutputCharacter");

	/* get the current text attribute */

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	PERR(bSuccess, "ConsoleScreenBufferInfo");

	/* now set the buffer's attributes accordingly */

	bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten);
	PERR(bSuccess, "FillConsoleOutputAttribute");

	/* put the cursor at (0, 0) */

	bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
	PERR(bSuccess, "SetConsoleCursorPosition");
	return;
}
/*
Функція рандому в заданому діапазоні
#include(time.h/string)
одноразовий запуск - srand(time(NULL));
*/
int randomo(const int &a, const int &b)
{
	if (a > b)
		return 0;
	return a + rand() % ((b - 1) - a);
}


template<typename T, typename T1, typename T2>
T step(T1 a, T2 b)
{
	T num = a;
	for (int i = 1; i < b; i++)
	{
		num = num * a;
	}
	if (b == 0)
		return 1;
	return num;
}

//Повертає середнє значення елементів масиву;
template<typename T>
static double middle(T a, int size)
{
	double sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += a[i];
	}
	return sum / size;
}

//Повертає массив мінімального і максимального елемента масиву та їх позиції;
template<typename T>
T min_and_max(T a, int size)
{
	int min[2], max[2];
	min[0] = a[0];
	max[0] = a[0];
	for (int i = 0; i < size; i++)
	{
		if (min[0] > a[i])
		{
			min[0] = a[i];
			min[1] = i;
		}

		if (max[0] < a[i])
		{
			max[0] = a[i];
			max[1] = i;
		}

	}
	int arr[4] = { 0 };
	arr[0] = min[0];
	arr[1] = min[1];
	arr[2] = max[0];
	arr[3] = max[1];
	return arr;
}

class min_max
{
public:
	int min, max, min_pos, max_pos;
	template<typename T>
	void min_and_max(T a, int size)
	{

		min = a[0];
		max = a[0];
		for (int i = 0; i < size; i++)
		{
			if (min > a[i])
			{
				min = a[i];
				min_pos = i;
			}

			if (max < a[i])
			{
				max = a[i];
				max_pos = i;
			}

		}

	}

};






//Повертає массив вектора від стрічки, яка розбивається символом "key";
vector<string> parsing_string(string a, char key)
{
	vector<string>arr;
	string tmp;
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] == key || i + 1 == a.length())
		{
			arr.push_back(tmp);
			tmp = "";
		}
		else
		{
			tmp = tmp + a[i];
		}
	}
	return arr;
}

//Полегшує роботу з cin
template<typename T1>
T1 Mwrite()
{
	T1 a;
	cin >> a;
	return a;
}


namespace Cursor
{
	//Ставить курсор консолі на вказані координати
	void SetCursorPos(int x, int y)
	{
		COORD position = { x,y };
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hConsole, position);
	}

	void SetCursorPos(HANDLE hConsole, COORD position)
	{
		SetConsoleCursorPosition(hConsole, position);
	}

}


//Повертає массив розміру вікна консолі в символах
int *GetConsoleMaxBuf()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	cout << columns << "  " << rows;
	int arr[2] = { columns,rows };
	return arr;
}

class GetConsoleMaxBuf
{
public:

	static int X()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int rows;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		rows = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		return rows;
	}
	static int Y()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int columns;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		columns = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		return columns;
	}
};


//Зчитування та запис в консоль
class ConsoleBuffer
{
private:
	char *buffer = new char;
	DWORD len;
	HANDLE hOutput;
	COORD coord = { 0,0 };
public:
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	void Read()
	{
		Delete();



		//std::cout << "My string???" << std::endl;
		hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hOutput, &csbi);
		buffer = new char[csbi.dwSize.X*csbi.dwSize.Y + 1];//Выделяем память для буфера
		wchar_t* ptr = chartoLPWSTR(buffer);
		ReadConsoleOutputCharacter(hOutput, ptr, csbi.dwSize.X*csbi.dwSize.Y, coord, &len);//Пишем в буфер
		coord.X = coord.Y = 0;
		delete ptr;



	}
	void Write()
	{
		//std::cout << "MY STRING. THIS IS HOROSHO :)" << std::endl;
		//Sleep(2000);//Делаем задержку, чтобы увидеть строку MY STRING
		wchar_t* ptr = chartoLPWSTR(buffer);
		WriteConsoleOutputCharacter(hOutput, ptr, len, coord, &len);//Пишем из буфера в консоль
		Cursor::SetCursorPos(hOutput, csbi.dwCursorPosition);

		delete ptr;
	}
	void Delete()
	{
		delete[] buffer;
	}
};



//Прототип для запису стрічки
class Write
{
public:


	int count;
	char *b;
	int pos;
	int pre_size;
	void write(char **out = (char **)' ', const char *pre_str = "")
	{
		int CP = GetConsoleCP();
		SetConsoleCP(1251);
		//SetConsoleOutputCP(1251);

		ConsoleBuffer Buf;
		Buf.Read();

		b = new char[1];
		b[0] = '\0';

		count = 1;
		pre_size = 0;
		pos = 0;
		for (; pre_str[pre_size] != '\0'; pre_size++) {}
		print_m(pre_str, Buf.csbi.dwCursorPosition.Y, &Buf);
		char a = _getch();
		while (a != 13)
		{
			if (a != 0)
			{
				if (a != '\b')
				{
					if (a == -32)
					{

						int b = _getch();
						if (b != 0)
						{
							if (b == 75)
							{
								a = -1;
								pos--;
								if (pos < 0)
								{
									pos = count - 1;
								}

							}
							else
							{
								if (b == 77)
								{
									a = -1;
									pos++;
									if (pos > count - 1)
									{
										pos = 0;
									}
								}
								else if (b == 83)
								{
									outsert(pos, false);
								}
								else
								{
									print_m(pre_str, Buf.csbi.dwCursorPosition.Y, &Buf);
								}
							}
						}
						else
						{
							insert(a, pos++);
						}
					}
					else
					{
						insert(a, pos++);

					}
				}
				else
				{
					outsert(pos - 1);

				}
			}
			print_m(pre_str, Buf.csbi.dwCursorPosition.Y, &Buf);
			a = _getch();
		}
		if (out != (char **)' ')
		{
			set(out);

		}
		Buf.Write();
		Buf.Delete();
		SetConsoleCP(CP);
		_getch();
	}



	void Delete()
	{
		delete[] b;
	}

	void set(char **to)
	{
		delete[] * to;
		*to = new char[count];
		for (int i = 0; i < count; i++)
		{
			(*to)[i] = b[i];
		}
	}
	char* resize(char *bi, int s)
	{
		char *tmp = new char[count];
		for (int i = 0; i < count; i++)
		{
			tmp[i] = bi[i];
		}

		//Delete();

		int min = 0;
		if (count > s)
			min = s;
		else
			min = count;

		delete[] bi;
		bi = new char[s];
		for (int i = 0; i < min; i++)
		{
			bi[i] = tmp[i];
		}
		count = s;
		delete[] tmp;
		return bi;
	}

	void insert(char c, int p)
	{
		b = resize(b, count + 1);
		for (int i = count - 1; i > p; i--)
		{
			b[i] = b[i - 1];
		}
		b[p] = c;
	}

	void outsert(int p, bool left = true)
	{
		if (p >= 0 && p < count - 1)
		{
			for (int i = p; i < count - 1; i++)
			{
				b[i] = b[i + 1];
			}
			b = resize(b, count - 1);
			if (left)
			{
				pos--;
			}
		}
	}

	void print_m(const char *pre_str, int y, ConsoleBuffer *Buf)
	{
		system("cls");
		(*Buf).Write();
		SetConsoleCP(866);
		for (int i = 0; pre_str[i] != '\0'; i++)
		{
			cout << pre_str[i];
		}
		for (int i = 0; i < count; i++)
		{
			cout << b[i];
		}
		Cursor::SetCursorPos((int)((pos + pre_size) % GetConsoleMaxBuf::X()), (int)(y + ((pos + pre_size) / GetConsoleMaxBuf::X())));
		SetConsoleCP(1251);
	}
};



//Функція яка повертає розмір константного массиву
//size_const(&b)
template<typename T>
int size_const(T a)
{
	return sizeof(*a) / sizeof(*a[0]);
}

int size_char_string(char *string)
{
	int i = 0;
	while (string[i++] != '\0')
	{
	}
	return i;
}



//Прототип динамічного масиву
template<class T>
class Array
{
private:
	bool chk;
	int SIZE;
	
public:
	T *arr;

	Array(int size) {
		chk = false;
		SIZE = (size > 0) ? size : 1;
		arr = (T*)malloc(SIZE * sizeof(T));
	}

	void dynamic_array(int new_size)
	{
		if (!SIZE)
			SIZE = 0;
		T *newarr = (T*)malloc(new_size * sizeof(T));
		if (SIZE > new_size)
		{
			for (int i = 0; i < new_size; i++)
				newarr[i] = arr[i];
		}
		else
		{
			for (int i = 0; i < SIZE; i++)
				newarr[i] = arr[i];
		}

		Delete();
		arr = (T*)malloc(new_size * sizeof(T));

		for (int i = 0; i < new_size; i++)
		{
			arr[i] = newarr[i];
		}
		free(newarr);
		SIZE = new_size;
	}


	int Size()
	{
		return SIZE;
	}

	T *at(int pos)
	{
		return &arr[pos];
	}
	void set_array(int poz, int nsize)
	{
		T *a = new T(nsize);
		arr[poz] = *a;
	}



	void Delete_all()
	{
		//delete [] arr;
		for (int i = 0; i < SIZE; i++)
		{
			delete[] arr[i].at(0);
		}
		delete[] arr;
	}

	void Delete()
	{
		delete[] arr;
	}

};

//Прототип меню

/*char** MymenuCache = new char*[2]{ 0 };
str_to_866(&MymenuCache[0], (char*)"Старт");
str_to_866(&MymenuCache[1], (char*)"Выйти");

menu<char *> Mymenu;
Mymenu.new_menu(MymenuCache, 2);
Mymenu.active_menu();
_getch();
delete[] MymenuCache[0];
delete[] MymenuCache[1];
delete[] MymenuCache;
*/
template<class T>
class menu
{

public:
	Array<T> *arr;
	int size, position;
	bool abort;
	void new_menu(T *g_arr, int vsize, bool ab)
	{
		position = 0;
		abort = ab;
		size = vsize;
		arr = new Array<T>(size);
		for (int i = 0; i < size; i++)
		{
			*(arr->at(i)) = g_arr[i];
		}
	}
	void print_menu()
	{
		cls(GetStdHandle(STD_OUTPUT_HANDLE));
		for (int i = 0; i < size; i++)
		{
			if (position == i)
			{
				cout << "> ";
			}
			cout << *(arr->at(i)) << endl;
		}
	}


	int active_menu()
	{
		print_menu();
		while (true)
		{
			int a = _getch();
			if (a == 13)
			{
				cls(GetStdHandle(STD_OUTPUT_HANDLE));
				return position;
			}
			else
				if (a == 80)
				{
					if (++position > arr->Size() - 1)
						position = 0;
					print_menu();
				}
				else
					if (a == 72)
					{
						if (--position < 0)
							position = arr->Size() - 1;
						print_menu();
					}
					else
						if(a==27 && abort)
						{ 
							cls(GetStdHandle(STD_OUTPUT_HANDLE));
								return -1;
						}
			


		}
	}

	void Delete()
	{
		arr->Delete();
		delete arr;
	}
};




//Видаляє динамічний вказівник;
template<typename T>
static void pointer(T **point)
{
	*point = nullptr;
	delete *point;
}


// Полегшує роботу з класом Write
char *My_Mwrite(const char* text = "")
{
	Write *Buf = new Write;
	char *string = new char;
	Buf->write(&string, text);
	Buf->Delete();
	delete Buf;
	pointer(&Buf);
	return string;
}


void local(const char* string)
{

	setlocale(LC_ALL, string);

}

int char_to_int(char c)
{
	switch (c)
	{
	case '0':return 0;
	case '1':return 1;
	case '2':return 2;
	case '3':return 3;
	case '4':return 4;
	case '5':return 5;
	case '6':return 6;
	case '7':return 7;
	case '8':return 8;
	case '9':return 9;
	default: return -1;
	}
}

/*int string_to_int(char *string, int size = 0, const char* del = "")
{
	if (size == 0)
		size = size_char_string(string);
	bool chk = true;
	bool negative = false;
	Array<int> arr(0);
	for (int i = 0; i < size; i++)
	{

		int g = char_to_int(string[i]);
		if (g != -1)
		{

			*arr.at(arr.Size() - 1) = g;
			arr.dynamic_array(arr.Size() + 1);
			chk = false;
		}
		else
		{
			if (string[i] == '-' && chk == true)
			{
				if (i + 1 < size)
				{
					g = char_to_int(string[i + 1]);
					if (g != -1)
					{
						chk = false;
						negative = true;
					}
				}
			}
		}
	}

	arr.dynamic_array(arr.Size() - 1);
	long int spec = step<long int>(10, arr.Size());
	long int num = 0;
	for (int i = 0; i < arr.Size(); i++)
	{

		num += step<long int>(10, arr.Size()-1-i) * *arr.at(i);

	}
	if (negative)
		num = -num;
	arr.Delete();
	if (del == "delete")
		delete[] string;
	return num;
}
*/
template<typename T>
T string_to_number(char *string, int size = 0, const char* del = "")
{

	if (size == 0)
		size = size_char_string(string);
	bool chk = true;
	bool negative = false;
	bool numm = false;
	bool point = false;
	int position = -1;
	Array<int> arr(0);
	bool stop = false;
	for (int i = 0; i < size && !stop; i++)
	{

		int g = char_to_int(string[i]);
		if (g != -1)
		{

			*arr.at(arr.Size() - 1) = g;
			arr.dynamic_array(arr.Size() + 1);
			numm = true;
			chk = false;
		}
		else
		{

			if (string[i] == '-' && chk)
			{
				if (i + 1 < size)
				{
					g = char_to_int(string[i + 1]);
					if (g != -1)
					{
						chk = false;
						negative = true;
					}
				}
			}
			else
			{


				if ((string[i] == '.' || string[i] == ',') && numm && !point)
				{
					if ((T).1 != (int).1)
					{
						point = true;
						position = arr.Size() - 1;
					}
					else
					{
						stop = true;
					}
				}

			}
		}
	}

	arr.dynamic_array(arr.Size() - 1);
	T num = 0;
	for (int i = 0; i < arr.Size(); i++)
	{

		num += step<T>(10, arr.Size() - 1 - i) * *arr.at(i);

	}
	if (position != -1 && position < arr.Size())
		num = num / step<T>(10, (arr.Size() - position));
	if (negative)
		num = -num;
	arr.Delete();
	if (del == "delete")
		delete[] string;
	return num;

}

void str_to_866(char *p, char* p2=0)
{
	
	if (!p)
		p = new char[size_char_string(p2)];
	if (p2 == 0)
		p2 = p;
	sprintf(p, "%s", p2);
	wchar_t* ptr = chartoLPWSTR(p2);
	CharToOem(ptr, p);
	delete ptr;
}

void str_to_866(char **p, char* p2 = 0)
{

	if (!*p)
		*p = new char[size_char_string(p2)];
	if (p2 == 0)
		p2 = *p;
	sprintf(*p, "%s", p2);
	wchar_t* ptr = chartoLPWSTR(p2);
	CharToOem(ptr, *p);
	delete ptr;
}
/*
class Screen
{
public:
	static void Fullscreen()
	{
		HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD maxWindow = GetLargestConsoleWindowSize(out_handle); // размер самого большого возможного консольного окна
		SMALL_RECT srctWindow = { 0, 0, maxWindow.X - 1, maxWindow.Y - 1 };
		SMALL_RECT minWindow = { 0, 0, 0, 0 };
		SetConsoleWindowInfo(out_handle, true, &minWindow);
		SetConsoleScreenBufferSize(out_handle, maxWindow);
		SetConsoleWindowInfo(out_handle, true, &srctWindow);
	}

	static void screen_size(int x,int y)
	{
		COORD Window;
		Window.X = x;
		Window.Y = y;
		HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD maxWindow = GetLargestConsoleWindowSize(out_handle);
		if (x > maxWindow.X)
			x = maxWindow.X;
		if (y > maxWindow.Y)
			y = maxWindow.Y;
		SMALL_RECT srctWindow = { 0, 0, x - 1, x - 1 };
		SMALL_RECT minWindow = { 0, 0, 0, 0 };
		SetConsoleWindowInfo(out_handle, true, &minWindow);
		SetConsoleScreenBufferSize(out_handle, Window);
		SetConsoleWindowInfo(out_handle, true, &srctWindow);
	//system("MODE CON: COLS=100 LINES=100");
	}

};

enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void SetColor(ConsoleColor text, ConsoleColor background = Black)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}*/