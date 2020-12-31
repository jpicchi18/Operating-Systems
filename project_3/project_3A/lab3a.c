// NAME: Joseph Picchi
// EMAIL: jpicchi22@g.ucla.edu

#include <stdio.h>
#include <stdlib.h>
#include "ext2_fs.h"
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <time.h>
#include <string.h>


char *input_file;
int input_file_fd, block_size;
unsigned int num_groups;
void error(char *msg, int exit_code);
struct ext2_super_block super_block_struct;
struct ext2_group_desc group_desc_struct;
struct ext2_inode inode_struct;


void process_args(int argc, char **argv);
void open_file();
void get_superblock_summary();
void get_group_summary(int group_num); // updates group_desc_struct with current group info
void cleanup_and_exit();
int block_num_to_byte_offset(int block_num);
void print_free_block_entries(int group_num);
void print_free_inode_entries(int group_num);
void handle_inodes(int group_num); // for all inodes in a block group: inode summary + indir block refs + dir entries
void inode_time_helper(__u32 time_since_epoch);
void handle_dirs(int parent_inode_num); // prints indir block refs + dir entries
void print_dir_entry_block(int parent_inode_num, int block_num, unsigned int *byte_offset_ptr);
void handle_indir_block(int parent_inode_num, int block_num, int indir_level, unsigned int *byte_offset_ptr, \
    int base_block_offset);
    // prints indir block refs + (for dirs) dir entries
void print_indir_entry(int parent_inode_num, int indir_level, int ref_logical_b_offset, int scan_b_num, \
    int ref_block_num);
unsigned int calc_logical_block_offset(int indir_level, int iter, int base_block_offset);
void process_all_indir_blocks(int parent_inode_number, unsigned int *byte_offset_ptr);


int main(int argc, char **argv) {
    process_args(argc, argv);

    // open passed image file for reading
    open_file();

    get_superblock_summary();

    for (int i=0; i<num_groups; i++) {
        get_group_summary(i);
        print_free_block_entries(i);
        print_free_inode_entries(i);
        handle_inodes(i);
    }



    //exit successfully
    cleanup_and_exit();
}


void process_all_indir_blocks(int parent_inode_num, unsigned int *byte_offset_ptr) {
    // calculate number of block pointers in a block
    int num_block_ptrs = block_size/sizeof(__u32);
    
    // read entires from indirect blockss
    if (inode_struct.i_block[12]) {
        int base_block_offset = 12;
        handle_indir_block(parent_inode_num, inode_struct.i_block[12], 1, byte_offset_ptr, base_block_offset);
    }
    if (inode_struct.i_block[13]) {
        int base_block_offset = 12 + num_block_ptrs;
        handle_indir_block(parent_inode_num, inode_struct.i_block[13], 2, byte_offset_ptr, base_block_offset);
    }
    if (inode_struct.i_block[14]) {
        int base_block_offset = 12 + num_block_ptrs + (num_block_ptrs * num_block_ptrs);
        handle_indir_block(parent_inode_num, inode_struct.i_block[14], 3, byte_offset_ptr, base_block_offset);
    }
}

// return 0 for error
unsigned int calc_logical_block_offset(int indir_level, int iter, int base_block_offset) {
    unsigned int ptrs_per_block = block_size/sizeof(__u32);

    if (indir_level == 1) {
        return base_block_offset + iter;
    }
    else if (indir_level == 2) {
        return base_block_offset + ptrs_per_block * iter;
    }
    else if (indir_level == 3) {
        return base_block_offset + (ptrs_per_block * ptrs_per_block * iter); 
    }
    else {
        return 0;
    }
}

void print_indir_entry(int parent_inode_num, int indir_level, int ref_logical_b_offset, int scan_b_num, \
int ref_block_num) {
    // print:
    // INDIRECT
    // I-node number of the owning file (decimal)
    // (decimal) level of indirection for the block being scanned ... 1 for single indirect,
            // 2 for double indirect, 3 for triple
    // logical block offset (decimal) represented by the referenced block. If the referenced block
            // is a data block, this is the logical block offset of that block within the file.
            // If the referenced block is a single- or double-indirect block, this is the same as the
            // logical offset of the first data block to which it refers.
    // block number of the (1, 2, 3) indirect block being scanned (decimal) . . . not the highest level
            // block (in the recursive scan), but the lower level block that contains the block reference
            // reported by this entry.
    // block number of the referenced block (decimal)
    printf("INDIRECT,%d,%d,%d,%d,%d\n", \
            parent_inode_num, \
            indir_level, \
            ref_logical_b_offset, \
            scan_b_num, \
            ref_block_num \
    );
}

