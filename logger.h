// Basic message logger in C.
// (c)m1lkweed 2021 GPLv3+
// Works on all ANSI-compliant terminals
//
// logfile should be fopen()-ed before logging or set to stdout/stderr
// Valid log_level values are: [ALL, TRACE, SPAM, DEBUG, EXTRA, INFO,
// ATTN, WARN, ERROR, CRIT, FATAL, NONE] in ascending order of importance
// These values are also valid for logger()
// logger() returns -1 if no logging occurs, 0 if logging does occur

#ifndef LOG_H_
#define LOG_H_

#ifndef LOGGER_ALWAYS_COLOR
#define LOGGER_ALWAYS_COLOR 0
#endif

#include <time.h>
#if defined(_WIN32)
#   define localtime_r(T,Tm) (localtime_s(Tm,T) ? NULL : Tm)
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#ifdef _REENTRANT
#include <pthread.h>
extern __thread pthread_mutex_t _logger_mutex;
extern _Atomic bool _logger_mutex_init_flag;
#endif //_REENTRANT

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#define write _write
#else
#include <unistd.h>
#endif

#include "termcolors.h"

enum log_levels{
	ALL,
	TRACE,
	SPAM,
	DEBUG,
	EXTRA,
	INFO,
	ATTN,
	WARN,
	ERROR,
	CRIT,
	FATAL,
	NONE
};

static const char *log_colors[] = {
	ANSI_BOLD ANSI_GREY,
	ANSI_CYAN,
	ANSI_BOLD ANSI_CYAN,
	ANSI_GREEN,
	ANSI_BLUE,
	ANSI_WHITE,
	ANSI_MAGENTA,
	ANSI_YELLOW,
	ANSI_RED,
	ANSI_BOLD ANSI_RED,
	ANSI_BOLD ANSI_WHITE ANSI_BG_RED,
	""
};

static const char *log_text_colors[] = {
	ANSI_RESET,
	ANSI_RESET,
	ANSI_RESET,
	ANSI_RESET,
	ANSI_RESET,
	ANSI_RESET,
	ANSI_RESET,
	ANSI_RESET,
	ANSI_RESET,
	ANSI_RESET ANSI_BOLD ANSI_RED,
	ANSI_BOLD ANSI_WHITE ANSI_BG_RED,
	""
};

static const char *log_level_names[] = {
	"       ",
	"[TRACE]",
	"[SPAM] ",
	"[DEBUG]",
	"[EXTRA]",
	"[INFO] ",
	"[ATTN] ",
	"[WARN] ",
	"[ERROR]",
	"[CRIT] ",
	"[FATAL]",
	""
};

static enum log_levels log_level = WARN;
extern FILE *logfile;

#ifdef LOGGER_IMPLEMENTATION

#define LOG_STR1(x) #x
#define LOG_STR(x) LOG_STR1(x)

#ifdef _REENTRANT
__thread pthread_mutex_t _logger_mutex;
_Atomic bool _logger_mutex_init_flag;
#endif //_REENTRANT

FILE *logfile = NULL;

#ifdef __GNUC__
void _logger_init(void) __attribute__((constructor));
void _logger_init(void){
	logfile = stderr;
	#ifdef _REENTRANT //defined by -pthread flag
	pthread_mutex_init(&_logger_mutex, NULL);
	_logger_mutex_init_flag = 1;
	#endif // _REENTRANT
}
#endif

int logger(enum log_levels level, const char *msg){
	#ifdef _REENTRANT
	if(!_logger_mutex_init_flag)
		pthread_mutex_init(&_logger_mutex, NULL);
	pthread_mutex_lock(&_logger_mutex);
	#endif // _REENTRANT
	fflush(logfile);
	if((level < log_level && level != ALL)|| logfile == NULL || level == NONE){
		#ifdef _REENTRANT
		pthread_mutex_unlock(&_logger_mutex);
		#endif // _REENTRANT
		return -1;
	}

	time_t t;
	time(&t);
	struct tm timestruct;
	localtime_r(&t, &timestruct);
	char time_str[64];
	strftime(time_str, sizeof(time_str), "%F %T", &timestruct);

	int logfd = fileno(logfile);
	int tty = isatty(logfd); // don't print colored outputs to non-terminals

	if(tty || LOGGER_ALWAYS_COLOR)
		fputs(log_colors[level], logfile);
	fputs(time_str, logfile);
	fputs(" ", logfile);
	fputs(log_level_names[level], logfile);
	if(tty || LOGGER_ALWAYS_COLOR)
		fputs(log_text_colors[level], logfile);
	fputs(" ", logfile);
	fputs(msg, logfile);
	if(tty || LOGGER_ALWAYS_COLOR)
		fputs(ANSI_RESET"\n", logfile);
	else fputs("\n", logfile);
	#ifdef _REENTRANT
	pthread_mutex_unlock(&_logger_mutex);
	#endif // _REENTRANT
	return 0;
}

int loggerf(enum log_levels level, const char *fmt, ...){
	#ifdef _REENTRANT
	if(!_logger_mutex_init_flag)
		pthread_mutex_init(&_logger_mutex, NULL);
	pthread_mutex_lock(&_logger_mutex);
	#endif // _REENTRANT
	fflush(logfile);
	if((level < log_level && level != ALL)|| logfile == NULL || level == NONE){
		#ifdef _REENTRANT
		pthread_mutex_unlock(&_logger_mutex);
		#endif // _REENTRANT
		return -1;
	}

	time_t t;
	time(&t);
	struct tm timestruct;
	localtime_r(&t, &timestruct);
	char time_str[64];
	strftime(time_str, sizeof(time_str), "%F %T", &timestruct);

	int logfd = fileno(logfile);
	int tty = isatty(logfd); // don't print colored outputs to non-terminals
	va_list args;
	va_start(args, fmt);
	if(tty || LOGGER_ALWAYS_COLOR)
		fputs(log_colors[level], logfile);
	fputs(time_str, logfile);
	fputs(" ", logfile);
	fputs(log_level_names[level], logfile);
	if(tty || LOGGER_ALWAYS_COLOR)
		fputs(log_text_colors[level], logfile);
	fputs(" ", logfile);
	vfprintf(logfile, fmt, args);
	va_end(args);
	if(tty || LOGGER_ALWAYS_COLOR)
		fputs(ANSI_RESET"\n", logfile);
	else fputs("\n", logfile);
	#ifdef _REENTRANT
	pthread_mutex_unlock(&_logger_mutex);
	#endif // _REENTRANT
	return 0;
}

#undef LOG_STR
#undef LOG_STR1
#endif // LOGGER_IMPLEMENTATION
#endif // LOG_H_
