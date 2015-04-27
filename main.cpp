#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "LogList.h"
using namespace std;

const string correct[] = { "40000810", "40000818", "40000C18", "40000C20", "4000101C" };

int main()
{
	ifstream inFile("data.log");     // open file with constructor

	if (!inFile.is_open())           // if file cannot be opened
		cout << "Cannot open the input file. Program Terminates!" << endl;

	int count = 1;
	int j = 0;
	int line_num[3630];
	string sample, bgl, relTime, AbsTime, Transfer, XAM, Address, Data, Size, Cycle, Status, Iack, Fail, IRQ;

	LogList theLog;

	while (!inFile.eof())
	{
		inFile >> sample >> bgl >> relTime >> AbsTime >> Transfer >> XAM
			>> Address >> Data >> Size >> Cycle >> Status >> Iack >> Fail >> IRQ;
		j++;
		for (int i = 0; i < 5; i++)
		{
			if (Address == correct[i])
			{
				theLog.appendNode(sample, bgl, relTime, AbsTime, Transfer, XAM, Address, Data, Size, Cycle, Status, Iack, Fail, IRQ);
				line_num[j] = count;
			}
		}
		count++;
	}

	theLog.displayList();
	for (int k = 0; k < j; k++){
		cout << line_num[k] << endl;
	}

	inFile.close();
	return 0;
}


