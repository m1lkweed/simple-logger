#define LOGGER_IMPLEMENTATION
#include "logger.h"

int main(){
	log_level = WARN;
	#ifndef __GNUC__
	pthread_mutex_init(&_logger_mutex, NULL);
	#endif //__GNUC__
	logfile = stdout;
	logger(ALL,   "Message!");
	logger(TRACE, "Message!");
	logger(SPAM,  "Message!");
	logger(DEBUG, "Message!");
	logger(EXTRA, "Message!");
	logger(INFO,  "Message!");
	logger(ATTN,  "Message!");
	logger(WARN,  "Message!");
	logger(ERROR, "Message!");
	logger(CRIT,  "Message!");
	logger(FATAL, "Message!");
	logger(NONE,  "Message!"); //Should never print
	int x = 0xdeadbeef;
	loggerf(ALL,   "Value of x is %d", x);
	loggerf(TRACE, "Value of x is %d", x);
	loggerf(SPAM,  "Value of x is %d", x);
	loggerf(DEBUG, "Value of x is %d", x);
	loggerf(EXTRA, "Value of x is %d", x);
	loggerf(INFO,  "Value of x is %d", x);
	loggerf(ATTN,  "Value of x is %d", x);
	loggerf(WARN,  "Value of x is %d", x);
	loggerf(ERROR, "Value of x is %d", x);
	loggerf(CRIT,  "Value of x is %d", x);
	loggerf(FATAL, "Value of x is %d", x);
	loggerf(NONE,  "Value of x is %d", x); //Should never print
}
