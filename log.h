#include <iostream>

#define DEBUG

#ifdef DEBUG
    #define LOG( message ) std::cerr << "[" << "DEBUG:" << __FILE__ << ":" << __LINE__ << "]: " << message
#else
    #define LOG( message )
#endif