#ifndef FS_H
#define FS_H

#include "disk.h"
#include <vector>
#include <cstring>

class INE5412_FS
{
public:
    static const unsigned int FS_MAGIC = 0xf0f03410;
    static const unsigned short int INODES_PER_BLOCK = 128;
    static const unsigned short int POINTERS_PER_INODE = 5;
    static const unsigned short int POINTERS_PER_BLOCK = 1024;

    class fs_superblock {
        public:
            unsigned int magic;
            int nblocks;
            int ninodeblocks;
            int ninodes;
    }; 

    class fs_inode {
        public:
            int isvalid;
            int size;
            int direct[POINTERS_PER_INODE];
            int indirect;
    };

    class fs_bitmap {
    public:

        fs_bitmap() : bitmap(0, false) {}
        fs_bitmap(int size) : bitmap(size, false) {}

        std::vector<bool> bitmap;

        bool is_free(int inumber) {
            return !bitmap[inumber];
        }

        void set_used(int inumber) {
            bitmap[inumber] = true;
        }

        void set_free(int inumber) {
            bitmap[inumber] = false;
        }

        void initialize() {
            is_initialized = true;
        }
    
        bool is_initialized = false;
    };

    union fs_block {
        public:
            fs_superblock super;
            fs_inode inode[INODES_PER_BLOCK];
            int pointers[POINTERS_PER_BLOCK];
            char data[Disk::DISK_BLOCK_SIZE];
    };

public:

    INE5412_FS(Disk *d) {
        disk = d;
    } 

    fs_bitmap inode_bitmap;
    fs_bitmap block_bitmap;
    bool is_mounted = false;

    void fs_debug();
    int fs_format();
    int fs_mount();

    int fs_create();
    int fs_delete(int inumber);
    int fs_getsize(int inumber);

    int fs_read(int inumber, char *data, int length, int offset);
    int fs_write(int inumber, const char *data, int length, int offset);
    int allocate_block();

private:
    Disk *disk;
};

#endif