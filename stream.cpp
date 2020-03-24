#pragma once
#include <fstream>

using namespace std;

void readFile(ifstream& istr, string& store,string filename)
{
	istr.open(filename);
	while (!istr.eof())
	{
		getline(istr, store); //until endl
		//getline(istr, store, ' ');//until delimier ' '
		//istr >> store; //until blank space
	}
	istr.close();
}