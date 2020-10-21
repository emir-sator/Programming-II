#include<iostream>
#include<memory>
#include<tuple>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URA?ENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)


enum VrstaAktivnosti { Poligon, Gradska, Nocna, Parkiranje };
const char* VrsteAktivnosti[] = { "Poligon", "Gradska", "Nocna", "Parkiranje" };
enum Kategorija { A, B, C, D, E };
const char* Kategorije[] = { "A", "B", "C", "D", "E" };
const int maxKategorija = 5;

struct Datum {
	int* _dan, * _mjesec, * _godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete  _dan; delete  _mjesec; delete _godina; }

	bool Jednak(Datum* d) {
		if (*_dan == *d->_dan && *_mjesec == *d->_mjesec && *_godina == *d->_godina)
			return true;

		return false;
	}

	int SumaDana() {
		return *_dan + *_mjesec * 30 + *_godina * 365;
	}
};
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char* _komentar;
	int _ocjena; // 1 - 5
	void Unos(VrstaAktivnosti vrsta, Datum* datum, int ocjena, const char* komentar) {
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(komentar) + 1;
		_komentar = new char[size];
		strcpy_s(_komentar, size, komentar);
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
	}
	void Ispis() {
		cout << VrsteAktivnosti[*_vrsta] << " " << _ocjena << " " << _komentar << " ";
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _komentar; _komentar = nullptr;
	}
};
struct VozackaKategorija {
	Datum* _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja kategorije tj. kada su ispunjene sve aktivnosti/obaveze
	Kategorija _kategorija;
	Aktivnost* _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;
	void Unos(Kategorija kategorija) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_kategorija = kategorija;
	}

	bool DodajIzvrsenuAktivnost(VrstaAktivnosti vrsta, Datum* datumIzvrsenja, int ocjena, const char* komentar) {
		//Onemoguciti dodavanje aktivnosti uspjesno polozenoj kategoriji.
		if (_datumPolaganja != nullptr)
			return false;

		if (ocjena < 1 || ocjena > 5)
			return false;

		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == vrsta) {
				//Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identicna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju od 3.

				if (_listaIzvrsenihAktivnosti[i]._datumIzvrsenja.Jednak(datumIzvrsenja) &&
					_listaIzvrsenihAktivnosti[i]._ocjena >= 3) {
					return false;
				}

				//svaka naredna identicna aktivnost, bez obzira da li je uspjesna ili ne,
				//moze biti dodana jedino ako je proslo najmanje 30 dana od izvrsenja prethodne.
				int DatumStareAktivnosti = _listaIzvrsenihAktivnosti[i]._datumIzvrsenja.SumaDana();
				int DatumNoveAktivnosti = datumIzvrsenja->SumaDana();

				if (DatumNoveAktivnosti >= DatumStareAktivnosti &&
					DatumNoveAktivnosti - DatumStareAktivnosti < 30)
					return false;
			}
		}

		//Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjene vozacke kategorije. Broj aktivnosti nije ogranicen.
		Aktivnost* temp = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			temp[i].Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._komentar);
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		}
		temp[_trenutnoIzvrsenihAktivnosti].Unos(vrsta, datumIzvrsenja, ocjena, komentar);
		_trenutnoIzvrsenihAktivnosti++;

		delete[]_listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = temp;

		return true;
	}
	void Sortiraj() {
		bool promjena;
		do {
			promjena = false;
			for (int i = 0; i < _trenutnoIzvrsenihAktivnosti - 1; i++)
			{
				if (_listaIzvrsenihAktivnosti[i]._ocjena < _listaIzvrsenihAktivnosti[i + 1]._ocjena) {
					swap(_listaIzvrsenihAktivnosti[i], _listaIzvrsenihAktivnosti[i + 1]);
					promjena = true;
				}
			}
		} while (promjena == true);
	}

	bool DaLiJePolozena() {
		/*Stjecanje kategorije zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakom novom kategorijom, npr.
		A: 1 x poligon, 1 x gradska, 1 x nocna, 1 x parkiranje;
		B: 2 x poligon, 2 x gradska, 2 x nocna, 2 x parkiranje; i td...*/
		int potrebnoAktivnosti = int(_kategorija) + 1;
		Datum* zadnjiDatum = nullptr;

		int brojNegativnih[4] = { 0, 0, 0, 0 };
		int brojPozitivnih[4] = { 0, 0, 0, 0 };
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			int vrsta = int(*_listaIzvrsenihAktivnosti[i]._vrsta);

			if (_listaIzvrsenihAktivnosti[i]._ocjena >= 3) {
				brojPozitivnih[vrsta]++;

				// pohrana datuma posljednje uspjesno realizovane aktivnosti
				if (zadnjiDatum == nullptr || zadnjiDatum->SumaDana() < _listaIzvrsenihAktivnosti[i]._datumIzvrsenja.SumaDana())
					zadnjiDatum = &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja;
			}
			else {
				brojNegativnih[vrsta]++;

				/*Funkcija vraca false u slucaju da : su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti(npr.tri negativne ocjene iz gradske voznje), onda se ta kategorija ne moze smatrati uspjesno polozenom*/
				if (brojNegativnih[vrsta] == 3)
					return false;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			// ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru te kategorije. 
			if (brojPozitivnih[i] < potrebnoAktivnosti)
				return false;
		}

		/*Ukoliko je kandidat uspjesno realizovao sve aktivnost, datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*zadnjiDatum->_dan, *zadnjiDatum->_mjesec, *zadnjiDatum->_godina);

		return true;
	}


	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra. Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	float PretragaRekrzivno(const char* tekst, int suma, int ukupno, int i) {
		if (i == _trenutnoIzvrsenihAktivnosti) {
			if (ukupno > 0)
				return suma / float(ukupno);

			return 0;
		}

		if (strstr(_listaIzvrsenihAktivnosti[i]._komentar, tekst) != nullptr ||
			strstr(VrsteAktivnosti[*_listaIzvrsenihAktivnosti[i]._vrsta], tekst) != nullptr) {
			suma += _listaIzvrsenihAktivnosti[i]._ocjena;
			ukupno++;
		}

		return PretragaRekrzivno(tekst, suma, ukupno, i + 1);
	}
	void Dealociraj() {
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Dealociraj();
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		cout << "Kategorija: " << Kategorije[_kategorija] << endl;

		if (_datumPolaganja != nullptr) {
			cout << "Datum polaganja: ";
			_datumPolaganja->Ispis();
		}
		else
			cout << "Nije polozena" << endl;

		cout << "Aktivnosti:" << endl;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Ispis();
	}
};

struct Kandidat {
	char* _imePrezime;
	shared_ptr<VozackaKategorija> _kategorije[maxKategorija];
	int _trenutnoKategorija;
	void Unos(const char* imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_trenutnoKategorija = 0;
	}
	bool DodajKategoriju(VozackaKategorija k) {
		if (_trenutnoKategorija == maxKategorija)
			return false;

		// vec polozio kategoriju, opciono
		for (int i = 0; i < _trenutnoKategorija; i++)
		{
			if (_kategorije[i]->_kategorija == k._kategorija)
				return false;
		}

		auto lambdaf = [this, k]() -> bool {

			// Za kategoriju A nema preduslova
			if (k._kategorija == A)
				return true;

			// Provjera polozenih kategorija
			int potrebnoKategorija = int(k._kategorija);

			bool* polozeneKategorije = new bool[potrebnoKategorija] { false };

			for (int i = 0; i < _trenutnoKategorija; i++)
			{
				if (_kategorije[i]->_datumPolaganja != nullptr) {
					polozeneKategorije[_kategorije[i]->_kategorija] = true;
				}
			}

			for (int i = 0; i < potrebnoKategorija; i++)
			{
				if (polozeneKategorije[i] == false)
					return false;
			}

			return true;
		};

		if (lambdaf() == false)
			return false;

		_kategorije[_trenutnoKategorija] = make_shared<VozackaKategorija>();
		_kategorije[_trenutnoKategorija]->Unos(k._kategorija);
		for (int i = 0; i < k._trenutnoIzvrsenihAktivnosti; i++)
		{
			_kategorije[_trenutnoKategorija]->DodajIzvrsenuAktivnost(*k._listaIzvrsenihAktivnosti[i]._vrsta, &k._listaIzvrsenihAktivnosti[i]._datumIzvrsenja, k._listaIzvrsenihAktivnosti[i]._ocjena, k._listaIzvrsenihAktivnosti[i]._komentar);
		}
		// poziv funkcije zbog ponovnog postavljanja vrijednosti atributa _datumPolganja
		_kategorije[_trenutnoKategorija]->DaLiJePolozena();

		// proba da li su se svi atributi uspjesno prenijeli
		//_kategorije[_trenutnoKategorija]->Ispis();

		_trenutnoKategorija++;

		return true;
	}

	//Funkcija GetNajboljuKategoriju  vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) kategorije sa najvecim prosjekom
	/*tie(kategorija, prosjek) = jasmin.GetNajboljuKategoriju();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom polaganja kategorije " << kategorija << endl;*/
	pair<Kategorija, float> GetNajboljuKategoriju() {

		float najboljiProsjek = 0;
		Kategorija najboljaKategorija = A;

		for (int i = 0; i < _trenutnoKategorija; i++)
		{
			float suma = 0;
			int brojac = 0;

			for (int j = 0; j < _kategorije[i]->_trenutnoIzvrsenihAktivnosti; j++)
			{
				if (_kategorije[i]->_listaIzvrsenihAktivnosti[j]._ocjena >= 3) {
					suma += _kategorije[i]->_listaIzvrsenihAktivnosti[j]._ocjena;
					brojac++;
				}

			}

			float prosjek = suma / brojac;
			if (prosjek > najboljiProsjek) {
				najboljiProsjek = prosjek;
				najboljaKategorija = Kategorija(i);
			}
		}

		return make_pair(najboljaKategorija, najboljiProsjek);
	}

	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _trenutnoKategorija; i++) {
			_kategorije[i]->Dealociraj();
			_kategorije[i].reset();
		}
	}
	void Ispis() {
		cout << _imePrezime << endl;
		for (size_t i = 0; i < _trenutnoKategorija; i++)
			_kategorije[i]->Ispis();
	}
};

