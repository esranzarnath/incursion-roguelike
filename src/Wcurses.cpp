/* Wcurses.CPP -- Copyright (c) 2015 Richard Tew
   Derived from WALLEGRO.CPP -- Copyright (c) 1999-2003 Julian Mensch
     Contains the class definition and member functions for the
   cursesTerm class, which is the only part of Incursion which
   contains libtcod-related calls. Includes 
   both the central screen functions, as well as wrapper functions
   for file management.

   Low-Level Read/Write
     void cursesTerm::Update() 
     void cursesTerm::CursorOn()
     void cursesTerm::CursorOff()
     int16 cursesTerm::CursorX()
     int16 cursesTerm::CursorY()
     void cursesTerm::Save()
     void cursesTerm::Restore()
     void cursesTerm::PutChar(Glyph g) 
     void cursesTerm::APutChar(int16 x, int16 y, Glyph g)
     void cursesTerm::PutChar(int16 x, int16 y, Glyph g) W
     void cursesTerm::GotoXY(int16 x, int16 y) 
     void cursesTerm::Clear() 
     void cursesTerm::Color(int16 _attr) 
     void cursesTerm::StopWatch(int16 milli)  
   Input Functions
     int16 cursesTerm::GetCharRaw()
     int16 cursesTerm::GetCharCmd()
     int16 cursesTerm::GetCharCmd(KeyCmdMode mode)
     bool  cursesTerm::CheckEscape()
   General Functions
     void cursesTerm::Initialize()
     void Fatal(const char*fmt,...)
     void Error(const char*fmt,...)
   Scroll Buffer
      void  cursesTerm::SPutChar(int16 x, int16 y,Glyph g);
      //int16 cursesTerm::SGetGlyph(int16 x, int16 y);
      void  cursesTerm::SPutColor(int16 x, int16 y, int16 col);
      int16 cursesTerm::SGetColor(int16 x, int16 y);
      void  cursesTerm::ClearScroll(bool full=false);
      void  cursesTerm::BlitScrollLine(int16 wn, int32 buffline, int32 winline);
   File I/O
     bool cursesTerm::Exists(const char *fn)
     void cursesTerm::Delete(const char *fn)
     bool cursesTerm::Scour(const char *fn, bool Manditory)
     void cursesTerm::OpenRead(const char*fn)
     void cursesTerm::OpenWrite(const char*fn)
     void cursesTerm::OpenUpdate(const char*fn)
     void cursesTerm::Close()
     void cursesTerm::FRead(void *vp,size_t sz)
     void cursesTerm::FWrite(void *vp,size_t sz)
     int32 cursesTerm::Tell()
     void cursesTerm::Seek(int32 pos,int8 rel)
     void cursesTerm::Cut(int32 amt)
    
*/

#define CURSES_TERM
#ifdef CURSES_TERM

#define _CRT_SECURE_NO_WARNINGS

#undef TRACE
#undef MIN
#undef MAX
#undef ERROR
#undef EV_BREAK

#include <direct.h>
#include <stdarg.h>
#include <ctype.h> 
#include <time.h>
#include <malloc.h>
#include <Windows.h>

#ifndef DEBUG
#ifdef USE_CRASHPAD
/* Google Breakpad. */
#include "client/windows/handler/exception_handler.h"
#undef ERROR
#undef MIN
#undef MAX
#undef EV_BREAK
#endif
#endif

#include "Incursion.h"
#undef ERROR
#undef MIN
#undef MAX

#include "curses.h"
//#define TCOD_NOBASETYPES
//#include "libtcod.h"
//#undef ERROR
//#undef MIN
//#undef MAX

//#define SDL_MAIN_HANDLED
//#include "SDL.h"

#define FORE(g) ((g & 0x0F00) / 256)
#define BACK(g) ((g & 0xF000) / (256*16))
#define ATTR(g) ((g & 0xFF00) / 256)
#define CHAR(g) ((g & 0x00FF))

#define MAX_COLORS 16

#define CURSOR_BLINK_MS 300
#define INPUT_IDLE_MS 50

/*

Add to release libs: exception_handler.lib;crash_generation_client.lib;common.lib
Add to release paths: _dependencies\google-breakpad\src\client\windows\Release\lib
Add to release includes: _dependencies\google-breakpad\src

#ifndef DEBUG
#ifdef USE_CRASHPAD
using google_breakpad::ExceptionHandler;
#endif
#endif
*/

/*
xterm has 256 colours, why do we only have 8 at most? huh pdcurses?
*/
int RGBValues[MAX_COLORS] = {
  COLOR_BLACK,  // BLACK
  COLOR_BLUE,  // BLUE
  COLOR_GREEN,  // GREEN
  COLOR_CYAN,  // CYAN
  COLOR_RED,  // RED
  COLOR_MAGENTA,  // PURPLE
  COLOR_WHITE,  // BROWN
  COLOR_WHITE,  // GREY
  COLOR_WHITE,  // SHADOW
  COLOR_BLUE,  // AZURE
  COLOR_WHITE,  // EMERALD
  COLOR_BLUE,  // SKYBLUE
  COLOR_RED,  // "PINK"
  COLOR_MAGENTA,  // MAGENTA
  COLOR_YELLOW,  // YELLOW
  COLOR_WHITE,  // WHITE
  };

