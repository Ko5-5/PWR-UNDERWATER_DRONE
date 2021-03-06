#ifndef DRON_HH
#define DRON_HH

#include <cmath>
#include <ostream>
#include <fstream>
#include "figura_geo.hh"
#include "macierz3D.hh"
#include "wirnik.hh"

/**
 * \brief Klasa pochodna figury geometrycznej realizujaca modelowanie drona
 **/
class Dron : public Figura_geo
{
    private:
    /**
     * \brief Pole translacji z ukladu lokalnego do globalnego
     **/
    Wektor3D translacja = Wektor3D(35,35,-50);
    /**
     * \brief Pole obrotu drona wzgledem oryginalnego polozenia
     **/
    Wektor3D obrot = Wektor3D(0,0,0);
    /** 
     * \brief Pola wirnikow drona
     **/
    Wirnik wir_prawy, wir_lewy;

    public:
    /**
     * \brief Konstruktor bezparametryczny klasy Dron
     **/
    Dron():Figura_geo() {};

    /**
     * \brief Konstruktor parametryczny Drona, bierze plik z wsp lokalnymi i zapisuje do tablicy lokalnej
     * \param[in] plik_wej - strumien plikowy wczytywania drona w ukladzie lokalnym
     **/
    Dron(std::ifstream & plik_wej):Figura_geo(plik_wej)
    {
        Wektor3D temp = Wektor3D(0,0,0);
        if(plik_wej.good())
        { 
            while(!plik_wej.eof())
            {
                plik_wej >> temp;
                tablica.push_back(temp);
            }
        }

        std::ifstream plik_wir;
        plik_wir.open("bryly/wirniki-dron-lok1.dat");
        wir_lewy = Wirnik(plik_wir,LEWY);
        plik_wir.close();
        plik_wir.open("bryly/wirniki-dron-lok2.dat");
        wir_prawy = Wirnik(plik_wir,PRAWY);
        plik_wir.close();
    }

    /**
     * \brief Przeciazenie operatora dostepu do translacji drona
     * \param[in] size - zadane pole
     * \return Zadane pole translacji drona
     **/
    double & operator [] (unsigned int size) {return translacja[size];}

    /**
     * \brief Przeciazenie operatora dostepu do translacji drona
     * \param[in] size - zadane pole
     * \return Zadane pole translacji drona
     **/
    double operator [] (unsigned int size) const {return translacja[size];}

    /**
     * \brief Przeciazenie operatora () pozwalajace na dostep do wirnikow drona
     * \param[in] size - nr wirnika
     * \return Objekt wirnika
     **/
    Wirnik & operator () (unsigned int size) {if(size==LEWY) return wir_lewy; else return wir_prawy;}

    /**
     * \brief Przeciazenie operatora () pozwalajace na dostep do wirnikow drona
     * \param[in] size - nr wirnika
     * \return Objekt wirnika
     **/
    Wirnik operator () (unsigned int size) const {if(size==LEWY) return wir_lewy; else return wir_prawy;}

    /**
     * \brief Pozwala na dostep do wektora translacji drona
     * \return Wektor translacji drona
     **/
    Wektor3D translacja_wek() {return translacja;}

    /**
     * \brief Realizuje translacje ukladu lokalnego do globalnego
     **/
    void translacja_glob();

    /** 
     * \brief Realizuje zapis ruchu drona do przodu
     * \param[in] il_krok - wielkosci ruchu
     * \param[in] kat - kat wznoszenia
     **/
    void ruch_przod_kat(int il_krok, double kat);

    /**
     * \brief Realizuje obrot drona wokol osi
     * \param[in] kat - kat obrotu
     **/
    void obrot_kat(int kat);

};

/**
 * \brief Przeciazenie wypisania drona na strumien wyjsciowy
 * \param[in] strm - strumien wyjsciowy
 * \param[in] tab - dron do wypisania
 * \return Strumien wyjsciowy
 **/
std::ostream & operator << (std::ostream & strm, Dron & tab);

#endif
