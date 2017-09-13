#include <pcap.h>
#include <fstream> 
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

// #define PG 1
// #define MY 2
// #define PL 3   //Type of sql

typedef unsigned short  u_short;
typedef unsigned char u_char;
typedef unsigned int u_int;

int To_int_from(const char *data, u_int &from) ;
int To_int(const char *data, u_int from);
short To_short_from(const char* data, u_int &from);
void point_from(char* &dest, char *src, u_int &from) ;

