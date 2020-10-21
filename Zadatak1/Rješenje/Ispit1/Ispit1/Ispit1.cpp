
#include<iostream>
#include<fstream>
using namespace std;

char crt[] = "\n----------------------------------------------------\n";
struct Datum {
	int* _dan, * _mjesec, * _godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}
	bool jelVeci(Datum d)
	{
		if (*d._godina > * _godina)
			return true;
		else if (*d._godina == *_godina)
		{
			if (*d._mjesec > * _mjesec)
				return true;
			else if (*d._mjesec == *_mjesec)
			{
				if (*d._dan > * _dan)
					return true;
			}
		}
		return false;
	}
};
struct Artikal {
	char* _naziv;
	float _cijena;
	void Unos(const char* naziv, float cijena) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_cijena = cijena;
	}
	void Dealociraj() { delete[]_naziv; _naziv = nullptr; }
	void Ispis() { cout << _naziv << " " << _cijena << " KM"; }
};
struct Racun {
	//format broja racuna: 006005491
	//osigurati automatsko generisanje broja racuna pocevsi od 000000001
	char _brojRacuna[10];
	Datum _datumKreiranja;
	Artikal* _artikli;
	int* _kolicine;//cuva informaciju o kolicini svakog artikla na racunu
	int _brojArtikala;
	void Unos(const char* br, Datum d) {
		strcpy_s(_brojRacuna, strlen(br) + 1, br);
		_datumKreiranja.Unos(*d._dan, *d._mjesec, *d._godina);
		_brojArtikala = 0;
		_artikli = nullptr;
		_kolicine = nullptr;
	}
	void Dalociraj() {
		for (size_t i = 0; i < _brojArtikala; i++)
		{
			_artikli[i].Dealociraj();
		}
		delete[]_artikli, _kolicine;
	}
	bool dodajartikal(Artikal a, int c)
	{
		for (size_t i = 0; i < _brojArtikala; i++)
		{
			if (strcmp(_artikli[i]._naziv, a._naziv) == 0)
			{
				_kolicine[i] += c;
				cout << "Uvecna kolicina za artikal : " << a._naziv << endl;
				return true;
			}

		}
		Artikal* temp = new Artikal[_brojArtikala + 1];
		int* kol = new int[_brojArtikala + 1];
		for (size_t i = 0; i < _brojArtikala; i++)
		{
			temp[i].Unos(_artikli[i]._naziv, _artikli[i]._cijena);
			kol[i] = _kolicine[i];
		}
		temp[_brojArtikala].Unos(a._naziv, a._cijena);
		kol[_brojArtikala] = c;
		delete[]_artikli, _kolicine;
		_artikli = temp;
		_kolicine = kol;
		cout << "Artikal : " << a._naziv << " uspjesno dodan " << endl;
		_brojArtikala++;
		return true;
	}
	void Ispis() {
		cout << crt;
		cout << "Broj racuna " << crt;
		float suma = 0;
		for (size_t i = 0; i < _brojArtikala; i++)
		{
			cout << _artikli[i]._naziv << " : " << " Kolicina : " << _kolicine[i] << " Cijena : " << _artikli[i]._cijena << endl;
			suma += _kolicine[i] * _artikli[i]._cijena;
		}
		cout << "Ukupan iznos je : " << suma << endl;
		cout << crt;
		ofstream upis(_brojRacuna);
		upis << crt << "Broj racuna " << crt;
		for (size_t i = 0; i < _brojArtikala; i++)
			upis << _artikli[i]._naziv << " : " << " Kolicina : " << _kolicine[i] << " Cijena : " << _artikli[i]._cijena << endl;
		upis << "Ukupan iznos je : " << suma << crt;

		ifstream ispis(_brojRacuna, ios::in || ios::_Nocreate);
		char znak;
		cout << crt << "FAJL: " << _brojRacuna << crt << "SADRZAJ:" << crt;
		while (ispis.get(znak))
		{
			cout << znak;
		}
		cout << crt << "Sadrzaj fajla prikazan!" << crt;
	}
	void sortiraj() {
		bool zamjena = false;
		cout << "sortiram..." << endl;
		do
		{
			zamjena = false;
			for (size_t i = 0; i < _brojArtikala - 1; i++)
			{
				if (strcmp(_artikli[i]._naziv, _artikli[i + 1]._naziv) > 0)
				{
					swap(_artikli[i], _artikli[i + 1]);
					swap(_kolicine[i], _kolicine[i + 1]);
					zamjena = true;
				}
			}
		} while (zamjena);
	}
	Artikal* BinarnaPretraga(const char* ime) {
		sortiraj();
		int prvi = 0, zadnji = _brojArtikala;
		int srednji;
		while (prvi <= zadnji)
		{
			srednji = (prvi + zadnji) / 2;
			if (strcmp(_artikli[srednji]._naziv, ime) == 0)
				return &_artikli[srednji];
			else if (strcmp(_artikli[srednji]._naziv, ime) < 0)
				prvi = srednji + 1;
			else if (strcmp(_artikli[srednji]._naziv, ime) > 0)
				zadnji = srednji - 1;
		}
		return nullptr;
	}
	//Potpisi funkcija trebaju odgovarati onima u main-u
	//1. Unos
	//2. Dealociraj
	//3. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom.
	//Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko
	//	korisnik vise puta pokusa dodati isti artikal.
	//	//4. Ispis - ispisuje racun u formatu prikazanom na slici (nije obavezno da bude
	//	identican, ali je svakako pozeljno).Prilikom svakog ispisa, racun
	//	je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj racuna.
	//	//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu naziva, pronalazi i vraca
	//	pokazivac na artikal.Ukoliko trazeni artikal ne postoji funkcija vraca nullptr.
};
//1. PronadjiNajskupljiArtikal - rekurzivna funkcija koja vraca pokazivac na artikal sa najvecom ukupnom cijenom (cijena*kolicina)
//2. GenerisiSljedeciBrojRacuna - generise i vraca naredni broj racuna
int Broj = 1;
float najskuplji = 0;
Artikal* PronadjiNajskupljiArtikal(Racun r, Artikal* s) {
	if (r._brojArtikala == 0)
		return s;
	if (r._artikli[r._brojArtikala - 1]._cijena > najskuplji)
	{
		najskuplji = r._artikli[r._brojArtikala - 1]._cijena;
		s = &r._artikli[r._brojArtikala - 1];
	}
	r._brojArtikala--;
	PronadjiNajskupljiArtikal(r, s);
}
char* GenerisiSljedeciBrojRacuna() {
	char* broj = new char[9];
	strcpy_s(broj, strlen("000000001") + 1, "000000001");
	int kopija = Broj;
	int brojac = 0;
	do
	{
		kopija /= 10;
		brojac++;
	} while (kopija);
	_itoa_s(Broj, broj + (8 - brojac), sizeof(broj), 10);
	Broj++;
	return broj;
}
void main() {
	//za eventualne nejasnoce analizirati testni program i ispise na slici
	Datum danas; danas.Unos(13, 6, 2016);
	Artikal cigarete, cokolada, sok;
	cigarete.Unos("Cigarete", 2.23);
	cokolada.Unos("Cokolada", 1.23);
	sok.Unos("Cappy", 2.10);
	Racun racun;
	racun.Unos(GenerisiSljedeciBrojRacuna(), danas);
	racun.dodajartikal(cokolada, 5);
	racun.dodajartikal(sok, 10);
	racun.dodajartikal(cigarete, 2);
	racun.dodajartikal(cigarete, 5);
	racun.Ispis();
	Artikal* s = nullptr;
	Artikal* p1 = racun.BinarnaPretraga("Cokolada");
	cout << "Binarnom pretragom pronadjen artikal -> ";
	p1->Ispis();
	racun.Ispis();
	Artikal* p2 = PronadjiNajskupljiArtikal(racun, s);
	cout << "Rekurzivno pronadjen najskuplji artikal -> ";
	p2->Ispis();
	cout << endl;
	danas.Dealociraj();
	cigarete.Dealociraj();
	cokolada.Dealociraj();
	sok.Dealociraj();
	racun.Dalociraj();
	system("pause");
}

