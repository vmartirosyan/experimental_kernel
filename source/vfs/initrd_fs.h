#ifndef INITRD_FS_H
#define INITRD_FS_H

#include "fs.h"
#include "array.h"

class InitrdFs
{
public:
	static void init(unsigned int location);
	
	static unsigned int read(FileNode*, unsigned int offset, unsigned int size, unsigned char* buffer);
	static dirent* readdir(FileNode* fileNode, unsigned int index);
	static FileNode* finddir(FileNode* fileNode, char* name);
	
	class FileHeader;
	static Array<FileHeader> fileHeaders_;
	static FileNode* root_;
	static FileNode* dev_;
	
	static dirent currDirent;

	static const unsigned int MAX_FILE_NAME_LEN = 64;
	
	static void printFileHeader(FileHeader fh);
	class FileHeader
	{
	public:
		unsigned char magic_;
		char name_[InitrdFs:: MAX_FILE_NAME_LEN];
		unsigned int offset_;
		unsigned int length_;
	}__attribute__((packed));
};

/*
class InitrdFs
{
public:
	InitrdFs(unsigned int location);
	
	void init(unsigned int location);
	static unsigned int read(FileNode*, unsigned int offset, unsigned int size, unsigned char* buffer);
	static dirent* readdir(FileNode* fileNode, unsigned int index);
	static FileNode* finddir(FileNode* fileNode, char* name);
	
	class FileHeader;
	vector<FileHeader> fileHeaders_;
	FileNode* root_;
	FileNode* dev_;
	
	dirent currDirent;

	class FileHeader
	{
	public:
		static const unsigned int MAX_FILE_NAME_LEN = 32;
			
		unsigned int magic_;
		char name_[MAX_FILE_NAME_LEN];	// max name length int ext4 = 256
		unsigned int offset_;
		unsigned int length_;
	};
};*/

#endif // INITRD_FS_H
