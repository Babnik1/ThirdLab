#pragma once

/// @file
/// 
/// Лог для отладки.
///

#include <iostream>

#ifndef NDEBUG 
    #define DEBUG
#endif

#ifdef DEBUG
    #define LOG( message ) std::cerr << "[DEBUG]: " << __FILE__ << ":" << __LINE__ << " " << message << "\n"
#else
    #define LOG( message )
#endif