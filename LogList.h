#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

#pragma once
class LogList
{
	struct ListNode
	{
		string sample, bgl, relTime, AbsTime, Transfer, XAM, Address, Data, Size, Cycle, Status, Iack, Fail, IRQ;
		struct ListNode* next;
	};
	ListNode* head; // Declare a pointer ot serve as the list head
public:
	LogList() { head = NULL; }	// Constructor initializes the head pointer to NULL
								// This establishes an empty linked list
	void appendNode(string, string, string, string, string, string, string, string, string, string, string, string, string, string);
	void insertNode(double); // Will add in the body in the 2nd example
	void deleteNode(double); // Will add in body in the 3rd example
	void displayList();
	void displayData();
};


void LogList::appendNode(string sample, string bgl, string relTime, string AbsTime, string Transfer, string XAM,
	string Address, string Data, string Size, string Cycle, string Status, string Iack, string Fail, string IRQ)
{
	ListNode* newNode, *nodePtr; // Allocate a new node & store num
	// The newNode pointer will be used to allocate
	// and point to the new node.
	// nodePtr will be used to travbel down the linked
	// list, looking for the last node
	newNode = new ListNode; // These 3 stmts create a new node and store num
	newNode->sample = sample;
	newNode->bgl = bgl;
	newNode->relTime = relTime;
	newNode->AbsTime = AbsTime;
	newNode->Transfer = Transfer;
	newNode->XAM = XAM;
	newNode->Address = Address;
	newNode->Data = Data;
	newNode->Size = Size;
	newNode->Cycle = Cycle;
	newNode->Status = Status;
	newNode->Iack = Iack;
	newNode->Fail = Fail;
	newNode->IRQ = IRQ;

	newNode->next = NULL;			// This node will become the last node in the list, so
									// it's next ptr must be set to NULL
	if (!head)						// If there are no nodes in the list, make newNode the 1st node
		head = newNode;
	else							// Otherwise, insert newNode at end
	{
		nodePtr = head;				// Init nodePtr to head of list
									// Note: nodePtr is used to travel down the list, so it
									// initially has to point to the head
		while (nodePtr->next)		// Find the last node in the list. When not true,
									// it's NULL, or last node
			nodePtr = nodePtr->next;
		nodePtr->next = newNode;	// Insert newNode as the last node.
									// Remember that newNode->next already points to NULL
	}
}

void LogList::displayList(void)
{
	ListNode* nodePtr;
	nodePtr = head;
	while (nodePtr)
	{
		cout << "Number: " << nodePtr->sample << endl;
		cout << "\t" << nodePtr->Address << endl;
		cout << "\t" << nodePtr->Data << endl;
		cout << "\t" << nodePtr->Size << endl;
		cout << "\t" << nodePtr->Cycle << endl;
		cout << "\t" << nodePtr->Status << endl;

		nodePtr = nodePtr->next;
	}
}
void LogList::displayData()
{
	string firstNum = "40000810";
	string secondNum = "40000C18";
	ListNode *nodePtr;
	nodePtr = head;
	while(nodePtr != NULL)
	{
		string newAddress = nodePtr->Address;
		if(newAddress.compare(firstNum) == 0 || newAddress.compare(secondNum) == 0)
		{
			string newData = nodePtr->Data;
			stringstream stringData;
			stringData << newData;
			int convertData;
			stringData >> hex >> convertData;
			cout << newAddress << " " << convertData << " " << convertData/2 << endl;
		}
		nodePtr = nodePtr->next;
	}
}
}
