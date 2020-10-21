#include<iostream>
#include<memory>
#include <cctype>
using namespace std;
char crt[] = "\n-------------------------------------------\n";

enum eNacinStudiranja { REDOVAN, DL };
const char* nacinStudiranjatxt[] = { "REDOVAN", "DL" };
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char* razredTxt[] = { "PRVI", "DRUGI", "TRECI", "CETVRTI" };

struct Datum {
	int _dan, _mjesec, _godina;
	void Unos(int d, int m, int g) { _dan = d; _mjesec = m; _godina = g; }
	void Ispis() { cout << _dan << "/" << _mjesec << "/" << _godina << endl; }

};

const Datum rokZaPrijavu = { 5, 7, 2017 };

const char* errVecDodan = "Istoimeni predmet se moze dodati najvise dva puta!";
const char* errProsaoRok = "Zao nam je. Rok za prijavu je vec prosao!";
const char* uspjesnoDodan = "Predmet uspjesno dodan!";

struct Predmet {
	char* _naziv;
	int* _ocjena;
	Datum _datumUnosa;
	void Unos(const char* naziv, int ocjena, Datum datumUnosa) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_ocjena = new int(ocjena);
		_datumUnosa.Unos(datumUnosa._dan, datumUnosa._mjesec, datumUnosa._godina);
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete _ocjena; _ocjena = nullptr;
	}

	void Ispis() {
		cout << _naziv << " (" << *_ocjena << ") "; _datumUnosa.Ispis();
	}
	void PromijeniOcjenu(int ocjena) { *_ocjena = ocjena; }
};
bool ValidanRok(Datum d) {
	if (d._godina > 2017)
		return false;
	else if (d._godina == 2017)
	{
		if (d._mjesec > 7)
			return false;
		else if (d._mjesec == 7)
		{
			if (d._dan >= 5)
				return false;
		}

	}
	return true;
}
struct Uspjeh {
	eRazred  _razred;
	bool _najboljiUcenik; // ako je kandidat u tom razredu proglasen najboljim ucenikom
	shared_ptr<Predmet>* _predmeti;
	int _brojPredmeta = 0;
	int brojacIstih = 0;
	void Unos(eRazred razred, bool najboljiUcenik) {
		_razred = razred;
		_najboljiUcenik = najboljiUcenik;
		_brojPredmeta = 0;
	}
	void Dealociraj() {

	}


	bool DodajPredmet(Predmet p) {
		shared_ptr<Predmet>* temp = new shared_ptr<Predmet>[_brojPredmeta + 1];
		for (size_t i = 0; i < _brojPredmeta; i++)
		{
			temp[i] = make_shared<Predmet>();
			temp[i]->Unos(_predmeti[i]->_naziv, *_predmeti[i]->_ocjena, _predmeti[i]->_datumUnosa);
		}
		temp[_brojPredmeta] = make_shared<Predmet>();
		temp[_brojPredmeta]->Unos(p._naziv, *p._ocjena, p._datumUnosa);
		delete[]_predmeti;
		_predmeti = temp;
		_brojPredmeta++;
		return true;
	}
	void Ispis() {
		cout << crt << "Razred -> " << razredTxt[_razred] << "Najbolji -> " << _najboljiUcenik << crt;
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i]->Ispis();
	}
};

struct Kandidat {
	eNacinStudiranja _nacinStudiranja;
	char* _imePrezime;
	Uspjeh* _uspjeh[4];


	void Unos(eNacinStudiranja nacinStudiranja, const char* imePrezime) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		_nacinStudiranja = nacinStudiranja;
		for (size_t i = 0; i < 4; i++)
			_uspjeh[i] = nullptr;
	}
	bool DodajPredmet(eRazred r, Predmet p) {
		int poz = r - 1;
		int brojacIstih = 0;
		if (_uspjeh[poz] == nullptr) {
			_uspjeh[poz] = new Uspjeh;
			_uspjeh[poz]->Unos(r, true);
		}
		if (ValidanRok(p._datumUnosa) == false)
		{
			cout << crt << errProsaoRok << endl;
			return false;
		}
		for (size_t i = 0; i < 4; i++)
		{
			if (_uspjeh[i] == nullptr)
				continue;
			for (size_t j = 0; j < _uspjeh[i]->_brojPredmeta; j++)
			{
				if (strcmp(p._naziv, _uspjeh[i]->_predmeti[j]->_naziv) == 0)
					brojacIstih++;
			}
		}
		if (brojacIstih >= 2)
		{
			cout << crt << errVecDodan << crt << endl;
			return false;
		}
		_uspjeh[poz]->DodajPredmet(p);
		cout << crt << uspjesnoDodan << endl;
		return true;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 4; i++) {
			_uspjeh[i]->Dealociraj();
			delete _uspjeh[i];
		}
	}
	void Ispis() {
		cout << crt << _imePrezime << " " << nacinStudiranjatxt[_nacinStudiranja];
		for (size_t i = 0; i < 4; i++)
		{
			if (_uspjeh[i] != nullptr)
				_uspjeh[i]->Ispis();
		}
	}
};

