#include<iostream>
#include <memory>
#include <tuple>
using namespace std;


enum VrstaAktivnosti { Tehnike, Kata, Borba };
const char* vrstaakitvnostiTekst[] = { "Tehnike", "Kata", "Borba" };
enum Pojas { Zuti, Narandzasti, Zeleni, Plavi, Smedji, Crni };
const char* PojasTekst[] = { "Zuti","Narandzasti"," Zeleni", "Plavi","Smedji","Crni" };
struct Datum {
	int* _dan, * _mjesec, * _godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; _dan = _mjesec = _godina = nullptr; }

};
int Dana15(Datum datum) {

	return *datum._dan + *datum._mjesec * 30 + *datum._godina * 365;
}
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char* _nazivOpis;
	int _ocjena; // 1 - 10 
	void Unos(VrstaAktivnosti vrsta, Datum* datum, int ocjena, const char* nazivOpis) {
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(nazivOpis) + 1;
		_nazivOpis = new char[size];
		strcpy_s(_nazivOpis, size, nazivOpis);
	}
	void Ispis() {
		cout << vrstaakitvnostiTekst[*_vrsta] << " " << _ocjena << " " << _nazivOpis << endl;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _nazivOpis; _nazivOpis = nullptr;
	}
};

bool jednakDatum(Datum d1, Datum d2) {

	return (d1._godina == d2._godina && d1._mjesec == d2._mjesec && d1._dan == d2._dan);

}

bool VeciOD15(Datum d1, Datum d2) {

	if (d1._godina != d2._godina)
	{
		return true;
	}
	else if (d1._mjesec != d2._mjesec)
	{
		return true;

	}
	else if (d1._dan + 15 < d2._dan) {
		return true;

	}
	else return false;
}

