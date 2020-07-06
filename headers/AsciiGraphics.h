#ifndef ASCII_GRAP_H
#define ASCII_GRAP_H
/**

Author:Lloyd Jay A. Edradan
Project:AschiiGraphics
Description:
	For displaying simple aschii graphics in the console

Created: April 29 2020
	

------------
May 3 2020
update 1.0.1
	-added some overloaded functions so that std::string and char * are now used without problems
------------
May 5 2020
update 1.0.2
	-fix some IDE compatibility issues
	-fix problem in Visual Studio throwing some exceptions due to illegal access in a null address
	-added some time structure (UtilTimer)
------------
*/
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
//Currently CharConverter is inefficient
#include "CharConverter.h"


//this preprocessors checks if the compiler is GNU which is used in CodeBlocks

//Unicode Checker
#if defined(UNICODE) || defined(_UNICODE)
#undef UNICODE
#undef _UNICODE
#endif



#ifdef __GNUC__
#define _WIN32_WINNT 0x0502
#include <windows.h>
//0x0601
#define wcscpy_s wcscpy
#define strcpy_s strcpy
typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT FontFamily;
    UINT FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;
/*
In Codeblocks v17.x
Some windows function are not define
*/


//this code below define those function that are not define in codeblocks
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput,BOOL bMaximumWindow,PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
BOOL WINAPI GetCurrentConsoleFontEx(HANDLE hConsoleOutput,BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif
#ifndef __GNUC__
#include <Windows.h>
#endif // !__GNUC__








//this is some macros for Window Resolution and the size of fontsize of text
//WR_High defines very small text
#define WR_High 0x001
#define WR_Med 0x002
#define WR_Low 0x003
//You can use this if you want more detailed aschii graphics


//textmode:define normal fontsize
#define WR_TextMode_Small 0x004
#define WR_TextMode_Med 0x005



#define CHR_BLOCK 254
#define CHR_WhiteShade 219
#define CHR_HalfShade 220
#define CHR_HoriShade 221


typedef struct _Points
{
 short int X;
 short int Y;
}Points;
typedef struct _LinePoints
{
    Points p1;
    Points p2;
}LPoints;

//planing to delete this struct in future updates
typedef struct _BoxObjects
{
    Points BoxPos;
    Points BoxSz;
    char * chrinfo = nullptr;
    bool LoadData(const std::string& filename,const Points& bsz,const Points& pos)
    {
        BoxSz = bsz;
        BoxPos = pos;
        chrinfo = new char[BoxSz.X * BoxSz.Y];
        std::ifstream file(filename);
        if(file.is_open())
        {
            for(short y = 0; y < BoxSz.Y; y++)
            {
                for(short x = 0;x < BoxSz.X; x++)
                {
                    file >> chrinfo[x + y * BoxSz.Y];
                }
            }
        }
        else
        {
            UnLoadData();
            return false;
        }

    }

    void UnLoadData()
    {
        delete[] chrinfo;
        chrinfo = nullptr;
    }
}BoxObj;



//You can use this struct if your ascii data/Strings are in a file
typedef struct _AsciiArt
{
    std::string graphics;
    COORD sz;
    _AsciiArt(const COORD& asSz)
    {
        sz = asSz;
    }

	//this function load the data from the file
    bool LoadDataFromFile(const char * filename)
    {
		std::string temp;
        std::ifstream file(filename);
        std::stringstream streamer;
        if(file.is_open())
        {
            while(std::getline(file,temp))
            {
                streamer << temp << '\n';
            }
            graphics = streamer.str();
        }
        else
            return false;


        return true;
    }



}AsciiArt;


Points operator-(const Points &p1,const Points &p2)
{
    Points result;
    result.X = p1.X - p2.X;
    result.Y = p1.Y - p2.Y;
    return result;
}




namespace txt
{



    namespace util
    {

		
		typedef struct _UtilTimer
		{

			std::chrono::high_resolution_clock::time_point initTime;
			std::chrono::high_resolution_clock::time_point deltaTime;
			std::chrono::duration<float> timeDuration;

			_UtilTimer()
			{
				initTime = std::chrono::high_resolution_clock::now();
				deltaTime = std::chrono::high_resolution_clock::now();
				timeDuration = deltaTime - initTime;
			}

			void UpdateDTime()
			{
				deltaTime = std::chrono::high_resolution_clock::now();
				timeDuration = deltaTime - initTime;
			}

			bool IsTimesUp(const unsigned int& Seconds)
			{
				if (timeDuration.count() >= Seconds)
				{
					initTime = std::chrono::high_resolution_clock::now();
					return true;
				}

				else
					return false;

			}

		}UtilTimer;

		//this struct will be re-written in future updates
        struct FrameCounter
        {
            std::chrono::high_resolution_clock::time_point initTime;
            std::chrono::high_resolution_clock::time_point deltaTime;

            std::chrono::high_resolution_clock::time_point StaticinitTime;
			
            std::chrono::duration<float> timePoint;
            std::chrono::duration<float> StatictimePoint;
            int tickframes = 0;
            int frames = 0;
            FrameCounter()
            {
                initTime = std::chrono::high_resolution_clock::now();
                deltaTime = std::chrono::high_resolution_clock::now();
            }
            void StartStaticTime()
            {
                StaticinitTime = std::chrono::high_resolution_clock::now();
            }
            void updateStaticTime()
            {
                auto deltaStatic = std::chrono::high_resolution_clock::now();
                StatictimePoint = deltaStatic - StaticinitTime;
            }

            void updateD()
            {
                deltaTime = std::chrono::high_resolution_clock::now();
                timePoint = deltaTime - initTime;
                tickframes++;
            }
            void updateI()
            {
                if(timePoint.count() >= 1)
                {
                    frames = tickframes;
                    tickframes = 0;
                    initTime = std::chrono::high_resolution_clock::now();

                }
            }

        };
		//round off the floating point
		//ex 10.5 = 10 or 7.3 = 7
        int RoundOff(const float &value)
        {
            short int v = (value - (int)value)*10;
            if(v > 5)
            {
                return (int)value+1;
            }
            else
                return value;

        }
    }






	







class TextDisplay
{
private:
HANDLE hdle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,CONSOLE_TEXTMODE_BUFFER,nullptr);
void (*CallBackFunc)(TextDisplay*) = nullptr;
public:
char * scrbuffer = nullptr;
COORD scrsz;
char clrmode = ' ';
bool IsRunning = true;
txt::util::FrameCounter fps;

char Title[50] = "TextDisplay";
public:
    TextDisplay(const int& Res,const COORD& scrbuffsz,void(*clback)(TextDisplay*))
    {
        COORD fontSz;

        switch(Res)
        {
        case WR_High:
            fontSz = {4,6};
            break;
        case WR_Med:
            fontSz = {8,8};
            break;
        case WR_Low:
            fontSz = {5,12};
            break;
        case WR_TextMode_Small:
            fontSz = {7,12};
            break;
        case WR_TextMode_Med:
            fontSz = {8,12};
            break;
        default:
            fontSz = {4,6};
            break;


        }


        SetConsoleActiveScreenBuffer(hdle);
        CallBackFunc = clback;
        CONSOLE_FONT_INFOEX CFIEX;
        GetCurrentConsoleFontEx(hdle,FALSE,&CFIEX);
        wcscpy_s(CFIEX.FaceName, L"Raster Fonts");
        CFIEX.cbSize = sizeof(CFIEX);
        CFIEX.dwFontSize = fontSz;
        CFIEX.FontFamily = FF_DONTCARE;
        CFIEX.FontWeight = FW_NORMAL;
        SetConsoleScreenBufferSize(hdle,scrbuffsz);
        SetCurrentConsoleFontEx(hdle,FALSE,&CFIEX);
        MoveWindow(GetConsoleWindow(),0,0,1400, 1400,TRUE);
        CONSOLE_SCREEN_BUFFER_INFO CSBI;
        GetConsoleScreenBufferInfo(hdle,&CSBI);
        scrbuffer = new char[CSBI.dwSize.X * CSBI.dwSize.Y];
        scrsz.Y = CSBI.dwSize.Y;
        scrsz.X = CSBI.dwSize.X;
        memset(scrbuffer,' ',CSBI.dwSize.X * CSBI.dwSize.Y *sizeof(CHAR));
        CONSOLE_CURSOR_INFO CCI;
        GetConsoleCursorInfo(hdle,&CCI);
        CCI.bVisible = FALSE;
        SetConsoleCursorInfo(hdle,&CCI);
        SetConsoleTitle(Title);


    }

    ~TextDisplay()
    {
        delete[] this->scrbuffer;

    }
    void MainLoop()
    {
		DWORD dwBytesWritten;
        while(IsRunning)
        {

            fps.updateD();
            memset(scrbuffer,clrmode,scrsz.X * scrsz.Y*(sizeof(char)));
            CallBackFunc(this);
            WriteConsoleOutputCharacter(hdle,scrbuffer,scrsz.X * scrsz.Y * sizeof(char),{0,0},&dwBytesWritten);
            fps.updateI();
        }

    }

	//DDA line drawing algorith
    void CreateLines_DDA(const LPoints &lpoints , const char &cChar)
    {
            short Steps = 0;
            Points def = lpoints.p2 - lpoints.p1;
            float incrementX = lpoints.p1.X, incrementY = lpoints.p1.Y;

            if(abs(def.X) > abs(def.Y))
                Steps = abs(def.X);
            else
                Steps = abs(def.Y);


            for(short i =0; i < Steps; i++)
            {

                incrementX += (float)def.X/Steps;
                incrementY += (float)def.Y/Steps;
                scrbuffer[util::RoundOff(incrementX)+util::RoundOff(incrementY)*scrsz.X] = cChar;
            }

    }

	//dont use this
    void DisplayBoxObjects(const BoxObj& bobj)
    {
        short i = 0;
        for(short y = bobj.BoxPos.Y; y < bobj.BoxPos.Y + bobj.BoxSz.Y; y++)
        {
            for(short x =bobj.BoxPos.X; x < bobj.BoxPos.X + bobj.BoxSz.X; x++)
            {
                scrbuffer[x + y * scrsz.X]= bobj.chrinfo[i];
                i++;
            }
        }
    }

    void DisplayText(const char*chtxt,const COORD& pos)
    {
        memcpy(&scrbuffer[pos.X + pos.Y * scrsz.X],chtxt,strlen(chtxt));
    }

	//overload
    void DisplayText(const std::string& chtxt,const COORD& pos )
    {
        memcpy(&scrbuffer[pos.X + pos.Y * scrsz.X],&chtxt,chtxt.size());
    }


    void DisplayAsiiDrawings(const std::string& drawings, const COORD& pos, const COORD& txtsz)
    {

        for(short y = 0; y < txtsz.Y; y++)
        {
            memcpy(&scrbuffer[pos.X + ((y+pos.Y) * scrsz.X)],&drawings[0 + y * txtsz.X],txtsz.X);
        }

    }
    //overload
    void DisplayAsiiDrawings(const char * drawings, const COORD& pos, const COORD& txtsz)
    {

        for(short y = 0; y < txtsz.Y; y++)
        {
            memcpy(&scrbuffer[pos.X + ((y+pos.Y) * scrsz.X)],&drawings[0 + y * txtsz.X],txtsz.X);
        }

    }



};





}
#endif // ASCII_GRAP_H
