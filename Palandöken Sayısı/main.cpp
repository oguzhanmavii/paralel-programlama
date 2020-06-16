#include "mpi.h"
#include <iostream>
#include <vector>

using namespace std;

#define MASTER 0

bool PalandokenMi(unsigned long long );
bool PalindromMu(vector<int>);
int digitCount(unsigned long long);

int main(int argc, char** argv) {
	
	
	int rank, size;
	unsigned long long altLimit, ustLimit;
	unsigned long long palandokenSayisi = 0; //toplam paland�ken sayilarinin sayisini tutacak de�i�ken
	
	
	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	if(rank == MASTER)
	{
		cout<<"Alt limiti Giriniz: ";
		cin>>altLimit;
		cout<<"Ust limiti Giriniz: ";
		cin>>ustLimit;
		
	}
	
	/*Alt limit ve �st limit b�t�n bilgisayarlara MASTER taraf�ndan da��t�ld�*/
	MPI_Bcast(&altLimit, 1, MPI_UNSIGNED_LONG_LONG, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&ustLimit, 1, MPI_UNSIGNED_LONG_LONG, MASTER, MPI_COMM_WORLD);
	
	unsigned long long kismiPalandokenSayisi=0; //her bir bilgisayarda bulunan paland�ken say�lar�n�n say�s�n� tutacak de�i�ken
	
	MPI_Barrier(MPI_COMM_WORLD);// d�nyadaki bilgisayarlar�n ayn� yere gelmesi bekleniyor.
	
	
	if(rank == MASTER){
		printf("Palandoken sayilar araniyor... Bu islemler girdiginiz araliga bagli olarak biraz zaman alabilir...\n");
	}
	
	/*Paland�ken say�lar aran�yor*/
	for(unsigned long long i = (altLimit+rank); i<=ustLimit ; i+=size){
		if(PalandokenMi(i)){
			kismiPalandokenSayisi++;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);// d�nyadaki bilgisayarlar�n ayn� yere gelmesi bekleniyor.
	
	/*
		her bir bilgisayar�n buldu�u paland�ken sayilar sayisi master da toplan�yor. 
		MASTER A TOPLAM PALAND�KEN SAYISI G�DECEK. Bu da MPI_Reduce yard�m�yla yap�lacak (MPI_SUM)
	*/
	MPI_Reduce(&kismiPalandokenSayisi, &palandokenSayisi, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, MASTER, MPI_COMM_WORLD);
	
	
	if(rank == MASTER){
		printf(" %llu ile %llu arasinda %llu tane palandoken sayisi var\n", altLimit, ustLimit, palandokenSayisi);
	}
	
	MPI_Finalize();
		
	return 0;
}

bool PalandokenMi(unsigned long long number){
	unsigned long long tmpNumber = number;
	int sayi, benzersizBasamakSayisi = 0;
	vector<int> basamaklar;
	
	do{
        sayi = tmpNumber%10;
        tmpNumber /= 10;
        basamaklar.push_back(sayi);
    }
    while(tmpNumber>0);
    
    if(PalindromMu(basamaklar)){
    	return digitCount(number) >=4 ? true : false;
	}
	return false;
}

bool PalindromMu(vector<int> basamaklar){
	int vectorSize = basamaklar.size();
	for(int i=0; i<vectorSize; i++){
		if(basamaklar[i] != basamaklar[basamaklar.size() - i - 1]){
			return false;
		}
	}
	return true;
}

int digitCount(unsigned long long a) {
	int count = 0;
    int ele[10]={0};

    if(a==0) return 1;
    if(a<0) a=a*-1;

    while(a)
    {
        int t=a%10;
        ele[t]=1;
        a=a/10;
    }

    for (int i=0;i<10;i++)
        if (ele[i])
            count++;

    return count;
}
