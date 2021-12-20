#ifndef debug_h
#define debug_h

//#define DEBUG_FLAG

#ifdef DEBUG_FLAG
#define debug(something) std::cout << something << std::endl;
#else
#define debug(nothing) 0;
#endif

#endif