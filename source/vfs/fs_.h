#ifndef VFS_FS_H
#define VFS_FS_H

class VfsMnt
{
	Dentry* mntRoot_;
	MntSuperBlock* superBlock_;
	int flags;
};

class Inode
{
};

class Dentry // path entries
{
public:
	char name[MAX_NAME_LEN];
	Inode* dInode_;
	unsigned int flags;
	Dentry* dParent_;
	MntSuperBlock superBlock_;
	
	Dentry *subDirectories_;
	
	//dentry operations
	const int MAX_NAME_LEN = 256;
};

class File
{
	VfsMnt* vfsMnt_;
	Dentry* dentry_;
	int flags_;
u32int mask;        // The permissions mask.
u32int uid;         // The owning user.
u32int gid;         // The owning group.
u32int flags;       // Includes the node type. See #defines above.
u32int inode;       // This is device-specific - provides a way for a filesystem to identify files.
u32int length;      // Size of the file, in bytes.
u32int impl;        // An implementation-defined number.
read_type_t read;
write_type_t write;
open_type_t open;
close_type_t close;
readdir_type_t readdir;
finddir_type_t finddir;
struct fs_node *ptr; // Used by mountpoints and symlinks.
};
#endif // VFS_FS_H
