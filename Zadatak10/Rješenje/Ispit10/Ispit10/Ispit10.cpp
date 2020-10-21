#include<iostream>
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
const char* vrstaObavezeTxt[] = { "Seminarski", "Parcijalni1", "Parcijalni2","Integralni","Prakticni" };
struct Datum {
	int* _dan, * _mjesec, * _godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() {
		cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl;
	}
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; _dan = _mjesec = _godina = nullptr; }

};
struct ObavezeNaPredmetu {
	shared_ptr<VrstaObaveze> _vrstaObaveze;
	Datum _datumIzvrsenja;
	char* _napomena;
	int _ocjena; // 5 - 10 
	void Unos(VrstaObaveze vrsta, Datum datum, int ocjena, const char* napomena) {
		_vrstaObaveze = make_shared<VrstaObaveze>(vrsta);
		_datumIzvrsenja = datum;
		_ocjena = ocjena;
		_napomena = new char[strlen(napomena) + 1];
		strcpy_s(_napomena, strlen(napomena) + 1, napomena);
	}
	void Ispis() {
		cout << vrstaObavezeTxt[*_vrstaObaveze] << " " << _ocjena << " " << _napomena;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _napomena; _napomena = nullptr;
	}
};

const char* _kljucneRijeci[15] = { NULL }; /*da bi se odredjena obaveza na predmetu smatrala uspjesno izvrsenom,
									  u napomeni (objekta ObavezeNaPredmetu) ne smije stajati niti jedna od rijeci koja
									  se nalazi u nizu _kljucneRijeci*/
int getDane(Datum d) { return *d._dan + *d._mjesec * 30 + *d._godina * 365; }
struct PolozeniPredmet {
	Datum _datumPolaganja;//datum koji ce se evidentirati kao datum kada je predmet polozen tj. kada je formirana konacna ocjena
	char* _nazivPredmeta;
	ObavezeNaPredmetu* _listaIzvrsenihObaveza;
	int _trenutnoIzvrsenihObaveza;
	int _minimalanBrojDana; //odnosi se na minimalan broj dana koji mora proci od izvrsenja pojedinih obaveza na predmetu (npr. 7 dana)
	int _konacnaOcjena; //formira se na osnovu ocjena izvrsenih obaveza

