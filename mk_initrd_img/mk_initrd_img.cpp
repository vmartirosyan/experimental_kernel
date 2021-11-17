#include <iostream>
using std:: fstream;
using std:: ofstream;
using std:: ifstream;
using std:: ios_base;
using std:: string;
using std:: cout;
using std:: cerr;
using std:: endl;

#include <fstream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


const unsigned int MAX_FILE_NAME_LEN = 64;

class FileNode
{
public:
	unsigned char magic_;
	char name_[MAX_FILE_NAME_LEN];			// max name length int ext4 = 256
	unsigned int offset_;
	unsigned int length_;
}__attribute__((packed));

string getFileNameFromPath(string path)
{
	size_t last = path.find_last_of('/');
	return path.substr(last+1);
}
/*
 * argc
 * argv - list files to include in initrd.img
 * */
int main(int argc, char** argv)
{
	int offset = 0;
	cout << "argc " << argc << endl;
	int fileNodesNum = argc - 1; // first arg is for program name
	const int FILE_NODES_NUM = 32;
	FileNode fileNodes[FILE_NODES_NUM]; // max 32 files
	if(fileNodesNum > FILE_NODES_NUM)
		fileNodesNum = FILE_NODES_NUM;
	offset += sizeof(unsigned int); 
	offset += fileNodesNum*sizeof(FileNode); 
	cout << "Init file nodes ..." << endl;
	for(int i = 1; i < argc; i++)
	{
		cout << "Name " << argv[i] << endl;
		if (strlen(argv[i]) > MAX_FILE_NAME_LEN)
		{
			cout << "Bad file name, " << endl;
		}
		else
		{
			fileNodes[i-1].magic_ = 0xAB;
			strcpy(fileNodes[i-1].name_, getFileNameFromPath(argv[i]).c_str());
			for(unsigned int j = strlen(fileNodes[i-1].name_); j < MAX_FILE_NAME_LEN; j++)
			{
				fileNodes[i-1].name_[j] = '\0';
			}
			cout << fileNodes[i-1].name_ << endl;
			ifstream ifstr(fileNodes[i-1].name_, ios_base::ate);
			
			fileNodes[i-1].offset_ = offset;
			fileNodes[i-1].length_ = ifstr.tellg();
			
			offset += fileNodes[i-1].length_;
			
			cout << "Offset " << offset << endl;
			cout << "Length " << fileNodes[i-1].length_ << endl;
			
			ifstr.close();
		}
		cout << endl;
	}

	// Create initrd.img file
	int initrdFd = -1;
	if ((initrdFd = open("initrd.img", O_CREAT | O_RDWR, S_IRWXU)) == -1)
	{
		cerr << "open failed" << endl;
	}
	
	cout << "file nodes num " << fileNodesNum << endl;
	if (write(initrdFd, (void*)(&fileNodesNum), sizeof(int)) == -1)
	{
		cerr << "write failed" << endl;
	}
	
	for (int i = 0; i < argc-1; i++)
	{
		if (write(initrdFd, (void*)(&fileNodes[i].magic_), sizeof(unsigned char)) == -1)
		{
			cerr << "write failed" << endl;
		}
		if (write(initrdFd, (void*)(&fileNodes[i].name_), MAX_FILE_NAME_LEN) == -1)
		{
			cerr << "write failed" << endl;
		}
		if (write(initrdFd, (void*)(&fileNodes[i].offset_), sizeof(fileNodes[i].offset_)) == -1)
		{
			cerr << "write failed" << endl;
		}		
		if (write(initrdFd, (void*)(&fileNodes[i].length_), sizeof(fileNodes[i].length_)) == -1)
		{
			cerr << "write failed" << endl;
		}
	}
	
	for (int i = 0; i < argc-1; i++)
	{
		int fd = -1;
		if ((fd = open(fileNodes[i].name_, O_RDWR, S_IRWXU)) == -1)
		{
			cerr << "open failed" << endl;
		}
		char* content = new char[(int)fileNodes[i].length_];
		if (read(fd, (void*)content, (int)fileNodes[i].length_) == -1)
		{
			cerr << "read failed" << endl;
		}		
		cout << (int)fileNodes[i].length_ << " " << content << endl;
		if (write(initrdFd, (void*)content, (int)fileNodes[i].length_) == -1)
		{
			cerr << "write failed" << endl;
		}
		delete [] content;
	}
	
	return 0;
}
