#include "mpi.h"
#include <iostream>
#include <vector>

using namespace std;

#define MASTER 0

bool TikkirMi(unsigned long long);
bool BasamaktaDortVarMi(unsigned long long );
int BasamakSayisi(unsigned long long );
bool ToplamSayiyaEsitMi(unsigned long long , vector<unsigned long long>);

int main(int argc, char** argv) {
	
	int rank, size;
	unsigned long long altLimit, ustLimit, tikkirSayisi;
	
	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	
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
	
	unsigned long long kismiTikkirSayisi=0;
	
	MPI_Barrier(MPI_COMM_WORLD);// dünyadaki bilgisayarlarýn ayný yere gelmesi bekleniyor.
	
	if(rank == MASTER){
		printf("Tikkir sayilar araniyor... Bu islemler girdiginiz araliga bagli olarak biraz zaman alabilir...\n");
	}
	/*Tikkir sayýlar aranýyor*/
	for(unsigned long long i = (altLimit+rank); i<=ustLimit; i+=size){
		if(TikkirMi(i)){
			kismiTikkirSayisi++;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);// dünyadaki bilgisayarlarýn ayný yere gelmesi bekleniyor.
	/*
		her bir bilgisayarýn bulduðu tikkir sayilar sayisi master'a toplanarak gönderiliyor. 
	*/
	MPI_Reduce(&kismiTikkirSayisi, &tikkirSayisi, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, MASTER, MPI_COMM_WORLD);
	//printf("ben %d bendeki tikkir sayisi %lld\n",rank, kismiTikkirSayisi);
	
	if(rank == MASTER){
		cout<<altLimit<<" ile "<<ustLimit<<" arasindaki Tikkir Sayilarinin sayisi : "<<tikkirSayisi<<endl;
	}
	
	MPI_Finalize();
	
	return 0;
}

bool TikkirMi(unsigned long long number){
	vector<unsigned long long> bolenler; //basamaklarýnda en az bir tane 4 olan bölenleri tutacak dizi
	
	for(unsigned long long i=1 ; i<number ; i++){
		if(number%i == 0){
			if(BasamaktaDortVarMi(i)){
				bolenler.push_back(i);
			}
		}
	}
	
	return ToplamSayiyaEsitMi(number,bolenler);
}

bool BasamaktaDortVarMi(unsigned long long number){
	int basamakS=BasamakSayisi(number);
	int deger;
	
	for (int i=0; i<basamakS; i++){
		deger = number%10;
		if (deger==4){
			return true;
		}else{
			number/=10;
		}
	}
	return false;
}

int BasamakSayisi(unsigned long long number){
	int basamaks = 1;
	do{
        number = number/10;
        basamaks++;
    }
    while(number >= 10);
    
    return basamaks;
}

bool ToplamSayiyaEsitMi(unsigned long long number, vector<unsigned long long> output){
	int toplam = 0;
	
	for (int i=0; i<output.size(); i++){
		toplam+=output[i];
	}
	
	return toplam == number ? true : false;
}