int32 RGBValues[MAX_COLORS][3] = {
	{ 0, 0, 0 },  // BLACK
	{ 0, 0, 192 },  // BLUE
	{ 0, 128, 0 },  // GREEN
	{ 0, 128, 128 },  // CYAN
	{ 128, 0, 0 },  // RED
	{ 128, 0, 128 },  // PURPLE
	{ 128, 128, 0 },  // BROWN
	{ 192, 192, 192 },  // GREY
	{ 128, 128, 128 },  // SHADOW
	{ 64, 64, 255 },  // AZURE
	{ 0, 255, 0 },  // EMERALD
	{ 0, 255, 255 },  // SKYBLUE
	{ 255, 0, 0 },  // "PINK"
	{ 255, 0, 255 },  // MAGENTA
	{ 255, 255, 0 },  // YELLOW
	{ 255, 255, 255 },  // WHITE
};


int32 RGBSofter[MAX_COLORS][3] = {
	{ 0, 0, 0 }, // BLACK
	{ 64, 64, 187 }, // BLUE
	{ 0, 128, 0 }, // GREEN
	{ 0, 128, 128 }, // CYAN
	{ 192, 64, 48 }, // RED
	{ 128, 0, 128 }, // PURPLE
	{ 128, 128, 0 }, // BROWN
	{ 192, 192, 192 }, // GREY
	{ 128, 128, 128 }, // SHADOW
	{ 85, 85, 230 }, // AZURE
	{ 0, 230, 0 }, // EMERALD
	{ 0, 230, 230 }, // SKYBLUE
	{ 230, 0, 0 }, // "PINK"
	{ 230, 0, 230 }, // MAGENTA
	{ 230, 230, 0 }, // YELLOW
	{ 230, 230, 230 }, // WHITE
};


class cursesTerm: public TextTerm
  {
    friend void Error(const char*fmt,...);
    friend void Fatal(const char*fmt,...);

    private:
      bool isWindowed, showCursor, cursorPulse;
	  WINDOW *bWindow;
	  WINDOW *bScreen, *bSave, *bScroll, *bCurrent;
      int16 Colors[MAX_COLORS][3];
	  int16 colour_pair_index;
	  int16 colour_pairs[256][2];
      int16 resX, resY, fontX, fontY, ocx, ocy;
      uint32 ticks_blink_last;
      int oldResX, oldResY;
      String debugText;
      /* File I/O Stuff */
      String CurrentDirectory;
      String CurrentFileName; 
      FILE *fp;
	  // Windows specific part of API, if make cross-platform, add Loonix version or whatever..
#ifdef WIN32
	  LARGE_INTEGER time_freq, time_0;
#endif

  public:
	  cursesTerm() {
#ifdef WIN32
		  QueryPerformanceFrequency(&time_freq);
		  QueryPerformanceCounter(&time_0);
#endif
	  }

	  // Mao a curses "color pair" index to a fg and bg colour index.
	  void colour_index_to_pair(int16 index, int16 *fg, int16 *bg);
	  // Map a fg and bg colour index to a curses "color pair" index.
	  int16 colour_pair_to_index(int16 fg, int16 bg);

      /* Low-Level Read/Write */
      virtual void Update();
      virtual void Redraw();
      virtual void CursorOn();
      virtual void CursorOff();
      virtual void BlinkCursor();
      virtual int16 CursorX() { return cx; }
      virtual int16 CursorY() { return cy; }
      virtual void Save();
      virtual void Restore();
      virtual void PutChar(Glyph g);
      virtual void APutChar(int16 x, int16 y, Glyph g);
      virtual void PutChar(int16 x, int16 y, Glyph g);
      virtual Glyph AGetChar(int16 x, int16 y);
      virtual void GotoXY(int16 x, int16 y);
      virtual void Clear();
      virtual void Color(int16 _attr);
      virtual void StopWatch(int16 milli);
      virtual uint32 GetElapsedMilli();
      
      /* Input Functions */
      virtual int16 GetCharRaw();
      virtual int16 GetCharCmd();
      virtual int16 GetCharCmd(KeyCmdMode mode);
      virtual bool CheckEscape();
      virtual void ClearKeyBuff();
      virtual void PrePrompt();
      
      /* General Functions */
      virtual void Initialize();
      virtual void ShutDown();
      virtual void Reset();
      virtual bool hideOption(int16 opt) { return false; }
      virtual void SetDebugText(const char *text);
      virtual void Title();
      
      /* Scroll Buffer */
      virtual void SPutChar(int16 x, int16 y, Glyph g);
      virtual uint16 SGetChar(int16 x, int16 y);
      virtual void SPutColor(int16 x, int16 y, int16 col);
      virtual int16 SGetColor(int16 x, int16 y);
      virtual void SClear();
      virtual void  BlitScrollLine(int16 wn, int32 buffline, int32 winline);
      
      /* System-Independant File I/O */
      virtual const char* SaveSubDir()    { return "save"; } 
      virtual const char* ModuleSubDir()  { return "mod"; } 
      virtual const char* LibrarySubDir() { return "lib"; } 
      virtual const char* LogSubDir()     { return "logs"; } 
      virtual const char* ManualSubDir()  { return "man"; } 
      virtual const char* OptionsSubDir() { return "."; } 
      virtual void ChangeDirectory(const char * c) { 
          CurrentDirectory = (const char*)IncursionDirectory;
          if (CurrentDirectory != c)
              CurrentDirectory += c;
          if (chdir(CurrentDirectory))
              Fatal("Unable to locate directory '%s'.", (const char*)CurrentDirectory);
      }
      virtual bool Exists(const char* fn);
      virtual void Delete(const char* fn);
      virtual void OpenRead(const char* fn);
      virtual void OpenWrite(const char* fn);
      virtual void OpenUpdate(const char* fn);
      virtual void Close();
      virtual void FRead(void*,size_t sz);
      virtual void FWrite(const void*,size_t sz);
      virtual void Seek(int32,int8);
      virtual void Cut(int32);
      virtual bool FirstFile(char * filespec);
      virtual bool NextFile();
      virtual char * MenuListFiles(const char * filespec, 
          uint16 flags, const char *title);
      virtual const char* GetFileName()
        { return CurrentFileName; }      
      virtual int32 Tell();

      void SetIncursionDirectory(const char *s);
};
  
