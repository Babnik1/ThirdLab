#include <iostream>

#define DEBUG

#ifdef DEBUG
    #define LOG( message ) std::cerr << "[DEBUG]: " << __FILE__ << ":" << __LINE__ << " " << message << "\n"
#else
    #define LOG( message )
#endif