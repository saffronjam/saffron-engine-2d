#pragma once

#define THROW(ExceptionClass, msg, format...)          \
    {                                                  \
        char buf[200];                                 \
        sprintf(buf, msg, format);                     \
        throw ExceptionClass(__LINE__, __FILE__, buf); \
    }