static int16 kbStandard[][3] = {
  { KY_HELP,   SHIFT, '?' },
  { KY_HELP,       0, '/' },
  { KY_COMMA,  SHIFT, '<' },
  { KY_COMMA,      0, ',' },
  { KY_PERIOD, SHIFT, '>' },
  { KY_PERIOD,     0, '.' },
  { KY_COLON,  SHIFT, ':' },
  { KY_COLON,      0, ';' },
  { KY_QUOTE,  SHIFT, '"' },
  { KY_QUOTE,      0, '\'' },
  { KY_MINUS,  SHIFT, '_' },
  { KY_MINUS,      0, '-' },
  { KY_EQUALS, SHIFT, '+' },
  { KY_EQUALS,     0, '=' },
  { 0,              0,  0  }  };
static int16 kbSpanish[][3] = {
  { 0x0C,       SHIFT, '?' },
  { 0x08,       SHIFT, '/' },
  { 0x56,           0, '<' },
  { 0x33,           0, ',' },
  { 0x56,       SHIFT, '>' },
  { 0x34,           0, '.' },
  { 0x34,       SHIFT, ':' },
  { 0x33,       SHIFT, ';' },
  { 0x03,       SHIFT, '"' },
  { 0x0C,           0, '\'' },
  { 0x35,       SHIFT, '_' },
  { 0x35,           0, '-' },
  { 0x1B,           0, '+' },
  { 0x0B,       SHIFT, '=' },
  { 0,              0,  0  }  };
static int16 kbNorwegian[][3] = {
  { 12,         SHIFT, '?' },
  { 95,             0, '/' },
  { 95,         SHIFT, '/' },
  { 45,             0, '<' },
  { 53,             0, ',' },
  { 45,         SHIFT, '>' },
  { 54,             0, '.' },
  { 54,         SHIFT, ':' },
  { 53,         SHIFT, ';' },
  { 3,          SHIFT, '"' },
  { 42,             0, '\'' },
  { 55,         SHIFT, '_' },
  { 55,             0, '-' },
  { 106,            0, '+' },
  { 11,         SHIFT, '=' },
  { 0,              0,  0  }  };
static int16 kbPolish[][3] = {
  { KY_HELP,   SHIFT, '?' },
  { KY_HELP,       0, '/' },
  { KY_COMMA,  SHIFT, '<' },
  { KY_COMMA,      0, ',' },
  { KY_PERIOD, SHIFT, '>' },
  { KY_PERIOD,     0, '.' },
  { KY_COLON,  SHIFT, ':' },
  { KY_COLON,      0, ';' },
  { KY_QUOTE,  SHIFT, '"' },
  { KY_QUOTE,      0, '\'' },
  { KY_MINUS,  SHIFT, '_' },
  { KY_MINUS,      0, '-' },
  { KY_EQUALS, SHIFT, '+' },
  { KY_EQUALS,     0, '=' },
  { 0,              0,  0  }  };

Term *T1;
cursesTerm *AT1;
#ifndef DEBUG
#ifdef USE_CRASHPAD
ExceptionHandler* crashdumpHandler;
#endif
#endif

/*****************************************************************************\
*                                 cursesTerm                                *
*                              main() Function                               *
\*****************************************************************************/

void GetIncursionDirectory(int argc, char *argv[], char *out) {
    String IncursionDirectory;
    if (argc >= 1) {
        const char *str = strrchr(argv[0], '\\');
        if (str == NULL) {
            IncursionDirectory = "";
        } else {
            int16 n = str - argv[0] + 1; /* Copy the separator too. */
            char *tstr = (char*)alloca(n+1);
            tstr[n] = '\0';
            IncursionDirectory = strncpy(tstr,argv[0],n);
        }
    } else
        IncursionDirectory = "";

    if (IncursionDirectory.GetLength() > 0) {
        /* Kludge to cope with directory structure for
           multiple builds on my machine */
        IncursionDirectory = IncursionDirectory.Replace("DebugAllegro\\","");     
        IncursionDirectory = IncursionDirectory.Replace("ReleaseAllegro\\","");
        IncursionDirectory = IncursionDirectory.Replace("Debug\\","");
        IncursionDirectory = IncursionDirectory.Replace("Release\\","");
        IncursionDirectory = IncursionDirectory.Replace("debug/src/","");
        IncursionDirectory = IncursionDirectory.Replace("release/src/","");
    }
    strncpy(out,(const char *)IncursionDirectory,IncursionDirectory.GetLength());
    out[IncursionDirectory.GetLength()] = '\0';
}

