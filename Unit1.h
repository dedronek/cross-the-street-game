//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <MPlayer.hpp>
#include <cstdlib>
#include <string>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TTimer *Timer1;
        TLabel *Label1;
        TShape *win;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *score;
        TPanel *Panel1;
        TButton *Button1;
        TColorBox *ColorBox1;
        TLabel *Label5;
        TLabel *Label6;
        TImage *Image1;
        TLabel *Label4;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *level;
        TScrollBar *ScrollBar1;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *l_samochodow;
        TLabel *p_slownie;
        TLabel *Label11;
        TScrollBar *ScrollBar2;
        TLabel *p_samochodow;
        TTimer *Timer2;
        TPanel *Panel2;
        TLabel *Label12;
        TButton *Button2;
        TLabel *Label13;
        TLabel *score_final;
        TLabel *Label14;
        TLabel *level_final;
        TLabel *Label15;
        TLabel *final_cars;
        TLabel *Label16;
        TLabel *speed_final;
        TButton *Button3;
        TMediaPlayer *s_crush;
        TMediaPlayer *s_candy;
        TMediaPlayer *s_win;
        TLabel *petla;
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall ScrollBar1Change(TObject *Sender);
        void __fastcall ScrollBar2Change(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        void random(int n);
        void check_crush(int n);
        void organize_cars(int n);
        void check_win();
        void spawn_candy();
        void check_candy();
        void set_player();
        void create_cars();
        void delete_cars();
        void rand_color();
        void set_width_cars();
        void delete_candies();

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
