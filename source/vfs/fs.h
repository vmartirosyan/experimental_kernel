#ifndef FS_H
#define FS_H

#include "array.h"

struct dirent
{
    char name[256];
    unsigned int inode;     // Inode number. Required by POSIX.
};

class FileNode
{
public:
	class FileOperations; 
	FileNode();
	FileNode(const char* name, unsigned int permMask, unsigned int uid, unsigned int gid, unsigned int flags, unsigned int inode, unsigned int length, FileOperations fileOperations);
	FileNode(const FileNode& fileNode);
	void set(const char* name, unsigned int mask, unsigned int uid, unsigned int gid, unsigned int flags, unsigned int inode, unsigned int length, FileOperations fileOperations);
	FileNode& operator=(FileNode& fileNode);
	void print();
	static const int MAX_NAME_LEN	= 64;
	static const int MAX_CHILD_NUM	= 256;
		
	char name_[MAX_NAME_LEN];
	unsigned int mask_;
	unsigned int uid_;
	unsigned int gid_;
	unsigned int flags_;
	unsigned int inode_;
	unsigned int length_;
	//unsigned int impl;
	
	Array<FileNode> childNodes_; // if flags & FS_DIRECTORY == 0 => =NULL
		
	class FileOperations
	{
	public:
		FileOperations()
		{
			read_	= 0;
			write_	= 0;
			open_	= 0;
			close_	= 0;
			readdir_	= 0;
			finddir_	= 0;
		}
		
		FileOperations(unsigned int (*read)(FileNode*, unsigned int, unsigned int, unsigned char*), unsigned int (*write)(FileNode*, unsigned int, unsigned int, unsigned char*), void (*open)(FileNode*), void (*close)(FileNode*), dirent* (*readdir)(FileNode*, unsigned int), FileNode* (*finddir)(FileNode*, char*))
		{
			read_	= read;
			write_	= write;
			open_	= open;
			close_	= close;
			readdir_	= readdir;
			finddir_	= finddir;
		}
		
		unsigned int (*read_)(FileNode*, unsigned int, unsigned int, unsigned char*);
		unsigned int (*write_)(FileNode*, unsigned int, unsigned int, unsigned char*);
		void (*open_)(FileNode*);
		void (*close_)(FileNode*);
		dirent* (*readdir_)(FileNode* fileNode, unsigned int index);
		FileNode* (*finddir_)(FileNode* fileNode, char* name);		
	} fileOperations_;
		
	static const int FS_FILE		= 0x01;
	static const int FS_DIRECTORY	= 0x02;
	static const int FS_CHARDEVICE	= 0x03;
	static const int FS_BLOCKDEVICE	= 0x04;
	static const int FS_PIPE		= 0x05;
	static const int FS_SYMLINK		= 0x06;
	static const int FS_MOUNTPOINT	= 0x08; // Is the file an active mountpoint?
};

// file system
class Vfs
{
public:
	Vfs();
	
	FileNode* root_;

	void open(FileNode*);
	void close(FileNode*);
	unsigned int read(FileNode*, unsigned int offset, unsigned int size, unsigned char* buffer);
	unsigned int write(FileNode*, unsigned int offset, unsigned int size, unsigned char* buffer);
	dirent* readdir(FileNode* fileNode, unsigned int index);
	FileNode* finddir(FileNode* fileNode, char* name);
};

#endif