int main(int argc, char *argv[]) {
    char executablePath[512];
	GetIncursionDirectory(argc, argv, executablePath);
    /* Google Breakpad is only compiled into Release builds, which get distributed.
     * Debug builds get the option to break out into the debugger, which makes it
     * superfluous in that case. */
#ifndef DEBUG
#ifdef USE_CRASHPAD
    std::wstring wsExecutablePath(strlen(executablePath), 0);
    mbstowcs(&wsExecutablePath[0],executablePath,strlen(executablePath));
    crashdumpHandler = new ExceptionHandler(wsExecutablePath,
                           NULL, /* &filter */
                           NULL /* &callback */,
                           NULL,
                           ExceptionHandler::HANDLER_ALL);
#endif
#endif
    theGame = new Game();
    {
        AT1 = new cursesTerm;
		AT1->SetIncursionDirectory(executablePath);
        T1 = AT1;
        // ww: otherwise we segfault checking options
        T1->SetPlayer(NULL);
        T1->Initialize();
        //T1->Title();
    }
    theGame->StartMenu();
    T1->ShutDown();
    delete theGame;
    return 0;
    /*
    T1->Initialize();
    T1->SetWin(WIN_SCREEN);
    T1->Clear();
    int16 i;
    for (i=0;i!=256;i++)
      T1->PutChar(i%16,i/16,i + WHITE*256);
    T1->GetCharRaw();
    T1->GetCharRaw();
    */
  }

int readkey(int wait) {
	if (wait) {
		// This does something, but also turns input blocking on.
		nocbreak();
		// Undo that something, leaving us with input blocking.
		raw();
	} else {
		// Ensure that input is character mode, not line mode.
		raw();
		// Block waiting for characters the minimum amount of time.
		halfdelay(1);
    }
	return getch();
}

void cursesTerm::colour_index_to_pair(int16 index, int16 *fg, int16 *bg) {
	*bg = colour_pairs[index][0];
	*fg = colour_pairs[index][1];
}

int16 cursesTerm::colour_pair_to_index(int16 fg, int16 bg) {
	int16 index;
	// Find out if we have already allocated the colour pair.  If so return it's index.
	for (index = 0; index < colour_pair_index; index++)
		if (colour_pairs[index][0] == bg && colour_pairs[index][1] == fg)
			return index;

	// Oops, we're out of allocatible pairs.
	if (colour_pair_index >= 256) {
		index = 0;
	} else {
		// Otherwise, add the pair in, and return the new index.
		index = colour_pair_index;
		colour_pairs[index][0] = bg;
		colour_pairs[index][1] = fg;
		colour_pair_index += 1;
	}
	return index;
}



  
/*****************************************************************************\
*                                 cursesTerm                                   *
*                           Low-Level Read/Write                              *
\*****************************************************************************/

void cursesTerm::SetDebugText(const char *text) {
    debugText = text;
}

void cursesTerm::Update() {
    static int update_count = 0;
	char buffer[256];

	wrefresh(bScreen);
	overwrite(bScreen, stdscr);
	overwrite(bScreen, bCurrent);
    if (debugText.GetLength()) {
        update_count++;
		sprintf(buffer, "%d: %s", update_count, (const char *)debugText);
		mvaddstr(0, 0, buffer);
        debugText = "";
    }
	refresh();

    updated = true;
}
  
void cursesTerm::Redraw() {
    Update();
}

void cursesTerm::CursorOn() {
    showCursor = true;
    ticks_blink_last = GetElapsedMilli();
    ocx = ocy = 0;
}

void cursesTerm::CursorOff() {
	chtype c = mvwinch(bCurrent, ocy, ocx);
	mvwaddch(stdscr, ocy, ocx, c);

	showCursor = false;
	cursorPulse = false;
	ocx = ocy = 0;
}
  
void cursesTerm::BlinkCursor() {
    // Sometimes the windows may leave the cursor off the RHS.
    if (cx >= sizeX)
        return;

    cursorPulse = !cursorPulse;
    if (cursorPulse) {
        int16 c = attr & 0x000F;
        if (c == 8) 
          c = 7;
        else if (c < 8)
          c += 8;
        TCOD_console_put_char_ex(NULL,cx,cy,'X',Colors[c], Colors[c]);
        ocy = cy; ocx = cx;
	} else {
		chtype c = mvwinch(bScreen, ocy, ocx);
		mvwaddch(stdscr, ocy, ocx, c);
	}
    /* Won't get an update necessarily otherwise */
	refresh();
}
  
void cursesTerm::Save() { 
    Update();
	overwrite(bScreen, bSave);
}

void cursesTerm::Restore() { 
	overwrite(bSave, bScreen);
    Update();    
}

void cursesTerm::PutChar(Glyph g) {
	chtype c = CHAR(g);
	if (g >> 8) {
		// Use default colours.
	} else {
		uint32 ga = attr * 256;
		int16 fg = FORE(ga), bg = BACK(ga);
		int16 colour_index = colour_pair_to_index(fg, bg);
		c |= COLOR_PAIR(colour_index);
	}
	mvwaddch(bScreen, cy, cx, c);

    cx++;
    updated = false;
}

void cursesTerm::APutChar(int16 x, int16 y, Glyph g) {
    int c = CHAR(g);
    uint32 ga;
    if (g >> 8)
        ga = ATTR(g)*256;
    else
        ga = attr*256;
	int16 fg = FORE(ga), bg = BACK(ga);
	int16 colour_index = colour_pair_to_index(fg, bg);
	c |= COLOR_PAIR(colour_index);
	mvwaddch(bScreen, y, x, c);

	updated = false;
}

void cursesTerm::PutChar(int16 x, int16 y, Glyph g) {
    x += activeWin->Left;
    y += activeWin->Top;
    APutChar(x,y,g);
}