void main() {

	//BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(28, 7, 2018);

	VozackaKategorija kategorijaA, kategorijaB, kategorijaC;
	kategorijaA.Unos(A);
	kategorijaB.Unos(B);
	kategorijaC.Unos(C);

	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjene vozacke kategorije. Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju od 3.
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 2, a svaka naredna identicna aktivnost, bez obzira da li je uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 30 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenoj kategoriji.*/
	if (kategorijaA.DodajIzvrsenuAktivnost(Poligon, &datumPolaganja1, 3, "izuzetno dobra orijentacija"))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja1, 2, "nesigurnost u raskrsnicama"))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja2, 4, "raskrsnice savladane"))//datum?
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja4, 4, "raskrsnice savladane"))
		cout << "Aktivnost uspjesno dodana!" << endl;

	// Dodano: potrebno kako bi kategorija A bila polozena
	//if (kategorijaA.DodajIzvrsenuAktivnost(Nocna, &datumPolaganja2, 5, "raskrsnice savladane"))//datum?
	//	cout << "Aktivnost uspjesno dodana!" << endl;
	//if (kategorijaA.DodajIzvrsenuAktivnost(Parkiranje, &datumPolaganja4, 5, "raskrsnice savladane"))
	//	cout << "Aktivnost uspjesno dodana!" << endl;


	//koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjene kategorije
	kategorijaA.Sortiraj();


	/*Stjecanje kategorije zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakom novom kategorijom, npr.
	A: 1 x poligon, 1 x gradska, 1 x nocna, 1 x parkiranje;
	B: 2 x poligon, 2 x gradska, 2 x nocna, 2 x parkiranje; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti (npr. tri negativne ocjene iz gradske voznje), onda se ta kategorija ne moze smatrati uspjesno polozenom
	,te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru te kategorije. Ukoliko je kandidat uspjesno realizovao sve aktivnost, datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	if (kategorijaA.DaLiJePolozena())
		kategorijaA.Ispis();

	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra. Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	cout << "Prosjecna ocjena za kategoriju A -> " << kategorijaA.PretragaRekrzivno("nesigurno", 0, 0, 0) << endl;

	//ispisuje sve dostupne podatke o kategoriji
	kategorijaA.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");

	/*
	Pretpostavka je da se kategorije moraju polagati po redoslijedu tj. ne smije se dozvoliti dodavanje kategorije C ukoliko prethodno nisu uspjesno polozene kategorije B i C. Za provjeru ispunjenosti preduslova dodavanja nove kategorije, unutar funkcije DodajKategoriju, kreirati lambda funkciju.
	*/
	if (jasmin.DodajKategoriju(kategorijaA))
		cout << "Kategorija A uspjesno dodana!" << endl;
	if (jasmin.DodajKategoriju(kategorijaC))
		cout << "Kategorija C uspjesno dodana!" << endl;
	if (jasmin.DodajKategoriju(kategorijaB))
		cout << "Kategorija B uspjesno dodana!" << endl;

	float prosjek = 0;
	Kategorija kategorija;
	//Funkcija GetNajboljuKategoriju  vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) kategorije sa najvecim prosjekom
	tie(kategorija, prosjek) = jasmin.GetNajboljuKategoriju();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom polaganja kategorije " << Kategorije[kategorija] << endl;


	datumPolaganja1.Dealociraj(), datumPolaganja2.Dealociraj(), datumPolaganja3.Dealociraj(), datumPolaganja4.Dealociraj();
	kategorijaA.Dealociraj(), kategorijaB.Dealociraj(), kategorijaC.Dealociraj();
	jasmin.Dealociraj();

	system("pause");
}
