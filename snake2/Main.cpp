#define _CRT_SECURE_NO_WARNINGS
#include "MrHagLibrary.h"
//#include <stdlib.h>
#include <thread>
#define HeroParamCount 3
#define Map_x 70
#define Map_y 35
char **buf=new char*[Map_y];
bool pause = false;
bool work = true;
bool lose = true;
void prnt(char * context, int size)
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	int len = size;
	DWORD l = size;

	wchar_t* ptr = chartoLPWSTR(context);

	WriteConsoleOutputCharacter(hOutput, ptr, len, { 0,0 }, &l);//Пишем из буфера в консоль

	delete ptr;

}



class point
{

public:
	int X;
	int Y;
	point(int x=-1, int y=-1)
	{
		if (x == -1 && y == -1)
		{
			int aX = randomo(0, Map_x - 1);
			int aY = randomo(0, Map_y - 1);
			while (aX < 0 || aX >= Map_x || aY < 0 || aY >= Map_y || buf[aY][aX] != ' ')
			{
				aX = randomo(0, Map_x - 1);
				aY = randomo(0, Map_y - 1);
			}
			X = aX;
			Y = aY;
		}
		else
		{

			X = x;
			Y = y;
		}

	}

};
point *poin;
class hero
{
public:
	int X = 0;
	int Y = 0;
	int SIZE = 4;
	bool dirischanged = false;
	//Array<int> **g = new Array<int>*(new Array<int>(SIZE));
	int **g = new int*[SIZE];
	int direction = 0;
	hero(int x, int y)
	{
		for (int i = 0; i < SIZE; i++)
		{

				g[i]=new int[HeroParamCount];

		}
		for (int i = 0; i < SIZE; i++)
		{
			
			for (int j = 0; j < HeroParamCount; j++)
			{
				g[i][j] = 0;
			}
		}
		X = x;
		Y = y;
	}

	int negative_direction(int di)
	{
	
		if (di != 1)
		{

			di -= 2;
			if (di < 0)
				di = -di;
		}
		else
		{

			di = 3;

		}
		return di;
	}
	void set_direction(int dir)
	{
		if (!dirischanged)
		{
			dirischanged = true;
			//if(dir+2 != direction && dir - 2 != direction)
			if (direction == negative_direction(dir))
			{
				dir = negative_direction(dir);

			}
			direction = dir;
		}
	}

	bool checkpos(int x, int y)
	{
		
			if (x >= 0 && x < Map_x && y >= 0 && y < Map_y && buf[y][x] != (char)254)
			{
				
				if (buf[y][x] == (char)15)
				{

					delete [] poin;
					poin = new point();
					add(1);

				}
				return true;
			}
			else
			{
				work = false;
				return false;
			}
	}

	void refresh_tale()
	{
	
		for (int i = SIZE - 1; i >= 0; i--)
		{
			if (i - 1 >= 0)
			{
				for (int j = 0; j < HeroParamCount; j++)
				{
					g[i][j] = g[i - 1][j];
				}
			}

		}
		g[0][0] = X;
		g[0][1] = Y;
		g[0][2] = direction;

	}

	void add(int count)
	{
		for (int i = 0; i < SIZE;i++)
		{
			int **newarr = new int*[SIZE+1];
			if (SIZE > SIZE + 1)
			{
				newarr[i] = new int[HeroParamCount];
				for (int j = 0; j < HeroParamCount; j++)
				{
					newarr[i][j] = g[i][j];
				}
				delete[] g[i];
			}
			else
			{
				for (int i = 0; i < SIZE; i++)
				{
					newarr[i] = new int[HeroParamCount];
					for(int j=0;j<HeroParamCount;j++)
					{
							newarr[i][j]=g[i][j];
					}
					delete[] g[i];
				}
			}
			
			newarr[SIZE] = new int[HeroParamCount];
			newarr[SIZE][0] = 0;
			newarr[SIZE][1] = 0;
			newarr[SIZE][2] = 0;
			delete[] g;
			g = new int*[SIZE + 1];

			for (int i = 0; i < SIZE + 1; i++)
			{
				g[i] = new int[HeroParamCount];
				for (int j = 0; j < HeroParamCount; j++)
				{
					g[i][j] = newarr[i][j];
					
				}
				delete newarr[i];
			}
			delete[] newarr;

			//g[i]->set_array(i, HeroParamCount);
		}
		SIZE = SIZE + 1;
		//g->dynamic_array(g->Size() + 1);
		
	}
	void change_pos()
	{
		if (direction == 0)
		{
			if (checkpos(X-1,Y))
			{
				refresh_tale();
				X--;
			}
		}
		else
			if (direction == 1)
			{

				if (checkpos(X, Y-1))
				{
					refresh_tale();
					Y--;
				}

			}
			else
				if (direction == 2)
				{

					if (checkpos(X + 1, Y))
					{
						refresh_tale();
						X++;
					}

				}
				else
					if (direction == 3)
					{

						if (checkpos(X, Y + 1))
						{
							refresh_tale();
							Y++;
						}

					}
		dirischanged = false;
	}


};

