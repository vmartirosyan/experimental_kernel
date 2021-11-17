#include "initrd_fs.h"
#include "memory_management.h"
#include "common.h"
#include "kernel_heap.h"
#include "screen.h"

extern Screen screen;
extern KernelHeap kernelHeap;

Array<InitrdFs:: FileHeader> InitrdFs:: fileHeaders_;
FileNode* InitrdFs:: root_;
FileNode* InitrdFs:: dev_;

dirent InitrdFs:: currDirent;

void InitrdFs:: init(unsigned int location)
{
	screen.printString("Init initrdFs\n");
	unsigned int size = *(reinterpret_cast<unsigned int*>(location));
	
	fileHeaders_.place(location + sizeof(unsigned int), size, size);
	
	root_ = (FileNode*)kernelHeap.allocate(sizeof(FileNode));	
	strncpy(root_->name_, "initrd", strlen("initrd"));	
	root_->mask_	= 0;
	root_->uid_		= 0;
	root_->gid_		= 0;
	root_->inode_	= 0;
	root_->length_	= 0;	
	root_->flags_ = FileNode:: FS_DIRECTORY;	
	root_->fileOperations_.read_ = 0;
	root_->fileOperations_.write_ = 0;
	root_->fileOperations_.readdir_ = readdir;
	root_->fileOperations_.finddir_ = finddir;
	root_->fileOperations_.open_ = 0;
	root_->fileOperations_.close_ = 0;
	
	dev_ = (FileNode*)kernelHeap.allocate(sizeof(FileNode));	
	strncpy(dev_->name_, "dev", strlen("dev"));
	dev_->mask_	= 0;
	dev_->uid_	= 0;
	dev_->gid_ = 0;
	dev_->inode_	= 0;
	dev_->length_	= 0;
	dev_->flags_ = FileNode:: FS_DIRECTORY;
	dev_->fileOperations_.read_ = 0;
	dev_->fileOperations_.write_ = 0;
	dev_->fileOperations_.readdir_ = readdir;
	dev_->fileOperations_.finddir_ = finddir;
	dev_->fileOperations_.open_ = 0;
	dev_->fileOperations_.close_ = 0;
			
	root_->childNodes_ = Array<FileNode>(fileHeaders_.getSize()); 
	/*screen.printString("fileHeaders_.size_: ");
	screen.printHex(root_->childNodes_.getSize());
	screen.printEndl();*/
	
	for(int i = 0; i < fileHeaders_.getSize(); i++)
	{
		fileHeaders_[i].offset_	+= location;	// offset - relative to initrd start
		FileNode childNode = FileNode(fileHeaders_[i].name_, 0, 0, 0, FileNode:: FS_FILE, i + 1, fileHeaders_[i].length_, FileNode:: FileOperations(read, 0, 0, 0, 0, 0));
		root_->childNodes_.add(childNode);
	}

}

unsigned int InitrdFs:: read(FileNode* fileNode, unsigned int offset, unsigned int size, unsigned char* buffer)
{
	FileHeader fileHeader = fileHeaders_[fileNode->inode_-1];
	if (offset + size > fileHeader.length_)
	{
		size = fileHeader.length_ - offset;
	}
	if (offset > fileHeader.length_)
	{
		return 0;
	}
	strncpy((char*)buffer, reinterpret_cast<char*>(fileHeader.offset_ + offset), size);
	return size;
	//return 0;
}

/*
 * readdir function
 * returns a pointer to a dirent structure representing the next directory entry in fileNode
 */
dirent* InitrdFs:: readdir(FileNode* fileNode, unsigned int index)
{
	/*screen.printString("root childs - ");
	screen.printHex(root_->childNodes_.getSize());
	screen.printEndl();
	for(int i=0; i<root_->childNodes_.getSize(); i++)
	{
		screen.printString(root_->childNodes_[i].name_);
		screen.printEndl();
	}*/
	
	if (fileNode == root_ && index == 0)
	{
		// next directory in root is dev directory
		strncpy(currDirent.name, "dev", strlen("dev"));
		currDirent.name[strlen("dev")] = '\0';
		currDirent.inode = 0;
		return &currDirent;
	}

	if (index > root_->childNodes_.getSize()) // if it is not root child
		return 0;

	strncpy(currDirent.name, root_->childNodes_[index-1].name_, strlen(root_->childNodes_[index-1].name_));
	currDirent.name[strlen(root_->childNodes_[index-1].name_)] = 0;
	currDirent.inode = root_->childNodes_[index-1].inode_;

	return &currDirent;
}

FileNode* InitrdFs:: finddir(FileNode* fileNode, char* name)
{
	if (fileNode == root_ && !strncmp(name, "dev", strlen("dev")) )
		return dev_;

	for (int i = 0; i < root_->childNodes_.getSize(); i++)
		if (!strncmp(name, root_->childNodes_[i].name_, strlen(name)))
			return &root_->childNodes_[i];
	return 0;
}

void InitrdFs:: printFileHeader(FileHeader fh)
{
	screen.printString("( ");
	screen.printHex(fh.magic_);
	screen.printString(",");
	screen.printString(fh.name_);
	screen.printString(",");
	screen.printHex(fh.offset_);
	screen.printString(",");
	screen.printHex(fh.length_);
	screen.printString(" )");
	screen.printEndl();
}
