#ifndef PROCESS_H
#define PROCESS_H

#include "linked_list.h"
#include "page.h"

class Process
{
public:
	Process();
	Process(unsigned int& pidCounter);
	Process(unsigned int& pidCounter, PageDirectory* pageDirectory);
	
	int pid_;
	
	unsigned int esp_;
	unsigned int ebp_;
	
	unsigned int eip_;
	
	PageDirectory* pageDirectory_;
	
private:
};

class ProcessSLinkedList: public SLinkedList<Process>
{
public:
	ProcessSLinkedList();
	bool pushBackCurrent(Process currProc);	// adds Process in the list and sets it as a current process
	Process* getCurrentProcessPtr();
	
	SNode<Process>* currentProcess_;
private:
};

class Sheduler
{
public:
	Sheduler();
	Sheduler(int a);
	~Sheduler();
	
	void init();
	
	unsigned int getEip();
	void switchTask();
	
	int fork();
	int getPid();
	
	void changeStack(void* newStartAddr, unsigned int newSize);
	const Process& getCurrentProcess();
	
	//Process* currentProcess_;
	ProcessSLinkedList processList_;
	unsigned int pidCounter_;
	
	PageDirectory* currPageDirectory_;
	
	static const unsigned int DUMMY_EIP = 0xABC123;
};

#endif /* PROCESS_H */
