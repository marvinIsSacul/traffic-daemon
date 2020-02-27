#ifndef DUMP_H_INCLUDED
#define DUMP_H_INCLUDED



#include <iostream>


/**
 * Dumps an exception. Indicating the exception's line number, source filename, class name and name of the function.
*/

#define ex_dump(e) \
    std::cerr << "EXCEPTION\n" \
              << typeid(*this).name() << "::" << (__func__) << std::endl \
              << __FILE__ << ':' << __LINE__ << std::endl \
              << e.what() << std::endl \
              << std::endl




#endif // DUMP_H_INCLUDED