void handle_indir_block(int parent_inode_num, int block_num, int indir_level, unsigned int *byte_offset_ptr, \
int base_block_offset) {
    // calculate number of block pointers in this block
    int num_block_ptrs = block_size/sizeof(__u32);

    // read in the data from the desired block
    unsigned char block_contents[block_size];
    int indir_offset = block_num_to_byte_offset(block_num);
    if (pread(input_file_fd, block_contents, block_size, indir_offset) != block_size) {
        error("lab3a error: failed to read indirect block", 2);
    }

    // follow each of the block pointers
    for (int i=0; i<num_block_ptrs; i++) {
        // get pointer to current element
        __u32 *current_element = (void *)block_contents + i*sizeof(__u32);

        // find logical block offset
        unsigned int logical_block_offset = calc_logical_block_offset(indir_level, i, base_block_offset);

        // only interested in nonzero block pointers
        if (!*current_element) {
            continue;
        }
        
        // print an INDIR entry
        print_indir_entry(parent_inode_num, indir_level, logical_block_offset, block_num, *current_element);

        // diff actions based on level of indirection
        if (indir_level == 1) {
            if (S_ISDIR(inode_struct.i_mode)) {
                print_dir_entry_block(parent_inode_num, *current_element, byte_offset_ptr);
            }
        }
        else if (indir_level == 2) {
            int new_bb_offset = base_block_offset + num_block_ptrs * i;
            handle_indir_block(parent_inode_num, *current_element, indir_level-1, byte_offset_ptr, new_bb_offset);
        }
        else if (indir_level == 3) {
            int new_bb_offset = base_block_offset + (num_block_ptrs * num_block_ptrs * i);
            handle_indir_block(parent_inode_num, *current_element, indir_level-1, byte_offset_ptr, new_bb_offset);
        }
        else {
            // should get here
            error("lab3a error: bad code logic", 2);
        }
    }
}

void print_dir_entry_block(int parent_inode_num, int block_num, unsigned int *byte_offset_ptr) {
    // var initialization
    unsigned int bytes_read = 0;
    struct ext2_dir_entry *dir_struct_ptr;
    unsigned char block_contents[block_size];
    unsigned int unsigned_block_size = block_size;

    if (pread(input_file_fd, block_contents, block_size, block_num_to_byte_offset(block_num)) != block_size) {
        error("lab3a error: failed to read a directory entry", 2);
    }

    dir_struct_ptr = (struct ext2_dir_entry *) block_contents;
    while (bytes_read < unsigned_block_size) {
        // get entry name
        char file_name[EXT2_NAME_LEN+1];
        memcpy(file_name, dir_struct_ptr->name, dir_struct_ptr->name_len);
        file_name[dir_struct_ptr->name_len] = 0;

        // print entry info: --> ONLY FOR ALLOCATED NODES (ie nonzero inode#)
        // DIRENT
        // parent inode number (decimal) ... the I-node number of the directory that contains this entry
        // logical byte offset (decimal) of this entry within the directory
        // inode number of the referenced file (decimal)
        // entry length (decimal)
        // name length (decimal)
        // name (string, surrounded by single-quotes). Don't worry about escaping, we promise there
            // will be no single-quotes or commas in any of the file names.
        if (dir_struct_ptr->inode) {
            printf("DIRENT,%u,%d,%d,%d,%d,'%s'\n", \
                parent_inode_num, \
                *byte_offset_ptr, \
                dir_struct_ptr->inode, \
                dir_struct_ptr->rec_len, 
                dir_struct_ptr->name_len, \
                dir_struct_ptr->name \
            );
        }

        // update fields for next iteration
        bytes_read += dir_struct_ptr->rec_len;
        if (dir_struct_ptr->inode) {
            *byte_offset_ptr += dir_struct_ptr->rec_len;
        }
        dir_struct_ptr = (void *)dir_struct_ptr + dir_struct_ptr->rec_len;
    }
}

void handle_dirs(int parent_inode_num) {
    // var initialization
    unsigned int byte_offset = 0;
    unsigned int unsigned_block_size = block_size;
    
    // read first 12 blocks from i_block
    for (int i=0; i<12; i++) {
        // print entries in the block
        if (inode_struct.i_block[i]) {
            print_dir_entry_block(parent_inode_num, inode_struct.i_block[i], &byte_offset);
        }
    }

    process_all_indir_blocks(parent_inode_num, &byte_offset);
}

void inode_time_helper(__u32 time_since_epoch) {
    // get the time struct
    const time_t epoch_time = (time_t)time_since_epoch;
    struct tm tm_struct = *gmtime(&epoch_time);

    // get the formatted time into "buf" in format "mm/dd/yy hh:mm:ss"
    char buf[50];
    strftime(buf, sizeof(buf), "%m/%d/%y %H:%M:%S", &tm_struct);

    // print resulting string
    printf("%s,", buf);
}