Glyph cursesTerm::AGetChar(int16 x, int16 y) {
	int32 fi = -1, bi = -1;
	int16 fg, bg;
	chtype c = mvwinch(bScreen, y, x);
	Glyph g = c & A_CHARTEXT;
	pair_content(COLOR_PAIR(c), &fg, &bg);
	bi = bg;
	fi = fg;
	if (bi != -1)
		g |= bi * 256 * 16;
	if (fi != -1)
		g |= fi * 256;
    return g;
}

void cursesTerm::GotoXY(int16 x, int16 y) {
    cx = x + activeWin->Left;
    cy = y + activeWin->Top;
}

void cursesTerm::Clear() {
	int i, clear_w, clear_h;
	// Colour should be curses "color pair" 0, the default.
	chtype clear_ch = ' ';

	clear_w = (activeWin->Right + 1) - activeWin->Left;
	clear_h = (activeWin->Bottom + 1) - activeWin->Top;
	for (i = 0; i < clear_h; i++) {
		move(activeWin->Top + i, activeWin->Left);
		vline(clear_ch, clear_w);
	}
	
    cWrap = 0; cx = cy = 0;
    if (activeWin == &Windows[WIN_SCREEN] || activeWin == &Windows[WIN_MESSAGE])
        linepos = linenum = 0;
    updated = false;
}

void cursesTerm::Color(int16 _attr) {
    attr = _attr;
}

void cursesTerm::StopWatch(int16 milli) {    
    switch (p ? p->Opt(OPT_ANIMATION) : 0) {
    case 0: napms(milli); return;
    case 1: napms((milli+3)/4); return; 
    case 2: return;
    }
}

uint32 cursesTerm::GetElapsedMilli() {
#ifdef WIN32
	LARGE_INTEGER time_now, time_result;
	QueryPerformanceCounter(&time_now);
	time_result.QuadPart = time_now.QuadPart - time_0.QuadPart;
	time_result.QuadPart *= 1000; // Milliseconds are 1/1000 of a second.
	time_result.QuadPart /= time_freq.QuadPart;
	return time_result.QuadPart;
#endif
}

/*****************************************************************************\
*                                 cursesTerm                                   *
*                             General Functions                               *
\*****************************************************************************/


void Fatal(const char*fmt,...) {
	va_list argptr; 
	va_start(argptr, fmt);
	if (!T1)
		{ printf(__buffer); exit(1); }
	T1->Clear();
	vsprintf(__buffer, fmt, argptr);
	va_end(argptr);
	sprintf(__buff2, "Fatal Error: %s\nPress [ENTER] to exit...",__buffer);
	((cursesTerm*)T1)->Box(WIN_SCREEN,BOX_NOPAUSE|BOX_NOSAVE,RED,PINK,__buff2);
	T1->Update();
	readkey(1);
#ifdef DEBUG
	BREAKOUT;
#endif
	T1->ShutDown();
	exit(1);
}

void Error(const char*fmt,...) {
	va_list argptr;
	va_start(argptr, fmt); 
	char ch;
	vsprintf(__buffer, fmt, argptr);
	if (!T1)
		{ printf(__buffer); exit(1); }

#ifdef DEBUG
	sprintf(__buff2, "Error: %s\n[B]reak, [E]xit or [C]ontinue?",__buffer);
#else
#ifdef USE_CRASHPAD
	crashdumpHandler->WriteMinidump();
	sprintf(__buff2, "Error: %s\n[E]xit or [C]ontinue?",__buffer);
#endif
#endif
	((cursesTerm*)T1)->Save();
	((cursesTerm*)T1)->Box(WIN_SCREEN,BOX_NOPAUSE|BOX_NOSAVE,RED,PINK,__buff2);
	T1->Update();

	do 
	ch = toupper(readkey(1));
	while (ch != 'C' && ch != 'E' && ch != 'S'    
#ifdef DEBUG
		&& ch != 'B'
#endif
		);
	if (ch == 'E')
		{ T1->ShutDown(); exit(1); }
	if (ch == 'C')
		return;
#ifdef DEBUG
	BREAKOUT;
#endif

	/*
	else if (ch == 'S')
	theGame->PanicSave();
	*/
	((cursesTerm*)T1)->Restore();
	va_end(argptr);
}

void cursesTerm::Reset() {
    int16 i;
	int desiredSizeX = 0, desiredSizeY = 0;

    if (bScreen != NULL) {
        delwin(bScreen);
		delwin(bCurrent);
		delwin(bSave);
    }

RetryFont:
	getmaxyx(bWindow, sizeY, sizeX);

	if (desiredSizeX && desiredSizeX != sizeX && desiredSizeY && desiredSizeY != sizeY) {
		endwin();

		printf("Error: Minimum terminal size is 80x48 characters (actual size is %dx%d).", sizeX, sizeY);
		exit(1);
	}

    if (sizeX < 80 || sizeY < 48) {
		desiredSizeX = max(sizeX, 80);
		desiredSizeY = max(sizeY, 48);
		resize_term(desiredSizeY, desiredSizeX);
	    goto RetryFont;
    }
    
	PDC_set_title("Incursion: Halls of the Goblin King");

    InitWindows();

	if (can_change_color()) { // CURSES API
		if (theGame->Opt(OPT_SOFT_PALETTE))
			memcpy(&Colors, &RGBSofter, sizeof(Colors[0][0]) * MAX_COLORS * 3);
		else
			memcpy(&Colors, &RGBValues, sizeof(Colors[0][0]) * MAX_COLORS * 3);

		for (i = 0; i < MAX_COLORS; i++) {
			// The curses RGB range is 0-1000, whereas our local range is 0-255.
			init_color(i, (Colors[i][0] * 1000) / 256, (Colors[i][1] * 1000) / 256, (Colors[i][2] * 1000) / 256);
		}

		colour_pair_index = 0;
		colour_pairs[colour_pair_index][0] = 0;
		colour_pairs[colour_pair_index++][1] = 15;
	}

	bScreen = newwin(sizeY, sizeX, 0, 0);
	bCurrent = newwin(sizeY, sizeX, 0, 0);
	bSave = newwin(sizeY, sizeX, 0, 0);

    if (bScroll == NULL) {
		bScroll = newwin(MAX_SCROLL_LINES, SCROLL_WIDTH, 0, 0);
    }

    if (Mode == MO_PLAY) { 
        ShowStatus();
        ShowTraits();
        ShowMap();
    } else if (Mode == MO_INV) {
        ShowStatus();
        ShowTraits();
        InvShowSlots();
    }

    Update();
    ocx = ocy = 0;
}    


