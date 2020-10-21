/// KURSEVI, POLAGANJA, POLAZNIK
#include<iostream>
#include<iomanip>
using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };

char crt[] = "\n--------------------------------------------\n";
struct Datum {
	int* _dan, * _mjesec, * _godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; _dan = nullptr; _mjesec = nullptr; _godina = nullptr; }
};
const char* imeKursa(enumKursevi kurs)
{
	if (kurs == 0)
		return "HtmlCSSJavaScript";
	if (kurs == 1)
		return "SoftwareEngeneeringFundamentals";
	if (kurs == 2)
		return "MasteringSQL";
	else
		return "WindowsSecurity";
}
struct Kurs {
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char* _imePredavaca;
	bool _aktivan;//SVAKI KURS JE NA POCETKU AKTIVAN
	void Unos(enumKursevi kurs, Datum pocetak, Datum kraj, const  char* imePredavaca)
	{
		_kurs = kurs;
		_pocetak.Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj.Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		_imePredavaca = new char[strlen(imePredavaca) + 1];
		strcpy_s(_imePredavaca, strlen(imePredavaca) + 1, imePredavaca);
		_aktivan = true;
	}
	void Dealociraj() {
		_pocetak.Dealociraj();
		_kraj.Dealociraj();
		delete[] _imePredavaca; _imePredavaca = nullptr;
	}
	void Ispis()
	{
		cout << "Ime kursa -> " << imeKursa(_kurs) << crt;
		cout << "Pocetak kursa -> ";
		_pocetak.Ispis();
		cout << "Kraj kursa -> ";
		_kraj.Ispis();
		cout << "PREDAVAC -> " << _imePredavaca << endl;
		if (_aktivan)
			cout << "Kurs je aktivan" << endl;
		else
			cout << "Kurs nije aktivan." << endl;
	}
};

bool provjera(Kurs kurs, Kurs** svi, int trenutno)
{
	int pocetak, kraj, novi_p;

	for (int i = 0; i < trenutno; i++)
	{
		if (kurs._kurs == svi[i]->_kurs)
		{
			pocetak = *svi[i]->_pocetak._godina * 10000 + *svi[i]->_pocetak._mjesec * 100 + *svi[i]->_pocetak._dan;
			kraj = *svi[i]->_kraj._godina * 10000 + *svi[i]->_kraj._mjesec * 100 + *svi[i]->_kraj._dan;
			novi_p = *kurs._pocetak._godina * 10000 + *kurs._pocetak._mjesec * 100 + *kurs._pocetak._dan;
			for (int j = 0; j < trenutno; j++)
			{
				if (novi_p >= pocetak && novi_p <= kraj)
					return true;
			}
		}
	}
	return false;
}
struct Polaznik {
	int _polaznikID;
	char* _imePrezime = nullptr;
	void Unos(int polaznikID, const  char* imePrezime) {
		_polaznikID = polaznikID;
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
	}
	void Ispis() { cout << "ID -> " << _polaznikID << endl << "Kandidat -> " << _imePrezime << endl; }
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
};
struct Polaganja {
	Polaznik _polaznik;
	Kurs _kurs;
	Datum _datumPolaganja;
	float _ostvareniUspjeh;
	void Unos(Polaznik ime, Kurs kurs, Datum datum, float uspjeh)
	{
		_polaznik.Unos(ime._polaznikID, ime._imePrezime);
		_kurs.Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
		_datumPolaganja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
		_ostvareniUspjeh = uspjeh;
	}
	void Ispis()
	{
		_polaznik.Ispis();
		_kurs.Ispis();
		cout << "Datum polaganja-> ";
		_datumPolaganja.Ispis();
		cout << "Ostvareni uspjeh-> " << _ostvareniUspjeh << endl;
	}
	void Dealociraj()
	{
		_polaznik.Dealociraj();
		_kurs.Dealociraj();
		_datumPolaganja.Dealociraj();

	}
};
Polaganja* SpasiStanje(Polaganja* polaganja, int trenutno)
{
	Polaganja* temp = new Polaganja[trenutno];
	for (int i = 0; i < trenutno; i++)
	{
		temp[i].Unos(polaganja[i]._polaznik, polaganja[i]._kurs, polaganja[i]._datumPolaganja, polaganja[i]._ostvareniUspjeh);
	}
	return temp;
}
bool Check(Kurs** svi, Polaganja novo, int trenutno, int trenutno_pol)
{
	int pocetak_polaganja, kraj_izabranog;
	for (int i = 0; i < trenutno; i++)
	{
		if (novo._kurs._kurs == svi[i]->_kurs)
		{
			if (novo._kurs._aktivan)
			{
				pocetak_polaganja = *novo._datumPolaganja._godina * 10000 + *novo._datumPolaganja._mjesec * 100 + *novo._datumPolaganja._dan;
				kraj_izabranog = *svi[i]->_kraj._godina * 10000 + *svi[i]->_kraj._mjesec * 100 + *svi[i]->_kraj._dan;
				if (pocetak_polaganja >= kraj_izabranog + 5)

					return true;
			}
		}
	}

	return false;
}
void SortirajKurseve(Kurs** kursevi, int trenutno)
{
	bool prolaz = true;
	Kurs temp;
	while (prolaz)
	{
		prolaz = false;
		for (int i = 0; i < trenutno - 1; i++)
		{
			int prvi = *kursevi[i]->_pocetak._godina * 10000 + *kursevi[i]->_pocetak._mjesec * 100 + *kursevi[i]->_pocetak._dan;
			int drugi = *kursevi[i + 1]->_pocetak._godina * 10000 + *kursevi[i + 1]->_pocetak._mjesec * 100 + *kursevi[i + 1]->_pocetak._dan;
			if (prvi > drugi)
			{
				temp.Unos(kursevi[i]->_kurs, kursevi[i]->_pocetak, kursevi[i]->_kraj, kursevi[i]->_imePredavaca);
				kursevi[i]->Unos(kursevi[i + 1]->_kurs, kursevi[i + 1]->_pocetak, kursevi[i + 1]->_kraj, kursevi[i + 1]->_imePredavaca);
				kursevi[i + 1]->Unos(temp._kurs, temp._pocetak, temp._kraj, temp._imePredavaca);
				prolaz = true;
			}
		}
		trenutno--;
	}
}

