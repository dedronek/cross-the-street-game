//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <windows.h>
#include <cstdlib>
#include <string>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
int k[255]; //kierunek samochodow (1 lub 0)
int p[255]; //wspolczynnik szerokosci (1 do 5)
int color[255]; //wspolczynnik koloru (1 do 5)
int p_wys,p_szer; //podstawowa szerokosc i wysokosc do cukierkow
int runda = 1; //zmienna przechowujaca aktualna runde
AnsiString str; //zmienna pomocnicza do sciezki pliku
TImage *candy[255]; //deklaracja tablicy obiektow candy uzywanej jako punkt
TShape *player; //deklaracja obiektu player uzywanego jako kulka gracza
TImage *cars[255]; //deklaracja tablicy obiektow cars uzywanej jako samochody
int n_cars; // liczba samochodow

//---------------------------------------------------------------------------
void TForm1::check_win()
{
        n_cars = StrToInt(l_samochodow->Caption); //odczytywanie z wartosci z labelu o naziw "l_samochodow"
        if (player->Left == win->Left && player->Top == win->Top) //jesli top gracza i left gracza sa takie same jak kulki gornej
        {
                Label3->Caption = "WIN"; //wartosc "WIN"
                delete_candies(); //usuwanie cukierkow
                s_win->Play(); // odglos przejscia poziomu ze sciezki "win.mp3"
                level->Caption = StrToInt(level->Caption)+1; //wyswietlany numer rundy++
                runda++;
                rand_color(); //losowanie wspolczynnikow do left samochodu oraz jego koloru
                delete player;
                delete_cars(); //usuniecie samochodow
                spawn_candy(); //stworzenie punktow
                set_player();  //przesuniecie gracza na punkt startowy
                create_cars(); //stworzenie samochodow
                set_width_cars(); //ustalenie ich szerokosic
                organize_cars(n_cars); //wyliczenie parametru top kazdego samochodu
                random(n_cars); //obsluga poruszania sie samochodow
                Label3->Caption = "123"; //zmiana wartosci (label przekazujacy wartosci, niewidoczny podczas dzialania aplikacji)
                Label3->Update();

        }
}

void TForm1::organize_cars(int n)
{
        int wys = Form1->ClientHeight-100; //pobranie wysokosci formularza do zmiennej
        double odl = wys/n; //stala szarokosc

        for(int i = 0;i<=n-1;i++) // petla odmierzajaca wysokosci, pierwszy samochod zawsze top=50, kazdy nastepny top+stala
        {
                if (i == 0)
                {
                        cars[i]->Top = 50;
                }
                else
                {
                        cars[i]->Top =cars[i-1]->Top +odl;
                }
        }
}

void TForm1::spawn_candy()
{

        for (int i = 1;i<=runda;i++)
        {
        candy[i] = new TImage(Form1); //inicjalizacja zmiennej tablicowej
        p_wys = (rand()%90); //podstawowa wysokosc
        p_szer = (rand()%114); //podstawowa szerokosc
        candy[i]->Top = (p_wys*5)+60; // przypisanie top okreslonemu punktowi tak aby nie wyszedl za okno
        candy[i]->Left = p_szer*5;// przypisanie left tak aby punkt nie wyszedl za okno
        candy[i]->Parent = Form1; //przypisanie rodzica
        candy[i]->Width = 30;
        candy[i]->Height = 30;
        candy[i]->Picture->LoadFromFile("coin.jpg"); //przypisanie obrazka
        candy[i]->Transparent = true; //zmiana parametru przezroczystosci
        }

}

void TForm1::delete_candies()
{
       for (int i = 1;i<=runda;i++) //usuwanie wszystkich utworzonych candy w petli
        {
         delete candy[i];
        }
}

