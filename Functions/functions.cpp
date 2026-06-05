#pragma once
#include <iostream>

void assign(const char* from, char* to){
    char c = from[0];
    for (size_t i = 0; c != '\0'; i++)
    {
        c = from[i];
        to[i] = c;
    }
}

bool compare_strings(const char* str1, const char* str2){
    char a = str1[0];
    char b = str2[0];
    if(a != b) return false;
    else{
        for (size_t i = 1; a != '\0'; i++)
        {
            a = str1[i];
            b = str2[i];
            if(a != b) return false;
        }
        return true;
    }    
}