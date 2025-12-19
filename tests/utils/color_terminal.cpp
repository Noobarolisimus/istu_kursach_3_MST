#include "color_terminal.hpp"

#if !WIN32
    TermColor::TermColor(const char* str, size_t size) : color(std::string(str, size)) {}
    TermColor::TermColor(std::string str) : color(str) {}

    TermColor operator| (const TermColor& lhs, const TermColor& rhs){
        return TermColor(lhs.color + rhs.color);    
    }

    std::ostream& operator<< (std::ostream& stream, const TermColor& rhs){
        #if COLORTERMINAL
            stream << rhs.color;
        #endif
        return stream;
    }

    std::ostream& operator<< (std::ostream& stream, TermColor& rhs){
        #if COLORTERMINAL
            stream << rhs.color;
        #endif
        return stream;
    }

    TermColor operator"" _TC(const char* str, size_t size){
        return TermColor(str, size);
    }

    #pragma region TERMCOLOR consts
    const TermColor TermColor::FG_DEFAULT = "\x1b[39m"_TC;
    const TermColor TermColor::BG_DEFAULT = "\x1b[49m"_TC;
    const TermColor TermColor::DEFAULT    = TermColor::FG_DEFAULT | TermColor::BG_DEFAULT;
    const TermColor TermColor::FG_BLACK   = "\x1b[30m"_TC;
    const TermColor TermColor::FG_RED     = "\x1b[31m"_TC;
    const TermColor TermColor::FG_GREEN   = "\x1b[32m"_TC;
    const TermColor TermColor::FG_YELLOW  = "\x1b[33m"_TC;
    const TermColor TermColor::FG_BLUE    = "\x1b[34m"_TC;
    const TermColor TermColor::FG_MAGENTA = "\x1b[35m"_TC;
    const TermColor TermColor::FG_CYAN    = "\x1b[36m"_TC;
    const TermColor TermColor::FG_WHITE   = "\x1b[37m"_TC;
    const TermColor TermColor::FG_LBLACK   = TermColor::FG_BLACK;
    const TermColor TermColor::FG_LRED     = "\x1b[38;5;9m"_TC;//TERMCOLOR::FG_RED;
    const TermColor TermColor::FG_LGREEN   = TermColor::FG_GREEN;
    const TermColor TermColor::FG_LYELLOW  = "\x1b[38;5;11m"_TC;//TERMCOLOR::FG_YELLOW;
    const TermColor TermColor::FG_LBLUE    = "\x1b[38;5;6m"_TC;//TERMCOLOR::FG_BLUE;
    const TermColor TermColor::FG_LMAGENTA = TermColor::FG_MAGENTA;
    const TermColor TermColor::FG_LCYAN    = TermColor::FG_CYAN;
    const TermColor TermColor::FG_LWHITE   = TermColor::FG_WHITE;
    const TermColor TermColor::BG_BLACK   = "\x1b[40m"_TC;
    const TermColor TermColor::BG_RED     = "\x1b[41m"_TC;
    const TermColor TermColor::BG_GREEN   = "\x1b[42m"_TC;
    const TermColor TermColor::BG_YELLOW  = "\x1b[43m"_TC;
    const TermColor TermColor::BG_BLUE    = "\x1b[44m"_TC;
    const TermColor TermColor::BG_MAGENTA = "\x1b[45m"_TC;
    const TermColor TermColor::BG_CYAN    = "\x1b[46m"_TC;
    const TermColor TermColor::BG_WHITE   = "\x1b[47m"_TC;
    const TermColor TermColor::BG_LBLACK   = TermColor::BG_BLACK;
    const TermColor TermColor::BG_LRED     = TermColor::BG_RED;
    const TermColor TermColor::BG_LGREEN   = TermColor::BG_GREEN;
    const TermColor TermColor::BG_LYELLOW  = TermColor::BG_YELLOW;
    const TermColor TermColor::BG_LBLUE    = TermColor::BG_BLUE;
    const TermColor TermColor::BG_LMAGENTA = TermColor::BG_MAGENTA;
    const TermColor TermColor::BG_LCYAN    = TermColor::BG_CYAN;
    const TermColor TermColor::BG_LWHITE   = TermColor::BG_WHITE;
    #pragma endregion
#else
    #if COLORTERMINAL
        StaticInit<HANDLE, WinColorTermHandleInit> __hConsole;
        HANDLE WinColorTermHandleInit::operator() (){
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hConsole == INVALID_HANDLE_VALUE)
            {
                std::cout << "There no color terminal support :(" << std::endl;
                std::cout << "Color terminal error: " << GetLastError() << std::endl;
            }
            return hConsole;
        }
    #endif
#endif