void control(hero *hero)
{
	
	while (work)
	{
		Sleep(1);
		while (work && !pause)
		{

			int a = _getch();
			if (a == 'w' || (char)a == 'ц')
			{
				hero->set_direction(1);
			}
			else
			{

				if (a == 'a' || (char)a == 'ф')
				{
					hero->set_direction(0);
				}
				else
					if (a == 's' || (char)a == 'ы')
					{
						hero->set_direction(3);
					}
					else
						if (a == 'd' || (char)a == 'в')
						{

							hero->set_direction(2);

						}
						else
							if (a == 27)
							{
								if (pause)
									pause = false;
								else
									pause = true;

							}
							
			}
		}
	}


}

int main()
{
	//local("RUS");
	srand(time(NULL));
	setlocale(LC_ALL, "RUS");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode con cols=70 lines=35");
start:
	char** MymenuCache = new char*[2]{ 0 };
	char* st = new char[5];
    strcpy(st,"Старт");
	char* ex = new char[5];
	strcpy(ex, "Выйти");
	

	str_to_866(st);
	str_to_866(ex);
	MymenuCache[0] = st;
	MymenuCache[1] = ex;

	menu<char *> Mymenu;
	Mymenu.new_menu(MymenuCache, 2, false);
	int a = Mymenu.active_menu();
	if (a == 1)
		exit(0);
	work = true;
	pause = false;
	Mymenu.Delete();
	delete[] MymenuCache;
	delete st;
	delete ex;

	if (true)
	{
		for (int i = 0; i < Map_y; i++)
		{
			buf[i] = new char[Map_x];
		}
		for (int i = 0; i < Map_y; i++)
		{
			for (int j = 0; j < Map_x; j++)
			{
				buf[i][j] = ' ';
			}
		}
		poin = new point();
	hero Sneak(25, 25);
	thread contr(control, &Sneak);
	contr.detach();
	while (work)
	{
		
		Sleep(100);
		while (work && !pause)
		{
			Sleep(100);
			Sneak.change_pos();
			for (int i = 0; i < Map_y; i++)
			{
				for (int j = 0; j < Map_x; j++)
				{
					buf[i][j] = ' ';
				}
			}
			buf[poin->Y][poin->X] = (char)15;
			for (int i = 0; i < Sneak.SIZE; i++)
			{
				char a = (char)254;
				/*if (Sneak.g[i].arr[2] == 0 || Sneak.g[i].arr[2] == 2)
				{
					a = (char)254;
				}
				else
				{
					a = (char)219;
				}*/
				buf[Sneak.g[i][1]][Sneak.g[i][0]] = a;
			}
			if (true)
			{
				char a;
				switch (Sneak.direction)
				{
				case 0:a = (char)17; break;
				case 1:a = (char)30; break;
				case 2:a = (char)16; break;
				case 3:a = (char)31; break;
				}
				buf[Sneak.Y][Sneak.X] = a;
			}


			char*mu = new char[Map_y * Map_x];
			for (int i = 0, s = 0; i < Map_y; i++)
			{
				for (int j = 0; j < Map_x; j++)
				{

					mu[s] = buf[i][j];
					s++;
				}
			}
			Cursor::SetCursorPos(0, 0);
			prnt(mu, Map_y * Map_x);
			Cursor::SetCursorPos(Map_x - 1, Map_y - 1);
			delete[] mu;

		}
		if (work)
		{
			char **MymenuCache = new char*[2]{ 0 };
			MymenuCache[0]=(char*)"Продолжить";
			MymenuCache[1]=(char*)"Выйти";
			menu<char *> Mymenu;
			Mymenu.new_menu(MymenuCache, 2, true);
			int a = Mymenu.active_menu();
			if (a == 1)
				exit(0);
			if (a == -1 || a == 0)
				pause = false;
			cls(GetStdHandle(STD_OUTPUT_HANDLE));
			Mymenu.Delete();
			delete[] MymenuCache[0];
			delete[] MymenuCache[1];
			delete[] MymenuCache;
		}
	}
	char *v;
	if (lose)
		v = (char*)"WASTED";
	else
		v = (char*)"WIN";

	for (int i = 0; i < size_char_string(v); i++)
	{
		buf[Map_y/2][(Map_x/2 - size_char_string(v)) + i+2] = v[i];
	}


	char*mu = new char[Map_y * Map_x];
	for (int i = 0, s = 0; i < Map_y; i++)
	{
		for (int j = 0; j < Map_x; j++)
		{

			mu[s] = buf[i][j];
			s++;
		}
	}
	Cursor::SetCursorPos(0, 0);
	prnt(mu, Map_y * Map_x);
	Cursor::SetCursorPos(Map_x - 1, Map_y - 1);
	delete[] mu;
	for (int i = 0; i < Sneak.SIZE; i++)
	{
		
		delete [] Sneak.g[i];
	}
	delete[] Sneak.g;
	_getch();
	_getch();
	_getch();
}
			goto start;
	/*delete[] MymenuCache[0];
	delete[] MymenuCache[1];
	delete[] MymenuCache;*/

	return 0;
}

