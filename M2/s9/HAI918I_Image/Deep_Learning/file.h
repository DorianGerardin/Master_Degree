#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char* stringToCharArray(string s) {
	int stringSize = s.length();
	char *charArr = (char*) malloc (sizeof (char) * (stringSize+1));
	strcpy(charArr, s.c_str());
	return charArr;
}