	void Unos(const char* naziv, int minimalanBrojDana) {
		_nazivPredmeta = new char[strlen(naziv) + 1];
		strcpy_s(_nazivPredmeta, strlen(naziv) + 1, naziv);
		_listaIzvrsenihObaveza = nullptr;
		_trenutnoIzvrsenihObaveza = 0;
		_minimalanBrojDana = minimalanBrojDana;
		_konacnaOcjena = 0;
	}
	void Dealociraj() {
		//_datumPolaganja.Dealociraj();
		delete[] _nazivPredmeta; _nazivPredmeta = nullptr;
		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			_listaIzvrsenihObaveza[i].Dealociraj();
		delete[]_listaIzvrsenihObaveza;
	}
	/*Na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu.
	Potrebno je onemoguciti dodavanje identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci
	najmanje onoliko dana koliko je definisano vrijednoscu atributa _minimalanBrojDana. Identicna vrsta
	obaveze se moze dodati samo u slucaju da je prethodno dodana obaveza (identicne vrste) imala ocjenu 5
	ili je u napomeni sadrzavala neku od zabranjenih rijeci (definisanih nizom _kljucneRijeci).*/
	bool DodajIzvrsenuObavezu(VrstaObaveze vrsta, Datum d, int ocjena, const char* napomena) {



		bool imaLi = false;
		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{

			if (abs(getDane(d) - getDane(_listaIzvrsenihObaveza[i]._datumIzvrsenja)) < _minimalanBrojDana)
				return false;
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == vrsta && _listaIzvrsenihObaveza[i]._ocjena > 5) {
				return false;


				for (int j = 0; j < 15; j++) {

					if (_kljucneRijeci[j] != NULL) {
						if (strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[j]) != nullptr)
							imaLi = true;
					}
				}

				if (!imaLi)
					return false;
				imaLi = false;
			}
		}



		ObavezeNaPredmetu* temp = new ObavezeNaPredmetu[_trenutnoIzvrsenihObaveza + 1];
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			temp[i].Unos(*_listaIzvrsenihObaveza[i]._vrstaObaveze, _listaIzvrsenihObaveza[i]._datumIzvrsenja, _listaIzvrsenihObaveza[i]._ocjena, _listaIzvrsenihObaveza[i]._napomena);
		temp[_trenutnoIzvrsenihObaveza].Unos(vrsta, d, ocjena, napomena);
		delete[] _listaIzvrsenihObaveza;
		_listaIzvrsenihObaveza = temp;
		_trenutnoIzvrsenihObaveza++;
		return true;

	}
	/*Konacna ocjene predstavlja prosjecnu ocjenu na predmetu (prosjecnu ocjenu izvrsenih obaveza koje ispunjavaju uslove za
	uspjesno polaganje predmeta),a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita.
	Ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena
	najmanje dva seminarska i jedan prakticni rad. Takodjer, datum polaganja drugog parcijalnog ispita ne smije biti raniji
	(manji) od datuma polaganja prvog parcijalnog ispita. Konacna ocjena ne smije biti formirana u slucaju da napomena bilo
	koje odbaveze (koja je ucestvovala u formiranju konacne ocjene) sadrzi neku od zabranjenih rijeci (niz _kljucneRijeci).
	U slucaju da neki od navedenih uslova nije zadovoljen konacna ocjena treba biti postavljena na vrijednost 5.
	Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, datum polaganja je potrebno postaviti na vrijednost
	datuma posljednje izvrsene obaveze sa najvecom ocjenom.*/


	float FormirajKonacnuOcjenu() {


		int brIntegralnih = 0, brParcijalnih = 0, brSeminarskih = 0, brPrakticnih = 0, indexParcijalnog1 = -1;
		int sumaIntegralnih = 0, sumaParcijalnih = 0, sumaSeminarskih = 0, sumaPrakticnih = 0, indexNajveceOcjene = -1;
		float najvecaOcjena = 0;
		bool imaZabranjenu = false, ParcijalniVeci = true;
		_konacnaOcjena = 5;
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++) {
			if (_listaIzvrsenihObaveza[i]._ocjena > 5) {
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Integralni) {
					brIntegralnih++;
					sumaIntegralnih = _listaIzvrsenihObaveza[i]._ocjena;
				}
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni1) {
					brParcijalnih++;
					sumaParcijalnih += _listaIzvrsenihObaveza[i]._ocjena;
					indexParcijalnog1 = i;
				}
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni2) {
					if (indexParcijalnog1 != -1) {
						if (getDane(_listaIzvrsenihObaveza[i]._datumIzvrsenja) <= getDane(_listaIzvrsenihObaveza[indexParcijalnog1]._datumIzvrsenja))
							ParcijalniVeci = false;
					}
					brParcijalnih++;
					sumaParcijalnih += _listaIzvrsenihObaveza[i]._ocjena;
				}
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Seminarski) {
					brSeminarskih++;
					sumaSeminarskih += _listaIzvrsenihObaveza[i]._ocjena;
				}
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Prakticni) {
					brPrakticnih++;
					sumaPrakticnih += _listaIzvrsenihObaveza[i]._ocjena;
				}
				for (int j = 0; j < 15; j++) {
					if (_kljucneRijeci[j] != NULL) {
						if (strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[j]) != nullptr)
							imaZabranjenu = true;
					}
				}
				if (_listaIzvrsenihObaveza[i]._ocjena > najvecaOcjena) {
					najvecaOcjena = _listaIzvrsenihObaveza[i]._ocjena;
					indexNajveceOcjene = i;
				}
			}
		}
		if (imaZabranjenu == true || ParcijalniVeci == false)
			return _konacnaOcjena;
		if (brIntegralnih > 0 && brSeminarskih >= 2)
			_konacnaOcjena = float(sumaIntegralnih + sumaSeminarskih) / (brIntegralnih + brSeminarskih);
		if (brParcijalnih == 2 && brSeminarskih >= 2 && brPrakticnih == 1)
			_konacnaOcjena = float(sumaParcijalnih + sumaSeminarskih + sumaPrakticnih) / (brParcijalnih + brSeminarskih + brPrakticnih);
		if (_konacnaOcjena > 5)
			_datumPolaganja.Unos(*_listaIzvrsenihObaveza[indexNajveceOcjene]._datumIzvrsenja._dan, *_listaIzvrsenihObaveza[indexNajveceOcjene]._datumIzvrsenja._mjesec, *_listaIzvrsenihObaveza[indexNajveceOcjene]._datumIzvrsenja._godina);
		return _konacnaOcjena;
	}

};

//*Rekurzivna funkcija treba vratiti prosjecnu ocjenu uspjesno izvrsenih obaveze na predmetu odredjene vrste u definisanom periodu. 
//Izbjegavati koristenje globalnih varijabli*/
float PretragaRekurzivno(PolozeniPredmet p, VrstaObaveze v, Datum d1, Datum d2, int lokacija, float suma, int brojac) {
	if (lokacija >= p._trenutnoIzvrsenihObaveza) {
		if (brojac != 0)
			return suma / brojac;


	}
	else {
		if (getDane(p._listaIzvrsenihObaveza[lokacija]._datumIzvrsenja) >= getDane(d1) &&
			getDane(p._listaIzvrsenihObaveza[lokacija]._datumIzvrsenja) <= getDane(d2) &&
			*p._listaIzvrsenihObaveza[lokacija]._vrstaObaveze == v)
		{
			suma += p._listaIzvrsenihObaveza[lokacija]._ocjena;
			return PretragaRekurzivno(p, v, d1, d2, lokacija + 1, suma, brojac + 1);

		}
		else {
			return PretragaRekurzivno(p, v, d1, d2, lokacija + 1, suma, brojac);
		}
	}
}