struct SkillsCentar {
	char* _nazivCentra;
	Kurs* _kursevi[50]; //JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva = 0;
	Polaganja* _polaganja;
	int _trenutnoPolaganja = 0;
	void Unos(const char* naziv)
	{
		_nazivCentra = new char[strlen(naziv) + 1];
		strcpy_s(_nazivCentra, strlen(naziv) + 1, naziv);
		for (int i = 0; i < 50; i++)
		{
			_kursevi[i] = nullptr;
		}

	}

	bool DodajKurs(Kurs kurs)
	{
		if (_trenutnoKurseva >= 50)
		{
			cout << "Skills centar moze imati maksimalno 50 kurseva." << endl;
			return false;
		}
		if (_kursevi[0] == nullptr)
		{
			_kursevi[0] = new Kurs;
			_kursevi[0]->Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
			return true;
		}
		else
		{
			if (provjera(kurs, _kursevi, _trenutnoKurseva))
			{
				return false;
			}
			else
			{
				_kursevi[_trenutnoKurseva] = new Kurs;
				_kursevi[_trenutnoKurseva]->Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
				return true;
			}
		}
	}

	bool DodajPolaganje(Polaganja polaganje)
	{
		if (Check(_kursevi, polaganje, _trenutnoKurseva, _trenutnoPolaganja))
		{
			for (int i = 0; i < _trenutnoPolaganja; i++)
			{
				if (strcmp(_polaganja[i]._polaznik._imePrezime, polaganje._polaznik._imePrezime) == 0)
				{
					if (polaganje._ostvareniUspjeh > _polaganja[i]._ostvareniUspjeh)
						break;
					else
					{
						cout << "Taj kandidat ne moze ponovo polagati. " << endl;
						return false;
					}
				}
			}
			if (_trenutnoPolaganja > 0)
			{
				Polaganja* temp = SpasiStanje(_polaganja, _trenutnoPolaganja);
				delete[] _polaganja;
				_polaganja = nullptr;
				_polaganja = new Polaganja[_trenutnoPolaganja + 1];
				for (int i = 0; i < _trenutnoPolaganja; i++)
				{
					_polaganja[i].Unos(temp[i]._polaznik, temp[i]._kurs, temp[i]._datumPolaganja, temp[i]._ostvareniUspjeh);
				}
				_polaganja[_trenutnoPolaganja].Unos(polaganje._polaznik, polaganje._kurs, polaganje._datumPolaganja, polaganje._ostvareniUspjeh);
				return true;
			}
			else
			{
				_polaganja = new Polaganja[_trenutnoPolaganja + 1];
				_polaganja[_trenutnoPolaganja].Unos(polaganje._polaznik, polaganje._kurs, polaganje._datumPolaganja, polaganje._ostvareniUspjeh);
				return true;
			}
		}
		else
			return false;
	}
	Polaganja* PolaganjaByDatum(Datum OD, Datum DO, int& brojPolaganja)
	{
		Polaganja* polozeni = nullptr;
		int temp[200]; //pomocni niz koji ce cuvati indekse nadjenih polaganja
		int poc, kraj, trazeni, temp_brojac = 0;
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			poc = *OD._godina * 10000 + *OD._mjesec * 100 + *OD._dan;
			kraj = *DO._godina * 10000 + *DO._mjesec * 100 + *DO._dan;
			trazeni = *_polaganja[i]._datumPolaganja._godina * 10000 + *_polaganja[i]._datumPolaganja._mjesec * 100 + *_polaganja[i]._datumPolaganja._dan;
			if (trazeni >= poc && trazeni <= kraj && _polaganja[i]._ostvareniUspjeh > 55)
			{
				temp[i] = 1;
				brojPolaganja++;
			}
			else
				temp[i] = 0;
		}
		polozeni = new Polaganja[brojPolaganja];
		for (int j = 0; j < _trenutnoPolaganja; j++)
		{
			if (temp[j] == 1)
			{
				polozeni[temp_brojac].Unos(_polaganja[j]._polaznik, _polaganja[j]._kurs, _polaganja[j]._datumPolaganja, _polaganja[j]._ostvareniUspjeh);
				temp_brojac++;
			}

		}
		return polozeni;
	}
	void Ispis()
	{
		SortirajKurseve(_kursevi, _trenutnoKurseva);
		cout << crt << "Edukacijski centar -> " << _nazivCentra << crt;
		cout << "Trenutno aktivnih kurseva " << _trenutnoKurseva << ": " << endl;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			cout << "\t" << imeKursa(_kursevi[i]->_kurs) << endl;
		}
		cout << "Trenutno polaganja u centru: " << _trenutnoPolaganja << ":" << endl;
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			cout << "\t" << imeKursa(_polaganja[i]._kurs._kurs) << endl;
		}
		cout << crt;
	}
	void Dealociraj()
	{
		delete[] _nazivCentra;
		_nazivCentra = nullptr;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			_kursevi[i]->Dealociraj();
			_kursevi[i] = nullptr;
		}
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			_polaganja[i].Dealociraj();
		}
		delete[] _polaganja;
		_polaganja = nullptr;
	}
};
float PretragaRekurzivno(SkillsCentar mostar, float prosjek, int  pozicija, int brojac)
{
	if (pozicija < 0)
	{
		if (brojac == 0)
			return 0;
		else
			return prosjek / brojac;
	}
	if (mostar._polaganja[pozicija]._kurs._kurs == 2 && *mostar._polaganja[pozicija]._datumPolaganja._godina == 2016)
		return PretragaRekurzivno(mostar, prosjek + mostar._polaganja[pozicija]._ostvareniUspjeh, pozicija - 1, brojac + 1);
	else
		return PretragaRekurzivno(mostar, prosjek, pozicija - 1, brojac);


}
void main() {

	/*
	1. BROJ I TIPOVI PARAMETARA MORAJU BITI IDENTICNI ONIMA U TESTNOM CODE-U (OSIM UKOLIKO IMATE OPRAVDANU POTREBU ZA MODIFIKACIJOM). U SUPROTNOM SE RAD NECE BODOVATI
	2. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE. TAKOÐER, UKLONITE SVE DIJELOVE CODE-A KOJI NISU IMPLEMENTIRANI
	3. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	4. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
	5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datum1, datum2, datum3, datum4, datum5, datum6;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(13, 7, 2016);
	datum6.Unos(22, 9, 2016);
	Kurs softverski, html, sql, security;
	softverski.Unos(SoftwareEngeneeringFundamentals, datum1, datum2, "Emina Junuz");
	html.Unos(HtmlCSSJavaScript, datum3, datum2, "Larisa Tipura");
	sql.Unos(MasteringSQL, datum3, datum4, "Jasmin Azemovic");
	security.Unos(WindowsSecurity, datum3, datum4, "Adel Handzic");

	Polaznik denis, zanin, indira;
	denis.Unos(1, "Denis Music");
	zanin.Unos(2, "Zanin Vejzovic");
	indira.Unos(3, "Indira Hamulic");

	Polaganja denisHtml, zaninHtml, indiraSql;
	denisHtml.Unos(denis, html, datum5, 61);
	//61 predstavlja ostvareni uspjeh/procenat, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvari 
	//vise od 55%
	zaninHtml.Unos(zanin, html, datum6, 93);
	indiraSql.Unos(indira, sql, datum6, 82); //test
	SkillsCentar mostar;
	mostar.Unos("Skills Center Mostar");
	////jedan edukacijski centar moze nuditi vise istih kurseva (npr. MasteringSQL), 
	//ali se oni moraju realizovati u razlictim periodima (pocetak-kraj) tj. ne smiju se preklapati
	////novi kurs istog tipa (npr. MasteringSQL) se mora realizovati nakon prethodno dodanog 
	// npr. ako prethodni kurs MasteringSQL traje 01.03.2016 - 01.04.2016, onda se novi kurs 
	// MasteringSQL moze dodati jedino ako pocinje nakon 01.04.2016
	if (mostar.DodajKurs(softverski)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		softverski.Ispis();
		cout << crt;
		mostar._trenutnoKurseva++;
	}
	else
	{
		cout << "Vec postoji kurs u tom terminu. " << endl;
	}
	if (mostar.DodajKurs(html)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		html.Ispis();
		cout << crt;
		mostar._trenutnoKurseva++;
	}
	else
	{
		cout << "Vec postoji kurs u tom terminu. " << endl;
	}
	if (mostar.DodajKurs(sql)) { //test
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		sql.Ispis();
		cout << crt;
		mostar._trenutnoKurseva++;
	}
	else
	{
		cout << "Vec postoji kurs u tom terminu. " << endl;
	}
	if (mostar.DodajKurs(security)) {  //test
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		security.Ispis();
		cout << crt;
		mostar._trenutnoKurseva++;
	}
	else
	{
		cout << "Vec postoji kurs u tom terminu. " << endl;
	}
	//nije moguce dodati polaganje onog kursa koji nije evidentiran 
	//(registrovan, ponudjen) u 
	//tom edukacijskom centru
	//moguce je polagati samo aktivne kurseve i to najkasnije 5 dana od 
	//dana zavrsetka (kraja) 
	//posljednje dodanog (aktivnog) kursa iste vrste (npr. MasteringSQL)
	///jedan polaznik moze vise puta polagati isti kurs, ali novi procenat 
	//mora biti veci od 
	//prethodno dodanog (na istom kursu)
	if (mostar.DodajPolaganje(denisHtml)) {
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		denisHtml.Ispis();
		mostar._trenutnoPolaganja++;
	}
	else
	{
		cout << "Ne moze se izvrsiti polaganje. " << endl;
	}
	if (mostar.DodajPolaganje(denisHtml)) {  //testiranje funkcionalnosti
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		denisHtml.Ispis();
		mostar._trenutnoPolaganja++;
	}
	else
	{
		cout << "Ne moze se izvrsiti polaganje. " << endl;
	}
	if (mostar.DodajPolaganje(zaninHtml)) { //ovo je dodano samo radi testiranja funkcionalnosti
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		zaninHtml.Ispis();
		mostar._trenutnoPolaganja++;
	}
	else
	{
		cout << "Ne moze se izvrsiti polaganje. " << endl;
	}
	if (mostar.DodajPolaganje(indiraSql)) { //ovo je dodano samo radi testiranja funkcionalnosti
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		indiraSql.Ispis();
		mostar._trenutnoPolaganja++;
	}
	else
	{
		cout << "Ne moze se izvrsiti polaganje. " << endl;
	}
	Datum OD, DO;
	int brojPolaganja = 0;
	OD.Unos(1, 6, 2016);
	DO.Unos(1, 8, 2016);
	//PolaganjaByDatum – vraca niz polaganja koja su uspjesno realizovana u periodu OD-DO
	Polaganja* polaganjaByDatum = mostar.PolaganjaByDatum(OD, DO, brojPolaganja);
	cout << "U periodu od ";
	OD.Ispis();
	cout << " do ";
	DO.Ispis();
	cout << " uspjesno je realizovano " << brojPolaganja << " polaganja--->>>" << endl;
	for (size_t i = 0; i < brojPolaganja; i++)
		polaganjaByDatum[i].Ispis();
	mostar.DodajPolaganje(indiraSql);
	//PretragaRekurzivno - rekurzivna funkcija pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na odredjenom kursu (npr. MasteringSQL)
	cout << "Prosjecan uspjeh na kursu MasteringSQL u 2016 godini je " << PretragaRekurzivno(mostar, 0, mostar._trenutnoPolaganja, 0) << endl;
	//Ispis - ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve na osnovu pocetka odrzavanja kursa
	mostar.Ispis();

	//izvrsiti potrebne dealokacije
	mostar.Dealociraj();
	system("pause>0");
}
