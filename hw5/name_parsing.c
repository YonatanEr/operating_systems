#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "name_parsing.h"


void split_stored(unsigned char name_ext[11], unsigned char name[8], unsigned char ext[3]){
    // splits stored filename into name and extension
    int i;
    for (i=0; i<8; i++){
        name[i] = name_ext[i];
    }
    for (int i=8; i<11; i++){
        ext[i-8] = name_ext[i];
    }
}


void stored2username(unsigned char name[8], unsigned char ext[3], unsigned char user_name[13]){
    // transform name, extension into a filename
    int i;
    for (i=0; i<=12; i++){
        user_name[i] = '\0';    
    }    
    i=0;
    while (name[i]>32 && i<8){
        user_name[i] = name[i];
        i++;
    }
    if (ext[0]>32){
        user_name[i] = '.';
        user_name[i+1] = ext[0];
        if (ext[1]>32){
            user_name[i+2] = ext[1];
            if (ext[2]>32){
                user_name[i+3] = ext[2];
            }
        }
    }
}


unsigned char ToUpperCase(unsigned char x)
{   // returns uppercase(x) of x if it is between  a,z, O.W returns x
    if ('a'<=x && x<='z'){
        return x - 'a' + 'A'; 
    }
    return x;
}


int find_dot(unsigned char user_name[13]){
    // returns the index of the first dot, if it does not exist returns -1
    int i=0;
    while (user_name[i]!='\0' && i<13){
        if (user_name[i] == '.'){
            return i;
        }
        i++;
    }
    return -1;
}


void username2stored(unsigned char user_name[13], unsigned char name[8], unsigned char ext[3]){
    // transform filename into name, extension
    int i=0;
    for (i=0; i<8; i++){
        name[i] = ' ';
    }
    for (i=0; i<3; i++){
        ext[i] = ' ';
    }
    int dot_ind = find_dot(user_name);
    if (dot_ind == -1){
        for (i=0; user_name[i]!='\0' && i<13; i++){
            name[i] = ToUpperCase(user_name[i]);
        }
    }
    else {
        for (i=0; i<dot_ind; i++){
            name[i] = ToUpperCase(user_name[i]);
        }
        for (i=dot_ind+1; user_name[i]!='\0' && i<dot_ind+4; i++){
            ext[i-dot_ind-1] = ToUpperCase(user_name[i]);
        }
    }
}


bool isEqual(unsigned char name[8], unsigned char ext[3], unsigned char user_name[13]){
    // returns True iff name, ext is equal to filename
    unsigned char temp_name[8];
    unsigned char temp_ext[3];
    username2stored(user_name, temp_name, temp_ext);
    int i;
    for (i=0; i<8; i++){
        if (name[i] != temp_name[i]){
            return false;
        }
    }
    for (i=0; i<3; i++){
        if (ext[i] != temp_ext[i]){
            return false;
        }
    }
    return true;
}


