#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

const std::string currentDateTime()
{
    time_t nowt = time(0);  // get time now
    struct tm nowtstruct = * localtime( & nowt );
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &nowtstruct);
    return buf;
}

