#include <iostream>
#include<tuple>
#include <iomanip>
const char* crt = "\n-------------------------------------\n";

using namespace std;

struct Putnik {
	char* _imePrezime;
	//pretpostavka je da ce se uvijek unijeti validna oznaka sjedista u formatu XXA gdje je XX dvocifreni broj, a A predstavlja veliko slovo. Jednocifrene oznake ce imati prefiks 0
	char _ozankaSjedista[4]; //npr. 02A, 12B, 14B, 28C -> broj predstavlja oznaku reda, a slovo oznaku kolone
};

struct Let {
	char* _relacija;//Mostar -> Sarajevo
	Putnik* _putnici;
	int _trenutnoPutnika;
	int _brojRedovaUAvionu;
	int _brojKolonaUAvionu;//broj kolona mora biti paran >=4 i <=10
};

void Dealociraj(Let& let) {
	delete[] let._relacija; let._relacija = nullptr;
	for (size_t i = 0; i < let._trenutnoPutnika; i++) {
		delete[] let._putnici[i]._imePrezime;
		let._putnici[i]._imePrezime = nullptr;
	}
	delete[] let._putnici;
	let._putnici = nullptr;
}

void Unos(Let& let, const char* relacija, int brojRedova, int brojkolona) {

	int vel = strlen(relacija) + 1;
	let._relacija = new char[strlen(relacija) + 1];
	strcpy_s(let._relacija, vel, relacija);
	let._putnici = nullptr;
	let._trenutnoPutnika = 0;
	let._brojRedovaUAvionu = brojRedova;
	let._brojKolonaUAvionu = brojkolona;


}

pair<int, int>GetPozicijuUReduIKoloni(const char* oznakaSjedista) {

	int red = 0, kolona = 0;
	char tempRed[3];
	strncpy_s(tempRed, oznakaSjedista, 2);
	red = atoi(tempRed);
	red--;
	kolona = oznakaSjedista[2] - 65;
	return make_pair(red, kolona);
}

void Kopiraj(Putnik& original, Putnik& novi) {

	strcpy_s(novi._ozankaSjedista, original._ozankaSjedista);
	int vel = strlen(original._imePrezime) + 1;
	novi._imePrezime = new char[vel];
	strcpy_s(novi._imePrezime, vel, original._imePrezime);
}
bool DodajPutnika(Let& let, Putnik putnik) {
	if (let._brojKolonaUAvionu * let._brojRedovaUAvionu <= let._trenutnoPutnika)
	{
		return false;
	}
	for (size_t i = 0; i < let._trenutnoPutnika; i++)
	{
		if (strcmp(let._putnici[i]._imePrezime, putnik._imePrezime) == 0) {
			return false;
		}
	}
	Putnik* temp = new Putnik[let._trenutnoPutnika + 1];
	for (size_t i = 0; i < let._trenutnoPutnika; i++)
	{
		Kopiraj(let._putnici[i], temp[i]);

	}
	Kopiraj(putnik, temp[let._trenutnoPutnika]);
	delete[] let._putnici;
	let._putnici = temp;
	let._trenutnoPutnika++;
	return true;

}

Putnik GetNovogPutnika(const char* oznakaSjedista, const char* imePrezime) {
	Putnik noviPutnik;
	strcpy_s(noviPutnik._ozankaSjedista, oznakaSjedista);
	int vel = strlen(imePrezime) + 1;
	noviPutnik._imePrezime = new char[vel];
	strcpy_s(noviPutnik._imePrezime, vel, imePrezime);
	return noviPutnik;

}

int rekBrojacKaraktera(Let let, int brojPutnika, int maxBrojKaraktera) {

	if (brojPutnika <= 0)
	{
		return maxBrojKaraktera;
	}
	if (strlen(let._putnici[brojPutnika - 1]._imePrezime) > maxBrojKaraktera) {
		maxBrojKaraktera = strlen(let._putnici[brojPutnika - 1]._imePrezime);
	}
	return rekBrojacKaraktera(let, brojPutnika - 1, maxBrojKaraktera);

}