void cursesTerm::Initialize() {
    p = NULL; m = NULL; isHelp = false;
    ActionsSinceLastAutoSave = 0;
    cx = cy = 0;
    showCursor = false;
    cursorPulse = false;
    OptionCount = 0;
    OffscreenC = 0;
    QueuedChar = 0;
    ControlKeys = 0;
    WViewListCount = 0; 
    OptionsShown = false;
    XOff = YOff = 0;
    s1 = s2 = 0;      
    MsgHistory[0] = 0;
    Mode = MO_SPLASH;
    isWindowed = true;

	bWindow = initscr();
	start_color();
	raw();
	noecho();
	keypad(bWindow, TRUE);
	bScreen = bCurrent = bSave = bScroll = NULL;

	Reset();
    
    InitWindows();   

    SetWin(WIN_SCREEN);
    Color(14);
}

void cursesTerm::ShutDown() {
	// Free allocated resources in pdcurses, and restore the tty.
	endwin();
}

void cursesTerm::SetIncursionDirectory(const char *s) {
    IncursionDirectory = (const char *)s;
}

/* Draw the intro screen header and display the start of game libtcod-specific
 * stuff in the footer area below it. */
void cursesTerm::Title() {
}

/*****************************************************************************\
*                                 cursesTerm                                  *
*                               Scroll Buffer                                *
\*****************************************************************************/

void  cursesTerm::SPutChar(int16 x, int16 y, Glyph g) {
#ifdef TODOTODO
    TCOD_console_put_char_ex(bScroll,x,y,CHAR(g),Colors[FORE(g)], Colors[BACK(g)]);
#else
	mvaddch(y, x, CHAR(g));
#endif
}

Glyph cursesTerm::SGetChar(int16 x, int16 y) {
	return mvwinch(bScroll, y, x) & A_CHARTEXT;
}

void cursesTerm::SPutColor(int16 x, int16 y, int16 col) {
#ifdef TODOTODO
    uint32 ga = col*256;
    uint8 c = TCOD_console_get_char(bScroll,x,y);
    TCOD_console_put_char_ex(bScroll,x,y,c,Colors[FORE(ga)],Colors[BACK(ga)]);
#endif
}

int16 cursesTerm::SGetColor(int16 x, int16 y) {
    int16 i = 0;
	int32 fi = -1, bi = -1;
	chtype c = mvwinch(bScroll, y, x);
	int16 pn = PAIR_NUMBER(c);
	int16 fg, bg;
	pair_content(pn, &fg, &bg);
	fi = fg;
	bi = bg;
	if (bi != -1)
		i |= bi << 4;
	if (fi != -1)
		i |= fi;
	return i;
}

void cursesTerm::SClear() {
	werase(bScroll);
}

void  cursesTerm::BlitScrollLine(int16 wn, int32 buffline, int32 winline) {
	copywin(bScroll, bScreen, buffline, 0, winline + Windows[wn].Top, Windows[wn].Left, winline + Windows[wn].Top, Windows[wn].Left + min(SCROLL_WIDTH, WinSizeX()), FALSE);
}

/*****************************************************************************\
*                                 cursesTerm                                   *
*                              Input Functions                                *
\*****************************************************************************/

int16 cursesTerm::GetCharRaw()
  { return GetCharCmd(KY_CMD_RAW); }

int16 cursesTerm::GetCharCmd()
  { return GetCharCmd(KY_CMD_NORMAL_MODE); }

