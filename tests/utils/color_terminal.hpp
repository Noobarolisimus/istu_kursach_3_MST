#pragma once
#include <iostream>

template<typename T>
struct __InitClassF{
    T operator() (){
        return T{};
    }
};

template <typename T, typename InitClassF = __InitClassF<T>>
class StaticInit{
public:
    T& Get(){
        static T o = InitClassF()();
        return o;
    }
};

// COLOR TERMINAL
#if WIN32
    enum TermColor{
        BG_BLACK = 0, FG_BLACK = 0,
        BG_BLUE = 0x10, FG_BLUE = 0x1,
        BG_GREEN = 0x20, FG_GREEN = 0x2,
        BG_CYAN = 0x30, FG_CYAN = 0x3,
        BG_RED = 0x40, FG_RED = 0x4,
        BG_MAGENTA = 0x50, FG_MAGENTA = 0x5,
        BG_YELLOW = 0x60, FG_YELLOW = 0x6,
        BG_WHITE = 0x70, FG_WHITE = 0x7,
        BG_GRAY = 0x80, FG_GRAY = 0x8,
        BG_LCYAN = 0x90, FG_LCYAN = 0x9,
        BG_LGREEN = 0xA0, FG_LGREEN = 0xA,
        BG_LBLUE = 0xB0, FG_LBLUE = 0xB,
        BG_LRED = 0xC0, FG_LRED = 0xC,
        BG_LMAGENTA = 0xD0, FG_LMAGENTA = 0xD,
        BG_LYELLOW = 0xE0, FG_LYELLOW = 0xE,
        BG_LWHITE = 0xF0, FG_LWHITE = 0xF,
        BG_DEFAULT = BG_BLACK, FG_DEFAULT = FG_WHITE,
        DEFAULT = BG_DEFAULT | FG_DEFAULT
    };
#else
    class TermColor{
        std::string color;
    public:
        TermColor() = default;
        TermColor(const char* str, size_t size);
        TermColor(std::string str);

        friend TermColor operator| (const TermColor& lhs, const TermColor& rhs);

        friend std::ostream& operator<< (std::ostream& stream, const TermColor& rhs);

        friend std::ostream& operator<< (std::ostream& stream, TermColor& rhs);

        #pragma region consts
        static const TermColor FG_DEFAULT;
        static const TermColor BG_DEFAULT;
        static const TermColor    DEFAULT;
        
        static const TermColor FG_BLACK  ;
        static const TermColor FG_RED    ;
        static const TermColor FG_GREEN  ;
        static const TermColor FG_YELLOW ;
        static const TermColor FG_BLUE   ;
        static const TermColor FG_MAGENTA;
        static const TermColor FG_CYAN   ;
        static const TermColor FG_WHITE  ;

        static const TermColor FG_LBLACK  ;
        static const TermColor FG_LRED    ;
        static const TermColor FG_LGREEN  ;
        static const TermColor FG_LYELLOW ;
        static const TermColor FG_LBLUE   ;
        static const TermColor FG_LMAGENTA;
        static const TermColor FG_LCYAN   ;
        static const TermColor FG_LWHITE  ;
        

        static const TermColor BG_BLACK  ;
        static const TermColor BG_RED    ;
        static const TermColor BG_GREEN  ;
        static const TermColor BG_YELLOW ;
        static const TermColor BG_BLUE   ;
        static const TermColor BG_MAGENTA;
        static const TermColor BG_CYAN   ;
        static const TermColor BG_WHITE  ;

        static const TermColor BG_LBLACK  ;
        static const TermColor BG_LRED    ;
        static const TermColor BG_LGREEN  ;
        static const TermColor BG_LYELLOW ;
        static const TermColor BG_LBLUE   ;
        static const TermColor BG_LMAGENTA;
        static const TermColor BG_LCYAN   ;
        static const TermColor BG_LWHITE  ;
        #pragma endregion
    };

    TermColor operator"" _TC(const char* str, size_t size);
#endif
#ifdef COLORTERMINAL
    #ifdef WIN32
        #include <Windows.h>
        struct WinColorTermHandleInit{
            HANDLE operator() ();
        };
        extern StaticInit<HANDLE, WinColorTermHandleInit> __hConsole;
        #define SETCOLOR(color) SetConsoleTextAttribute(__hConsole.Get(), color);
    #else
        // Для линукса нет светлых цветов
        // UPD1: пиздёж сверху, https://habr.com/ru/company/macloud/blog/558316/


        #define SETCOLOR(color) std::cout << color;
    #endif
#else
    #define SETCOLOR(color)
#endif
