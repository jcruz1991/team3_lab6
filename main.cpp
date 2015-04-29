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
}
