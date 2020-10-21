#include <iostream>
#include <conio.h>
#include<cstring>
using namespace std;
char  crt[] = "\n-------------------------------\n";

struct Kandidat {
	char* _imePrezime = nullptr;
	char  _JMBG[14];
	void Unos(const char* ip, const char* jmbg) {
		_imePrezime = new char[strlen(ip) + 1];
		strcpy_s(_imePrezime, strlen(ip) + 1, ip);
		strcpy_s(_JMBG, jmbg);
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	void Ispis() {
		cout << _imePrezime << " " << _JMBG << " ";
	}
};
void obaviBrisanje(char* _odgovori[], int* _tacni, int lokacija) //funkcija za brisanje odabranog odgovora
{

	char* pokazivacnovi[10];
	int j = 0;
	for (int i = 0; i < 10; i++)
	{
		if (_odgovori[i] != nullptr)
		{
			pokazivacnovi[i] = nullptr;
			if (i != lokacija)
			{
				pokazivacnovi[j] = new char[strlen(_odgovori[i]) + 1];
				strcpy_s(pokazivacnovi[j], strlen(_odgovori[i]) + 1, _odgovori[i]);
				j++;
			}
			else
				continue;
		}

	}
	for (int i = 0; i < 10; i++)
	{
		if (_odgovori[i] != nullptr)
		{
			delete[] _odgovori[i];
			_odgovori[i] = nullptr;
		}
		else
			break;
	}
	for (int i = 0; i < 10; i++)
	{
		if (pokazivacnovi[i] != nullptr)
		{
			_odgovori[i] = new char[strlen(pokazivacnovi[i]) + 1];
			strcpy_s(_odgovori[i], strlen(pokazivacnovi[i]) + 1, pokazivacnovi[i]);
		}
		else
			break;
	}


}
int kolikoIma(int* odgovori) //broji koliko ima tacnih odgovora
{
	int brojac = 0;
	for (int i = 0; i < 10; i++)
	{
		if (odgovori[i] != -1)
		{
			brojac++;
		}
	}
	return brojac;
}
int kolikoImaodgukupno(char** odgovori) //broji koliko ima ukupno odgovora
{
	int brojac = 0;
	for (int i = 0; i < 10; i++)
	{
		if (odgovori[i] != nullptr)
		{
			brojac++;
		}
	}
	return brojac;
}
float brojBodova(int buffer, float jednopitanje, int* tacniodgovori, int kazneni, int ukupno) //vraca broj bodova pojedinacnog odgovora
{
	for (int i = 0; i < ukupno; i++)
	{
		if (buffer == tacniodgovori[i])
		{
			return jednopitanje;
		}
	}
	return -kazneni;
}
bool jelPostoji(const char* noviodgovor, char** sviodgovori, int doSada) //provjerava da li postoji vec dodati odgovor
{
	for (int i = 0; i < doSada; i++)
	{
		if (strcmp(sviodgovori[i], noviodgovor) == 0)
		{
			return true;
		}
	}
	return false;
}
bool jelPostojiv2(Kandidat** svikandidati, const char* novijmbg, int doSada) //Provjerava da li je kandidat s odredjenim jmbg vec dodan 
{
	for (int i = 0; i < doSada; i++)
	{
		if (strcmp(svikandidati[i]->_JMBG, novijmbg) == 0)
		{
			return true;
		}
	}
	return false;
}
struct Pitanje {
	char* _tekstPitanja = nullptr;
	char* _odgovori[10];//maksimalno 10 odgovora 
	int _tacni[10];
	/*lokacije tacnih odgovora - svako pitanje moze imati jedan ili vise tacnih odgovora, a njihove lokacije se pohranjuju u niz _tacni */
	int _bodova;
	/*ako pitanje ima vise tacnih odgovora, onda se broj osvojenih bodova (tokom izrade testa) odredjuje na osnovu
	broj tacnih odgovora (npr. ako pitanje nosi 5 boda i ima 2 tacna odgovora, a student oznaci 1 tacan odgovor
	onda ce osvojeni broj bodova na tom pitanju iznositi 2.5. drugim rijecima, vrijednost ovog atributa je
	bitna zbog odredjivanja uspjeha kandidata prilikom izrade ispita)*/

