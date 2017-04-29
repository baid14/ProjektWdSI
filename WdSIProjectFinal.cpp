#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <string>
#include <sstream>
using namespace std;

const int psize = 50;				//rozmiar populacji
const int itmax = 3;				//iloœæ kolejnych populacji
const double pmut = 0.01;			//wspó³czynnik prawdopodobieñstwa mutacji
const double pkrz = 0.25;			//wspó³czynnik prawdopodobieñstwa krzy¿owania
#define RAND ((float)rand()/(RAND_MAX+1))		//liczba losowa z przedzia³u <0,1>

class osobnik
{
public:
	int chromosom[23];
	double fitness;
	osobnik();
};

osobnik::osobnik()
{
	for (int i = 0; i < 23; i++)
		chromosom[i] = rand() % 2;			//losowanie kolejnych bitów
	fitness = 0.0;
}

double funkcja(double x)
{
	return pow(x, 1 / 3) + sin(5 * x);		
}

double change(int x[])			//funkcja zamieniaj¹ca bity dwójkowe na liczbê dziesiêtn¹
{
	double z = 0.0, ld;
	for (int i = 0; i < 23; i++)
		z = z + x[i] * pow(2.0, i);
	return ld = (1.5 + 4.5 * z) / (pow(2.0, 23) - 1.0);
}

vector<osobnik> pop, pop2;

void init()				//inicializacja wektora pierwszej populacji
{
	for (int i = 0; i < psize; i++)
	{
		osobnik a;
		pop.push_back(a);
	}
}

void init2()			//inicjalizacja wektora drugiej populacji
{
	for (int i = 0; i < psize; i++)
	{
		osobnik b;
		pop2.push_back(b);
	}
}

void ocena()				//funkcja licz¹ca ocenê przystosowania jako wartoœæ zadanej funkcji dla liczby powsta³ej z bitów chromosomu
{
	for (int i = 0; i < psize; i++)
		pop[i].fitness = funkcja(change(pop[i].chromosom));
}

void mutacja(string &a)				//podmiana losowego bitu z 0 na 1 lub odwrotnie, dla zadanego wspó³czynnika mutacji
{
	if (RAND < pmut)
	{
		int m;
		m = rand() % 23;
		if (a[m] == '1')
			a[m] = '0';
		else
			a[m] = '1';
	}
}

void crossover(string &a, string &b)			//krzy¿owanie dwupunktowe, dla zadanego wspó³czynnika krzy¿owania
{
	if (RAND < pkrz)
	{
		int k1 = 0, k2 = 0, tmp;
		while (k1 == k2)
		{
			k1 = rand() % 23;
			k2 = rand() % 23;
		}
		if (k1 > k2)
		{
			tmp = k1;
			k1 = k2;
			k2 = tmp;
		}
		string pom1 = a.substr(0, k1) + b.substr(k1, k2 - k1) + a.substr(k2, 23 - k2);
		string pom2 = b.substr(0, k1) + a.substr(k1, k2 - k1) + b.substr(k2, 23 - k2);
		a = pom1;
		b = pom2;
	}	
}

string convert(int x[], int n)				//funkcja do zamiany tablicy int na wartoœæ string - potrzebne do funkcji turniej
{
	ostringstream oss("");
	for (int i = 0; i < n; i++)
		oss << x[i];
	return oss.str();
}

string turniej()			//losowanie piêciu osobników z populacji i wybieranie tego o najlepszym przystosowaniu - mniejszej wartoœci fitness
{
	int x[23], t[5], t0;
	double min;
	for (int i = 0; i < 5; i++)
		t[i] = rand() % 50;
	min = pop[t[0]].fitness;
	t0 = t[0];
	for (int k = 1; k < 5; k++)
		if (pop[t[k]].fitness < min)
			t0 = t[k];
	for (int j = 0; j < 23; j++)
			 x[j] = pop[t0].chromosom[j];	
	return convert(x, 23);
}

bool compare(osobnik a, osobnik b)				//porównywanie kolejnych osobników pod wzglêdem przystosowania 
{
	return (a.fitness > b.fitness);
}

int _tmain(int argc, _TCHAR* argv[])
{
	system("COLOR F0");
	srand(time(0));
	init();							//inicjalizacja
	init2();
	int iter = 0;			
	for (int i = 0; i < itmax; i++)
	{
		ocena();				//wyliczanie fitness dla obecnej populacji
		sort(pop.begin(), pop.end(), compare);				//sortowanie osobników po wzglêdem wartoœci fitness
		cout << endl << "Populacja - " << i << endl << endl;
		cout << "  Osobnik                  fitness" << endl;
		for (int i = 0; i < pop.size(); i++)				//wypisanie obecnej populacji
		{
			for (int j = 0; j < 23; j++)
				cout << pop[i].chromosom[j];
			cout << "    " << pop[i].fitness << endl;
		}
		while (iter < psize)			//pêtla do generowania nowej populacji
		{
			string potomek1 = turniej();		//tworzenie potomnych osobników metod¹ selekcji turniejowej
			string potomek2 = turniej();
			crossover(potomek1, potomek2);			//ewentualne krzy¿owanie
			mutacja(potomek1);				//ewentualne mutacje
			mutacja(potomek2);
			for (int j = 0; j < 23; j++)		//dodawanie kolejnych osobników do nowej populacji
			{
				pop2[iter].chromosom[j] = potomek1[j] - '0';
			}
			iter++;
			for (int j = 0; j < 23; j++)
			{
				pop2[iter].chromosom[j] = potomek2[j] - '0';
			}
			iter++;
		}
		iter = 0;
		pop.swap(pop2);			//podmiana, dziêki której nowa populacja staje siê star¹ populacj¹
	}
system("pause");
return 0;
}

