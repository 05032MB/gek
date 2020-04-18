#ifndef GEK_EXCEPT_
#define GEK_EXCEPT_

#include <string.h>
#include <sstream>

struct except
{
    std::string what;

    except(std::string w) : what(w) {};
    
    template<typename ... T>
    except(T const& ... t)
    {
        std::stringstream ss;
        (ss << ... << t);
        what = ss.str();
    }

    std::string operator()(void) const
    {
        return what;
    }

};

struct recoverableExcept : public except
{
    using except::except;
};

struct failExcept : public except
{
    using except::except;
};

#endif