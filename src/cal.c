/* cal.c — print a simple calendar for the current month */
#include <stdio.h>
#include <time.h>
#include "utilbox.h"

static int days_in_month(int month, int year)
{
    static const int dim[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 1) {
        int leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        return leap ? 29 : 28;
    }
    return dim[month];
}

int cal_main(int argc, char **argv)
{
    (void)argc; (void)argv;

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    int year = tm_info->tm_year + 1900;
    int month = tm_info->tm_mon;
    int today = tm_info->tm_mday;

    struct tm first = *tm_info;
    first.tm_mday = 1;
    mktime(&first);
    int start_wday = first.tm_wday;

    static const char *mnames[] = {"January","February","March","April","May","June",
        "July","August","September","October","November","December"};

    printf("    %s %d\n", mnames[month], year);
    printf("Su Mo Tu We Th Fr Sa\n");

    for (int i = 0; i < start_wday; i++) printf("   ");

    int nd = days_in_month(month, year);
    for (int d = 1; d <= nd; d++) {
        if (d == today) printf("%2d*", d);
        else printf("%2d ", d);

        if ((start_wday + d) % 7 == 0) printf("\n");
    }
    printf("\n");

    return 0;
}
