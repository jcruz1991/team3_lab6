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

	if (!inFile.is_open())
		cout << "Cannot open the input file. Program Terminates!" << endl;
	
	string sample, bgl, relTime, AbsTime, Transfer, XAM, Address, Data, Size, Cycle, Status, Iack, Fail, IRQ;
	int arrCounter = 0;
	
	LogList theLog;

	while (!inFile.eof())
	{
		inFile >> sample >> bgl >> relTime >> AbsTime >> Transfer >> XAM
			>> Address >> Data >> Size >> Cycle >> Status >> Iack >> Fail >> IRQ;

		for (int i = 0; i < 5; i++)
		{
			if (Address == correct[i])
			{
				theLog.appendNode(sample, bgl, relTime, AbsTime, Transfer, XAM, Address, Data, Size, Cycle, Status, Iack, Fail, IRQ);
			}
		}
	}
	
	theLog.displayList();

	inFile.close();
	return 0;
}