struct KaratePojas {
	Datum* _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja pojasa tj. kada su ispunjene sve aktivnosti/obaveze
	Pojas _pojas;
	Aktivnost* _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;
	void Unos(Pojas pojas) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_pojas = pojas;
	}
	void Dealociraj() {
		if (_datumPolaganja != nullptr) {
			_datumPolaganja->Dealociraj(); _datumPolaganja = nullptr;
		}
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		if (_datumPolaganja != nullptr) {
			_datumPolaganja->Ispis();
		}
		cout << PojasTekst[_pojas] << endl;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Ispis();
	}
	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjenog pojasa.
	Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja)
	imala ocjenu manju od 6.


	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 5, a svaka naredna identicna aktivnost,
	bez obzira da li je uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 15 dana od izvrsenja prethodne.


	Onemoguciti dodavanje aktivnosti
	uspjesno polozenom pojasu.*/

	bool DodajIzvrsenuAktivnost(Aktivnost* aktivnost)
	{
		/*for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			if (Dana15(_listaIzvrsenihAktivnosti[i]._datumIzvrsenja) == Dana15(aktivnost->_datumIzvrsenja) && *_listaIzvrsenihAktivnosti[i]._vrsta == *aktivnost->_vrsta)
			{
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
					return false;
				if (Dana15(aktivnost->_datumIzvrsenja) - Dana15(_listaIzvrsenihAktivnosti[i]._datumIzvrsenja) <= 15)
					return false;
			}
		}
		_trenutnoIzvrsenihAktivnosti++;

		Aktivnost* temp = new Aktivnost[_trenutnoIzvrsenihAktivnosti];
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti - 1; i++)
		{
			swap(_listaIzvrsenihAktivnosti[i], temp[i]);
		}
		temp[_trenutnoIzvrsenihAktivnosti - 1].Unos(*aktivnost->_vrsta, &aktivnost->_datumIzvrsenja, aktivnost->_ocjena, aktivnost->_nazivOpis);
		_listaIzvrsenihAktivnosti = temp;
		return true;*/


		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{


			if (Dana15(_listaIzvrsenihAktivnosti[i]._datumIzvrsenja) == Dana15(aktivnost->_datumIzvrsenja) &&
				(*_listaIzvrsenihAktivnosti[i]._vrsta == *aktivnost->_vrsta))
			{
				if (Dana15(aktivnost->_datumIzvrsenja) - Dana15(_listaIzvrsenihAktivnosti[i]._datumIzvrsenja) <= 15)
				{
					return false;
				}
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
				{
					return false;
				}
			}
		}

		Aktivnost* temp = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			temp[i].Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja,
				_listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._nazivOpis);

		}
		temp[_trenutnoIzvrsenihAktivnosti].Unos(*aktivnost->_vrsta, &aktivnost->_datumIzvrsenja, aktivnost->_ocjena, aktivnost->_nazivOpis);
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = temp;
		_trenutnoIzvrsenihAktivnosti++;
		return true;

	}
	void Sortiraj() {

		bool zamjena = false;
		do
		{
			zamjena = false;
			for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti - 1; i++)
			{
				if (_listaIzvrsenihAktivnosti[i]._ocjena < _listaIzvrsenihAktivnosti[i + 1]._ocjena) {
					swap(_listaIzvrsenihAktivnosti[i], _listaIzvrsenihAktivnosti[i + 1]);
					zamjena = true;
				}
			}
		} while (zamjena);

	}
	/* Karate pojas zahtijeva uspjesnu realizaciju svih planiranih aktivnosti,
	a one se za jedan povecavaju sa svakim novim pojasom, npr.


	zuti pojas: 1 x tehnika, 1 x kata, 1 x borba;
	narandzasti pojas: 2 x tehnika, 2 x kata, 2 x borba; i td...



	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne
	vrste aktivnosti (npr. tri negativne ocjene iz borbi), onda se taj pojas ne moze smatrati uspjesno stecenim
	i, te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru tog pojasa.

	Ukoliko je kandidat uspjesno realizovao sve aktivnost,
	datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	bool DaLiJePolozen() {


		int brojacNBorba = 0;
		int brojacNTehnike = 0;
		int brojacNKata = 0;
		int lokacija = 0;


		int brojacUspjesnihBorbi = 0, brojacUpsjenihKata = 0, brojacUspjesnihTehnika = 0;


		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == Borba) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena < 6)
				{
					brojacNBorba++;
				}
				else {
					brojacUspjesnihBorbi++;
				}
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == Tehnike) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena < 6) brojacNTehnike++;
				else {
					brojacUspjesnihTehnika++;
				}
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == Kata) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena < 6) brojacNKata++;
				else {
					brojacUpsjenihKata++;
				}
			}
			if (Dana15(_listaIzvrsenihAktivnosti[i]._datumIzvrsenja) > lokacija && _listaIzvrsenihAktivnosti[i]._ocjena > 5)
				lokacija = Dana15(_listaIzvrsenihAktivnosti[i]._datumIzvrsenja);
		}

		if (brojacNBorba > 2 || brojacNKata > 2 || brojacNTehnike > 2) {

			for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			{
				if (Dana15(_listaIzvrsenihAktivnosti[i]._datumIzvrsenja) == lokacija)
				{
					_datumPolaganja = new Datum();
					_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._godina);
				}
				return true;
			}
			return false;
		}


		if (_pojas == Zuti) {
			if (brojacUspjesnihBorbi > 0 && brojacUpsjenihKata > 0 && brojacUspjesnihTehnika > 0) return true;
		}

		if (_pojas == Narandzasti) {
			if (brojacUspjesnihBorbi > 1 && brojacUpsjenihKata > 1 && brojacUspjesnihTehnika > 1) return true;
		}

		if (_pojas == Zeleni) {
			if (brojacUspjesnihBorbi > 2 && brojacUpsjenihKata > 2 && brojacUspjesnihTehnika > 2) return true;
		}


		if (_pojas == Plavi) {
			if (brojacUspjesnihBorbi > 3 && brojacUpsjenihKata > 3 && brojacUspjesnihTehnika > 3) return true;
		}

		if (_pojas == Smedji) {
			if (brojacUspjesnihBorbi > 4 && brojacUpsjenihKata > 4 && brojacUspjesnihTehnika > 4) return true;
		}
		if (_pojas == Crni) {
			if (brojacUspjesnihBorbi > 5 && brojacUpsjenihKata > 5 && brojacUspjesnihTehnika > 5) return true;
		}
		return false;
	}

	/*int polozeni[6];
	Datum date;

	for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
	{
		if (_listaIzvrsenihAktivnosti[i]._ocjena >= 3) {
			polozeni[*_listaIzvrsenihAktivnosti[i]._vrsta] = 1;
			date.Unos(*_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._godina);
		}
	}
	int brojac = 0;
	for (size_t i = 0; i < 6; i++)
	{
		if (polozeni[i] == 1)
			brojac++;
	}
	if (brojac >= 3) {
		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*date._dan, *date._mjesec, *date._godina);
		return true;
	}
	else {
		return false;
	}*/





	void Kopiraj(KaratePojas p) {
		Unos(p._pojas);
		_trenutnoIzvrsenihAktivnosti = p._trenutnoIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = new Aktivnost[p._trenutnoIzvrsenihAktivnosti];
		for (size_t i = 0; i < p._trenutnoIzvrsenihAktivnosti; i++)
		{
			_listaIzvrsenihAktivnosti[i].Unos(*p._listaIzvrsenihAktivnosti[i]._vrsta, &p._listaIzvrsenihAktivnosti[i]._datumIzvrsenja,
				p._listaIzvrsenihAktivnosti[i]._ocjena, p._listaIzvrsenihAktivnosti[i]._nazivOpis);
		}
		if (p._datumPolaganja != nullptr)
			DaLiJePolozen();
	}
	float PretragaRekrzivno(const char* rijec, float prosjecna, int brojac, int trenutnoIzvrsenihAktivnosti) {

		if (trenutnoIzvrsenihAktivnosti == _trenutnoIzvrsenihAktivnosti)
			return prosjecna / brojac;

		if (_listaIzvrsenihAktivnosti[trenutnoIzvrsenihAktivnosti]._ocjena > 5 &&
			strstr(_listaIzvrsenihAktivnosti[trenutnoIzvrsenihAktivnosti]._nazivOpis, rijec) != nullptr)
		{
			brojac++;
			prosjecna += _listaIzvrsenihAktivnosti[trenutnoIzvrsenihAktivnosti]._ocjena;
		}
		return PretragaRekrzivno(rijec, prosjecna, brojac, trenutnoIzvrsenihAktivnosti + 1);


	}




};

