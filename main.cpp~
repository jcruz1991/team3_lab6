#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;
int calculateData(string data);
const char* hex_char_to_bin(char c);
string hex_str_to_bin_str(const std::string& hex);
void getWord1(vector<string> defaultWord, vector<int> line);
void getWord2(vector<string> defaultWord, vector<int> line);
int readBinary(string binaryNum);
double converTime(string time);

class getData
{
public:
    //string address, data, cycle;
    string sample, bgl, relTime, AbsTime, Transfer, am, address, data, size, cycle, status, iack, Fail, irq;
};

int main()
{
    vector<string> vAddress;
    vector<string> vData;
    vector<string> vCycle;
    vector<string> vRelTime;

    getData first;
    string sample, bgl, relTime, AbsTime, Transfer, am, address, data, size, cycle, status, iack, Fail, irq;
    ifstream inFile("test_data.log");
    if (inFile.is_open())
    {
        //inFile >> address >> data >> cycle;
        inFile >> sample >> bgl >> relTime >> AbsTime >> Transfer >> am >> address >> data >> size >> cycle >> status >> iack >> Fail >> irq;
        first.address = address;
        first.data = data;
        first.cycle = cycle;
        first.relTime = relTime;
        vAddress.push_back(first.address);
        vData.push_back(first.data);
        vCycle.push_back(first.cycle);
        vRelTime.push_back(first.relTime);
        while (inFile.good())
        {
            getData next;
            //++it;

            //inFile >> address >> data >> cycle;
            inFile >> sample >> bgl >> relTime >> AbsTime >> Transfer >> am >> address >> data >> size >> cycle >> status >> iack >> Fail >> irq;
            next.address = address;
            next.data = data;
            next.cycle = cycle;
            next.relTime = relTime;
            vAddress.push_back(next.address);
            vData.push_back(next.data);
            vCycle.push_back(next.cycle);
            vRelTime.push_back(next.relTime);
        }
    }
    else
        cout << "Cannot open file" << endl;

    inFile.close();

void getWord1(vector<string> defaultWord, vector<int> line)
{

	unsigned i = 0;
	while (i < defaultWord.size())
	{
		if (i == 0)
		{
			string word0 = defaultWord[i].substr(1, 2);
			int hex0 = readBinary(word0);

			cout << "Line " << line[i] << ": Word " << i << ": Rec_Ctrl = ";
			if (hex0 == 0)
				cout << hex0 << " (no recording)" << endl;
			else if (hex0 == 2)
				cout << hex << " (no processing)" << endl;
			else if (hex0 == 3)
				cout << hex0 << " (processing & recording)" << endl;
			else
				cout << hex0 << " (unknown) " << endl;
		}
		else if (i == 1)
		{
			string word1 = defaultWord[i].substr(0, 3);
			int hex1 = readBinary(word1);
			cout << "Line " << line[i] << ": Word " << i << ": Cmd_Type = ";
			if (hex1 == 4)
				cout << hex1 << " (Type A) " << endl;
			else if (hex1 == 5)
				cout << hex1 << " (Type B) " << endl;
			else if (hex1 == 6)
				cout << hex1 << " (Type C) " << endl;
			else
				cout << hex1 << " (unknown) " << endl;
		}
		else if (i == 4)
		{
			string word4 = defaultWord[i].substr(15, 1);
			int hex4 = readBinary(word4);
			cout << "Line " << line[i] << ": Word " << i << ": Rec_Raw =";
			if (hex4 == 0)
				cout << hex4 << " (disable) " << endl;
			else if (hex4 == 1)
				cout << hex4 << " (enable) " << endl;
			else
				cout << hex4 << endl;
		}


		else if (i == 5)
		{
			string word5 = defaultWord[i].substr(9, 7);
			int hex5 = readBinary(word5);
			cout << "Line " << line[i] << ": Word " << i << " : Cmd_ID =" << hex5 << endl;
		}
		else if (i == 10)
		{
			string word10 = defaultWord[i].substr(0, 5);
			int hex10 = readBinary(word10);
			cout << "Line " << line[i] << ": Word " << i << " : Num_Responses =" << hex10 << endl;
		}

		else if (i == 15)
		{
			string word15 = defaultWord[i].substr(13, 1);
			int hex15 = readBinary(word15);
			cout << "Line " << line[i] << ": Word " << i << ": Reset_Enable = ";
			if (hex15 == 0)
				cout << hex15 << " (disable) " << endl;
			else if (hex15 == 1)
				cout << hex15 << " (enable) " << endl;
			else
				cout << hex15 << endl;
		}
}

