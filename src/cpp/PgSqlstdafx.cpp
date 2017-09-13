#include "PgSqlstdafx.h"
int To_int_from(const char *data, u_int &from) {//传参的长度
	unsigned char c[4];
	int result = 0;
	for (int i = 0; i < 4; i++)
	{
		c[i] = data[from + i];
		result += (unsigned int)(c[i]) << (8 * (3 - i));
	}
	if (data[from] >= 0x80) {

		~result;
		result += 1;
	}
	from += 4;
	return result;
}


int To_int(const char *data, u_int from) {//不传参的长度
	unsigned char c[4];
	int result = 0;
	for (int i = 0; i < 4; i++)
	{
		c[i] = data[from + i];
		result += (u_int)(c[i]) << (8 * (3 - i));
	}
	if (data[from] >= 0x80) {
		~result;
		result += 1;

	}
	return result;
}

short To_short_from(const char* data, u_int &from) {

	unsigned char c[2];
	short result = 0;
	for (int i = 0; i < 2; i++)
	{
		c[i] = data[from + i];
		result += (u_int)(c[i]) << (8 * (1 - i));
	}
	if (data[from] >= 0x80) {

		~result;
		result += 1;

	}
	from += 2;
	return result;
}


void point_from(char* &dest, char *src, u_int &from) {
	dest = src + from;

	from += (u_int)strlen(dest) + 1;
}