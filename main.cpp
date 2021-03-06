/*
	Assignment:	Lab Project #6 - Log File Parser
	Team:		3
	Authors:	Julio Cruz, Chase Delgadillo, Kristian Rosales, Javier Torres, Tevin Tuan Vu
	Course: 	CPSC 254
	Purpose:	Design, implement, and test a log file parser that parses raw command data produced by another program.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

void outputToFile(ofstream &outFile, vector<string> vAddress, vector<string> vData, vector<string> vCycle, vector<string> vRelTime, vector<string> vSize);
int calculateData(string data);
const char* hex_char_to_bin(char c);
string hex_str_to_bin_str(const string& hex);
void getWord1(vector<string> defaultWord, vector<int> line, ofstream&);
void getWord2(vector<string> defaultWord, vector<int> line, ofstream&);
int readBinary(string binaryNum);
double converTime(string time);

//getData holds all the variables that will be pulled from the input file
class getData
{
	public:
	string sample, bgl, relTime, AbsTime, Transfer, am, address, data, size, cycle, status, iack, Fail, irq;
};

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <input file>" << endl;
		return 1;
	}
		
        vector<string> vAddress;
        vector<string> vData;
        vector<string> vCycle;
        vector<string> vRelTime;
        vector<string> vSize;
        getData fileData;	
 	
        ifstream inFile(argv[1]);  //Name the file we will be pulling data from
       
        if (inFile.is_open())
        {
                while (inFile.good())
                {      
                        //Grab the next line from the input log file
                        inFile >> fileData.sample >> fileData.bgl >> fileData.relTime >> fileData.AbsTime
                               >> fileData.Transfer >> fileData.am >> fileData.address >> fileData.data
                               >> fileData.size >> fileData.cycle >> fileData.status >> fileData.iack
                               >> fileData.Fail >> fileData.irq;
                                                                 
                        //Place the important data in the vector that pertains to their data type
                        vAddress.push_back(fileData.address);
                        vData.push_back(fileData.data);
                        vCycle.push_back(fileData.cycle);
                        vRelTime.push_back(fileData.relTime);
                        vSize.push_back(fileData.size);
                }
        }
 
        else
        {
                cout << "Cannot open file '" << argv[1] << "'." << endl;
                return 1;
        }
               
        inFile.close();
        ofstream outFile("output.log");         //Name the file we will be writing to
 
        outputToFile(outFile, vAddress, vData, vCycle, vRelTime, vSize);  //Call output function
 
        return 0;
}

//Write all the command line to file
void outputToFile(ofstream &outFile, vector<string> vAddress, vector<string> vData, vector<string> vCycle, vector<string> vRelTime, vector<string> vSize)
{
	string firstNum = "40000810";
	string secondNum = "40000C18";
	string str1;
	int calData;
	double tRSD = 0.0;
	double totaltimeRSD = 0.0;
	double totalbitsRSD = 0.0;
	double tRDS = 0.0;
	double totaltimeRDS = 0.0;
	double totalbitsRDS = 0.0;
	double tWSD = 0.0;
	double totaltimeWSD = 0.0;
	double totalbitsWSD = 0.0;
	double tWDS = 0.0;
	double totaltimeWDS = 0.0;
	double totalbitsWDS = 0.0;
	
	//run through vector from first data line address to last line
	for (unsigned i = 1; i < vAddress.size(); ++i)
	{
		str1 = vAddress[i];

		if (str1.compare(firstNum) == 0 || str1.compare(secondNum) == 0)
		{
			string con = vData[i];
			calData = calculateData(con);

			int word = calData;
			if(word >= 0 && word <= 84)
			{			
				if (word == 0)
				{	
					if (str1.compare(firstNum) == 0)
					{
						//output the line has 0 word and it is the Write S-to-D command
						if (vCycle[i] == "Wr")
						{
							double tempTime1 = 0.0;
							tempTime1 = converTime(vRelTime[i + 1]);
							string sizeTemp1 = vSize[i].substr(1, 2);
							double dataSize1 = 0.0;
							dataSize1 = atof(sizeTemp1.c_str());
							totaltimeWSD += tempTime1;
                        				totalbitsWSD += dataSize1;
							outFile << "Line " << std::dec << i + 1 << ": " << "Write S-to-D command: " << word << " words" << endl;
						}

						else
						{
							double tempTime2 = 0.0;
							tempTime2 = converTime(vRelTime[i + 1]);
							string sizeTemp2 = vSize[i].substr(1, 2);
							double dataSize2 = 0.0;
							dataSize2 = atof(sizeTemp2.c_str());
							totaltimeRSD += tempTime2;
							totalbitsRSD += dataSize2;
							outFile << "Line " << std::dec << i + 1 << ": " << "Read S-to-D command: " << word << " words" << endl;
						}
					}

					else
					{
						if (vCycle[i] == "Wr")
						{
							double tempTime3 = 0.0;
							tempTime3 = converTime(vRelTime[i + 1]);                    
							string sizeTemp3 = vSize[i].substr(1, 2);
							double dataSize3 = 0.0;
							dataSize3 = atof(sizeTemp3.c_str());
							totaltimeWDS += tempTime3;
							totalbitsWDS +=dataSize3;	
							outFile << "Line " << std::dec << i + 1 << ": " << "Write D-to-S command: " << word << " words" << endl;
						}
						else
						{
							double tempTime4 = 0.0 ;
							tempTime4 = converTime(vRelTime[i + 1]);
							string sizeTemp4 = vSize[i].substr(1, 2);
							double dataSize4 = 0.0;
							dataSize4 = atof(sizeTemp4.c_str());
							totaltimeRDS += tempTime4;
							totalbitsRDS += dataSize4;
							outFile << "Line " << std::dec << i + 1 << ": " << "Read D-to-S command: " << word << " words" << endl;
						}
					}

				}

				else
				{
					word = calData / 2;
					if (str1.compare(firstNum) == 0)
					{
						if (vCycle[i] == "Wr")
						{
							double tempTime5 = 0.0;
							string sizeTemp5;
							double dataSize5 = 0.0 ;
							for(unsigned int j = i; j < i + word /2 + 1; ++j)
						    	{
								tempTime5 += converTime(vRelTime[j + 1]);
								sizeTemp5 = vSize[j].substr(1, 2);
								dataSize5 += atof(sizeTemp5.c_str());
						    	}
						    	totaltimeWSD += tempTime5;
                            			    	totalbitsWSD += dataSize5;
							outFile << "Line " << std::dec << i + 1 << ": " << "Write S-to-D command: " << word << " words" << endl;
						}
						else
						{
							double tempTime6 = 0.0;
							string sizeTemp6;
							double dataSize6 = 0.0 ;
							for(unsigned int j = i; j < i + word/2 + 1; ++j)
							    {
								tempTime6 += converTime(vRelTime[j + 1]);
								sizeTemp6 = vSize[j].substr(1, 2);
								dataSize6 += atof(sizeTemp6.c_str());
							    }
								totaltimeRSD += tempTime6;
								totalbitsRSD += dataSize6;
							outFile << "Line " << std::dec << i + 1 << ": " << "Read S-to-D command: " << word << " words" << endl;
						}
					}

					else if (str1.compare(secondNum) == 0)
					{
						if (vCycle[i] == "Wr")
						{
						    double tempTime7 = 0.0;
						    string sizeTemp7;
						    double dataSize7 = 0.0 ;
						    for(unsigned int j = i; j < i + word/2 + 1; ++j)
						    {
							tempTime7 += converTime(vRelTime[j + 1]);
							sizeTemp7 = vSize[j].substr(1, 2);
							dataSize7 += atof(sizeTemp7.c_str());
						    }
						    totaltimeWDS += tempTime7;
						    totalbitsWDS +=dataSize7;
						outFile << "Line " << std::dec << i + 1 << ": " << "Write D-to-S command: " << word << " words" << endl;
						}

						else
						{		
						    double tempTime8 = 0.0;
						    string sizeTemp8;
						    double dataSize8 = 0.0 ;
						    for(unsigned int j = i; j < i + word/2 + 1; ++j)
						    {
							tempTime8 += converTime(vRelTime[j + 1]);
							sizeTemp8 = vSize[j].substr(1, 2);
							dataSize8 += atof(sizeTemp8.c_str());
						    }
						    totaltimeRDS += tempTime8;
						    totalbitsRDS +=dataSize8;
							outFile << "Line " << std::dec << i + 1 << ": " << "Read D-to-S command: " << word << " words" << endl;
						}
					}
					
					string part1;
					string part2;
					string binPart1;
					string binPart2;
					string start;
					string end;

					start = calculateData(vAddress[i + 1]);
					end = calculateData(vAddress[i + word / 2]);
					//compare the start and end of address has the total word
					if (end > start)
					{
						vector<string> defaultWord1;
						vector<int> line1;
						//low-to-high word ordering
						for (unsigned int j = i + 1; j < i + word / 2 + 1; ++j)
						{
							//get the first part and second part of each word's data
							part1 = vData[j].substr(0, 4);
							part2 = vData[j].substr(4, 4);
							binPart1 = hex_str_to_bin_str(part1);
							binPart2 = hex_str_to_bin_str(part2);
							defaultWord1.push_back(binPart1);
							line1.push_back(j + 1);
							defaultWord1.push_back(binPart2);
							line1.push_back(j + 1);
						}
						getWord1(defaultWord1, line1, outFile);
						defaultWord1.clear();
						line1.clear();	
					}
						else
						{
							vector<string> defaultWord2;
							vector<int> line2;
							//high to low word ordering
							for (unsigned int j = i + word / 2; j >= i + 1; j--)
						{
							//get the first part and second part of each word's data
							part1 = vData[j].substr(0, 4);
							part2 = vData[j].substr(4, 4);
							binPart1 = hex_str_to_bin_str(part1);
							binPart2 = hex_str_to_bin_str(part2);
							defaultWord2.push_back(binPart1);
							line2.push_back(j + 1);
							defaultWord2.push_back(binPart2);
							line2.push_back(j + 1);
						}
						getWord2(defaultWord2, line2, outFile);
						defaultWord2.clear();
						line2.clear();
						}
				}
			}
				else
					outFile << "Invalid word" << endl;
				
				outFile << endl;
		}
	}
	
	//calculate the data rate 
	outFile.precision(2);
	tRSD = totalbitsRSD/totaltimeRSD;
	tRDS = totalbitsRDS/totaltimeRDS;
	tWSD = totalbitsWSD/totaltimeWSD;
	tWDS = totalbitsWDS/totaltimeWDS;
	outFile << "Read S-to-D: " << fixed << tRSD  << " Megabits/sec" << endl;
	outFile << "Read D-to-S: " << fixed << tRDS << " Megabits/sec" << endl;
	outFile << "Write S-to-D: " << fixed << tWSD << " Megabits/sec"<< endl;
	outFile << "Write D-to-S: " << fixed << tWDS << " Megabits/sec" << endl;
}

//get the number of bytes from the data
int calculateData(string data)
{
	stringstream stringData;    //creates a stream for the data being passed
	stringData << data;
	int total;
	stringData >> hex >> total;    //formats total to hexadecimal

	return total;
}

const char* hex_char_to_bin(char c)
{
	//Convert hexadecimal to binary by checking by categorizing the hex digits 
	switch (toupper(c))
	{
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'A': return "1010";
		case 'B': return "1011";
		case 'C': return "1100";
		case 'D': return "1101";
		case 'E': return "1110";
		case 'F': return "1111";
		default : return "";
	}
}

string hex_str_to_bin_str(const std::string& hex)
{
	std::string bin;  //creates string bin for hex
	
	for (unsigned i = 0; i != hex.length(); ++i)
		bin += hex_char_to_bin(hex[i]);   //adds hex string into bin string
			
	return bin;
}


void getWord1(vector<string> defaultWord, vector<int> line, ofstream &outFile)
{
	unsigned i = 0;

        //if i is less than size of the default word, grabs appropriate substring and outputs to outFile
	while (i < defaultWord.size())
	{
		if (i == 0)
		{	
			//get the bits 14-13
			string word0 = defaultWord[i].substr(1, 2);    
			int hex0 = readBinary(word0);                    

			outFile << "Line " << line[i] << ": Word " << i << ": Rec_Ctrl = ";    //outputs to outFile

			if (hex0 == 0)
				outFile << hex0 << " (no recording)" << endl;		// if equal to 0 then no recording

			else if (hex0 == 2)
				outFile << hex << " (no processing)" << endl;		// if equal to 2 then no processing

			else if (hex0 == 3)
				outFile << hex0 << " (processing & recording)" << endl;	// if equal to 3 then processing & recording

			else
				outFile << hex0 << " (unknown)" << endl;		// if none of the above, then unknown
		}
		
		//if word size is 1, grabs appropriate substring and outputs to outFile
		else if (i == 1)
		{
			//get the bits 15-13
			string word1 = defaultWord[i].substr(0, 3);  
			int hex1 = readBinary(word1);                   

			outFile << "Line " << line[i] << ": Word " << i << ": Cmd_Type = ";      //outputs to outFile

			if (hex1 == 4)  //checks if hex1 is equal to four
				outFile << hex1 << " (Type A)" << endl;                 // if equal to 4 then Type A

			else if (hex1 == 5) //checks if hex1 is equal to five
				outFile << hex1 << " (Type B)" << endl;                 //if equal to 5 then Type B

			else if (hex1 == 6) //checks if hex1 is equal to 6
				outFile << hex1 << " (Type C)" << endl;                 //if equal to 5 then Type C

			else
				outFile << hex1 << " (unknown)" << endl;		// if none of the above, then unknown
		}

                //if word size is 4, grabs appropriate substring and outputs to outFile
		else if (i == 4)
		{
			//get the bit 0
			string word4 = defaultWord[i].substr(15, 1);   
			int hex4 = readBinary(word4);                 

			outFile << "Line " << line[i] << ": Word " << i << ": Rec_Raw = ";  //outputs to outFile

			if (hex4 == 0)
				outFile << hex4 << " (disable)" << endl;   // if hex4 equal to zero then set disable

			else if (hex4 == 1)
				outFile << hex4 << " (enable)" << endl;    //if hex4 equal to one then set enable

			else
				outFile << hex4 << endl;
		}

                //if word size is 5, grabs appropriate substring and outputs to outFile
		else if (i == 5)
		{
			//get the bits 6-0
			string word5 = defaultWord[i].substr(9, 7);
			int hex5 = readBinary(word5);

			outFile << "Line " << line[i] << ": Word " << i << ": Cmd_ID = " << hex5 << endl;
		}

                //if word size is 10, grabs appropriate substring and outputs to outFile
		else if (i == 10)
		{
			//get the bits 15-11
			string word10 = defaultWord[i].substr(0, 5);
			int hex10 = readBinary(word10);

			outFile << "Line " << line[i] << ": Word " << i << ": Num_Responses = " << hex10 << endl;
		}

                //if word size is 15, grabs appropriate substring and outputs to outFile
		else if (i == 15)
		{
			//get bit 2
			string word15 = defaultWord[i].substr(13, 1);
			int hex15 = readBinary(word15);

			outFile << "Line " << line[i] << ": Word " << i << ": Reset_Enable = ";

			if (hex15 == 0)
				outFile << hex15 << " (disable)" << endl;             //if hex15 equal to zero then disable

			else if (hex15 == 1)
				outFile << hex15 << " (enable)" << endl;              //if hex15 equal to one then enable

			else
				outFile << hex15 << endl;
		}

                //if word size is 22, grabs appropriate substring and outputs to outFile
		else if (i == 22)
		{
			//get bit 3
			string word22 = defaultWord[i].substr(12, 1);
			int hex22 = readBinary(word22);

			outFile << "Line " << line[i] << ": Word " << i << ": Direction = ";

			if (hex22 == 0)
				outFile << hex22 << " (Right)" << endl;            //if hex22 is equal to zero then Right

			else if (hex22 == 1)
				outFile << hex22 << " (Left)" << endl;             //if hex22 is equal to one then Left

			else
				outFile << hex22 << endl;
		}

                //if size is 32, grabs appropriate substring and outputs to outFile
		else if (i == 32)
		{
			//get bits 14-0
			string word32 = defaultWord[i].substr(1, 15);
			int hex32 = readBinary(word32);

			outFile << "Line " << line[i] << ": Word " << i << ": Num_Samples = " << hex32 << endl;
		}

                //if word size is 37, grabs appropriate substring and outputs to outFile
		else if (i == 37)
		{
			//get bit 15
			string word37 = defaultWord[i].substr(0, 1);
			int hex37 = readBinary(word37);

			outFile << "Line " << line[i] << ": Word " << i << ": Parity = ";

			if (hex37 == 0)
				outFile << hex37 << " (even)" << endl;           //if hex37 equal to zero then even

			else if (hex37 == 1)
				outFile << hex37 << " (odd)" << endl;            //if hex37 equal to one then odd

			else
				outFile << hex37 << endl;
		}

                //if word size is 38, grabs appropriate substring and outputs to outFile
		else if (i == 38)
		{
			//get bit 14
			string word38 = defaultWord[i].substr(1, 1);
			int hex38 = readBinary(word38);

			outFile << "Line " << line[i] << ": Word " << i << ": Test = ";

			if (hex38 == 0)
				outFile << hex38 << " (disable)" << endl;        //if hex38 equal to zero set disable

			else if (hex38 == 1)
				outFile << hex38 << " (enable)" << endl;         //if hex38 equal to one set enable

			else
				outFile << hex38 << endl;
		}

                //if word size is 40, grabs appropriate substring and outputs to outFile
		else if (i == 40)
		{
			//get bit 7
			string word40 = defaultWord[i].substr(8, 1);
			int hex40 = readBinary(word40);

			outFile << "Line " << line[i] << ": Word " << i << ": Ctrl_Enable = ";

			if (hex40 == 0)
				outFile << hex40 << " (disable)" << endl;   //if hex40 equal to zero then set disable

			else if (hex40 == 1)
				outFile << hex40 << " (enable)" << endl;    //if hex40 equal to one then set enable

			else
				outFile << hex40 << endl;
		}

                //if word size is 41, grabs appropriate substring and outputs to outFile
		else if (i == 41)
		{
			//get bit 14-8
			string word41 = defaultWord[i].substr(1, 7);  
			int hex41 = readBinary(word41);

			outFile << "Line " << line[i] << ": Word " << i << ": Code = " << hex41 << endl;
		}

		i++;
	}
}

void getWord2(vector<string> defaultWord, vector<int> line, ofstream &outFile)
{
	int i = defaultWord.size();  //sets i to size of the defaultWord

	do
	{
                //if word size is 0, grabs appropriate substring and outputs to outFile
		if (i == 0)
		{
			string word0 = defaultWord[i].substr(1, 2);    
			int hex0 = readBinary(word0);                    

			outFile << "Line " << line[i] << ": Word " << i << ": Rec_Ctrl = ";    //outputs to outFile

			if (hex0 == 0)
				outFile << hex0 << " (no recording)" << endl;		// if equal to 0 then no recording

			else if (hex0 == 2)
				outFile << hex << " (no processing)" << endl;		// if equal to 2 then no processing

			else if (hex0 == 3)
				outFile << hex0 << " (processing & recording)" << endl;	// if equal to 3 then processing & recording

			else
				outFile << hex0 << " (unknown)" << endl;		// if none of the above, then unknown
		}

                //if word size is 1, grabs appropriate substring and outputs to outFile
		else if (i == 1)
		{
			string word1 = defaultWord[i].substr(0, 3);  
			int hex1 = readBinary(word1);                   

			outFile << "Line " << line[i] << ": Word " << i << ": Cmd_Type = ";      //outputs to outFile

			if (hex1 == 4)  //checks if hex1 is equal to four
				outFile << hex1 << " (Type A)" << endl;                 // if equal to 4 then Type A

			else if (hex1 == 5) //checks if hex1 is equal to five
				outFile << hex1 << " (Type B)" << endl;                 //if equal to 5 then Type B

			else if (hex1 == 6) //checks if hex1 is equal to 6
				outFile << hex1 << " (Type C)" << endl;                 //if equal to 5 then Type C

			else
				outFile << hex1 << " (unknown)" << endl;		// if none of the above, then unknown                               
		}

                //if word size is 4, grabs appropriate substring and outputs to outFile
		else if (i == 4)
		{
			string word4 = defaultWord[i].substr(15, 1);   
			int hex4 = readBinary(word4);                 

			outFile << "Line " << line[i] << ": Word " << i << ": Rec_Raw = ";  //outputs to outFile

			if (hex4 == 0)
				outFile << hex4 << " (disable)" << endl;   // if hex4 equal to zero then set disable

			else if (hex4 == 1)
				outFile << hex4 << " (enable)" << endl;    //if hex4 equal to one then set enable

			else
				outFile << hex4 << endl;
		}

                //if word size is 5, grabs appropriate substring and outputs to outFile
		else if (i == 5)
		{
			string word5 = defaultWord[i].substr(9, 7);
			int hex5 = readBinary(word5);

			outFile << "Line " << line[i] << ": Word " << i << ": Cmd_ID = " << hex5 << endl;
		}

                //if word size is 10, grabs appropriate substring and outputs to outFile
		else if (i == 10)
		{
			string word10 = defaultWord[i].substr(0, 5);
			int hex10 = readBinary(word10);

			outFile << "Line " << line[i] << ": Word " << i << ": Num_Responses = " << hex10 << endl;
		}

                //if word size is 15, grabs appropriate substring and outputs to outFile
		else if (i == 15)
		{
			string word15 = defaultWord[i].substr(13, 1);
			int hex15 = readBinary(word15);

			outFile << "Line " << line[i] << ": Word " << i << ": Reset_Enable = ";

			if (hex15 == 0)
				outFile << hex15 << " (disable)" << endl;             //if hex15 equal to zero then disable

			else if (hex15 == 1)
				outFile << hex15 << " (enable)" << endl;              //if hex15 equal to one then enable

			else
				outFile << hex15 << endl;
		}

                //if word size is 22, grabs appropriate substring and outputs to outFile
		else if (i == 22)
		{
			string word22 = defaultWord[i].substr(12, 1);
			int hex22 = readBinary(word22);

			outFile << "Line " << line[i] << ": Word " << i << ": Direction = ";

			if (hex22 == 0)
				outFile << hex22 << " (Right)" << endl;            //if hex22 is equal to zero then Right

			else if (hex22 == 1)
				outFile << hex22 << " (Left)" << endl;             //if hex22 is equal to one then Left

			else
				outFile << hex22 << endl;
		}

                //if size is 32, grabs appropriate substring and outputs to outFile
		else if (i == 32)
		{
			string word32 = defaultWord[i].substr(1, 15);
			int hex32 = readBinary(word32);

			outFile << "Line " << line[i] << ": Word " << i << ": Num_Samples = " << hex32 << endl;
		}

                //if word size is 37, grabs appropriate substring and outputs to outFile
		else if (i == 37)
		{
			string word37 = defaultWord[i].substr(0, 1);
			int hex37 = readBinary(word37);

			outFile << "Line " << line[i] << ": Word " << i << ": Parity = ";

			if (hex37 == 0)
				outFile << hex37 << " (even)" << endl;           //if hex37 equal to zero then even

			else if (hex37 == 1)
				outFile << hex37 << " (odd)" << endl;           //if hex37 equal to one then odd

			else
				outFile << hex37 << endl;
		}

                //if word size is 38, grabs appropriate substring and outputs to outFile
		else if (i == 38)
		{
			string word38 = defaultWord[i].substr(1, 1);
			int hex38 = readBinary(word38);

			outFile << "Line " << line[i] << ": Word " << i << ": Test = ";

			if (hex38 == 0)
				outFile << hex38 << " (disable)" << endl;        //if hex38 equal to zero set disable

			else if (hex38 == 1)
				outFile << hex38 << " (enable)" << endl;         //if hex38 equal to one set enable

			else
				outFile << hex38 << endl;
		}

                //if word size is 40, grabs appropriate substring and outputs to outFile
		else if (i == 40)
		{
			string word40 = defaultWord[i].substr(8, 1);
			int hex40 = readBinary(word40);

			outFile << "Line " << line[i] << ": Word " << i << ": Ctrl_Enable = ";

			if (hex40 == 0)
				outFile << hex40 << " (disable)" << endl;   //if hex40 equal to zero then set disable

			else if (hex40 == 1)
				outFile << hex40 << " (enable)" << endl;    //if hex40 equal to one then set enable

			else
				outFile << hex40 << endl;
		}

                //if word size is 41, grabs appropriate substring and outputs to outFile
		else if (i == 41)
		{
			string word41 = defaultWord[i].substr(1, 7);  
			int hex41 = readBinary(word41);

			outFile << "Line " << line[i] << ": Word " << i << ": Code = " << hex41 << endl;
		}

		i--;

	} while (i >= 0);
}

int readBinary(string binaryNum)
{
	long dec = 0;
	int x = 0;
	int base = pow(2, binaryNum.length() - 1);

	//convert from binary to decimal
	while (binaryNum[x])
	{
		char currBit = binaryNum[x];
		int binNum = currBit - '0';

		dec += binNum * base;
		base /= 2;
		x++;
	}

	return dec;	//return binary number in decimal form
}

double converTime(string time)
{
        //sets flags for nanoseconds microseconds and milliseconds
	bool nanoFlag = false;
	bool microFlag = false;
	bool milliFlag = false;
	double finalTime = 0.0;

        //checks to see if time is ns, us, or ms and sets flags accordingly
	
		if (time.substr(time.length() - 2) == "ns")
			nanoFlag = true;

		if (time.substr(time.length() - 2) == "us")
			microFlag = true;

		if (time.substr(time.length() - 2) == "ms")
			milliFlag = true;
        
	//removes ns, us, or ms from time string
	string justTime = time.substr(0, time.length() - 2);
	double theTime = atof(justTime.c_str());     //converts to double
        
	//converts nanoseconds to microseconds
	if (nanoFlag)
		finalTime = theTime * 0.001;

        //keep number as microseconds
	if (microFlag)
		finalTime = theTime;

        //converts milliseconds to microseconds
	if (milliFlag)
		finalTime = theTime * 1000;

	return finalTime;   //returns the final time that it took to complete action
}
