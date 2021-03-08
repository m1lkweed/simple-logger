// Basic message logger in C.
// (c)m1lkweed 2021 GPLv3+
// Works on all ANSI-compliant terminals
//
// logfile should be fopen()-ed before logging or set to stdout/stderr
// Valid log_level values are: [ALL, TRACE, SPAM, DEBUG, EXTRA, INFO,
// ATTN, WARN, ERROR, CRIT, FATAL, NONE] in ascending order of importance
// These values are also valid for logger()
// logger() returns -1 if no logging occurs, 0 if logging does occur

#ifndef _LOG_H_
#define _LOG_H_

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#define write _write
#else
#include <unistd.h>
#endif

#include "termcolors.h"

#define LOG_STR1(x) #x
#define LOG_STR(x) LOG_STR1(x)

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

const char *log_colors[] = {
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

const char *log_text_colors[] = {
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

const char *log_level_names[] = {
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

enum log_levels log_level = WARN;
FILE *logfile = NULL;

int logger(enum log_levels level, const char *msg){
	if(level < log_level || logfile == NULL || level == NONE)
		return -1;

	time_t t = time(NULL);
	struct tm *timestruct = localtime(&t);
	char time_str[64];
	strftime(time_str, sizeof(time_str), "%F %T", timestruct);

	int logfd = fileno(logfile);
	int tty = isatty(logfd); // don't print colored outputs to non-terminals

	if(tty)write(logfd, log_colors[level], strlen(log_colors[level]));
	write(logfd, time_str, strlen(time_str));
	write(logfd, " ", 1);
	write(logfd, log_level_names[level], strlen(log_level_names[level]));
	if(tty)write(logfd, log_text_colors[level], strlen(log_text_colors[level]));
	write(logfd, " ", 1);
	write(logfd, msg, strlen(msg));
	if(tty)write(logfd, ANSI_RESET"\n", strlen(ANSI_RESET"\n"));
	else write(logfd, "\n", 1);
	return 0;
}

int loggerf(enum log_levels level, const char *fmt, ...){
	if(level < log_level || logfile == NULL || level == NONE)
		return -1;

	time_t t = time(NULL);
	struct tm *timestruct = localtime(&t);
	char time_str[64];
	strftime(time_str, sizeof(time_str), "%F %T", timestruct);

	int logfd = fileno(logfile);
	int tty = isatty(logfd); // don't print colored outputs to non-terminals
	va_list args;
	va_start(args, fmt);
	if(tty)fputs(log_colors[level], logfile);
	fputs(time_str, logfile);
	fputs(" ", logfile);
	fputs(log_level_names[level], logfile);
	if(tty)fputs(log_text_colors[level], logfile);
	fputs(" ", logfile);
	vfprintf(logfile, fmt, args);
	va_end(args);
	if(tty)fputs(ANSI_RESET"\n", logfile);
	else fputs("\n", logfile);
	return 0;
}

#undef LOG_STR
#undef LOG_STR1
#endif //_LOG_H_