int GetBrojObavezaIznadProsjeka(PolozeniPredmet p, float prosjek) {
	auto veceOD = [&p](float prosjek) {
		int brojac = 0;
		for (int i = 0; i < p._trenutnoIzvrsenihObaveza; i++) {
			if (p._listaIzvrsenihObaveza[i]._ocjena >= prosjek)
				brojac++;
		}
		return brojac;
	};
	return veceOD(prosjek);
}

void main() {
	_kljucneRijeci[0] = "prepisivao";
	_kljucneRijeci[1] = "ometao";
	_kljucneRijeci[2] = "nije dostavio";

	Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2, datumIntegralni;

	datumSeminarski1.Unos(10, 6, 2017);
	datumSeminarski2.Unos(18, 6, 2017);
	datumParcijalni1.Unos(22, 6, 2017);
	datumParcijalni2.Unos(30, 6, 2017);

	PolozeniPredmet prII;
	//naziv predmeta, minimalan broj dana
	prII.Unos("Programiranje II", 7);

	/*Na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu.
	Potrebno je onemoguciti dodavanje identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci
	najmanje onoliko dana koliko je definisano vrijednoscu atributa _minimalanBrojDana. Identicna vrsta
	obaveze se moze dodati samo u slucaju da je prethodno dodana obaveza (identicne vrste) imala ocjenu 5
	ili je u napomeni sadrzavala neku od zabranjenih rijeci (definisanih nizom _kljucneRijeci).*/
	if (prII.DodajIzvrsenuObavezu(Parcijalni1, datumParcijalni1, 6, "uslovno polozen ispit"))
		cout << "Parcijalni 1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 5, "previse gresaka, a tokom ispita ometao druge studente"))
		cout << "Parcijalni 2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski1, 8, "implementirani svi algoritmi"))
		cout << "Seminarski1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski2, 6, "rad slican kao kod studenta IB150388"))
		cout << "Seminarski2...dodan" << endl;

	/*Konacna ocjene predstavlja prosjecnu ocjenu na predmetu (prosjecnu ocjenu izvrsenih obaveza koje ispunjavaju uslove za
	uspjesno polaganje predmeta),a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita.
	Ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena
		najmanje dva seminarska i jedan prakticni rad. Takodjer, datum polaganja drugog parcijalnog ispita ne smije biti raniji
		(manji) od datuma polaganja prvog parcijalnog ispita. Konacna ocjena ne smije biti formirana u slucaju da napomena bilo
		koje odbaveze (koja je ucestvovala u formiranju konacne ocjene) sadrzi neku od zabranjenih rijeci (niz _kljucneRijeci).
		U slucaju da neki od navedenih uslova nije zadovoljen konacna ocjena treba biti postavljena na vrijednost 5.
		Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, datum polaganja je potrebno postaviti na vrijednost
		datuma posljednje izvrsene obaveze sa najvecom ocjenom.*/

	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;

	datumParcijalni2.Unos(22, 7, 2017);
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 6, ""))
		cout << "Parcijalni 2...dodan" << endl;

	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;
	Datum OD, DO; OD.Unos(1, 1, 2017); DO.Unos(1, 7, 2017);

	///*Rekurzivna funkcija treba vratiti prosjecnu ocjenu uspjesno izvrsenih obaveze na predmetu odredjene vrste u definisanom periodu. 
	//Izbjegavati koristenje globalnih varijabli*/
	float prosjek = PretragaRekurzivno(prII, Seminarski, OD, DO, 0, 0, 0);
	cout << "Prosjecna ocjena izvrsenih obaveza je: " << prosjek << endl;

	///*Koristeci lambda-u, unutar funkcije GetBrojObavezaIznadProsjeka pronaci broj 
	//izvrsenih obaveza koje imaju istu ili vecu ocjenu od prosjecne*/
	int iznadProsjeka = GetBrojObavezaIznadProsjeka(prII, prosjek);
	cout << "Broj izvrsenih obaveza iznad prosjeka je: " << iznadProsjeka << endl;

	/*datumParcijalni1.Dealociraj();
	datumParcijalni2.Dealociraj();
	datumSeminarski1.Dealociraj();
	datumSeminarski2.Dealociraj();
	datumIntegralni.Dealociraj();*/
	prII.Dealociraj();

	system("pause");
}
