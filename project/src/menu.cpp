#include "menu.hh"

void opcje_menu()
{
    std::cout << std::endl;
    std::cout << " r - ruch przed siebie pod zadanym katem" << std::endl;
    std::cout << " o - obrot wokol osi Z pod zadanym katem" << std::endl;
    std::cout << " m - wyswietl menu programu" << std::endl << std::endl;
    std::cout << " k - zakoncz program" << std::endl << std::endl;

    Wektor3D licznik;

    licznik.Pokaz_Wek_lacznie();
    licznik.Pokaz_wek_teraz();

    std::cout << std::endl;
}

void inicjalizuj_lacze(PzG::LaczeDoGNUPlota & Lacze)
{
    Lacze.Inicjalizuj();

    Lacze.ZmienTrybRys(PzG::TR_3D);

    Lacze.UstawRotacjeXZ(75,80);

    Lacze.UstawZakresZ(-110,10);
    Lacze.UstawZakresX(0,120);
    Lacze.UstawZakresY(0,120);

    Lacze.DodajNazwePliku("dat/scena.dat");
}

bool menu()
{
    char znak = 'm';
    std::ofstream Plik;
    srand(time(NULL));

    PzG::LaczeDoGNUPlota Lacze;
    inicjalizuj_lacze(Lacze);

    Scena scena = Scena(0,100,0,100);

    while(znak!='k')
    {
        std::cout << "___DRON (POD)WODNY___" << std::endl;
        opcje_menu();
        std::cout << " Twoj wybor> ";
        std::cin >> znak;

        switch(znak)
        {
            case 'r':
            {
                std::cout << "Podaj wartosc ruchu oraz kat wznoszenia (ruch kat)" << std::endl;
                std::cout << "Twoj wybor> ";
                int krok, kat;
                std::cin >> krok >> kat;
                if(std::cin.fail())
                {
                    std::cerr << "Blad wczytywania wartosci ruchu" << std::endl;
                    exit(1);
                }
                while(kat>89 || kat<(-89))
                {
                    std::cout << "Podany zly kat ruchu, podaj kat z zakresu -89/89" << std::endl;
                    std::cin >> kat;
                }
                for(int i=0; i<krok; i++)
                {
                    Plik.open("dat/scena.dat");
                    Lacze.UstawZakresX(scena()[X]-OTOCZENIE_DRONA,scena()[X]+OTOCZENIE_DRONA);
                    Lacze.UstawZakresY(scena()[Y]-OTOCZENIE_DRONA,scena()[Y]+OTOCZENIE_DRONA);
                    scena.generuj_dno_woda();
                    scena.generuj_przeszkody(5);
                    int kolizja;
                    Scena * test_kolizja = new Scena(scena);
                    (*test_kolizja)().ruch_przod_kat(1,kat);
                    (*test_kolizja)()(PRAWY).obrot_prawa(5);
                    (*test_kolizja)()(LEWY).obrot_lewa(5);
                    (*test_kolizja)().translacja_glob();
                    kolizja = (*test_kolizja).czy_kolizja();
                    delete test_kolizja;
                    switch(kolizja)
                    {
                        case BRAK_KOLIZJI:
                        {
                            scena().ruch_przod_kat(1,kat);
                            scena()(PRAWY).obrot_prawa(5);
                            scena()(LEWY).obrot_lewa(5);
                            scena().translacja_glob();
                            if(!scena.zapisz_plik(Plik))
                            {
                                cerr << "Blad zapisu sceny do pliku dat" << endl;
                                return false;
                            }
                            Lacze.Rysuj();
                            break;
                        }
                        case KOLIZJA_DNO:
                        {
                            std::cout << "---\nKolizja z dnem\n---" << std::endl;
                            break;
                        }
                        case KOLIZJA_WODA:
                        {
                            scena().ruch_przod_kat(1,0);
                            scena()(PRAWY).obrot_prawa(5);
                            scena()(LEWY).obrot_lewa(5);
                            scena().translacja_glob();
                            if(!scena.zapisz_plik(Plik))
                            {
                                cerr << "Blad zapisu sceny do pliku dat" << endl;
                                return false;
                            }
                            Lacze.Rysuj();
                            std::cout << "---\nMaksymalne wynurzenie\n---" << std::endl;
                            break;
                        }
                        case KOLIZJA_PRZESZKODA:
                        {
                            std:: cout << "---\nKolizja z przeszkoda\n---" << std::endl;
                        }
                    }
                    Plik.close();
                    if(kolizja!=BRAK_KOLIZJI && kolizja!=KOLIZJA_WODA) break;
                    /*
                    if((scena()[Z]-10) < POZ_DNA)
                    {
                        std::cout << "Dron uderza w dno, koniec programu" << std::endl;
                        exit(-1);
                    }
                    */
                    usleep(50000);
                }
                break;
            }
            case 'o':
            {
                std::cout << "Podaj wartosc kata obrotu wokol osi Z" << std::endl;
                std::cout << "Twoj wybor> ";
                double kat;
                std::cin >> kat;
                if(std::cin.fail())
                {
                    std::cerr << "Blad wczytywania wartosci ruchu" << std::endl;
                    exit(1);
                }
                for(int i=0; i<abs(kat); i++)
                {
                    Plik.open("dat/scena.dat");
                    if(kat>0)
                    {
                        scena().obrot_kat (1);
                        scena()(PRAWY).obrot_prawa(5);
                    }
                    else
                    {
                        scena().obrot_kat (-1);
                        scena()(LEWY).obrot_lewa(5);
                    }
                    scena().translacja_glob();
                    if(!scena.zapisz_plik(Plik))
                    {
                        cerr << "Blad zapisu sceny do pliku dat" << endl;
                        return 1;
                    }
                    Plik.close();
                    Lacze.Rysuj();
                    usleep(50000);
                }
                break;
            }
            case 'm':
            {
                opcje_menu();
                break;
            }
            case 'k':
            {
                std::cout << "Dzięki za plywanie z nami!!!" << std::endl;
                exit(0);
                break;
            }
            default:
            {
                std::cout << "Brak podanej opcji, prosze wybrac opcje z listy" << std::endl;
                break;
            }

        }
    }
    return true;
}
