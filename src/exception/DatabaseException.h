#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <exception>

namespace exception{

class DatabaseException : public std::exception
{
private:
    const char *_what;

    public:
        DatabaseException(const char* what) : _what(what) {
        }

        virtual ~DatabaseException(){
        }

        virtual const char *what() const noexcept { return this->_what; }
};

}

#endif // DATABASEEXCEPTION_H