void TForm1::check_candy()
{
        if(Label3->Caption != "WIN") //jesli ten label nie jest "WIN"
        {
                for (int i = 1; i<=runda;i++)
                {
                        if(candy[i]->Top == player->Top && candy[i]->Left == player->Left && candy[i]->Visible == true)
                        { //jesli top i left punktu i gracze sa identyczne i punkt jest widoczny
                                score->Caption = StrToInt(score->Caption)+1;//dodaj punkt
                                candy[i]->Visible = false; //ukryj widocznosc punktu
                                s_candy->Play(); //odtworz dzwiek zebrania cukierka ("candy.mp3")
                        }
                }
        }


}
void TForm1::check_crush(int n)
{
        for (int i = 0;i<=n-1;i++)
        {
        if((cars[i]->Left == player->Left || cars[i]->Left + cars[i]->Width == player->Left || player->Left + player->Width == cars[i]->Left) && (player->Top < cars[i]->Top + cars[i]->Height && player->Top > cars[i]->Top - player->Height ))
               { //skomplikowany warunek sprawdzajacy nachodzenie na siebie kulki gracza i samochodu wykonywany w co okreslony interwal
                Label3->Caption = "LOSE"; //zmiana labelu przekazujacego informacje na "LOSE"
                score_final->Caption = score->Caption; //przepisanie punktow na panel
                level_final->Caption = level->Caption; //przepisanie pozioimu na panel
                final_cars->Caption = ScrollBar1->Position; //przepisanie liczby samochodow na panel
                speed_final->Caption = p_samochodow->Caption; //przepisanie szybkosci na panel
                Panel2->Visible = true; //wyswietlenie panelu
                Panel2->Left = 160;


                }
        }


}
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
    Form1->DoubleBuffered = true;  //wlasciwosc pomagajaca przy wielokrotnym odswiezaniu formularza, unikanie migania podczas odswiezen
    p_samochodow->Caption = FloatToStrF(5/(double)ScrollBar2->Position*1000,ffGeneral, 4, 2)+" px/s";//zapisanie w sposob przystepny dla uzytkownika szybkosci samochodow
}
//---------------------------------------------------------------------------

void TForm1::random(int n)
{

      for (int i = 0;i<=n-1;i++) //obsluga wyjscia za ekran z lewej lub prawej strony
      {
                if (cars[i]->Left+cars[i]->Width < 0)
                        cars[i]->Left = cars[i]->Width +Form1->ClientWidth; //jesli wyjdzie za lewo ustaw left na taki aby byl znowu po prawej ( dla poruszajacych sie w lewo)
                if (cars[i]->Left > Form1->ClientWidth + cars[i]->Width)
                        cars[i]->Left = 0 - cars[i]->Width;  //jesli wyjdzie za prawo ustaw left na taki aby byl znowu po lewej (dla poruszajacych sie w prawo)
      }

      for (int i = 0;i<=n-1;i++)
      {
                if (k[i] % 2 == 0)//wspolczynnik kierunku losowany w create_cars();
                {
                        cars[i]->Left = cars[i]->Left - 5; //jesli 0 to porusza sie w lewo - zwiekszamy left;
                        str = "l"+IntToStr(color[i])+".bmp"; //utworzenie zmiennej pomocniczej zawierajacej sciezke do pliku z samochodem (l bo w lewa strone potem zmienna pseudolosowa do koloru wybierana w rand_color() i na koncu rozszerzenie)
                        cars[i]->Picture->LoadFromFile(str); //wlasciwosc picture jako plik o sciezce z powyzszej zmiennej pomocniczej
                }
                else
                {
                        cars[i]->Left = cars[i]->Left + 5;//jesli 1 to porusza sie w prawo - zmniejszamy left
                        str = "p"+IntToStr(color[i])+".bmp"; //to samo co wyzej tylko p gdyz samochod porusza sie w prawo
                        cars[i]->Picture->LoadFromFile(str); //analogicznie do poprzedniej sytuacji
                }
      }


}

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
        //to timer ktorego szybkosc ustawiamy na poczatku jako szybkosc samochodow
        n_cars = StrToInt(Label1->Caption); //n_cars jako liczba samochodow
        Label3->Refresh();
        if (Label3->Caption != "LOSE") //dopoki label3 rozny od "LOSE"
        {
        random(n_cars); //ruszaj samochodami
        petla->Caption = 0; //zmienna pomocnicza do ustalania dzwieku zderzenia
        petla->Update();
        }
        else
        {
                if (StrToInt(petla->Caption) == 0)
                {
                        s_crush->Play();
                        s_crush->Wait;
                        petla->Caption = StrToInt(petla->Caption) +1;
                }
                //caly powyzszy warunke stworzony po to aby dzwiek po zderzeniu odtwarzal sie tylko raz
        }



}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
n_cars = StrToInt(Label1->Caption);  //n_cars jako liczba samochodow
check_crush(n_cars); //z kazdym nacisnieciem przycisku sprawdzamy czy aby na pewno nie ma zderzenia
if(Label3->Caption != "LOSE") //dopoki label3 rozny od "LOSE"
{
        if (Key == VK_LEFT)
        {
                if (player->Left != 0)//ograniczenie poruszania sie w lewo
                {
                   player->Left = player->Left - 5; //jesli nacisniety lewa strzalka to wtedy 5px w lewo wartosc left
                }
        }
        if (Key == VK_RIGHT)
        {
                if (player->Left != Form1->ClientWidth - player->Width) //ogranicznie poruszania sie w prawo
                {
                   player->Left = player->Left + 5; //jesli nacisniety prawa strzalka to wtedy 5px w prawo wartosc left
                }
        }
        if (Key == VK_UP)
        {
                if (player->Top != 0) //ogranicznie ruchu w gore
                {
                   player->Top = player->Top - 5;//strzalka w gore nacisnieta to top-=5
                }
        }
        if (Key == VK_DOWN)
        {
                if (player->Top != Form1->ClientHeight - player->Height)//ograniczenie ruchu w dol
                {
                player->Top = player->Top + 5;//strzalka w dol nacisnieta to top+=5
                }

        }

}

}
//---------------------------------------------------------------------------
void TForm1::set_player()
{
        player = new TShape(Form1); //utworzenie obiektu player
        player->Parent = Form1;   ///rodzic
        player->Shape = stCircle;
        player->Width = 30;
        player->Height = 30;
        player->Left = (Form1->ClientWidth/2);// na srodku w poziomie
        player->Top = (Form1->ClientHeight-player->Height-10);//na samym dole
        player->Brush->Color = ColorBox1->Selected; //zmiana koloru na ten wybrany na starcie, domyslnie czarny
}

