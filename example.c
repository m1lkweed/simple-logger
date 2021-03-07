#include "logger.h"

int main(){
	log_level = ALL;
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
}