struct Kandidat {
	char* _imePrezime;
	shared_ptr<KaratePojas> _pojasevi[6];
	void Unos(const char* imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		for (size_t i = 0; i < 6; i++)
			_pojasevi[i] = nullptr;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;

	}
	void Ispis() {
		cout << _imePrezime << endl;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)
				_pojasevi[i]->Ispis();
	}

	bool DodajPojas(KaratePojas p) {
		auto GetLokaciju = [this, p]() {
			for (int i = 0; i < 6; i++)
			{
				if (_pojasevi[i] == nullptr) {
					if (i == (int)p._pojas) {
						return i;
					}
					return -1;
				}
			}
			return -1;
		};
		int lokacija = GetLokaciju();
		if (lokacija != -1) {
			_pojasevi[lokacija] = make_shared<KaratePojas>(p);

			return true;
		}
		return false;
	}


	pair <Pojas, float> GetNajbolji() {

		float prosjecna = 0, suma = 0;
		int brojac = 0;
		Pojas pojas;
		for (size_t i = 0; i < 6; i++)
		{
			if (_pojasevi[i] != nullptr)
			{
				for (size_t j = 0; j < _pojasevi[i]->_trenutnoIzvrsenihAktivnosti; j++)

				{
					if (_pojasevi[i]->_listaIzvrsenihAktivnosti[j]._ocjena > 5) {

						suma += _pojasevi[i]->_listaIzvrsenihAktivnosti[j]._ocjena;
						brojac++;
					}
				}
				suma /= brojac;
				if (suma > prosjecna) {
					prosjecna = suma;
					pojas = _pojasevi[i]->_pojas;
				}
				brojac = 0;
				suma = 0;

			}
		}
		return pair<Pojas, float>(pojas, prosjecna);
		//return make_pair(pojas,prosjecna);
	}
};

