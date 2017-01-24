#include <iostream>
#include <fstream>
#include <cmath>
#include <queue>
#include <string>
#include <cstdio>
#include <cstdlib>

int rozmiar_s=0;
int j=0;
int k=0;
using namespace std;


/***********************************Aktualizacja LP i Harmonogram**********************************/
void Aktualizuj(int liczba_operacji, int tablica_poprzednikow_T[], int tablica_poprzednikow_M[], int LP[], queue <int> kolejka, int harmonogram[])
{
	int temp2=0; //zmienna pomocnicza
	int temp3=0; //zmienna pomocnicza2
	int k=0; //zmienna pomocnicza3
	int u=0; //zmienna pomocnicza4
		
		for (int i=0;i<liczba_operacji;i++) 
		{	
			for (int l=0;l<liczba_operacji;l++)
			{
				if(LP[l] == 0)
				{		
					kolejka.push(l+1); //zeby sie numer zgadza³
					temp2=kolejka.front();
					harmonogram[k]=temp2;
					k++;
					kolejka.pop();
					LP[l] = -1; //zeby nie wchodzil 2 razy w to samo miejsce
				}
			}
			for (int j=0;j<liczba_operacji;j++)
			{
				if (tablica_poprzednikow_T[j] == harmonogram[u])
				LP[j]--;
				if (tablica_poprzednikow_M[j] == harmonogram[u])
				LP[j]--;
			}
				u++;
		}
}
/***********************************************************************************************/

/*******************************Bloki krytyczne*************************************************/
void Bloki_krytyczne(int liczba_operacji, int sciezka[], int tablica_poprzednikow_T[], int tablica_nastepnikow_T[], int tablica_poprzednikow_M[], int tablica_nastepnikow_M[], int Bloki[][1])
{
	int temp=0;
	
	if(tablica_nastepnikow_M[sciezka[0]-1] == sciezka[1])
	{
			Bloki[j][0] = sciezka[0];
	}
	for(int i=1;i<rozmiar_s;i++)
	{
		if(tablica_nastepnikow_M[sciezka[i-1]] == sciezka[i+1] && tablica_poprzednikow_M[sciezka[i]] != sciezka[i-1])
		{
			Bloki[j][0] = sciezka[i];
		}	
		else if(tablica_poprzednikow_M[sciezka[i]-1] == sciezka[i-1] && (tablica_nastepnikow_M[sciezka[i]-1] != sciezka[i+1]))
		{
			Bloki[j][1] = sciezka[i];
			cout << "COS";
		}
		if(Bloki[j][0] != 0 && Bloki[j][1] != 0)
		{
			j++;
		}
		
	}
	if(tablica_poprzednikow_M[sciezka[rozmiar_s]] == sciezka[rozmiar_s-1])
	{
		Bloki[j][1] = sciezka[rozmiar_s];
	}
}

/***********************************************************************************************/

/*******************************Sciezka krytyczna***********************************************/
void Sciezka_krytyczna(int liczba_operacji, int Cmax, int Cindeks, int tablica_poprzednikow_T[],int S[], int C[], int tablica_poprzednikow_M[],int sciezka[])
{
	sciezka[0] = Cindeks;
	int temp=0; // zmienna pomocnicza
	int k=0; // zmienna pomocnicza2
	int l=0; //zmienna pomocnicza3
	temp = S[Cindeks];
	for (int i=1; i<liczba_operacji; i++)
	{
		if(C[tablica_poprzednikow_T[Cindeks-1]] == temp)
		{
			sciezka[i] = tablica_poprzednikow_T[Cindeks-1];
			Cindeks = tablica_poprzednikow_T[Cindeks-1];
			temp = S[Cindeks];
		}
		else if(C[tablica_poprzednikow_M[Cindeks-1]] == temp)
		{
			sciezka[i] = tablica_poprzednikow_M[Cindeks-1];
			Cindeks = tablica_poprzednikow_M[Cindeks-1];
			temp = S[Cindeks];
		}
	}
	for(int i=0; i<liczba_operacji; i++)
	{
		if(sciezka[i] != 0)
		{
			k++;
		}
	}
	k=k-1;
	rozmiar_s=k;
	l=k/2;
	for(int i=0; i<=l;i++)
	{
		temp=sciezka[i];
		sciezka[i]=sciezka[k];
		sciezka[k]=temp;
		k--;
	}
	
}
/***********************************************************************************************/

