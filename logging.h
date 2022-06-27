#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>   // FILE
#include <stdarg.h>  // va_start    va_end
#include <time.h>    // time()  localtime()  strftime()  tzset()

// EXAMPLE : log("INFO : Window Closed");

void _log(const char *fmt, ...);
void initLogging();
void quitLogging();

#define log(...) _log(__VA_ARGS__)

#endif // LOGGING_H

#ifdef LOGGER_IMPLEMENTATION

FILE *file = NULL;

void initLogging()
{
    file = fopen("log.txt", "ab");
}

void quitLogging()
{
    fclose(file);
}

void _log(const char *fmt, ...)
{
  va_list ap;
  time_t t;
  char datestr[51];

  file = (file == NULL) ? stderr : file;

  t = time(NULL);
  tzset();
  strftime(datestr, sizeof(datestr) - 1, "%b/%d/%Y %H:%M", localtime(&t));
  fprintf(file, "%s - ", datestr);

  va_start(ap, fmt);
  vfprintf(file, fmt, ap);
  va_end(ap);

  fprintf(file, "\n");
}
#endif // LOGGER_IMPLEMENTATION
