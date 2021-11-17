#include "common.h"

int strlen(const char *str)
{
    int len = 0;
    while (*str++ != '\0')
        len++;
    return len;
}

int strncmp(const char* s1, const char* s2, unsigned int num)
{
	unsigned int i;
	for (i = 0; i < num; i++)
	{
		if (s1[i] != s2[i])
			break;
	}
	if (s1[i] < s2[i])
	{
		return -(i+1);
	}
	else if (s1[i] > s2[i])
	{
		return i+1;
	}

	return 0;
}

char* strncpy(char *dest, const char *src, int num)
{
	char* src1 = const_cast<char*>(src);
	while(*src1 != '\0' && num > 0)
	{
		*dest = *src1;
		dest++;
		src1++;
		num--;
	}
	*dest = '\0';
}

char* strcat(char* dest, char* src)
{
	char* res;
	while(*dest != 0)
	{
		dest++;
	}
	while(*src != 0)
	{
		*dest++ = *src++;
	}
	return dest;
}
