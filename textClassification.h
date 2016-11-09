#ifndef _TEXT_H_
#define _TEXT_H_

using namespace std;

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_YELLOW 	= 33,
        FG_BLUE     = 36,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}

// declare inside each function
// Color::Modifier red(Color::FG_RED);
// Color::Modifier def(Color::FG_DEFAULT);
// Color::Modifier blue(Color::FG_BLUE);
// Color::Modifier green(Color::FG_GREEN);
// Color::Modifier yellow(Color::FG_YELLOW);


#endif