void TForm1::create_cars()
{
        n_cars = StrToInt(l_samochodow->Caption); //n_cars jako liczba samochodow
           for (int i = 0; i<=n_cars-1; i++)
        {
                k[i] = (rand()%2); //przypisywanie do tablicy wartosci 0 lub 1 odpowiadajacych kierunkowi jazdy, tablica uzywana w random(int n)
                cars[i] = new TImage(Form1); //inicjalizacja obiektu w zmiennej tablicowej
                cars[i]->Parent = Form1; //rodzic aby sie wyswietlalo, bez rodzice elementy sie nie wyswietlaja
                cars[i]->Height = 30;
                cars[i]->Width = 100;
        }
}

void TForm1::delete_cars()
{
        n_cars = StrToInt(l_samochodow->Caption);  //n_cars jako liczba samochodow
           for (int i = 0; i<=n_cars-1; i++)
        {
                delete cars[i];  //usuwanie wszystkich samochodow
        }
}

void TForm1::rand_color()
{
        n_cars = StrToInt(l_samochodow->Caption); //n_cars jako liczba samochodow
                for (int i=0; i<=n_cars-1;i++)
        {
                p[i] = (rand()%5)+1; //losowanie wspolczynnika szerokosci uzywanego w set_width_cars()
                color[i] = (rand()%5)+1;//losowanie wspolczynnika koloru uzywanego w random(int n)
        }
}