int16 cursesTerm::GetCharCmd(KeyCmdMode mode) {
    KeySetItem * keyset = theGame->Opt(OPT_ROGUELIKE) ? RoguelikeKeySet : StandardKeySet;
    int16 keyset_start = 0, keyset_delta = 0, keyset_last;
    int16 i, ox, oy, ch;
    TextWin *wn;
	uint32 ticks_last;

    ActionsSinceLastAutoSave++;
    if (p) p->GameTimeInfo.keystrokes++;

    if (QueuedChar) {
        ch = QueuedChar;
        QueuedChar = 0;
        return ch;
    }
      
    for (keyset_last = 0; keyset[keyset_last].cmd != KY_CMD_LAST; keyset_last++)
        ;
    keyset_last--;
      
    switch (mode) {
    case KY_CMD_NORMAL_MODE: 
        keyset_start = keyset_last ; 
        keyset_delta = -1 ; 
        break; 
    case KY_CMD_TARGET_MODE: 
        keyset_start = 0; 
        keyset_delta = +1; 
        break; 
    case KY_CMD_ARROW_MODE:
        keyset_start = 0; 
        keyset_delta = +1;
        break; 
    } 
      
    ox = cx; oy = cy; wn = activeWin;
    if (theGame->InPlay()) {
        //ShowViewList(); 
        if (p->UpdateMap && Mode == MO_PLAY) {
            RefreshMap();
        }
        if (p->statiChanged) {
            ShowTraits();
            ShowStatus();
        }
    }

    Update();
    activeWin = wn;
    cx = ox; cy = oy;
    ticks_last = GetElapsedMilli();

    for(;;) {
		uint32 ticks0 = GetElapsedMilli(), ticks1;
        int key = readkey(0);
		unsigned long key_modifiers = PDC_get_key_modifiers();

        if (Mode == MO_PLAY && p->UpdateMap)
            RefreshMap();

        if (Mode == MO_PLAY || Mode == MO_INV)
            if (p->statiChanged) {
                ShowTraits();
                ShowStatus();
            }
          
        ticks1 = GetElapsedMilli();
        if (showCursor && ticks1 > ticks_blink_last + CURSOR_BLINK_MS) {
            BlinkCursor();
            ticks_blink_last = ticks1;
        }

        if (key == ERR) {
            uint32 ticks_next = ticks1 + INPUT_IDLE_MS;
            if (showCursor && ticks_next > ticks_blink_last + CURSOR_BLINK_MS)
                ticks_next = ticks_blink_last + CURSOR_BLINK_MS;
            if (ticks_next > ticks1)
                napms(ticks_next - ticks1);

            /* Allow clearing the message window ONLY if we aren't
               repeating keys, i.e., holding down arrow to run. In
               other words, if no keys are down, or keys are down 
               that weren't down earlier, it is safe to clear. */
            ClearMsgOK = true;
            continue;
        }
        
        if (theGame->Opt(OPT_CLEAR_EVERY_TURN) && ClearMsgOK && mode == KY_CMD_NORMAL_MODE && GetMode() == MO_PLAY) {
            ox = cx; oy = cy; wn = activeWin;
            linenum = linepos = 0;
            SetWin(WIN_MESSAGE);
            Clear();
            activeWin = wn;
            cx = ox; cy = oy;
            ClearMsgOK = false;
        }
        
        ticks_blink_last = GetElapsedMilli();

        ControlKeys = 0;
		if (key_modifiers & PDC_KEY_MODIFIER_CONTROL)
            ControlKeys |= CONTROL;
		if (key_modifiers & PDC_KEY_MODIFIER_SHIFT)
            ControlKeys |= SHIFT;
		if (key_modifiers & PDC_KEY_MODIFIER_ALT)
            ControlKeys |= ALT; 

        if (key == 10)
            if (ControlKeys & ALT) {
                isWindowed = !isWindowed;
                Reset();
                Update();
                return KY_REDRAW;
            }

		//if ((ch == 'c' || key == KEY_BREAK) && (ControlKeys & CONTROL))
        //    goto CtrlBreak;

#ifdef HACK_KEY_DEBUGGING
        {
            static bool debug_keys = false;
            if ((ch == 'd') && (ControlKeys & CONTROL)) {
                debug_keys = !debug_keys;
                if (debug_keys)
                    T1->SetDebugText("Key debugging started.");
                else
                    T1->SetDebugText("Key debugging stopped.");
            } else if (debug_keys) {
                char formatted[256];
                if (tcodKey.vk != TCODK_CONTROL) {
                    int result = sprintf(formatted, "Key code: %d, Char-number: %d Char-letter: '%c' CtrlKeys: %x", tcodKey.vk, tcodKey.c, tcodKey.c, ControlKeys);
                    if (result != -1)
                        T1->SetDebugText(formatted);
                    else
                        T1->SetDebugText("Failed to format key debugging info.");
                }
            }
        }
#endif

        switch (key) {
        case 0x0A:             ch = KY_ENTER;     break; 
		case PADENTER:	       ch = KY_ENTER;     break;
        case 0x1B:             ch = KY_ESC;       break;
        case 0x08:             ch = KY_BACKSPACE; break;
        case 0x09:             ch = KY_TAB;       break;
		case KEY_RIGHT:        ch = KY_RIGHT;     break;
		case KEY_UP:           ch = KY_UP;        break;
		case KEY_LEFT:         ch = KY_LEFT;      break;
		case KEY_DOWN:         ch = KY_DOWN;      break;
		case KEY_NPAGE:        ch = KY_PGUP;      break;
		case KEY_PPAGE:        ch = KY_PGDN;      break;
		case KEY_HOME:         ch = KY_HOME;      break;
		case KEY_END:          ch = KY_END;       break;

		/* Users with numpads should turn numlock off. */
        case KEY_A1:           ch = KY_HOME;      break;
		case KEY_A2:           ch = KY_UP;        break;
		case KEY_A3:           ch = KY_PGUP;      break;
		case KEY_B1:           ch = KY_LEFT;      break;
		case KEY_B3:           ch = KY_RIGHT;     break;
		case KEY_C1:           ch = KY_END;       break;
		case KEY_C2:           ch = KY_DOWN;      break;
		case KEY_C3:           ch = KY_PGDN;      break;
		case PADSTOP:          ch = KY_PERIOD;    break;

        case KEY_F(1):         ch = KY_CMD_MACRO1; break;
		case KEY_F(2):         ch = KY_CMD_MACRO2; break;
		case KEY_F(3):         ch = KY_CMD_MACRO3; break;
		case KEY_F(4):         ch = KY_CMD_MACRO4; break;
		case KEY_F(5):         ch = KY_CMD_MACRO5; break;
		case KEY_F(6):         ch = KY_CMD_MACRO6; break;
		case KEY_F(7):         ch = KY_CMD_MACRO7; break;
		case KEY_F(8):         ch = KY_CMD_MACRO8; break;
		case KEY_F(9):         ch = KY_CMD_MACRO9; break;
		case KEY_F(10):        ch = KY_CMD_MACRO10; break;
		case KEY_F(11):        ch = KY_CMD_MACRO11; break;
		case KEY_F(12):        ch = KY_CMD_MACRO12; break;
        default:
            continue;
        }

        if (mode == KY_CMD_RAW)
            return ch;

        for (i = keyset_start;i>=0 && keyset[i].cmd != KY_CMD_LAST;i+=keyset_delta) { 
            if (keyset[i].raw_key != toupper(ch)) 
                continue;
            if (keyset[i].raw_key_flags != -1)
                if (keyset[i].raw_key_flags != (ControlKeys & (CONTROL|SHIFT|ALT)))
                    continue;
            if (mode == KY_CMD_ARROW_MODE && (keyset[i].cmd < KY_CMD_FIRST_ARROW || keyset[i].cmd > KY_CMD_LAST_ARROW))
                continue; 
            return keyset[i].cmd;
        }
        return ch;
    }
}

