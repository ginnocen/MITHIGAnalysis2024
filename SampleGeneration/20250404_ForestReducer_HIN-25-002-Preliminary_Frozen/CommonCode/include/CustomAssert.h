#ifndef CUSTOMASSERT_H
#define CUSTOMASSERT_H

// Taken from Eugene Magdalits
// https://stackoverflow.com/questions/3692954/add-custom-messages-in-assert

#include <iostream>

#define Assert(Expr, Msg) __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)

void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
    if(!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}

#endif

