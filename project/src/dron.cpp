#include "dron.hh"

void Dron::ruch_przod_kat(int il_krok, double kat)
{
    double kat_rad = (kat/180) * (M_PI);
    double kat_rad_obrot = (obrot[Z]/180) * (M_PI);

    translacja[Z] += il_krok * sin(kat_rad);

    translacja[X] += (il_krok * cos(kat_rad))*(-sin(kat_rad_obrot));
    translacja[Y] += (il_krok * cos(kat_rad))*(cos(kat_rad_obrot));
}

void Dron::translacja_glob()
{
    tablica_glob.clear();
    wir_lewy.uk_glob().clear();
    wir_prawy.uk_glob().clear();

    if(obrot[Z]!=0)
    {
        double kat_rad = (obrot[Z]/180) * (M_PI);
        Wektor3D temp = Wektor3D(0,0,0);
        Macierz3D mac_obrot = Macierz3D(Wektor3D(cos(kat_rad),-sin(kat_rad),0),Wektor3D(sin(kat_rad),cos(kat_rad),0),Wektor3D(0,0,1));
        //std::cout << mac_obrot[Z] << std::endl;
        for(int i=0; i<(tablica.size()); i++)
        {
            temp = translacja + mac_obrot*tablica[i];
            //std::cout << temp << std::endl;
            tablica_glob.push_back(temp);
            //std::cout << tablica_glob[i] << std::endl;
        }
        kat_rad = (wir_prawy[4]/180) * (M_PI);
        Macierz3D mac_obrot_wir = Macierz3D(Wektor3D(cos(kat_rad),0,sin(kat_rad)),Wektor3D(0,1,0),Wektor3D(-sin(kat_rad),0,cos(kat_rad)));
        for(int i=0; i<(wir_prawy.uk_lok().size()); i++)
        {
            temp = translacja + mac_obrot*(wir_prawy.translacja() + mac_obrot_wir*wir_prawy.uk_lok()[i]);
            //std::cout << temp << std::endl;
            wir_prawy.uk_glob().push_back(temp);
            //std::cout << tablica_glob[i] << std::endl;
        }
        kat_rad = (wir_lewy[4]/180) * (M_PI);
        mac_obrot_wir = Macierz3D(Wektor3D(cos(kat_rad),0,sin(kat_rad)),Wektor3D(0,1,0),Wektor3D(-sin(kat_rad),0,cos(kat_rad)));
        for(int i=0; i<(wir_lewy.uk_lok().size()); i++)
        {
            temp = translacja + mac_obrot*(wir_lewy.translacja() + mac_obrot_wir*wir_lewy.uk_lok()[i]);
            //std::cout << temp << std::endl;
            wir_lewy.uk_glob().push_back(temp);
            //std::cout << tablica_glob[i] << std::endl;
        }
    }
    else
    {    
        for(int i=0; i<(tablica.size()); i++)
        {
            if(translacja[Z]+tablica[i][Z]>(POZ_WOD-5))
            {
                tablica_glob.push_back(Wektor3D(translacja[X]+tablica[i][X],translacja[Y]+tablica[i][Y],POZ_WOD+1));
            }
            else
            {
                tablica_glob.push_back(Wektor3D(translacja[X]+tablica[i][X],translacja[Y]+tablica[i][Y],translacja[Z]+tablica[i][Z]));
            }
        }
        Wektor3D temp = Wektor3D(0,0,0);
        double kat_rad = (wir_prawy[4]/180) * (M_PI);
        Macierz3D mac_obrot = Macierz3D(Wektor3D(cos(kat_rad),0,sin(kat_rad)),Wektor3D(0,1,0),Wektor3D(-sin(kat_rad),0,cos(kat_rad)));
        for(int i=0; i<(wir_prawy.uk_lok().size()); i++)
        {
            temp = translacja + wir_prawy.translacja() + mac_obrot*wir_prawy.uk_lok()[i];
            //std::cout << temp << std::endl;
            wir_prawy.uk_glob().push_back(temp);
            //std::cout << tablica_glob[i] << std::endl;
        }
        kat_rad = (wir_lewy[4]/180) * (M_PI);
        mac_obrot = Macierz3D(Wektor3D(cos(kat_rad),0,sin(kat_rad)),Wektor3D(0,1,0),Wektor3D(-sin(kat_rad),0,cos(kat_rad)));
        for(int i=0; i<(wir_lewy.uk_lok().size()); i++)
        {
            temp = translacja + wir_lewy.translacja() + mac_obrot*wir_lewy.uk_lok()[i];
            //std::cout << temp << std::endl;
            wir_lewy.uk_glob().push_back(temp);
            //std::cout << tablica_glob[i] << std::endl;
        }
    }
}

void Dron::obrot_kat (int kat)
{
    obrot[Z]+=kat;
}

std::ostream & operator << (std::ostream & strm, Dron & tab)
{
    for(int i=0; i<(tab.tablica.size()); i++)
    {
        if(i%4==0 && i!=0)
        {
            strm << std::endl;
        }
        strm << tab.tablica_glob[i] << std::endl;
        if(strm.fail())
        {
            std::cerr << "Blad zapisu drona do pliku" << std::endl;
            return strm;
        }
    }

    strm << std::endl << std::endl;
    strm << tab(LEWY) << std::endl;
    strm << std::endl << std::endl;
    strm << tab(PRAWY) << std::endl;
    strm << std::endl;

    return strm;
}