/*
Swallow all keypresses, until we get either of two desired outcomes.
1) No key is pressed.
2) Escape is pressed.
*/
bool cursesTerm::CheckEscape() {
    int key;
    while (1) {
        key = readkey(0);
        if (key == ERR)
            return false;
        if (key == 0x1B)
            return true;
    }
}
  
void cursesTerm::ClearKeyBuff() {
	while (readkey(0) != ERR)
		;
}
  
void cursesTerm::PrePrompt() {
	while (readkey(0) != ERR)
		;
}
  
/*****************************************************************************\
*                                 cursesTerm                                   *
*                               File I/O Code                                 *
\*****************************************************************************/

bool cursesTerm::Exists(const char *fn) {
    return (TCOD_sys_file_exists(fn) != 0);
}

void cursesTerm::Delete(const char *fn)
  {
    if (!TCOD_sys_delete_file(fn))
        Error("Can't delete file \"%s\".",fn);
  }

void cursesTerm::OpenRead(const char*fn)
  {
    fp = fopen(fn,"rb");
    if (!fp)
      throw ENOFILE;
    CurrentFileName = fn;
  }

void cursesTerm::OpenWrite(const char*fn)
  {
    fp = fopen(fn,"wb+");
    if (!fp)
      throw ENODIR;
    CurrentFileName = fn;
  }

void cursesTerm::OpenUpdate(const char*fn)
  {
    fp = fopen(fn,"rb+");
    if (!fp)
      fp = fopen(fn,"wb+");
    if (!fp)
      throw ENODIR;
    CurrentFileName = fn;
  }

void cursesTerm::Close()
  {
    if (fp) {
      fclose(fp);
      fp = NULL;
      }
  }
  
void cursesTerm::FRead(void *vp,size_t sz)
  {
    if (fread(vp,1,sz,fp)!=sz)
      throw EREADERR;
  }

void cursesTerm::FWrite(const void *vp,size_t sz)
  {
    if (!fp)
      return;
    if (fwrite(vp,1,sz,fp)!=sz)
      throw EWRIERR;
  }

int32 cursesTerm::Tell()
  { return ftell(fp); }  

void cursesTerm::Seek(int32 pos,int8 rel)
  { if(fseek(fp,pos,rel))
      throw ECORRUPT; }

void cursesTerm::Cut(int32 amt)
  {
    /* Fix this later! */
    int32 start, end;
    void *block;
    start = ftell(fp);
    fseek(fp,0,SEEK_END);
    end = ftell(fp);
    fseek(fp,start+amt,SEEK_SET);
    block = malloc(end - (start+amt));
    fread(block,end - (start+amt),1,fp);
    fseek(fp,start,SEEK_SET);
    fwrite(block,end - (start+amt),1,fp);
  }

char * cursesTerm::MenuListFiles(const char * filespec, uint16 flags, const char * title) {
    char * file;
    TCOD_list_t l = TCOD_sys_get_directory_content(CurrentDirectory, filespec);
    for (int *iterator = (int *)TCOD_list_begin(l); iterator != (int *)TCOD_list_end(l); iterator++) {
        const char *fileName = (const char *)(*iterator);
        char *str = strdup(fileName);
        /* TODO: Check if 'str' can leak */
        LOption(str,(int32)str);
    }
    TCOD_list_clear_and_delete(l);
    file = (char*)LMenu(flags,title);
    if (file == (char*)(-1))
      return NULL;
    return file;
}

bool cursesTerm::FirstFile(char * filespec) {
    alf_it = NULL;
    if (alf != NULL && TCOD_list_is_empty(alf))
        TCOD_list_clear_and_delete(alf);
    alf = TCOD_sys_get_directory_content(CurrentDirectory, filespec);
    if (TCOD_list_size(alf) == 0)
        return false;
    alf_it = (int32 *)TCOD_list_begin(alf);
    try {
        OpenRead((const char *)*alf_it);
    } catch (int) {
        fp = NULL;
        return false;
    }
    ASSERT(fp);
    return true;
}

bool cursesTerm::NextFile() {
Retry:
    alf_it += 1;
    if (alf_it == (int32 *)TCOD_list_end(alf)) {
        TCOD_list_clear_and_delete(alf);
        return false;
    }
    try {
        OpenRead((const char *)*alf_it);
    } catch (int) {
        fp = NULL;
        goto Retry;
    }
    ASSERT(fp);
    return true;
}

#endif