void handle_inodes(int group_num) {
    // find #indoes in the group
    int inodes_in_group = (group_num == num_groups-1) 
                            ? (super_block_struct.s_inodes_count - (num_groups-1)*super_block_struct.s_inodes_per_group)
                            : super_block_struct.s_inodes_per_group ;
    
    // for each inode in the group...
    for (int i=1; i<inodes_in_group+1; i++) {
        // read in current inode
        int offset = block_num_to_byte_offset(group_desc_struct.bg_inode_table) + (i-1)*sizeof(inode_struct);

        if (pread(input_file_fd, &inode_struct, sizeof(inode_struct), offset) != sizeof(inode_struct)) {
            error("lab3a error: failed to read entirety of inode", 2);
        }

        // if not allocated, move on
        if (!inode_struct.i_mode || !inode_struct.i_links_count) {
            continue;
        }

        // find file type
        char filetype = '?';
        if (S_ISREG(inode_struct.i_mode)) {
            filetype = 'f';
        }
        else if (S_ISDIR(inode_struct.i_mode)) {
            filetype = 'd';
        }
        else if (S_ISLNK(inode_struct.i_mode)) {
            filetype = 's';
        }

        // get mode
        int mode = inode_struct.i_mode & 0xFFF;

        // print everything up until time fields:
        // INODE
        // inode number (decimal)
        // file type ('f' for file, 'd' for directory, 's' for symbolic link, '?" for anything else)
        // mode (low order 12-bits, octal ... suggested format "%o")
        // owner (decimal)
        // group (decimal)
        // link count (decimal)
        printf("INODE,%d,%c,%o,%d,%d,%d,", \
                i, \
                filetype, \
                mode, \
                inode_struct.i_uid, \
                inode_struct.i_gid, \
                inode_struct.i_links_count
        );
        
        // print the rest of the fields:
        // time of last I-node change (mm/dd/yy hh:mm:ss, GMT)
        // modification time (mm/dd/yy hh:mm:ss, GMT)
        // time of last access (mm/dd/yy hh:mm:ss, GMT)
        // file size (decimal)
        // number of (512 byte) blocks of disk space (decimal) taken up by this file
        inode_time_helper(inode_struct.i_ctime);
        inode_time_helper(inode_struct.i_mtime);
        inode_time_helper(inode_struct.i_atime);
        printf("%d,%d", inode_struct.i_size, inode_struct.i_blocks);

        // print the next 15 fields (which are all block addresses) for most files
        if (S_ISREG(inode_struct.i_mode) || S_ISDIR(inode_struct.i_mode) \
        || (S_ISLNK(inode_struct.i_mode) && inode_struct.i_size > 60)) {
            for (int i=0; i<15; i++) {
                printf(",%d", inode_struct.i_block[i]);
            }
        }
        printf("\n");

        // if it's a directory, print it's entries
        if (S_ISDIR(inode_struct.i_mode)) {
            handle_dirs(i);
        }

        // if it's a file, print the INDIR block info
        if (S_ISREG(inode_struct.i_mode)) {
            // using a dummy var...
            unsigned int byte_offset;
            process_all_indir_blocks(i, &byte_offset);
        }

    }
}

void print_free_inode_entries(int group_num) {
    // find number of inodes in the group
    int inodes_in_group = (group_num == num_groups-1) 
                            ? (super_block_struct.s_inodes_count - (num_groups-1)*super_block_struct.s_inodes_per_group)
                            : super_block_struct.s_inodes_per_group ;

    char bitmap_buf[block_size];
    int bitmap_offet = block_num_to_byte_offset(group_desc_struct.bg_inode_bitmap);
    if (pread(input_file_fd, bitmap_buf, sizeof(bitmap_buf), bitmap_offet) != sizeof(bitmap_buf)) {
        error("lab3a error: failed to read entirety of inode bitmap", 2);
    }

    // print a line for every free inode (NOTE: inode numbering uses the assumption that the file provided
    // only contains 1 block group)
    for (int i=1; i<inodes_in_group+1; i++) {
        // get the bit value for the inode we're checking
        int index = (i-1)/8;
        int offset = (i-1)%8;
        int bit_val = bitmap_buf[index] & (1 << offset);
        
        // print an entry if the block is free
        if (!bit_val) {
            printf("IFREE,%d\n", i);
        }
    }
}