	void Unos(const char* tekstPitanja, int bodova) {
		_tekstPitanja = new char[strlen(tekstPitanja) + 1];
		strcpy_s(_tekstPitanja, strlen(tekstPitanja) + 1, tekstPitanja);

		/*umjesto dodavanja brojaca iskoristite defaultne vrijednosti clanova niza kako biste manipulisali njihovim vrijednostima (dodavanje, uklanjanje)*/
		for (size_t i = 0; i < 10; i++) {
			_odgovori[i] = nullptr;
			_tacni[i] = -1;


		}
		_bodova = bodova;
	}
	void Dealociraj() {

		delete[] _tekstPitanja; _tekstPitanja = nullptr;
		for (size_t i = 0; i < 10; i++)
		{
			if (_odgovori[i] == nullptr)
				break;
			delete[] _odgovori[i]; _odgovori[i] = nullptr;
		}
	}
	bool AddOdgovor(const char* odgovor, bool jelTacan)
	{
		for (int i = 0; i < 10; i++)//
		{
			if (_odgovori[i] == nullptr)
			{
				_odgovori[i] = new char[strlen(odgovor) + 1];
				strcpy_s(_odgovori[i], strlen(odgovor) + 1, odgovor);
				if (jelTacan)
					_tacni[i] = i;
				if (jelPostoji(_odgovori[i], _odgovori, i))
				{

					delete[] _odgovori[i];
					_odgovori[i] = nullptr;
					return false;
				}
				else
				{
					return true;
				}
			}

		}

	}
	bool RemoveOdgovor(int lokacija)
	{
		lokacija--;//od 0 krece niz pa je fojnica 3 a proslijedjeno je 4
		for (int i = 0; i < 10; i++)
		{
			if (_tacni[i] != -1 && _tacni[i] == lokacija)
			{
				if (kolikoIma(_tacni) < 2)
				{
					cout << "mora postojati bar jedan tacan odgovor " << endl;
					return false;
				}
				else
				{

					obaviBrisanje(&_odgovori[0], &_tacni[0], lokacija);
					cout << "uspjesno obavljeno" << endl;
					return true;

				}
			}
			else
			{
				if (i == lokacija)
				{
					obaviBrisanje(&_odgovori[0], &_tacni[0], lokacija);
					cout << "uspjesno obavljeno" << endl;
					return true;
				}
			}


		}
		cout << "doso sam do kraja ne valja" << endl;
		return false;
	}
	void Ispis()
	{
		cout << crt;
		cout << "." << _tekstPitanja << crt;
		for (int i = 0; _odgovori[i] != nullptr; i++)
		{
			cout << i + 1 << ". " << _odgovori[i] << endl;
		}
		cout << crt;
	}
};
struct PrijemniIspit {
	//kandidati prijavljeni za prijemni ispit 
	Kandidat* _prijavljeniKandidati[100];
	/*uspjeh ostvaren na prijemnom ispitu za svakog pojedinog kandidata(prijemnom ispitu moze pristupiti maksimalno 100 kandidata i
	za svakog od njih se cuva ostvareni uspjeh u okviru niza _uspjehKandidata (nakon sto urade ispit tj. nakon sto odgovore na pitanja)*/
	float* _uspjehKandidata[100];
	//prag prolaznosti na prijemnom ispitu npr. 60 procenata
	float _pragProlaznosti;
	float _kazneniBodovi;
	/*vrijednost kazenog boda (npr. 0.30) se mnozi sa svakim netacnim odgovorom na pitanje i oduzima od osvojenih bodova za to pitanje
	(npr. ako pitanje nosi 5 bodova i ima 2 tacna odgovora, a student odabere 1 tacan i 2 netacna odgovora, broj osvojenih bodova ce biti 1.9
	(1 tacan = 2.5; 2 netacna * 0.30 kaznenih = 0.6; 2.5 - 0.6 = 1.9)*/

