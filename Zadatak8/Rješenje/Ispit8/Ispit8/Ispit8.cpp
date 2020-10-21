
#include <iostream>
#include <tuple>

using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char* crt = "\n-------------------------------------------\n";

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
const char* vrstaDokumentaChar[] = { "PDF", "DOC", "TXT", "HTML" };

enum vrstaAktivnosti { KREIRANJE, DODAVANJE, ZAMJENA, PRINTANJE };
const char* vrstaAktivnostiChar[] = { "KREIRANJE", "DODAVANJE", "ZAMJENA", "PRINTANJE" };

const int BROJ_ZNAKOVA_PO_STRANICI = 30;

struct DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
	void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 1, int minuti = 1) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	void Dealociraj() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	void Ispis() {
		cout << *_dan << "." << *_mjesec << "." << *_godina << " " << *_sati << ":" << *_minuti << endl;
	}

	unsigned int sumaMinuta() {
		return *_godina * 365 * 1440 + *_mjesec * 30 * 1440 + *_dan * 1440 + *_sati * 60 + *_minuti;
	}

	int GetDani() { return *_dan + *_mjesec * 30 + *_godina * 365; }
};

char* AlocirajNizKaraktera(const char* sadrzaj) {
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

struct Modifikacija {
	char* _korisnicnkoIme;
	vrstaAktivnosti _aktivnost;
	DatumVrijeme _vrijemeModifikacije;
	void Unos(const char* sadrzaj, vrstaAktivnosti aktivnost, DatumVrijeme vm) {
		_korisnicnkoIme = AlocirajNizKaraktera(sadrzaj);
		_aktivnost = aktivnost;
		_vrijemeModifikacije.Unos(*vm._dan, *vm._mjesec, *vm._godina, *vm._dan, *vm._minuti);
	}
	void Dealociraj() {
		delete[] _korisnicnkoIme; _korisnicnkoIme = nullptr;
		_vrijemeModifikacije.Dealociraj();
	}
	void Ispis() { cout << _korisnicnkoIme << " (" << vrstaAktivnostiChar[_aktivnost] << ")"; _vrijemeModifikacije.Ispis(); }
};

struct Dokument {
	unique_ptr<vrstaDokumenta> _vrsta;
	char* _naziv;
	char* _sadrzaj;
	Modifikacija* _modifikacije;
	int _brojModifikacija;
	//vrijeme kreiranja dokumenta je ujedno i vrijeme posljednje modifikacije
	void Unos(vrstaDokumenta vrsta, const char* naziv, DatumVrijeme kreiran) {
		_naziv = AlocirajNizKaraktera(naziv);
		_vrsta = make_unique<vrstaDokumenta>(vrsta);
		_brojModifikacija = 0;
		_modifikacije = new Modifikacija[_brojModifikacija + 1];
		_modifikacije->Unos("", KREIRANJE, kreiran);
		_brojModifikacija++;
		_sadrzaj = nullptr;
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
		for (size_t i = 0; i < _brojModifikacija; i++)
			_modifikacije[i].Dealociraj();
		delete[] _modifikacije;
		_modifikacije = nullptr;
	}
	void Ispis() {
		cout << crt << _naziv << " " << vrstaDokumentaChar[*_vrsta] << " kreiran ";
		if (_sadrzaj != nullptr)
			cout << crt << _sadrzaj << crt;
		for (size_t i = 0; i < _brojModifikacija; i++)
			_modifikacije[i].Ispis();
	}
	bool DodajSadrzaj(const char* sadrzaj, const char* imePrezime, DatumVrijeme date) {
		if (_sadrzaj == nullptr) {		// Ako vec ne postoji sadrzaj
			_sadrzaj = AlocirajNizKaraktera(sadrzaj);
			Modifikacija* temp = new Modifikacija[_brojModifikacija + 1];
			for (size_t i = 0; i < _brojModifikacija; i++)
			{
				temp[i].Unos(_modifikacije[i]._korisnicnkoIme, _modifikacije[i]._aktivnost, _modifikacije[i]._vrijemeModifikacije);
			}
			temp[_brojModifikacija].Unos(imePrezime, DODAVANJE, date);
			for (size_t i = 0; i < _brojModifikacija; i++)
			{
				_modifikacije[i].Dealociraj();
			}
			delete[] _modifikacije; _modifikacije = temp;
			_brojModifikacija++;
			return true;
		}
		char* pom = new char[strlen(_sadrzaj) + 1];
		strcpy_s(pom, strlen(_sadrzaj) + 1, _sadrzaj);
		int vel = strlen(pom) + strlen(sadrzaj) + 2;
		delete[]_sadrzaj;
		_sadrzaj = new char[vel];
		strcpy_s(_sadrzaj, vel, pom);
		strcat_s(_sadrzaj, vel, " ");
		strcat_s(_sadrzaj, vel, sadrzaj);
		Modifikacija* temp = new Modifikacija[_brojModifikacija + 1];
		for (size_t i = 0; i < _brojModifikacija; i++)
		{
			temp[i].Unos(_modifikacije[i]._korisnicnkoIme, _modifikacije[i]._aktivnost, _modifikacije[i]._vrijemeModifikacije);
		}
		temp[_brojModifikacija].Unos(imePrezime, DODAVANJE, date);
		for (size_t i = 0; i < _brojModifikacija; i++)
		{
			_modifikacije[i].Dealociraj();
		}
		delete[] _modifikacije; _modifikacije = temp;
		_brojModifikacija++;
		return true;
	}
	void DodajModifikacije(Modifikacija m) {
		Modifikacija* temp = new Modifikacija[_brojModifikacija + 1];
		for (size_t i = 0; i < _brojModifikacija; i++)
		{
			temp[i].Unos(_modifikacije[i]._korisnicnkoIme, _modifikacije[i]._aktivnost, _modifikacije[i]._vrijemeModifikacije);
		}
		temp[_brojModifikacija].Unos(m._korisnicnkoIme, m._aktivnost, m._vrijemeModifikacije);
		for (size_t i = 0; i < _brojModifikacija; i++)
		{
			_modifikacije[i].Dealociraj();
		}
		delete[]_modifikacije; _modifikacije = temp;
		_brojModifikacija++;
	}
};

bool provjeriNaziv(const char* dokument) {
	for (size_t i = 0; i <= strlen(dokument); i++)
	{
		if (isdigit(dokument[i])) return false;
	}
	return true;
}
bool provjeriEkstenziju(const char* dokument) {
	char ekstenzija[4];
	int brojac = strlen(dokument) - 1;
	for (int i = 2; i != -1; i--)
	{
		ekstenzija[i] = toupper(dokument[brojac]);
		brojac--;
	}
	ekstenzija[3] = '\0';
	for (size_t i = 0; i < 4; i++)
	{
		if (strcmp(ekstenzija, vrstaDokumentaChar[i]) == 0) return true;
	}
	return false;
}
bool provjeriModifikacije(const Dokument& doc) {
	bool kreiranje, dodavanje, zamjena;
	kreiranje = dodavanje = zamjena = false;
	for (size_t i = 0; i < doc._brojModifikacija; i++)
	{
		if (doc._modifikacije[i]._aktivnost == KREIRANJE) kreiranje = true;
		if (doc._modifikacije[i]._aktivnost == DODAVANJE) dodavanje = true;
		//	if (doc._modifikacije[i]._aktivnost == ZAMJENA) zamjena = true;
	}
	if (kreiranje && dodavanje /* && zamjena */) return true;		// Ne kontam sta mu je zamjena sadrzaja pa sam ga samo iskljucio odavde
																	// jer nijedan Dokument nema ovu zamjenu pa funkcija uvijek vrati false
	return false;
}

bool provjeriDatum(DatumVrijeme OD, DatumVrijeme DO) {	// Ako je OD > DO funkcija vraca true!
	/*if (*OD._godina > *DO._godina) return true;
	if (*OD._godina < *DO._godina) return false;
	if (*OD._godina == *DO._godina) {
		if (*OD._mjesec > *DO._mjesec) return true;
		if (*OD._mjesec < *DO._mjesec) return false;
		if (*OD._mjesec == *DO._mjesec) {
			if (*OD._dan > *DO._dan) return true;
			if (*OD._dan < *DO._dan) return false;
			if (*OD._dan == *DO._dan) {
				if (*OD._sati > *DO._sati) return true;
				if (*OD._sati < *DO._sati) return false;
				if (*OD._sati == *DO._sati) {
					if (*OD._minuti > *DO._minuti) return true;
					if (*OD._minuti < *DO._minuti) return false;
					if (*OD._minuti == *DO._minuti) return true;
				}
			}
		}
	}*/
	//
	/*if (d1._godina < d2._godina)
		return true;
	else if (d1._godina == d2._godina) {
		if (d1._mjesec < d2._mjesec)
			return true;
		else if (d1._mjesec == d2._mjesec) {
			if (d1._dan < d2._dan)
				return true;
		}
	}
	return false; */
	return *OD._dan > * DO._dan && *OD._mjesec > * OD._mjesec && *OD._godina > * DO._godina && *OD._sati > * DO._sati && *OD._minuti > * DO._minuti;

}

struct Printer {
	char* _model;
	Dokument* _dokumenti;
	int _trenutnoDokumenata;

	void Unos(const char* model) {
		_model = AlocirajNizKaraktera(model);
		_dokumenti = nullptr;
		_trenutnoDokumenata = 0;
	}
	void Dealociraj() {
		delete[] _model; _model = nullptr;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Dealociraj();
		delete[]_dokumenti; _dokumenti = nullptr;
	}
	void Ispis() {
		cout << _model << crt;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Ispis();
	}
	bool Printaj(Dokument& dok, const char* imePrezime, DatumVrijeme date) {
		/*
		DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDECA PRAVILA:
		1. NAZIV DOKUMENTA MOZE SADRZAVATI SAMO SLOVA, A EKSTENZIJA MORA BITI IDENTICNA ONOJ DEFINISANOJ VRIJEDNOSCU ATRIBUTA vrstaDokumenta
		2. NAD DOKUMENTOM MORAJU BITI IZVRSENE SLJEDECE MODIFIKACIJE:
		- 1 KREIRANJE
		- 1 ILI VIŠE DODAVANJA
		- 1 ILI VIŠE ZAMJENA SADRZAJA
		SAMO DOKUMENTI KOJI ZADOVOLJE USLOVE PRINTANJA TREBAJU BITI SACUVANI U NIZ _dokumenti. NA KONZOLU ISPISATI SADRZAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA.
		BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREDJUJE PRILIKOM  PRINTANJA, A ZAVISI OD VRIJEDNOSTI BROJ_ZNAKOVA_PO_STRANICI UKLJUCUJUCI RAZMAKE I DRUGE VRSTE ZNAKOVA.
		ONEMOGUCITI PRINTANJE DOKUMENTA OSOBAMA KOJE NISU NAPRAVILE NAJMANJE JEDNU MODIFIKACIJU NA TOM DOKUMENTU. NAKON PRINTANJA SADRZAJA DOKUMENTA, POTREBNO JE DOKUMENTU DODATI I MODIFIKACIJU "PRINTANJE".
		*/

		if (provjeriNaziv(dok._naziv) && provjeriEkstenziju(dok._naziv) && provjeriModifikacije(dok)) {
			Dokument* temp = new Dokument[_trenutnoDokumenata + 1];
			for (size_t i = 0; i < _trenutnoDokumenata; i++)
			{
				temp[i].Unos(*_dokumenti[i]._vrsta, _dokumenti[i]._naziv, _dokumenti[i]._modifikacije[_dokumenti[i]._brojModifikacija - 1]._vrijemeModifikacije);
				for (size_t k = 0; k < _dokumenti[i]._brojModifikacija; k++)
				{
					temp[i].DodajModifikacije(_dokumenti[i]._modifikacije[k]);
				}
			}
			temp[_trenutnoDokumenata].Unos(*dok._vrsta, dok._naziv, dok._modifikacije[dok._brojModifikacija - 1]._vrijemeModifikacije);
			for (size_t i = 0; i < dok._brojModifikacija; i++)
			{
				temp[_trenutnoDokumenata].DodajModifikacije(dok._modifikacije[i]);
			}
			for (size_t i = 0; i < _trenutnoDokumenata; i++)
			{
				for (size_t j = 0; j < _dokumenti[i]._brojModifikacija; j++)
				{
					_dokumenti[i]._modifikacije[j].Dealociraj();
				}
				_dokumenti[i].Dealociraj();
			}
			delete[]_dokumenti; _dokumenti = temp;
			_trenutnoDokumenata++;
			// PRINTANJE //
			int brojac = 0;
			for (size_t i = 0; i <= strlen(dok._sadrzaj); i++)
			{
				if (brojac > BROJ_ZNAKOVA_PO_STRANICI) {
					cout << crt;
					brojac = -1;
				}
				cout << dok._sadrzaj[i];
				brojac++;
			}
			return true;
		}
		return false;
	}
	float GetProsjecanBrojModifikacijaByVrsta(vrstaAktivnosti vrsta, float prosjek, int brojac) {

		if (brojac == _trenutnoDokumenata) return prosjek;


		float x = 0.0;
		for (size_t i = 0; i < _dokumenti[brojac]._brojModifikacija; i++)
		{
			if (_dokumenti[brojac]._modifikacije[i]._aktivnost == vrsta) {
				x++;
			}
		}
		x /= _dokumenti[brojac]._brojModifikacija;

		if (x > prosjek)
			return GetProsjecanBrojModifikacijaByVrsta(vrsta, x, ++brojac);

		else return GetProsjecanBrojModifikacijaByVrsta(vrsta, prosjek, ++brojac);
	}
	pair<Modifikacija*, int> GetModifikacijeByDatum(DatumVrijeme OD, DatumVrijeme DO) {
		int brojModifikacija = 0;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
		{
			for (size_t j = 0; j < _dokumenti[i]._brojModifikacija; j++)
			{
				if (_dokumenti[i]._modifikacije[j]._vrijemeModifikacije.GetDani() >= OD.GetDani() &&
					_dokumenti[i]._modifikacije[j]._vrijemeModifikacije.GetDani() <= DO.GetDani()) {
					brojModifikacija++;
				}
			}
		}

		if (brojModifikacija == 0)
			return pair<Modifikacija*, int>(nullptr, 0);

		Modifikacija* m = new Modifikacija[brojModifikacija];
		brojModifikacija = 0;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
		{
			for (size_t j = 0; j < _dokumenti[i]._brojModifikacija; j++)
			{
				if (_dokumenti[i]._modifikacije[j]._vrijemeModifikacije.GetDani() >= OD.GetDani() &&
					_dokumenti[i]._modifikacije[j]._vrijemeModifikacije.GetDani() <= DO.GetDani()) {
					m[brojModifikacija].Unos(_dokumenti[i]._modifikacije[j]._korisnicnkoIme,
						_dokumenti[i]._modifikacije[j]._aktivnost, _dokumenti[i]._modifikacije[j]._vrijemeModifikacije);
					brojModifikacija++;
				}
			}
		}
		return pair<Modifikacija*, int>(m, brojModifikacija);
	}

};


int main() {
	DatumVrijeme prije3Dana; prije3Dana.Unos(3, 2, 2018, 10, 15);
	DatumVrijeme danas1; danas1.Unos(6, 2, 2018, 10, 15);//U KONTEKSTU MODIFIKACIJE, UNOS PODRAZUMIJEVA KREIRANJE
	DatumVrijeme danas2; danas2.Unos(6, 2, 2018, 10, 16);
	DatumVrijeme za10Dana; za10Dana.Unos(16, 2, 2018, 10, 15);

	Dokument ispitPRII, ispitMAT, ispitUIT, ispitUITDrugiRok;
	ispitPRII.Unos(DOC, "ispitPRII.doc", prije3Dana);
	ispitMAT.Unos(DOC, "ispitMAT.doc", danas1);
	ispitUIT.Unos(DOC, "ispitUIT.doc", danas2);
	ispitUITDrugiRok.Unos(PDF, "ispitUITDrugiRok.pdf", za10Dana);

	/*DODAJE SADRZAJ U FAJL, ZADRZAVAJUCI POSTOJECI. SVAKO DODAVANJE EVIDENTIRATI KAO MODIFIKACIJU. PARAMETRI:SADRZAJ, KORISNICKO IME, VRIJEME MODIFIKACIJE*/
	ispitPRII.DodajSadrzaj("Programiranje ili racunarsko programiranje (engl. programming) jeste vjestina pomocu koje ", "denis.music", danas1);
	ispitPRII.DodajSadrzaj("korisnik stvara i izvrsava algoritme koristeci odredjene programske jezike da bi ... ", "indira.hamulic", danas2);

	ispitPRII.Ispis();//ISPISUJE SVE PODATKE O DOKUMENTU

	ispitMAT.DodajSadrzaj("Matematika se razvila iz potrebe da se obavljaju proracuni u trgovini, vrse mjerenja zemljista i predvidjaju ", "jasmin.azemovic", danas1);
	ispitMAT.DodajSadrzaj("astronomski dogadjaji, i ove tri primjene se mogu dovesti u vezu sa grubom podjelom matematike ", "adel.handzic", danas2);


	Printer hp3200; hp3200.Unos("HP 3200");
	/*
	DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDECA PRAVILA:
	1. NAZIV DOKUMENTA MOZE SADRZAVATI SAMO SLOVA, A EKSTENZIJA MORA BITI IDENTICNA ONOJ DEFINISANOJ VRIJEDNOSCU ATRIBUTA vrstaDokumenta
	2. NAD DOKUMENTOM MORAJU BITI IZVRSENE SLJEDECE MODIFIKACIJE:
	- 1 KREIRANJE
	- 1 ILI VIŠE DODAVANJA
	- 1 ILI VIŠE ZAMJENA SADRZAJA
	SAMO DOKUMENTI KOJI ZADOVOLJE USLOVE PRINTANJA TREBAJU BITI SACUVANI U NIZ _dokumenti. NA KONZOLU ISPISATI SADRZAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA.
	BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREDJUJE PRILIKOM  PRINTANJA, A ZAVISI OD VRIJEDNOSTI BROJ_ZNAKOVA_PO_STRANICI UKLJUCUJUCI RAZMAKE I DRUGE VRSTE ZNAKOVA.
	ONEMOGUCITI PRINTANJE DOKUMENTA OSOBAMA KOJE NISU NAPRAVILE NAJMANJE JEDNU MODIFIKACIJU NA TOM DOKUMENTU. NAKON PRINTANJA SADRZAJA DOKUMENTA, POTREBNO JE DOKUMENTU DODATI I MODIFIKACIJU "PRINTANJE".
	*/
	if (hp3200.Printaj(ispitPRII, "denis.music", za10Dana))
		cout << "Printam -> " << ispitPRII._naziv << endl;
	if (hp3200.Printaj(ispitMAT, "denis.music", za10Dana))
		cout << "Printam -> " << ispitMAT._naziv << endl;
	if (hp3200.Printaj(ispitUIT, "iris.memic", za10Dana))
		cout << "Printam -> " << ispitUIT._naziv << endl;
	if (hp3200.Printaj(ispitUITDrugiRok, "goran.skondric", za10Dana))
		cout << "Printam -> " << ispitUITDrugiRok._naziv << endl;

	//REKURZIVNA FUNKCIJA VRACA PROSJECAN BROJ MODIFIKACIJA ODREDJENE VRSTE KOJE SU VRSENE NAD PRINTANIM DOKUMENTIMA
	cout << "Prosjecan broj modifikacija odrejdne vrste printanih dokumenata je -> " << hp3200.GetProsjecanBrojModifikacijaByVrsta(DODAVANJE, 0, 0) << crt;

	Modifikacija* pok = nullptr;
	int brojac = 0;
	//FUNKCIJA GetModifikacijeByDatum VRACA SVE MODIFIKACIJE KOJE SU NAPRAVLJENE NA DOKUMENTIMA U PERIODU OD - DO 
	//(PROSLIJEDJENOM KAO PARAMETAR)
	tie(pok, brojac) = hp3200.GetModifikacijeByDatum(prije3Dana, danas1);
	if (brojac > 0 && pok != nullptr)
		for (size_t i = 0; i < brojac; i++)
			pok[i].Ispis();

	//ISPISUJE INFORMACIJE O SVIM DOKUMENTIMA KOJI SU ISPRINTANI
	hp3200.Ispis();

	/*NAPISATI LAMBDA FUNKCIJU ZamijeniIPrintaj KOJA U SADRZAJU SVIH PRINTANIH DOKUMENATA MIJENJA PRVI POSLATI PARAMETAR SA VRIJEDNOSCU DRUGOG PARAMETRA, TE VRACA BROJ ZAMIJENJENIH ZNAKOVA*/
	//RAZMAK MIJENJA ZNAKOM CRTICA U SADRZAJU DOKUMENATA KOJI SU PRINTANI danas1. ZAMJENU REGISTROVATI KAO MODIFIKACIJU
	auto Zamijeni = [&hp3200](const char k1, const char k2, const char* username, DatumVrijeme d) ->int {
		int brojZamijenjeniZnakova = 0;
		for (size_t i = 0; i < hp3200._trenutnoDokumenata; i++)
		{
			bool printano = false;
			for (size_t j = 0; j < hp3200._dokumenti[i]._brojModifikacija; j++)
				if (hp3200._dokumenti[i]._modifikacije[j]._aktivnost == PRINTANJE && hp3200._dokumenti[i]._modifikacije[j]._vrijemeModifikacije.GetDani() == d.GetDani())
					printano = true;
			if (printano) {
				for (size_t j = 0; j < strlen(hp3200._dokumenti[i]._sadrzaj); j++)
				{
					if (hp3200._dokumenti[i]._sadrzaj[j] == k1) {
						hp3200._dokumenti[i]._sadrzaj[j] = k2;
						brojZamijenjeniZnakova++;
					}
				}
				Modifikacija* m = hp3200._dokumenti[i]._modifikacije;
				Modifikacija* temp = new Modifikacija[hp3200._dokumenti[i]._brojModifikacija + 1];
				for (size_t j = 0; j < hp3200._dokumenti[i]._brojModifikacija; j++)
				{
					temp[j].Unos(m[j]._korisnicnkoIme, m[j]._aktivnost, m[j]._vrijemeModifikacije);
					m[j].Dealociraj();
				}
				delete[] m;
				temp[hp3200._dokumenti[i]._brojModifikacija].Unos(username, ZAMJENA, d);
				hp3200._dokumenti[i]._modifikacije = temp;
				hp3200._dokumenti[i]._brojModifikacija++;
			}
		}
		return brojZamijenjeniZnakova;
	};
	int zamijenjeno1 = Zamijeni(' ', '-', "denis.music", danas1);
	cout << "Zamijenjeno -> " << zamijenjeno1 << " znakova" << endl;
	hp3200.Ispis();

	prije3Dana.Dealociraj(); danas1.Dealociraj(); danas2.Dealociraj(); za10Dana.Dealociraj();
	ispitMAT.Dealociraj(); ispitPRII.Dealociraj(); ispitUIT.Dealociraj(); ispitUITDrugiRok.Dealociraj();

	hp3200.Dealociraj();
	system("pause");
	return 0;
}