void print_free_block_entries(int group_num) {
    // find number of blocks in the group
    int blocks_in_group = (group_num == num_groups-1) 
                          ? (super_block_struct.s_blocks_count - (num_groups-1)*super_block_struct.s_blocks_per_group)
                          : super_block_struct.s_blocks_per_group ;

    // read in the block with the bitmap for this group
    char bitmap_buf[block_size];
    int bitmap_offet = block_num_to_byte_offset(group_desc_struct.bg_block_bitmap);
    if (pread(input_file_fd, bitmap_buf, sizeof(bitmap_buf), bitmap_offet) != sizeof(bitmap_buf)) {
        error("lab3a error: failed to read entirety of block bitmap", 2);
    }

    // print a line for every free block (NOTE: block numbering uses the assumption that the file provided
    // only contains 1 block group)
    for (int i=0; i<blocks_in_group; i++) {
        // block 0 always used for superblock
        if (i==0) {
            continue;
        }

        // get the bit value for the block we're checking
        int index = (i-1)/8;
        int offset = (i-1)%8;
        int bit_val = bitmap_buf[index] & (1 << offset);
        
        // print an entry if the block is free
        if (!bit_val) {
            printf("BFREE,%d\n", i);
        }
    }
}

int block_num_to_byte_offset(int block_num) {
    return block_num*block_size;
}

void cleanup_and_exit() {
    close(input_file_fd);
    exit(0);
}

void get_group_summary(int group_num) {
    // find which block is the group descriptor list located on
    int descr_block = (block_size > 1024) ? 1 : 2 ;

    // find the byte offset of the desired group descriptor
    int group_descr_offset = descr_block * block_size + group_num * sizeof(group_desc_struct);

    // read in group descriptor contents
    if (pread(input_file_fd, &group_desc_struct, sizeof(group_desc_struct), group_descr_offset) \
    != sizeof(group_desc_struct)) {
        error("lab3a error: failed to read entirety of group descriptor", 2);
    }

    // find total num blocks in group --> last group has residual blocks
    int blocks_in_group = (group_num == num_groups-1) 
                          ? (super_block_struct.s_blocks_count - (num_groups-1)*super_block_struct.s_blocks_per_group)
                          : super_block_struct.s_blocks_per_group ;

    // find total num inodes in group --> last group has residual inodes
    int inodes_in_group = (group_num == num_groups-1) 
                            ? (super_block_struct.s_inodes_count - (num_groups-1)*super_block_struct.s_inodes_per_group)
                            : super_block_struct.s_inodes_per_group ;

    // print:
    // GROUP
    // group number (decimal, starting from zero)
    // total number of blocks in this group (decimal)
    // total number of i-nodes in this group (decimal)
    // number of free blocks (decimal)
    // number of free i-nodes (decimal)
    // block number of free block bitmap for this group (decimal)
    // block number of free i-node bitmap for this group (decimal)
    // block number of first block of i-nodes in this group (decimal)
    printf("GROUP,%d,%d,%d,%d,%d,%d,%d,%d\n", \
        group_num, \
        blocks_in_group, \
        inodes_in_group, \
        group_desc_struct.bg_free_blocks_count, \
        group_desc_struct.bg_free_inodes_count, \
        group_desc_struct.bg_block_bitmap, \
        group_desc_struct.bg_inode_bitmap, \
        group_desc_struct.bg_inode_table
    );
}

void get_superblock_summary() {
    //read superblock
    if (pread(input_file_fd, &super_block_struct, sizeof(super_block_struct), 1024) \
    != sizeof(super_block_struct)) {
        error("lab3a: failed to read entirety of superblock", 2);
    }

    // calculate block size
    block_size = EXT2_MIN_BLOCK_SIZE << super_block_struct.s_log_block_size;
    
    // print:
    // SUPERBLOCK
    // total number of blocks (decimal)
    // total number of i-nodes (decimal)
    // block size (in bytes, decimal)
    // i-node size (in bytes, decimal)
    // blocks per group (decimal)
    // i-nodes per group (decimal)
    // first non-reserved i-node (decimal)
    printf("SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n", \
        super_block_struct.s_blocks_count, \
        super_block_struct.s_inodes_count, \
        block_size, \
        super_block_struct.s_inode_size, \
        super_block_struct.s_blocks_per_group, \
        super_block_struct.s_inodes_per_group, \
        super_block_struct.s_first_ino);

    // calculate # block groups for a global var to be used later
    num_groups = ((super_block_struct.s_blocks_count-1) / super_block_struct.s_blocks_per_group) + 1;
}

void error(char *msg, int exit_code) {
    perror(msg);
    exit(exit_code);
}

void open_file() {
    input_file_fd = -1;
    input_file_fd = open(input_file, O_RDONLY);

    if (input_file_fd < 0) {
        error("lab3a: unable to open image file", 1);
    }
}

void process_args(int argc, char* argv[]) {
    // var declarations
    char *usage = "usage: lab3a file_system_image\n";
    
    // only accept 1 command line arg
    if (argc != 2) {
        fprintf(stderr, "error: %s", usage);
        exit(1);
    }

    input_file = argv[1];
}