void TForm1::set_width_cars()
{
     for (int i=0;i<=n_cars-1;i++)
                cars[i]->Left = p[i]*100; //ustalanie parametru left kazdego z samochodow
}
void __fastcall TForm1::Button1Click(TObject *Sender)
{
        //rozpoczecie pierwszej gry
        srand( time( NULL ) ); //zainicjowanie losowania
        spawn_candy(); //stworzenie punktow
        n_cars = StrToInt(l_samochodow->Caption); //n_cars jako liczba samochow
        Label1->Caption = n_cars; //Label1 jako liczba samochodow

        create_cars(); //tworzenie samochodow
        set_player(); //umiejscowanie gracza na starcie

        win->Left = (Form1->ClientWidth/2); //okreslanie polozenia punktu wygranej
        win->Top = 10;

        rand_color();//losowanie wspolczynnikow do szerokosci i koloru samochodow

        set_width_cars(); //rozmieszczenie samochodow w poziomie

        organize_cars(n_cars); //rozmieszczenie samochodow w pionie

        level->Caption = runda; //wyswietlenie numeru rundy

        Timer1->Interval = ScrollBar2->Position; //ustawienie interwalu Timer'a Timer1 wedlug scrollbara w menu glownym
        Timer1->Enabled = true; //wlaczenie timerow
        Timer2->Enabled = true;
        Panel1->Visible = false; //wylaczenie widocznosci menu glownego
        player->Brush->Color = ColorBox1->Selected; //kolor gracza jako ten wybrany w menu glownym
        Form1->Update();
        Label3->Caption = "123"; //Label3 odpowiada za stan gry, jesli "LOSE" gra sie zatrzymuje, jesli "WIN" nastepny poziom. Kazda rozna wartosc od tych oznacza trwajaca gre
        Label3->Update();

}

void __fastcall TForm1::Button2Click(TObject *Sender)
{
        //przycisk "One more time" rozpoczynajacy gre na nowo
        Label3->Caption = "123"; //analogicznie do wyzej
        Label3->Update();
        delete_cars();   //usuniecie samochodow
        delete_candies(); //usuniecie cukierkow
        delete player; //usuniecie obiektu gracza
        score->Caption = "0"; // zerowanie punktow
        runda = 1; //runda jako 1
        level->Caption = "1"; //wyswietlana runda jako 1
        Panel2->Visible = false; //panel2 z wyborem czy gramy jeszcze raz czy glowne menu znika
        rand_color(); //losujemy wspolczynniki kolorowe i szerokosciowe
        spawn_candy(); //tworzenie cukierkow
        set_player(); //tworzenie kulki gracza na punkt startowy
        create_cars(); //stworzenie samochodow
        set_width_cars(); //umiejscowienie samochodow w poziomie
        organize_cars(n_cars); //umiejscowienie samochodow w pionie
        random(n_cars); //zainicjalizowanie poruszania sie samochodow

}
//---------------------------------------------------------------------------





void __fastcall TForm1::ScrollBar1Change(TObject *Sender)
{
         l_samochodow->Caption = ScrollBar1->Position; //liczba samochodow jako pozycja ScrollBar1

         //zmiana wyswietlania poziomu trudnosci wedlug liczby samochodow

         if(ScrollBar1->Position >= 1 && ScrollBar1->Position <= 2)
                p_slownie->Caption = "The easiest ;)";
         if(ScrollBar1->Position > 2 && ScrollBar1->Position <= 7)
                p_slownie->Caption = "Easy";
         if(ScrollBar1->Position > 7 && ScrollBar1->Position <= 20)
                p_slownie->Caption = "Hard";
         if(ScrollBar1->Position > 20)
                p_slownie->Caption = "HaRdCoRe";
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ScrollBar2Change(TObject *Sender)
{
        p_samochodow->Caption = FloatToStrF(5/(double)ScrollBar2->Position*1000,ffGeneral, 4, 2)+" px/s"; //zmiana szybkosci wyswietlanej w postaci ***px/s
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------



void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
      n_cars = StrToInt(Label1->Caption); //n_cars jako liczba samochodow
        check_crush(n_cars); //sprawdzanie czy zderzenie
        check_win(); //czy wygrana
        check_candy();  //czy zebranie cukierka
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
 delete_candies(); //usuwanie cukierków
 player->Top = 1000; //top kulki gracza jako 1000
 Panel2->Visible = false; //ukrycie panelu "Game Over"
 Panel1->Visible = true; //pokazanie menu glownego
 score->Caption = "0"; //punkty na zero
 runda = 1; //runda na 1
 level->Caption = "1"; //runda wyswietlana na 1
 delete player; //usuwanie kulki gracza
 delete_cars(); //usuwanie samochodow
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------



