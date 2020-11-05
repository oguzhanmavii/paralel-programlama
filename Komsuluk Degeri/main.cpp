#include "mpi.h"
#include <iostream>
#include <vector>

#define MASTER 0
using namespace std;

int digitCount(unsigned long long);
int BasamakSayisi(unsigned long long);
vector<int> Basamaklar(unsigned long long);
int KomsulukDegeri(unsigned long long );

int main(int argc, char** argv) {
	
	int rank, size;
	unsigned long long altLimit, ustLimit;
	unsigned long long enBuyuk = 0, buyukSayi = 0;
	
	
	
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
	/*Alt limit ve üst limit bütün bilgisayarlara MASTER tarafýndan daðýtýldý*/
	MPI_Bcast(&altLimit, 1, MPI_UNSIGNED_LONG_LONG, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&ustLimit, 1, MPI_UNSIGNED_LONG_LONG, MASTER, MPI_COMM_WORLD);
	
	unsigned long long kismiKomsulukDegeri = 0, kismiEnBuyukSayi = 0;
	
	MPI_Barrier(MPI_COMM_WORLD);// dünyadaki bilgisayarlarýn ayný yere gelmesi bekleniyor.
	
	
	if(rank == MASTER){
		printf("Komsuluk Degeri En Buyuk Sayi Araniyor... Bu islemler girdiginiz araliga bagli olarak biraz zaman alabilir...\n");
	}
	/*Tikkir sayýlar aranýyor*/
	for(unsigned long long i = (altLimit+rank); i<=ustLimit ; i+=size){
		if(digitCount(i)==BasamakSayisi(i)){
			if(KomsulukDegeri(i) > kismiKomsulukDegeri){
				kismiKomsulukDegeri = KomsulukDegeri(i);
				kismiEnBuyukSayi = i;
			}
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);// dünyadaki bilgisayarlarýn ayný yere gelmesi bekleniyor.
	/*
		her bir bilgisayarýn bulduðu tikkir sayilar sayisi master da toplanýyor. 
		MASTER A TOPLAM TÝKKÝR SAYISI GÝDECEK. Bu da MPI_Reduce yardýmýyla yapýlacak (MPI_SUM)
	*/
	MPI_Reduce(&kismiEnBuyukSayi, &buyukSayi, 1, MPI_UNSIGNED_LONG_LONG, MPI_MAX, MASTER, MPI_COMM_WORLD);
	//printf("ben %d bendeki tikkir sayisi %lld\n",rank,kismiTikkirSayisi);
	
	if(rank == MASTER){
		cout<<altLimit<<" ile "<<ustLimit<<" arasindaki Komsuluk degeri en buyuk sayi : "<<buyukSayi<<endl;
	}
	
	MPI_Finalize();
	
	return 0;
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

int BasamakSayisi(unsigned long long number){
	/*KONTROL EDÝLECEK SAYININ BASAMAK SAYISINI BULUP GERÝ DÖNDÜREN FONKSÝYON*/
	/*BASAMAK SAYISI 1 DEN BASLADIGINDAN 10 DAN KÜÇÜK ÝSE DONGU SONLANACAK*/
	int basamaks = 1;
	do{
        number=number/10;
        basamaks++;
    }
    while(number>=10);
    
    return basamaks;
}
int KomsulukDegeri(unsigned long long number){
	vector<int> basamaklar = Basamaklar(number);
	int toplam=0;
	for(int i = 0; i<basamaklar.size()-1; i++){
		toplam+= basamaklar[i]* basamaklar[i+1];
	}
	return toplam;	
}

vector<int> Basamaklar(unsigned long long number){
	vector<int> basamaklar;
	int sayi;
	do{
        sayi = number%10;
        number /= 10;
        basamaklar.push_back(sayi);
    }
    while(number>0);
    
    return basamaklar;
}
