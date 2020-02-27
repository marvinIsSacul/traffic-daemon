#ifndef PCAPEXCEPTION_H_INCLUDED
#define PCAPEXCEPTION_H_INCLUDED

#include <exception>


namespace exception{

class PCapException : public std::exception
{
private:
    const char *_what;

    public:
        PCapException(const char* what) : _what(what) {
        }

        virtual ~PCapException(){
        }

        virtual const char *what() const noexcept { return this->_what; }
};

}


#endif // PCAPEXCEPTION_H_INCLUDED