/*******************************Liczenie Cmax***************************************************/
void Policz_Cmax (int liczba_operacji, int C[], int S[], int Cmax, int Cindeks)
{
	Cmax=0;
	for (int i=0;i<liczba_operacji+1;i++)
	{
		if(C[i] >= Cmax)
		{
			Cmax=C[i];
			Cindeks=i;
		}
	}
	C[0] = Cmax; //bo inaczej zwraca³ 0 ^.-
	S[0] = Cindeks; //bo inaczej zwraca³ 0 ^.-
	
}
/***********************************************************************************************/

/*******************************Liczenie C i S**************************************************/
void Policz_czasy(int liczba_operacji, int tablica_poprzednikow_T[], int tablica_poprzednikow_M[], int S[], int C[] , int harmonogram[], int tablica_czasow[], int liczba_maszyn)
{ 
	S[0]=0;
	C[0]=0;
	int zad; //zmienna pomocnicza
	for (int i=0;i<liczba_operacji;i++)
	{
		zad = harmonogram[i];
		if(tablica_poprzednikow_T[zad-1]==0 && tablica_poprzednikow_M[zad-1]==0)
		{
			S[zad] = 0;
			C[zad] = S[zad] + tablica_czasow[zad-1];
		}
		else
		{
		S[zad] = max(C[tablica_poprzednikow_T[zad-1]], C[tablica_poprzednikow_M[zad-1]]);
		C[zad] = S[zad] + tablica_czasow[zad-1];
		}
	}
}
/***********************************************************************************************/
/******************************************Tabu*************************************************/
void Tabu(int tablica_nastepnikow_M[], int Bloki[][1], int tablica_zabronien[][800],int tablica_nastepnikow_T[],int LP[] , int harmonogram[],queue <int> kolejka, int tablica_czasow[], int liczba_maszyn, int tablica_poprzednikow_M[], int tablica_poprzednikow_T[], int Cindeks, int Cmax, int liczba_operacji, int C[],int S[],int sciezka[] )
{  
int indeks_zabronien=0;
int nowy_Cmax=Cmax;
int	tablica_nastepnikow_M_temp_2[800];
//glowna petla po ilosci blokow k=
for(int g=0;g<j;g++){

	int Cmax_poczatek=0,Cmax_koniec=0;
	int poczatek=0;
	int koniec=0;
	int temp=0;
	int tablica_nastepnikow_M_temp[800];
	
	int temp2=0;
	bool status1 =true;
	bool statusg=true;
	for(int i=0;i<800;i++)
	{
		tablica_nastepnikow_M_temp[i] = tablica_nastepnikow_M[i];
	}
	
	for(int i=0;i<k;i++)
	{
		if(sciezka[i] == Bloki[g][0])
		{
			poczatek=i;
		}
		if(sciezka[i] == Bloki[g][1])
		{
			koniec=i;
		}
	}
		temp = Bloki[g][0];
		temp2 = tablica_nastepnikow_M_temp[temp];
		tablica_nastepnikow_M_temp[temp] = tablica_nastepnikow_M_temp[sciezka[poczatek+1]];
		tablica_nastepnikow_M_temp[sciezka[poczatek+1]] = temp2;
	
		//sprawdzam czypermutacja w zabronieniach
/*
		for(int m=0 ; m<20 ;m++)		
		{		for(int i=0;i<liczba_operacji;i++)
			{
				if(	tablica_nastepnikow_M_temp[i]==tablica_zabronien[m][i])
				status1=false;
				
			}

		}
		if(status1==false)
			statusg=false;	*/
	//POPRZEDNICY
	//wykluczam zabronienie
	if(statusg==true){
		
		for (int i=0;i<liczba_operacji+1;i++)
		{
			tablica_poprzednikow_T[tablica_nastepnikow_T[i]] = i+1;
			tablica_poprzednikow_M[tablica_nastepnikow_M[i]] = i+1;
		}
		temp=0;
		for (int i=0;i<liczba_operacji+1;i++) //bo zerowy element sie pieprzy
		{
			temp = tablica_poprzednikow_T[i+1];
			tablica_poprzednikow_T[i]=temp;
		}
		temp=0;	
		for (int i=0;i<liczba_operacji+1;i++) //bo zerowy element sie pieprzy
		{
			temp = tablica_poprzednikow_M[i+1];
			tablica_poprzednikow_M[i]=temp;
		}
			//LP
	for (int i=0;i<liczba_operacji;i++)
	{
		if(tablica_poprzednikow_T[i] != 0 && tablica_poprzednikow_M[i] != 0)
		{
			LP[i]=2;
		}
		if(tablica_poprzednikow_T[i] != 0 && tablica_poprzednikow_M[i] == 0)
		{
			LP[i]=1;
		}
		if(tablica_poprzednikow_T[i] == 0 && tablica_poprzednikow_M[i] != 0)
		{
			LP[i]=1;
		}
		if(tablica_poprzednikow_T[i] == 0 && tablica_poprzednikow_M[i] == 0)
		{
			LP[i]=0;
		}
	}
	/*******************************************************************************************/
		
		Aktualizuj(liczba_operacji,tablica_poprzednikow_T,tablica_poprzednikow_M,LP,kolejka,harmonogram);
		Policz_czasy(liczba_operacji,tablica_poprzednikow_T,tablica_poprzednikow_M,S,C,harmonogram,tablica_czasow,liczba_maszyn);
		Policz_Cmax(liczba_operacji,C,S,Cmax,Cindeks);
		Cmax_poczatek = C[0]; //problem z przekazaniem z funkcji
		Cindeks = S[0]; //problem z przekazaniem z funkcji
		
		cout << "nowy cmax: ";
		cout << Cmax_poczatek;
		if (Cmax_poczatek < nowy_Cmax)
		{
			nowy_Cmax=Cmax_poczatek;
			
			for(int i=0;i<liczba_operacji;i++)
			{
				tablica_nastepnikow_M_temp_2[i] = tablica_nastepnikow_M_temp[i];
			}
		 
		}
	}
	////////////////////UWAGA KONIEC Z POPRZEDNIKIEM KONCA ZAMIANA
	///////////////////////////////////////////////////////////////
	status1=true;
	statusg=true;

		for(int i=0;i<liczba_operacji;i++)
	{
		tablica_nastepnikow_M_temp[i] = tablica_nastepnikow_M[i];
	}
		temp = Bloki[g][1];
		temp2 = tablica_nastepnikow_M_temp[temp];
		tablica_nastepnikow_M_temp[temp] = tablica_nastepnikow_M_temp[sciezka[koniec-1]];
		tablica_nastepnikow_M_temp[sciezka[koniec-1]] = temp2;
	
		/*	for(int m=0 ; m<20 ;m++)		
		{		for(int i=0;i<liczba_operacji;i++)
			{
				if(	tablica_nastepnikow_M_temp[i]==tablica_zabronien[m][i])
				status1=false;
				
			}

		}
		if(status1==false)
			statusg=false;	*/
	//POPRZEDNICY
	//wykluczam zabronienie
	if(statusg==true){

		
	//POPRZEDNICY
	for (int i=0;i<liczba_operacji+1;i++)
	{
		tablica_poprzednikow_T[tablica_nastepnikow_T[i]] = i+1;
		tablica_poprzednikow_M[tablica_nastepnikow_M[i]] = i+1;
	}
	temp=0;
	for (int i=0;i<liczba_operacji+1;i++) //bo zerowy element sie pieprzy
	{
		temp = tablica_poprzednikow_T[i+1];
		tablica_poprzednikow_T[i]=temp;
	}
	temp=0;	
	for (int i=0;i<liczba_operacji+1;i++) //bo zerowy element sie pieprzy
	{
		temp = tablica_poprzednikow_M[i+1];
		tablica_poprzednikow_M[i]=temp;
	}
		//LP
for (int i=0;i<liczba_operacji;i++)
{
	if(tablica_poprzednikow_T[i] != 0 && tablica_poprzednikow_M[i] != 0)
	{
		LP[i]=2;
	}
	if(tablica_poprzednikow_T[i] != 0 && tablica_poprzednikow_M[i] == 0)
	{
		LP[i]=1;
	}
	if(tablica_poprzednikow_T[i] == 0 && tablica_poprzednikow_M[i] != 0)
	{
		LP[i]=1;
	}
	if(tablica_poprzednikow_T[i] == 0 && tablica_poprzednikow_M[i] == 0)
	{
		LP[i]=0;
	}
}
/*******************************************************************************************/
	
	Aktualizuj(liczba_operacji,tablica_poprzednikow_T,tablica_poprzednikow_M,LP,kolejka,harmonogram);
	Policz_czasy(liczba_operacji,tablica_poprzednikow_T,tablica_poprzednikow_M,S,C,harmonogram,tablica_czasow,liczba_maszyn);
	Policz_Cmax(liczba_operacji,C,S,Cmax,Cindeks);
	Cmax_koniec = C[0]; //problem z przekazaniem z funkcji
	Cindeks = S[0]; //problem z przekazaniem z funkcji
	
	cout << "nowy cmax: ";
	cout << nowy_Cmax;
		if (Cmax_koniec < nowy_Cmax)
		{
			nowy_Cmax=Cmax_koniec;
			
			for(int i=0;i<liczba_operacji;i++)
			{
				tablica_nastepnikow_M_temp_2[i] = tablica_nastepnikow_M_temp[i];
			}
		 
		}	
	}
	indeks_zabronien++;
}//koniec petli po g
	if(nowy_Cmax<Cmax){

			for(int i=0;i<liczba_operacji;i++)
			{
				tablica_nastepnikow_M[i] = tablica_nastepnikow_M_temp_2[i];
				tablica_zabronien[0][i]=	tablica_nastepnikow_M[i];
			}
		}
}
/***********************************************************************************************/