void main() {

	//BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(22, 7, 2018);
	datumPolaganja1.Ispis();


	Aktivnost aktivnost1, aktivnost2, aktivnost3, aktivnost4, aktivnost5;
	Aktivnost akitvnost6, aktivnost7, aktivnost8, aktivnost9, aktivnost10;
	Aktivnost aktivnost11, aktivnost12, aktivnost13, aktivnost14, aktivnost15;
	Aktivnost aktivnost16, aktivnost17, aktivnost18, aktivnost19, aktivnost20;
	Aktivnost aktivnost21, aktivnost22, aktivnost23, aktivnost24, aktivnost25;
	Aktivnost aktivnost26, aktivnost27, aktivnost28, aktivnost29, aktivnost30;

	aktivnost1.Unos(Tehnike, &datumPolaganja1, 6, "Tehnike za zuti pojas");
	aktivnost2.Unos(Kata, &datumPolaganja1, 8, "Taiki joko shodan - zuti pojas");
	aktivnost3.Unos(Borba, &datumPolaganja1, 2, "Jednostavne borbene tehnike sa partnerom");
	aktivnost4.Unos(Borba, &datumPolaganja1, 6, "Jednostavne borbene tehnike sa partnerom");
	aktivnost5.Unos(Borba, &datumPolaganja4, 6, "Jednostavne borbene tehnike sa partnerom");

	akitvnost6.Unos(Tehnike, &datumPolaganja1, 7, "test za akitvnost 6");
	aktivnost7.Unos(Kata, &datumPolaganja1, 9, "test za akitvnost 7");
	aktivnost8.Unos(Borba, &datumPolaganja1, 3, "test za akitvnost 8");
	aktivnost9.Unos(Borba, &datumPolaganja1, 9, "test za akitvnost 9");
	aktivnost10.Unos(Borba, &datumPolaganja4, 9, "test za aktivnost 10");

	aktivnost11.Unos(Tehnike, &datumPolaganja1, 8, "test za akitvnost 11");
	aktivnost12.Unos(Kata, &datumPolaganja1, 10, "test za akitvnost 12");
	aktivnost13.Unos(Borba, &datumPolaganja1, 4, "test za akitvnost 13");
	aktivnost14.Unos(Borba, &datumPolaganja1, 8, "test za akitvnost 14");
	aktivnost15.Unos(Borba, &datumPolaganja4, 8, "test za aktivnost 15");

	aktivnost16.Unos(Tehnike, &datumPolaganja1, 9, "test za akitvnost 16");
	aktivnost17.Unos(Kata, &datumPolaganja1, 11, "test za akitvnost 17");
	aktivnost18.Unos(Borba, &datumPolaganja1, 2, "test za akitvnost 18");
	aktivnost19.Unos(Borba, &datumPolaganja1, 9, "test za akitvnost 19");
	aktivnost20.Unos(Borba, &datumPolaganja4, 9, "test za aktivnost 20");

	aktivnost21.Unos(Tehnike, &datumPolaganja1, 10, "test za akitvnost 21");
	aktivnost22.Unos(Kata, &datumPolaganja1, 12, "test za akitvnost 22");
	aktivnost23.Unos(Borba, &datumPolaganja1, 3, "test za akitvnost 23");
	aktivnost24.Unos(Borba, &datumPolaganja1, 10, "test za akitvnost 24");
	aktivnost25.Unos(Borba, &datumPolaganja4, 10, "test za aktivnost 25");

	aktivnost26.Unos(Tehnike, &datumPolaganja1, 11, "test za akitvnost 26");
	aktivnost27.Unos(Kata, &datumPolaganja1, 13, "test za akitvnost 27");
	aktivnost28.Unos(Borba, &datumPolaganja1, 4, "test za akitvnost 28");
	aktivnost29.Unos(Borba, &datumPolaganja1, 11, "test za akitvnost 29");
	aktivnost30.Unos(Borba, &datumPolaganja4, 11, "test za aktivnost 30");

	aktivnost10.Ispis();
	aktivnost1.Ispis();
	aktivnost14.Ispis();

	KaratePojas pojasZuti, pojasNarandzasti, pojasZeleni;
	pojasZuti.Unos(Zuti);
	pojasZuti.Ispis();
	pojasZeleni.Unos(Zeleni);
	pojasZeleni.Ispis();
	pojasNarandzasti.Unos(Narandzasti);
	pojasNarandzasti.Ispis();

	//cout << jednakDatum(datumPolaganja1, datumPolaganja2);

	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjenog pojasa.
	Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja)
	imala ocjenu manju od 6.
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 5, a svaka naredna identicna aktivnost,
	bez obzira da li je uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 15 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti
	uspjesno polozenom pojasu.*/
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost1))
		cout << "Aktivnost1 uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost2))
		cout << "Aktivnost2 uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost3))
		cout << "Aktivnost3 uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost4))//15 dana...
		cout << "Aktivnost4 uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost5))
		cout << "Aktivnost5 uspjesno dodana!" << endl;




	if (pojasZeleni.DodajIzvrsenuAktivnost(&akitvnost6))
		cout << "Aktivnost6 uspjesno dodana!" << endl;
	if (pojasZeleni.DodajIzvrsenuAktivnost(&aktivnost7))
		cout << "Aktivnost7 uspjesno dodana!" << endl;
	if (pojasZeleni.DodajIzvrsenuAktivnost(&aktivnost8))
		cout << "Aktivnost8 uspjesno dodana!" << endl;
	if (pojasZeleni.DodajIzvrsenuAktivnost(&aktivnost9))//15 dana...
		cout << "Aktivnost9 uspjesno dodana!" << endl;
	if (pojasZeleni.DodajIzvrsenuAktivnost(&aktivnost10))
		cout << "Aktivnost10 uspjesno dodana!" << endl;
	if (pojasZeleni.DodajIzvrsenuAktivnost(&aktivnost11))
		cout << "Aktivnost11 uspjesno dodana!" << endl;
	if (pojasZeleni.DodajIzvrsenuAktivnost(&aktivnost12))
		cout << "Aktivnost12 uspjesno dodana!" << endl;
	if (pojasZeleni.DodajIzvrsenuAktivnost(&aktivnost13))
		cout << "Aktivnost13 uspjesno dodana!" << endl;
	if (pojasZeleni.DodajIzvrsenuAktivnost(&aktivnost14))//15 dana...
		cout << "Aktivnost14 uspjesno dodana!" << endl;
	if (pojasZeleni.DodajIzvrsenuAktivnost(&aktivnost15))
		cout << "Aktivnost15 uspjesno dodana!" << endl;



	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost16))
		cout << "Aktivnost16 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost17))
		cout << "Aktivnost17 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost18))
		cout << "Aktivnost18 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost19))//15 dana...
		cout << "Aktivnost19 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost20))
		cout << "Aktivnost20 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost21))
		cout << "Aktivnost21 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost22))
		cout << "Aktivnost2 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost23))
		cout << "Aktivnost23 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost24))//15 dana...
		cout << "Aktivnost24 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost25))
		cout << "Aktivnost25 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost26))
		cout << "Aktivnost26 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost27))
		cout << "Aktivnost27 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost28))
		cout << "Aktivnost28 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost29))//15 dana...
		cout << "Aktivnost29 uspjesno dodana!" << endl;
	if (pojasNarandzasti.DodajIzvrsenuAktivnost(&aktivnost30))
		cout << "Aktivnost30 uspjesno dodana!" << endl;


	//Koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjenog pojasa


	cout << "-------------------------------------------------------------------" << endl;
	pojasZuti.Ispis();
	cout << "nakon sort." << endl;
	pojasZuti.Sortiraj();
	pojasZuti.Ispis();


	/* Karate pojas zahtijeva uspjesnu realizaciju svih planiranih aktivnosti,
	a one se za jedan povecavaju sa svakim novim pojasom, npr.
	zuti pojas: 1 x tehnika, 1 x kata, 1 x borba;
	narandzasti pojas: 2 x tehnika, 2 x kata, 2 x borba; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne
	vrste aktivnosti (npr. tri negativne ocjene iz borbi), onda se taj pojas ne moze smatrati uspjesno stecenim
	i, te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru tog pojasa.
	Ukoliko je kandidat uspjesno realizovao sve aktivnost,
	datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/


	cout << "-------------------------------------------------------------------" << endl;


	if (pojasZuti.DaLiJePolozen())
		pojasZuti.Ispis();
	if (pojasNarandzasti.DaLiJePolozen())
		pojasNarandzasti.Ispis();
	if (pojasZeleni.DaLiJePolozen())
		pojasZeleni.Ispis();




	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra.
	Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/


	cout << "Prosjecna ocjena za zuti pojas -> " << pojasZuti.PretragaRekrzivno("pojas", 0, 0, 0) << endl;

	//ispisuje sve dostupne podatke o pojasu
	//pojasZuti.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");

	jasmin.Ispis();
	/*
	Karate pojasevi se moraju dodavati po redoslijedu tj.
	ne smije se dozvoliti dodavanje zelenog pojasa ukoliko prethodno nije dodan zuti i narandzasti
	. Za provjeru lokacije (unutar funkcije DodajPojas) na koju ce se dodati novi karate pojas,
	te da li su nizi pojasevi prethodno dodani koristiti lambda funkciju.

	*/
	if (jasmin.DodajPojas(pojasZuti))
		cout << "Pojas zuti uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasZeleni))//prethodno je trebao biti dodan narandzasti pojas
		cout << "Pojas zeleni uspjesno  dodan!" << endl;
	if (jasmin.DodajPojas(pojasNarandzasti))
		cout << "Pojas  narandzasti uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasZeleni))
		cout << "Pojas  zeleni, nakon provjere da li je narandzasti polozen prije, je uspjesno dodan!" << endl;

	float prosjek = 0;
	Pojas pojas;
	////Funkcija GetNajbolji vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) pojasa sa najvecim prosjekom
	tie(pojas, prosjek) = jasmin.GetNajbolji();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom stjecanja pojasa " << PojasTekst[pojas] << endl;


	datumPolaganja1.Dealociraj(), datumPolaganja2.Dealociraj(), datumPolaganja3.Dealociraj(), datumPolaganja4.Dealociraj();
	aktivnost1.Dealociraj(), aktivnost2.Dealociraj(), aktivnost3.Dealociraj(), aktivnost4.Dealociraj(), aktivnost5.Dealociraj();
	pojasZuti.Dealociraj(); pojasNarandzasti.Dealociraj(), pojasZeleni.Dealociraj();
	jasmin.Dealociraj();

	system("pause");
}
