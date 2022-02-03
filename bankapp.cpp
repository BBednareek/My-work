#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

class konto_info
{
    private: 
        char numer_konta[20];
        char imie[10];
        char nazwisko[10];
        float saldo;
    
    public:
        void read_data();
        void show_data();
        void save_data();
        void read_reg();
        void search_reg();
        void edit_reg();
        void del_reg();
};

void konto_info::read_data()
{
    cout << "Podaj numer konta: ";  cin >> numer_konta;
    cout << "Podaj imiê: ";         cin >> imie;
    cout << "Podaj nazwisko: ";     cin >> nazwisko;
    cout << "Podaj stan konta: ";   cin >> saldo;
}

void konto_info::show_data()
{
    cout << "Numer konta: "        << numer_konta << endl;
    cout << "Imiê: "               << imie << endl;
    cout << "Nazwisko: "           << nazwisko << endl;
    cout << "Stan konta:   "       << saldo << endl;
    cout << "--------------------------------" << endl;
}

void konto_info::save_data()
{
    ofstream plik_out;
    plik_out.open("info.bank", ios::binary|ios::app);
    read_data();
    plik_out.write(reinterpret_cast<char*>(this), sizeof(*this));
    plik_out.close(); 
}

void konto_info::read_reg()
{
    ifstream plik_in;
    plik_in.open("info.bank", ios::binary);

    if(!plik_in){cout<<"Plik nie istnieje!"<<endl; return;}

    cout << endl << "***Konto: Informacje***" << endl;

    while(!plik_in.eof())
    {
        if(plik_in.read(reinterpret_cast<char*>(this), sizeof(*this))>0){show_data();}
    }
    plik_in.close();
}

void konto_info::search_reg()
{
    int ilosc_wpisow, nr_wpisu;
    ifstream plik_in;
    plik_in.open("info.bank", ios::binary);
    if(!plik_in){cout<<"Plik nie istnieje!"<<endl; return;}
    plik_in.seekg(0, ios::end);
    ilosc_wpisow = plik_in.tellg()/sizeof(*this);

    cout << endl << "Jest " << ilosc_wpisow << " wpisów w pliku" << endl;
    cout << "Podaj numer wpisu, który chcesz wyszukaæ: "; cin >> nr_wpisu;

    plik_in.seekg((nr_wpisu-1)*sizeof(*this));
    plik_in.read(reinterpret_cast<char*>(this),sizeof(*this));
    show_data();
}

void konto_info::edit_reg()
{
    int ilosc_wpisow, nr_wpisu;
    fstream plik_io;
    plik_io.open("info.bank", ios::in|ios::binary);

    if(!plik_io){cout<<"Plik nie istnieje!"<<endl; return;}
    plik_io.seekg(0, ios::end);
    ilosc_wpisow = plik_io.tellg()/sizeof(*this);

    cout << endl << "Jest " << ilosc_wpisow << " wpisów w pliku" << endl;
    cout << endl << "Podaj numer wpisu, który chcesz wyszukaæ: "; cin >> nr_wpisu;

    plik_io.seekg((nr_wpisu-1)*sizeof(*this));
    plik_io.read(reinterpret_cast<char*>(this), sizeof(*this));
    show_data();
    plik_io.close();
    plik_io.open("info.bank", ios::in|ios::out|ios::binary);

    cout << endl << "Podaj wpis, który chcesz zmodyfikowaæ: "; cin >> nr_wpisu;
    read_data();
    plik_io.write(reinterpret_cast<char*>(this),sizeof(*this));
}

void konto_info::del_reg()
{
    int ilosc_wpisow, nr_wpisu;
    fstream plik_in, tempfile;

    plik_in.open("info.bank", ios::binary);
    if(!plik_in){cout<<"Plik nie istnieje!"<<endl; return;}
    plik_in.seekg(0, ios::end);
    ilosc_wpisow = plik_in.tellg()/sizeof(*this);

    cout << endl << "Jest " << ilosc_wpisow << " wpisów w pliku" << endl;
    cout << "Podaj numer wpisu, który chcesz usun¹æ: "; cin >> nr_wpisu;

    tempfile.open("tempfile.bank", ios::out|ios::binary);
    plik_in.seekg(0);
    for(int i=0; i<ilosc_wpisow; i++)
    {
        plik_in.read(reinterpret_cast<char*>(this),sizeof(*this));
        if(i==(nr_wpisu-1))
            continue;
        tempfile.write(reinterpret_cast<char*>(this),sizeof(*this));
    }
    plik_in.close();
    tempfile.close();
    remove("info.bank");
    rename("tempfile.bank", "info.bank");
}

int main()
{
    setlocale(LC_ALL,"");

    konto_info info;
    int wybor;

    cout << "***Informacje o koncie***" << endl;

    while(true){
        cout << "Wybierz jedn¹ z opcji poni¿ej";
        cout << "\n\t1-->Dodaj nowe konto";
        cout << "\n\t2-->Poka¿ wszystkie konta";
        cout << "\n\t3-->Szukaj informacji";
        cout << "\n\t4-->Zmieñ informacjê";
        cout << "\n\t5-->Usuñ informacjê";
        cout << "\n\t6-->Wyjœcie";
        cout << "\nTwój wybór:"; cin >> wybor;
    
        switch(wybor)
        {
            case 1: info.save_data();   break;
            case 2: info.read_reg(); 	break;
            case 3: info.search_reg();  break;
            case 4: info.edit_reg();  	break;
            case 5: info.del_reg();   	break;
            case 6:     exit(0);      	break;
            default:    cout<"\nPodaj prawid³ow¹ opcjê";
        }
    }
    system("pause");
    cout << endl;
    return 0;
}