int main(int argc, char** argv) 
{
	
	int tablica_nastepnikow_T[800]; //tablica nastêpników technologicznych
	int tablica_zabronien[20][800];
	int tablica_nastepnikow_M[800]; //tablica nastêpników maszynowych
	int tablica_poprzednikow_T[800]; //tablica poprzedników technologicznych
	int tablica_poprzednikow_M[800]; //tablica poprzedników maszynowych
	int S[800]; //tablica czasów rozpoczêcia
	int C[800]; //tablica czasów zakoñczenia
	int harmonogram[800];
	int sciezka[800]; // tablica ze sciezka krytyczna
	int LP[800]; //Harmonogram rozwi¹zywania zadañ
	int Bloki[100][1];
	int tablica_czasow[800]; //tablica czasów trwania operacji
	int liczba_operacji=0;
	int liczba_maszyn=0;
	int temp=0; //zmienna pomocnicza
	int permutacje[800];
	int rozmiar; //rozmiar kolejki
	int Cmax;
	int Cindeks; //indeks Cmax w tablicy C
	
	C[0] = 0;
	
	queue <int> kolejka;
	
	ifstream plik;
	plik.open("data5.txt");
	
	plik >> liczba_operacji; //1
	for (int i=0;i<liczba_operacji;i++)
	{
		plik >> tablica_nastepnikow_T[i];
	}
		for (int i=0;i<liczba_operacji;i++)
	{
		plik >> tablica_nastepnikow_M[i];
	}
		for (int i=0;i<liczba_operacji;i++)
	{
		plik >> tablica_czasow[i];
	}
	plik >> liczba_maszyn;
	
/***********************************Poprzednicy maszynowi i technologiczni******************/
	for (int i=0;i<liczba_operacji+1;i++)
	{
		tablica_poprzednikow_T[tablica_nastepnikow_T[i]] = i+1;
		tablica_poprzednikow_M[tablica_nastepnikow_M[i]] = i+1;
	}
	temp=0;
	for (int i=0;i<liczba_operacji+1;i++) //bo zerowy element sie pieprzy
	{
		temp = tablica_poprzednikow_T[i+1];
		tablica_poprzednikow_T[i]=temp;
	}
	temp=0;	
	for (int i=0;i<liczba_operacji+1;i++) //bo zerowy element sie pieprzy
	{
		temp = tablica_poprzednikow_M[i+1];
		tablica_poprzednikow_M[i]=temp;
	}
/*******************************************************************************************/
/***********************************Harmonogram*********************************************/
for (int i=0;i<liczba_operacji;i++)
{
	if(tablica_poprzednikow_T[i] != 0 && tablica_poprzednikow_M[i] != 0)
	{
		LP[i]=2;
	}
	if(tablica_poprzednikow_T[i] != 0 && tablica_poprzednikow_M[i] == 0)
	{
		LP[i]=1;
	}
	if(tablica_poprzednikow_T[i] == 0 && tablica_poprzednikow_M[i] != 0)
	{
		LP[i]=1;
	}
	if(tablica_poprzednikow_T[i] == 0 && tablica_poprzednikow_M[i] == 0)
	{
		LP[i]=0;
	}
}
/*******************************************************************************************/
	
	Aktualizuj(liczba_operacji,tablica_poprzednikow_T,tablica_poprzednikow_M,LP,kolejka,harmonogram);
	Policz_czasy(liczba_operacji,tablica_poprzednikow_T,tablica_poprzednikow_M,S,C,harmonogram,tablica_czasow,liczba_maszyn);
	Policz_Cmax(liczba_operacji,C,S,Cmax,Cindeks);
	Cmax = C[0]; //problem z przekazaniem z funkcji
	Cindeks = S[0]; //problem z przekazaniem z funkcji
	Sciezka_krytyczna(liczba_operacji,Cmax,Cindeks,tablica_poprzednikow_T,S,C,tablica_poprzednikow_M,sciezka);
	Bloki_krytyczne(liczba_operacji,sciezka,tablica_poprzednikow_T,tablica_nastepnikow_T,tablica_poprzednikow_M,tablica_nastepnikow_M,Bloki);
	
	
	Tabu(tablica_nastepnikow_M,Bloki,tablica_zabronien,tablica_nastepnikow_T,LP,harmonogram,kolejka,tablica_czasow,liczba_maszyn,tablica_poprzednikow_M,tablica_poprzednikow_T,Cindeks,Cmax,liczba_operacji,C,S,sciezka);
	
/************************************Wyœwietlanie*******************************************/

	cout << "Liczba operacji: ";
	cout << liczba_operacji << endl;
	cout << "Nastepnicy technologiczni: ";
	for (int i=0;i<liczba_operacji+1;i++)
	{
		cout << tablica_nastepnikow_T[i] << " ";
	}
	cout << endl;
	cout << "Nastepnicy maszynowi: ";
	for (int i=0;i<liczba_operacji+1;i++)
	{
		cout << tablica_nastepnikow_M[i] << " ";
	}
	cout << endl;
	cout << "Poprzednicy technologiczni: ";
	for (int i=0;i<liczba_operacji+1;i++)
	{
		cout << tablica_poprzednikow_T[i] << " ";
	}
	cout << endl;
	cout << "Poprzednicy maszynowi: ";
	for (int i=0;i<liczba_operacji+1;i++)
	{
		cout << tablica_poprzednikow_M[i] << " ";
	}
	cout << endl;
	cout << "Czasy rozpoczecia: ";
	for (int i=0;i<liczba_operacji+1;i++)
	{
		cout << S[i] << " ";
	}
	cout << endl;
	cout << "Czasy zakonczenia: ";
	for (int i=0;i<liczba_operacji+1;i++)
	{
		cout << C[i] << " ";
	}
	cout << endl;
	cout << "Czasy trwania: ";
	for (int i=0;i<liczba_operacji;i++)
	{
		cout << tablica_czasow[i] << " ";
	}
	cout << endl;
	cout << "LP: ";
	for (int i=0;i<liczba_operacji;i++)
	{
		cout << LP[i] << " ";
	}
	cout << endl;
	cout << "Harmonogram: ";
	for (int i=0;i<liczba_operacji;i++)
	{
		cout << harmonogram[i] << " ";
	}
	cout << endl;
	cout << "Sciezka krytyczna: ";
	for (int i=0;i<liczba_operacji;i++)
	{
		cout << sciezka[i] << " ";
	}
	cout << endl;
	cout << "Bloki krytyczne: ";
	for (int i=0;i<2;i++)
	{
		cout << Bloki[0][i] << " ";
	}
	cout << endl;
	cout << "Cmax: ";
	cout << Cmax;
	cout << endl;
	cout << "Cindeks: ";
	cout << Cindeks;
	cout << endl;
/*******************************************************************************************/
	plik.close();
	return 0;
}