	void Unos(float pragProlaznosti, float kazneniBodovi) {
		_pragProlaznosti = pragProlaznosti;
		_kazneniBodovi = kazneniBodovi;
		/*umjesto dodavanja brojaca iskoristite defaultne vrijednosti clanova niza kako biste manipulisali njihovim vrijednostima
		(dodavanje, uklanjanje)*/
		for (size_t i = 0; i < 100; i++)
		{
			_prijavljeniKandidati[i] = nullptr;
			_uspjehKandidata[i] = nullptr;
		}
	}
	bool DodajKandidata(Kandidat nekikandat)
	{
		for (int i = 0; i < 100; i++)
		{
			if (_prijavljeniKandidati[i] == nullptr)
			{
				_prijavljeniKandidati[i] = new Kandidat;
				_prijavljeniKandidati[i]->Unos(nekikandat._imePrezime, nekikandat._JMBG);
				if (jelPostojiv2(_prijavljeniKandidati, nekikandat._JMBG, i))
				{
					cout << "POSTOJI KANDIDAT S ISTIM JMBGOM ,DODAVANJE NEUSPJELO" << endl;
					_prijavljeniKandidati[i]->Dealociraj();
					_prijavljeniKandidati[i] = nullptr;
					return false;
				}
				else
					return true;
			}
			else
				continue;
		}
	}
	bool PokreniIspit(const char* jmbg, Pitanje* nizPitanja, int brojpit)
	{
		int odgovor, k = 0;
		float bodovi_jednog = 0;

		int broj_ukupnobodova = 0;
		int buffer[10];
		for (int i = 0; i < 100; i++)
		{
			if (strcmp(jmbg, _prijavljeniKandidati[i]->_JMBG) == 0)
			{
				for (int j = 0; j < brojpit; j++)
				{
					float br_jednogodg = float(nizPitanja[j]._bodova) / float((kolikoIma(nizPitanja[j]._tacni)));
					nizPitanja[j].Ispis();
					if (kolikoIma(nizPitanja[j]._tacni) < 2)
					{

						do {
							cout << "unesite broj jednog odgovora " << endl;
							cin >> odgovor;
						} while (odgovor<1 || odgovor>kolikoImaodgukupno(nizPitanja[j]._odgovori));
						odgovor--;
						bodovi_jednog += brojBodova(odgovor, br_jednogodg, nizPitanja[j]._tacni, _kazneniBodovi, kolikoImaodgukupno(nizPitanja[j]._odgovori));
					}
					else
					{
						do {
							cout << "unesite koliko odgovora unosite ->" << endl;
							cin >> k;
						} while (k<1 || k>kolikoImaodgukupno(nizPitanja[j]._odgovori));
						for (int y = 0; y < k; y++)
						{
							cin >> buffer[y];
							buffer[y]--;//korisnik unosi 1 2 3 4 5 .. pa zbog toga -1
						}

						for (int x = 0; x < k; x++)
						{
							bodovi_jednog += brojBodova(buffer[x], br_jednogodg, nizPitanja[j]._tacni, _kazneniBodovi, kolikoImaodgukupno(nizPitanja[j]._odgovori));
						}

					}
					broj_ukupnobodova += nizPitanja[j]._bodova;


				}
				_uspjehKandidata[i] = new float((bodovi_jednog / broj_ukupnobodova) * 100);
				if (*_uspjehKandidata[i] >= _pragProlaznosti)
				{
					cout << *_uspjehKandidata[i] << "%" << endl;
					cout << "Cestitke prosli ste" << endl;
					return true;
				}
				else
				{
					cout << *_uspjehKandidata[i] << "%" << endl;
					cout << "Niste polozili" << endl;
					return true;

				}
			}

		}


	}
};
int main() {

	Kandidat jasmin, adel, denis;
	jasmin.Unos("Jasmin Azemovic", "1111111111111");
	adel.Unos("Adel Handzic", "2222222222222");
	denis.Unos("Denis Music", "1111111111111");//ovo je cisto provjera funkcionalisti funkcije dodaj kandidata

	jasmin.Ispis();
	cout << endl;
	adel.Ispis(); // testiramo ispis funkciju
	cout << endl;


	int brojPitanja = 3;
	Pitanje* pitanja = new Pitanje[brojPitanja];
	//parametri: tekstPitanja, brojBodova
	pitanja[0].Unos("U kojem gradu se nalazi FIT?", 4);

	/*dodaje novi odgovor u listu ponudjenih odgovora. onemoguciti ponavljanje odgovora, te voditi racuna da vise odgovora moze biti tacno*/
	//parametri: tekstOdgovora,daLiJeTacanTajOdgovor
	if (pitanja[0].AddOdgovor("Mostar", true))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[0].AddOdgovor("Sarajevo", false))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (!pitanja[0].AddOdgovor("Sarajevo", false))//Sarajevo je vec dodano
		cout << "Odgovor nije uspjesno dodan!" << crt;
	if (pitanja[0].AddOdgovor("Tuzla", false))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[0].AddOdgovor("Fojnica", false))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[0].AddOdgovor("Zenica", false))
		cout << "Odgovor uspjesno dodan zenica!" << crt;

	/*
	na osnovu primljene lokacije uklanja podatke o ponudjenom odgovoru. prilikom uklanjanja ocuvati redoslijed dodavanja odgovora. onemoguciti uklanjanje svih tacnih odgovora tj. osigurati da pitanje u svakom momentu posjeduje najmanje jedan tacan odgovor
	*/
	if (pitanja[0].RemoveOdgovor(0))//uklanja odgovor Fojnica
		cout << "Odgovor uspjesno uklonjen!" << crt;

	for (int i = 0; i < 5; i++)
		cout << "pisem nesto " << pitanja[0]._odgovori[i] << endl;





	/*
	-------------------------------
	1. U kojem gradu se nalazi FIT?
	-------------------------------
	1. Mostar
	2. Sarajevo
	3. Tuzla
	4. Zenica
	-------------------------------
	*/
	pitanja[0].Ispis();//prethodno je prikazan ocekivani ispis pitanja

	pitanja[1].Unos("Odaberite tacne odgovore.", 5);

	if (pitanja[1].AddOdgovor("FIT se nalazi u Mostaru", true))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[1].AddOdgovor("FIT je clanica UNMO-a", true))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[1].AddOdgovor("U Mostaru je uvijek jako vruce", false))
		cout << "Odgovor uspjesno dodan!" << crt;

	pitanja[2].Unos("Studij na FIT-u traje 4 godine?", 2);
	if (pitanja[2].AddOdgovor("Ne", true))
		cout << "Odgovor uspjesno dodan!" << crt;
	if (pitanja[2].AddOdgovor("Da", false))
		cout << "Odgovor uspjesno dodan!" << crt;
	/*....*/
	pitanja[1].Ispis();
	pitanja[2].Ispis();

	PrijemniIspit prijemni2017;
	//parametri: pragProlaznosti, kazneniBodovi
	prijemni2017.Unos(55, 0.30);
	/*dodaje novog kandidata u listu prijavljenih. onemoguciti dodavanje kandidata sa istim jmbg-om*/
	if (prijemni2017.DodajKandidata(jasmin))
		cout << "Kandidat uspjesno dodan" << crt;
	if (prijemni2017.DodajKandidata(adel))
		cout << "Kandidat uspjesno dodan" << crt;

	if (prijemni2017.DodajKandidata(denis))
		cout << "Kandidat uspjesno dodan" << crt;

	/*kandidatu sa JMBG 1111111111111 pokrece ispit na nacin da mu se prikazuju pitanja, od njega zahtijevaju odgovori,
	te izracunava ostvareni uspjeh (da li je polozio ili ne, te sa kojim procentom uspjesnosti).
	prilikom izracuvanja uspjesnosti voditi racuna o kaznenim bodovima. po okoncanju ispita, uspjeh kandidata se pohranjuje u
	niz _uspjehKandidata. za pokretanje ispita kandidat prethodno mora biti dodan u listu. po vlastitoj zelji implementirajte naci
	oznacavanja veceg broja tacnih odgovoran na postavljeno pitanje (jedno pitanje moze imati vise tacnih odgovora)*/
	if (prijemni2017.PokreniIspit("1111111111111", pitanja, brojPitanja))
		cout << "Ispit uspjesno okoncan!" << crt;

	jasmin.Dealociraj();
	adel.Dealociraj();
	denis.Dealociraj();
	for (size_t i = 0; i < brojPitanja; i++)
		pitanja[i].Dealociraj();
	delete[] pitanja;
	system("pause>0");
	return 0;
}
