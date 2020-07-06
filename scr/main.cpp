#include "AsciiGraphics.h"
/*
Space to spin
UP arrow Key to add bet
DOWN arrow Key to subtract bet
*/



#define txtsz {8,5}

const COORD boxLocs[9] = { {53,6} , {64,6} , {75,6} , {53,12} , {64,12} , {75,12} , {53,18} , {64,18}, {75,18} };
bool spin = false;
enum
{
    appleSt = 0, barSt,
    diamondSt  , sevenSt
};

short boxSt[9] = {0};

std::string arrowLeft =
"------>"
"------>"
"------>"
;


std::string arrowRight =
"<------"
"<------"
"<------"
;
std::string hud =
"____________________________________"
"||||||||||||||||||||||||||||||||||||"
"|+|        |+|        |+|        |+|"
"|+|        |+|        |+|        |+|"
"|+|        |+|        |+|        |+|"
"|+|        |+|        |+|        |+|"
"|+|        |+|        |+|        |+|"
"::::::::::::::::::::::::::::::::::::"
"|>|        [-]        [-]        |<|"
"|>|        [-]        [-]        |<|"
"|>|        [-]        [-]        |<|"
"|>|        [-]        [-]        |<|"
"|>|        [-]        [-]        |<|"
"::::::::::::::::::::::::::::::::::::"
"|+|        |+|        |+|        |+|"
"|+|        |+|        |+|        |+|"
"|+|        |+|        |+|        |+|"
"|+|        |+|        |+|        |+|"
"|+|        |+|        |+|        |+|"
"||||||||||||||||||||||||||||||||||||"
"---------------|spin|---------------"
"BET:                                "
"CASH:                               "
"____________________________________";
AsciiArt apple({8,5});
AsciiArt diamond({8,5});
AsciiArt seven({8,5});
AsciiArt bar({8,5});
std::default_random_engine eng;
unsigned int UserMoney = 100;
unsigned int Bet = 0;
char betTxt[5] = " ";
char cashTxt[5] = " ";
static bool isShuffling = false;
bool GatherCoints = false;
void SpinAnim(txt::TextDisplay*p)
{

    if(spin)
    {
        isShuffling = true;
        p->fps.StartStaticTime();
        srand((unsigned)p->fps.tickframes);
    }

    if(isShuffling)
    {
        spin = false;


        p->fps.updateStaticTime();
        if(p->fps.StatictimePoint.count()<= 1)
        {
            eng.seed(p->fps.tickframes);
            for(short i  =0; i < 9; i++)
            {
                boxSt[i] = eng()%4;
            }

            for(short i  =0; i < 9; i++)
            {
                switch(boxSt[i])
                {
                case 0:
                    p->DisplayAsiiDrawings(apple.graphics,boxLocs[i],txtsz);
                    break;
                case 1:
                    p->DisplayAsiiDrawings(bar.graphics,boxLocs[i],txtsz);
                    break;
                case 2:
                    p->DisplayAsiiDrawings(diamond.graphics,boxLocs[i],txtsz);
                    break;
                case 3:
                    p->DisplayAsiiDrawings(seven.graphics,boxLocs[i],txtsz);
                    break;
                default:

                    break;
                }
            }
        }
        else
        {
            isShuffling = false;
            GatherCoints = true;
        }
    }
    else
    {
        for(short i  =0; i < 9; i++)
        {
            switch(boxSt[i])
            {
            case 0:
                p->DisplayAsiiDrawings(apple.graphics,boxLocs[i],txtsz);
                break;
            case 1:
                p->DisplayAsiiDrawings(bar.graphics,boxLocs[i],txtsz);
                break;
            case 2:
                p->DisplayAsiiDrawings(diamond.graphics,boxLocs[i],txtsz);
                break;
            case 3:
                p->DisplayAsiiDrawings(seven.graphics,boxLocs[i],txtsz);
                break;
            default:

                break;
            }
        }
        if(GatherCoints)
        {
            bool won = false;
            if(boxSt[3] == boxSt[4] && boxSt[3] == boxSt[5])
            {

                won = true;

            }
            if(won)
            {
                UserMoney += Bet * 10;
            }
            else
                UserMoney -= Bet;

            if(UserMoney <= 0)
                Bet = 0;

            GatherCoints = false;
        }
    }

}

void UserInput()
{

if(GetAsyncKeyState(VK_SPACE)!=0)
{
    if(Bet > 0)
        spin = true;

}

if(UserMoney > 0)
{
    if(GetAsyncKeyState(VK_UP)%4 != 0)
        if(Bet >= 0)
        {
            Bet++;

        }
}
if(Bet > 0)
    if(GetAsyncKeyState(VK_DOWN)%4 != 0)
        if(UserMoney >= 0)
        {
          Bet--;

        }




}


//this is the drawCallback function
//this called once every frame
void DrawCall(txt::TextDisplay*t)
{

if(t->fps.timePoint.count() >= 1)
{
    IntToCharPTR(&t->Title[16],t->fps.frames);
    SetConsoleTitle(t->Title);


}


t->DisplayAsiiDrawings(hud,{50,4},{36,24});
t->DisplayAsiiDrawings(arrowLeft,{43,13},{7,3});
t->DisplayAsiiDrawings(arrowRight,{86,13},{7,3});

IntToCharPTR(betTxt,Bet);
IntToCharPTR(cashTxt,UserMoney);

t->DisplayText(betTxt,{55,25});
t->DisplayText(cashTxt,{55,26});
SpinAnim(t);
UserInput();


if(GetAsyncKeyState(VK_ESCAPE)!=0)
t->IsRunning = false;


}
//Load the assets/string data from the file
void LoadAssets()
{
    apple.LoadDataFromFile("assets/newapple.txt");
    diamond.LoadDataFromFile("assets/newdiamond.txt");
    bar.LoadDataFromFile("assets/newbar.txt");
    seven.LoadDataFromFile("assets/newseven.txt");
}

int main()
{
    LoadAssets();
    txt::TextDisplay tDip(WR_TextMode_Med,{150,50},DrawCall);
    strcpy_s(tDip.Title,"SlotMachine FPS:");
    SetConsoleTitle(tDip.Title);
    tDip.MainLoop();
return 0;
}