void PrikaziRasporedSjedenja(Let let) {

	int dimenzijeKolone = rekBrojacKaraktera(let, let._trenutnoPutnika, 0);
	cout << "Relacija -> " << let._relacija << endl;
	for (size_t i = 0; i < let._brojRedovaUAvionu; i++) {
		for (size_t j = 0; j < let._brojKolonaUAvionu; j++)
		{
			cout << "+";
			for (size_t k = 0; k < dimenzijeKolone; k++)
			{
				cout << "-";
			}

		}
		cout << "+" << endl;
		for (size_t j = 0; j < let._brojKolonaUAvionu; j++)
		{
			cout << "|" << setw(dimenzijeKolone);
			char* imePrezime = nullptr;
			int red, kolona;
			for (size_t k = 0; k < let._trenutnoPutnika; k++)
			{
				tie(red, kolona) = GetPozicijuUReduIKoloni(let._putnici[k]._ozankaSjedista);
				if (red == i && kolona == j) {
					imePrezime = let._putnici[k]._imePrezime;
					break;
				}
			}
			if (imePrezime != nullptr)
				cout << imePrezime;
			else
				cout << " ";
		}
		cout << "|" << endl;

	}
	for (size_t j = 0; j < let._brojKolonaUAvionu; j++)
	{
		cout << "+";
		for (size_t k = 0; k < dimenzijeKolone; k++)
		{
			cout << "-";
		}

	}
}


int main() {
	int oznakaR = 0, oznakaK = 0;
	Let mostar_sarajevo;
	//inicijalizuje vrijednosti atributa objekta mostar_sarajevo
	Unos(mostar_sarajevo, "Mostar -> Sarajevo", 10, 4);//relacija, broj_redova, broj_kolona

 //GetPozicijuUReduIKoloni - na osnovu oznake sjedista vraca pair red i kolona tj. poziciju u matrici
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("15B");
	cout << crt << "GetPozicijuUReduIKoloni(15B) ->" << oznakaR << "/" << oznakaK << endl;//ispisuje 14/1 
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("01B");
	cout << "GetPozicijuUReduIKoloni(01B) ->" << oznakaR << "/" << oznakaK << crt;//ispisuje 0/1
																				  /*
	GetNovogPutnika - vraca objekat (ne pokazivac) tipa Putnik koji je inicijalizovan vrijednsotima proslijedjenih parametara
	DodajPutnika - podatke o novom putniku dodaje u listu (niz) putnika na proslijedjenom letu.
	Onemoguciti dodavanje putnika sa istim imenom i prezimenom, nepostojecom lokacijom sjedista ili u slucaju da su sva mjesta popunjena
																				  */
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("01A", "Denis Music")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("07C", "Zanin Vejzovic")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("10D", "Adel Handzic")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	//PrikaziRasporedSjedenja - na osnovu oznake sjedista prikazuje raspored
	//sjedenja u avionu za let koji je proslijedjen kao parametar
	PrikaziRasporedSjedenja(mostar_sarajevo);

	//rekBrojacKaraktera - rekurzivna funkcija koja vraca maksimalan broj karaktera u imenu i prezimenu putnika na odredjenom letu
	cout << crt << "Maksimalna broj karaktera u imenu i prezimenu putnika je -> " << rekBrojacKaraktera(mostar_sarajevo,
		mostar_sarajevo._trenutnoPutnika, 0) << crt; //broj 0 je pocetno stanje prilikom brojanja max karaktera

													 /*
													 DODATNO:
 1. prilikom ispisa, sirinu kolone prilagoditi maksimalnom broju karaktera u imenu i prezimenu
 2. automatski generisati oznaku sjedista na osnovu narednog slobodnog mjesta na letu (povratnom vrijednoscu sugerisati na slucaj
 u kome su sva mjesta na letu vec zauzeta). ovu funkciju nije potrebno integrisati u postojece rjesenje, vec se moze i zasebno testirati
													 */

	Dealociraj(mostar_sarajevo);
	system("pause>0");
	return 0;
}