void main() {
	Datum datum19062017, datum20062017, datum30062017, datum05072017;
	datum19062017.Unos(19, 6, 2017);
	datum20062017.Unos(20, 6, 2017);
	datum30062017.Unos(30, 6, 2017);
	datum05072017.Unos(5, 7, 2017);
	//cout << datum19062017.GetDatumKaoNizKaraktera();
	Predmet Matematika, Fizika, Hemija, Engleski;
	//	2 - ocjena na predmetu; datum - datum evidentiranja uspjeha na predmetu jer postoji krajnji rok za evidentiranje
	Matematika.Unos("Matematika", 2, datum19062017);
	Fizika.Unos("Fizika", 5, datum20062017);
	Hemija.Unos("Hemija", 2, datum20062017);
	Engleski.Unos("Engleski", 5, datum05072017);

	int brojKandidata = 2;

	Kandidat* prijave2017 = new Kandidat[brojKandidata];
	prijave2017[0].Unos(DL, "Jasmin Azemovic");
	prijave2017[1].Unos(REDOVAN, "Indira Hamulic");

	//	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda. prilikom dodavanja onemoguciti:
	//	- istoimeni predmet se moze dodati samo dva puta, bez obzira sto se dodaje na razlicitim godinama,
	//	- dodavanje predmeta nakon dozvoljenog roka za prijavu (rokZaPrijavu).
	//	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom
	//	(npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
	//	Funkcija vraca neku od predefinisanih poruka u zavisnosti od (ne)uspjesnost izvrsenja

	if (prijave2017[0].DodajPredmet(DRUGI, Engleski)) //ne bi trebao dodati jer je prosao postavljeni rok za dodavanje predmeta
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Fizika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;

	Matematika.PromijeniOcjenu(5);
	Hemija.PromijeniOcjenu(3);
	cout << endl;
	cout << "******************************** NOVO DODAVANJE ****************************************" << endl;
	if (prijave2017[1].DodajPredmet(PRVI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Matematika))//ne bi trebalo ponovo dodati Matematiku!
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Engleski))
		cout << "Predmet uspjesno dodan!" << crt;


	//	koristeci Lambda izraz kreirati funkciju koja ce vratiti uspjeh kandidata koji je ostvario najveci prosjek (na nivou razreda, 
	//	a ne ukupni prosjek).funkcija prima parametre tipa eNacinStudiranja (pretrazuje samo kandidate za Redovan ili DL nacin studiranja)
	//	i bool kojim se omogucava pretraga samo onih razreda na kojim su kandidati bili najbolji ucenici (ako se funkciji proslijedi
	//	vrijednost true, ona ce uzeti u obzir samo one razrede kada su kandidati  kao najbolji).
	//	ukoliko vise kandidata ima isti prosjek funkcija vraca uspjeh (najboljeg razreda) prvog pronadjenog kandidata


	auto najboljiUspjeh = [prijave2017, brojKandidata](eNacinStudiranja n, bool najbolji) {
		Uspjeh* najboljiUspjeh = nullptr;
		float suma = 0;
		float prosjek = 0;
		float prosjekNajboljegRaz = 0;
		for (int i = 0; i < brojKandidata; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				if (prijave2017[i]._uspjeh[j] == nullptr)
					continue;
				if (prijave2017[i]._nacinStudiranja != n)
					continue;

				if (prijave2017[i]._uspjeh[j]->_najboljiUcenik != najbolji)
					continue;

				for (size_t k = 0; k < prijave2017[i]._uspjeh[j]->_brojPredmeta; k++)
				{
					suma += *prijave2017[i]._uspjeh[j]->_predmeti[k]->_ocjena;
				}
				prosjek = suma / prijave2017[i]._uspjeh[j]->_brojPredmeta;
				if (prosjek > prosjekNajboljegRaz) {
					prosjekNajboljegRaz = prosjek;
					najboljiUspjeh = prijave2017[i]._uspjeh[j];
				}
			}
		}
		return najboljiUspjeh;

	};
	auto najboljiUspjeh = [prijave2017, brojKandidata](eNacinStudiranja n, bool najbolji) {
		Uspjeh* x = nullptr;
		float veci = 0;
		float suma = 0;
		for (size_t i = 0; i < brojKandidata; i++)
		{

			for (size_t j = 0; j < 4; j++)
			{
				if (prijave2017[i]._uspjeh[j] == nullptr)
					continue;
				if (prijave2017[i]._uspjeh[j]->_najboljiUcenik != najbolji)
					continue;
				if (prijave2017[i]._nacinStudiranja != n)
					for (size_t k = 0; k < prijave2017[i]._uspjeh[j]->_brojPredmeta; k++)
					{

						suma += *prijave2017[i]._uspjeh[j]->_predmeti[k]->_ocjena;
					}
				suma = suma / prijave2017[i]._uspjeh[j]->_brojPredmeta;
				if (suma > veci)
				{
					veci = suma;
					x = prijave2017[i]._uspjeh[j];
				}

			}
		}
		return x;

	};


	Uspjeh* najbolji = najboljiUspjeh(DL, true);
	najbolji->Ispis();


	//	napisati rekurzivnu funkciju koja ce vratiti pokazivac na kandidata sa najvecim prosjekom u razredu koji je proslijedjen kao parametar. 
	//	ukoliko je vise kandidata ostvarilo isti prosjek, funkcija treba da vrati posljednje pronadjenog kandidata. u slucaju da niti jedan kandidat nije
	//	evidentirao uspjeh u trazenom razredu, funkcija vraca nullptr. u nastavku je prikazan primjer poziva rekurzivne funkcije, a ostale parametre dodajte po potrebi. 
	//	Kandidat * kandidatSaNajboljimProsjekom = rekNajboljiProsjek(prijave2017, brojKandidata, DRUGI);

	for (size_t i = 0; i < brojKandidata; i++)
	{
		prijave2017[i].Ispis();
		prijave2017[i].Dealociraj();
	}
	system("pause>0");
}

