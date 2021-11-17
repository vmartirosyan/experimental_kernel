#include "fs.h"
#include "common.h"
#include "screen.h"
#include "memory_management.h"

extern Screen screen;
FileNode:: FileNode()
{
	memset(name_, '\0', MAX_NAME_LEN);
	mask_		= 0;
	uid_		= 0;
	gid_		= 0;
	flags_		= 0;
	inode_		= 0;
	length_		= 0;
	
	fileOperations_.read_	= 0;
	fileOperations_.write_	= 0;
	fileOperations_.open_	= 0;
	fileOperations_.close_	= 0;
	fileOperations_.readdir_	= 0;
	fileOperations_.finddir_	= 0;
}

FileNode:: FileNode(const char* name, unsigned int mask, unsigned int uid, unsigned int gid, unsigned int flags, unsigned int inode, unsigned int length, FileOperations fileOperations)
{
	int nameLen = strlen(name) > MAX_NAME_LEN ? MAX_NAME_LEN : strlen(name);
	strncpy(name_, name, nameLen);
	mask_		= mask;
	uid_		= uid;
	gid_		= gid;
	flags_		= flags;
		
	inode_		= inode;
	length_		= length;
	
	fileOperations_.read_	= fileOperations.read_;
	fileOperations_.write_	= fileOperations.write_;
	fileOperations_.open_	= fileOperations.open_;
	fileOperations_.close_	= fileOperations.close_;
	fileOperations_.readdir_	= fileOperations.readdir_;
	fileOperations_.finddir_	= fileOperations.finddir_;
	
	/*for (int i = 0; i < MAX_CHILD_NUM; i++)
	{
		childNodes[i] = NULL;
	}*/
}

FileNode:: FileNode(const FileNode& fileNode)
{
	int nameLen = strlen(fileNode.name_) > MAX_NAME_LEN ? MAX_NAME_LEN : strlen(fileNode.name_);
	strncpy(name_, fileNode.name_, nameLen);
	mask_		= fileNode.mask_;
	uid_		= fileNode.uid_;
	gid_		= fileNode.gid_;
	flags_		= fileNode.flags_;
		screen.printString("!!! ");
	screen.printHex(flags_);
	screen.printEndl();
	screen.printHex(fileNode.flags_);
	screen.printEndl();
	inode_		= fileNode.inode_;
	length_		= fileNode.length_;
	
	fileOperations_.read_	= fileNode.fileOperations_.read_;
	fileOperations_.write_	= fileNode.fileOperations_.write_;
	fileOperations_.open_	= fileNode.fileOperations_.open_;
	fileOperations_.close_	= fileNode.fileOperations_.close_;
	fileOperations_.readdir_	= fileNode.fileOperations_.readdir_;
	fileOperations_.finddir_	= fileNode.fileOperations_.finddir_;
}

void FileNode:: set(const char* name, unsigned int mask, unsigned int uid, unsigned int gid, unsigned int flags, unsigned int inode, unsigned int length, FileOperations fileOperations)
{
	screen.printString("set\n");
	int nameLen = strlen(name) > MAX_NAME_LEN ? MAX_NAME_LEN : strlen(name);
	strncpy(name_, name, nameLen);
	screen.printString(name_);
	mask_		= mask;
	uid_		= uid;
	gid_		= gid;
	flags_		= flags;
	
	inode_		= inode;
	length_		= length;
	
	fileOperations_.read_	= fileOperations.read_;
	fileOperations_.write_	= fileOperations.write_;
	fileOperations_.open_	= fileOperations.open_;
	fileOperations_.close_	= fileOperations.close_;
	fileOperations_.readdir_	= fileOperations.readdir_;
	fileOperations_.finddir_	= fileOperations.finddir_;
}

FileNode& FileNode:: operator=(FileNode& fileNode)
{
	int nameLen = strlen(fileNode.name_) > MAX_NAME_LEN ? MAX_NAME_LEN : strlen(fileNode.name_);
	strncpy(name_, fileNode.name_, nameLen);
	mask_		= fileNode.mask_;
	uid_		= fileNode.uid_;
	gid_		= fileNode.gid_;
	flags_		= fileNode.flags_;

	inode_		= fileNode.inode_;
	length_		= fileNode.length_;
	
	fileOperations_.read_	= fileNode.fileOperations_.read_;
	fileOperations_.write_	= fileNode.fileOperations_.write_;
	fileOperations_.open_	= fileNode.fileOperations_.open_;
	fileOperations_.close_	= fileNode.fileOperations_.close_;
	fileOperations_.readdir_	= fileNode.fileOperations_.readdir_;
	fileOperations_.finddir_	= fileNode.fileOperations_.finddir_;
	
	return *this;
}

void FileNode:: print()
{
	screen.printString("( ");
	screen.printString(name_);
	screen.printString(", ");
	screen.printHex(mask_);
	screen.printString(", ");
	screen.printHex(uid_);
	screen.printString(", ");
	screen.printHex(gid_);
	screen.printString(", ");
	screen.printHex(flags_);
	screen.printString(", ");
	screen.printHex(inode_);
	screen.printString(", ");
	screen.printHex(length_);
	screen.printString(" )");
}

//
Vfs:: Vfs()
{
	root_ = 0;
}

void Vfs:: open(FileNode* fileNode)
{
	if (fileNode->fileOperations_.open_ != 0)
		fileNode->fileOperations_.open_(fileNode);
}

void Vfs:: close(FileNode* fileNode)
{
	if (fileNode->fileOperations_.close_ != 0)
		fileNode->fileOperations_.close_(fileNode);
}

unsigned int Vfs:: read(FileNode* fileNode, unsigned int offset, unsigned int size, unsigned char* buffer)
{
	unsigned int ret = -1;
	if (fileNode->fileOperations_.read_ != 0)
		ret = fileNode->fileOperations_.read_(fileNode, offset, size, buffer);
	return ret;
}

unsigned int Vfs:: write(FileNode* fileNode, unsigned int offset, unsigned int size, unsigned char* buffer)
{
	int ret = -1;
	if (fileNode->fileOperations_.write_ != 0)
		ret = fileNode->fileOperations_.write_(fileNode, offset, size, buffer);
	return ret;
}

dirent* Vfs:: readdir(FileNode* fileNode, unsigned int index)
{	
	if (fileNode->fileOperations_.readdir_ != 0)
		return fileNode->fileOperations_.readdir_(fileNode, index);
	return 0;
}

FileNode* Vfs:: finddir(FileNode* fileNode, char* name)
{
	if (fileNode->fileOperations_.finddir_ != 0)
		return fileNode->fileOperations_.finddir_(fileNode, name);
	return 0;
}
