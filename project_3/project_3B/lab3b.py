#!/usr/bin/env python3

# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu

import sys
import csv


# GLOBAL VARS
errors = 0

# CLASS DEFINITIONS
class superblock:
    def __init__(self, entry):
        self.total_num_blocks = int(entry[1])
        self.total_num_inodes = int(entry[2])
        self.block_size = int(entry[3])
        self.inode_size = int(entry[4])
        self.blocks_per_group = int(entry[5])
        self.inodes_per_group = int(entry[6])
        self.first_nonreserved_inode = int(entry[7])

class groupdescriptor:
    def __init__(self, entry):
        self.group_num = int(entry[1])
        self.n_blocks = int(entry[2])
        self.n_inodes = int(entry[3])
        self.n_free_blocks = int(entry[4])
        self.n_free_inodes = int(entry[5])
        self.bn_block_bitmap = int(entry[6])
        self.bn_inode_bitmap = int(entry[7])
        self.bn_first_inode = int(entry[8])

class inode:
    def __init__(self, entry):
        self.inode_num = int(entry[1])
        self.file_type = entry[2]
        self.mode = octal_to_decimal(int(entry[3]))
        self.owner = int(entry[4])
        self.group = int(entry[5])
        self.link_count = int(entry[6])
        self.t_last_inode_change = entry[7]
        self.t_modification = entry[8]
        self.t_last_access = entry[9]
        self.file_size = int(entry[10])
        self.n_blocks = int(entry[11])

        self.blocks_list = self.get_blocks(entry)

    def get_blocks(self, entry):
        blocks_list = []
        if (self.file_type == 's' and self.file_size <= 60):
            return blocks_list
        else:
            for i in range(12, 27):
                blocks_list.append(int(entry[i]))
        
        return blocks_list

class dirent:
    def __init__(self, entry):
        self.dir_inode_num = int(entry[1])
        self.logical_byte_offset = int(entry[2])
        self.referenced_file_inode_num = int(entry[3])
        self.entry_length = int(entry[4])
        self.name_length = int(entry[5])
        self.name = entry[6]

class indirect:
    def __init__(self, entry):
        self.owner_inode_n = int(entry[1])
        self.indir_level = int(entry[2])
        self.logical_block_offset = int(entry[3])
        self.block_n_scanned_block = int(entry[4])
        self.block_n_referenced_block = int(entry[5])

# FUNCTIONS

def octal_to_decimal(octal_num):
    decimal_num = 0
    remaining_digits = octal_num
    base = 1
    
    while(remaining_digits):
        last = remaining_digits % 10
        remaining_digits = int(remaining_digits/10)
        decimal_num += last * base
        base *= 8

    return decimal_num

# returns the name of the csv summary file
def process_args():
    usage_msg = "Usage: lab3b file_sys_summary.csv"
    if (len(sys.argv) != 2):
        error("incorrect number of arguments\n" + usage_msg)

    return sys.argv[1]

# prints error message_string and exits with code 1
def error(message_string):
    print("lab3b error: " + message_string, file = sys.stderr)
    exit(1)

# returns parsed csv contents organized by type:
# superblock_list, group_list, freeblock_list, freeinode_list, inode_list, dirent_list, indir_list
def parse_csv_file(filename):
    # var initialization
    csv_contents = []
    
    # open csv file
    try:
        csvfile = open(filename)
    except:
        error("failed to open csv file")

    try:
        reader_object = csv.reader(csvfile, lineterminator = '\n')
    except:
        error("failed to create reader object for csv file")

    for x in reader_object:
        csv_contents.append(x)

    # close csv file when done
    try:
        csvfile.close()
    except:
        error("failed to close csv file")

    # separate lines by types
    superblock_list = []
    group_list = []
    freeblock_list = []
    freeinode_list = []
    inode_list = []
    dirent_list = []
    indir_list = []
    for x in csv_contents:
        if (x[0] == 'SUPERBLOCK'):
            superblock_list.append(superblock(x))
        elif (x[0] == 'GROUP'):
            group_list.append(groupdescriptor(x))
        elif (x[0] == 'BFREE'):
            freeblock_list.append(int(x[1]))
        elif (x[0] == 'IFREE'):
            freeinode_list.append(int(x[1]))
        elif (x[0] == 'INODE'):
            inode_list.append(inode(x))
        elif (x[0] == 'DIRENT'):
            dirent_list.append(dirent(x))
        elif (x[0] == 'INDIRECT'):
            indir_list.append(indirect(x))

    # check for bad csv contents
    if (len(superblock_list) < 1 or len(group_list) < 1):
        error("no superblock or group descriptor in csv file")

    return superblock_list, group_list, freeblock_list, freeinode_list, inode_list, dirent_list, indir_list

    # # populate the global vars
    # for x in csv_contents:
    #     if 'SUPERBLOCK' in x:
    #         my_superblock = superblock(x[1], x[2], x[3], x[4], x[5], x[6], x[7])
    #     if 'GROUP' in x:
    #         my_groupdescriptor = groupdescriptor(x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8])
    #     if 'BFREE' in x:
    #         free_block_nums_list.append[x[1]]
    #     if 'IFREE' in x:
    #         free_inode_nums_list.append[x[1]]
    #     if 'INODE' in x:
    #         if (len(x) < 13):
    #             new_inode = inode(x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11])
    #         else:
    #             new_inode = inode(x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], \
    #                 x[12], x[13], x[14], x[15], x[16], x[17], x[18], x[19], x[20], x[21], x[22], x[23], \
    #                     x[24], x[25], x[26])
    #         inode_list.append(new_inode)
    #     if 'DIRENT' in x:
    #         dirent_list.append(dirent(x[1], x[2], x[3], x[4], x[5], x[6]))
    #     if 'INDIRECT' in x:
    #         indir_list.append(indirect(x[1], x[2], x[3], x[4], x[5]))

    # # check for errors
    # if (my_superblock == None):
    #     error("lab3b error: no superblock found in file system summary")
    # if (my_groupdescriptor == None):
    #     error("lab3b error: no group descriptor found in file system summary")

# return a set containing the reserved block numbers
def get_first_unreserved_block(superblock_list, group_list):
    # extract necessary params
    first_b_inodes = group_list[0].bn_first_inode
    inode_size_in_bytes = superblock_list[0].inode_size
    n_inodes_in_group = group_list[0].n_inodes
    block_size = superblock_list[0].block_size

    # calculate where the non-reserved block nums start
    first_unreserved_block = first_b_inodes + inode_size_in_bytes * n_inodes_in_group / block_size
    return int(first_unreserved_block)

def block_consistency_audits(superblock_list, freeblock_list, inode_list, group_list, indir_list):
    # extract useful info to be used later
    max_block_num = superblock_list[0].total_num_blocks - 1
    first_unreserved_block = get_first_unreserved_block(superblock_list, group_list)

    n_pointers_per_block = get_n_pointers_per_block(superblock_list)
    used_block_dict = {} # block num --> set of file info lists

    # examine every block pointer in every inode
    used_block_dict = block_consistency_inodes(inode_list, freeblock_list, first_unreserved_block, \
        n_pointers_per_block, max_block_num, used_block_dict)

    # examine every block pointer in every indirect entry
    used_block_dict = block_consistency_indir(indir_list, freeblock_list, first_unreserved_block, \
        n_pointers_per_block, max_block_num, used_block_dict)

    # handle unreferenced and duplicate errors
    unreferenced_and_duplicate_errors(used_block_dict, freeblock_list, max_block_num, first_unreserved_block)

def unreferenced_and_duplicate_errors(used_block_dict, freeblock_list, max_block_num, first_unreserved_block):
    # iterate over all non-reserved blocks
    for block in range(first_unreserved_block, max_block_num+1):
        # blocks that are not free and not used are unreferenced
        if (block not in freeblock_list and block not in used_block_dict):
            print_unreferenced_error(block)
        
        # blocks that have multiple entries in used_block_dict are duplicates
        if (block in used_block_dict and len(used_block_dict[block]) > 1):
            for list in used_block_dict[block]:
                print_duplicate_error(get_indir_string(list[0]), list[1], list[2], list[3])

def block_consistency_inodes(inode_list, freeblock_list, first_unreserved_block, n_ptrs_per_block, \
    max_block_num, used_block_dict):
    for inode in inode_list:
        # if inode not used or it's a short length symlink, continue
        if (inode.mode == 0 or inode.link_count == 0 or (inode.file_type == 's' and inode.file_size <= 60)):
            continue

        for i in range(len(inode.blocks_list)):
            # don't process the block if its value is 0
            if (inode.blocks_list[i] == 0):
                continue
            
            # get the indirection string and offset
            if (i == 12):
                indirection_level = 1
                offset = 12
            elif (i == 13):
                indirection_level = 2
                offset = 12 + n_ptrs_per_block
            elif (i == 14):
                indirection_level = 3
                offset = 12 + n_ptrs_per_block + n_ptrs_per_block**2
            else:
                indirection_level = 0
                offset = i

            # check for invalid error or reserved error
            if (inode.blocks_list[i] < 0 or inode.blocks_list[i] > max_block_num):
                print_invalid_error(get_indir_string(indirection_level), inode.blocks_list[i], \
                    inode.inode_num, offset)
            elif (inode.blocks_list[i] < first_unreserved_block):
                print_reserved_error(get_indir_string(indirection_level), inode.blocks_list[i], \
                    inode.inode_num, offset)
            
            # check for allocated error
            if (inode.blocks_list[i] in freeblock_list):
                print_allocated_error(inode.blocks_list[i])

            # add to the dictionary
            if (inode.blocks_list[i] not in used_block_dict):
                used_block_dict[inode.blocks_list[i]] = []
            
            used_block_dict[inode.blocks_list[i]].append([indirection_level, inode.blocks_list[i], \
                inode.inode_num, offset])

    return used_block_dict

def block_consistency_indir(indir_list, freeblock_list, first_unreserved_block, n_ptrs_per_block, \
    max_block_num, used_block_dict):
    for indir in indir_list:
        indirection_level = indir.indir_level

        # check for invalid error or reserved error
        if (indir.block_n_referenced_block < 0 or indir.block_n_referenced_block > max_block_num):
            print_invalid_error(get_indir_string(indirection_level), indir.block_n_referenced_block, \
                indir.owner_inode_n, indir.logical_block_offset)
        elif (indir.block_n_referenced_block < first_unreserved_block):
            print_reserved_error(get_indir_string(indirection_level), indir.block_n_referenced_block, \
                indir.owner_inode_n, indir.logical_block_offset)
        
        # check for allocated error
        if (indir.block_n_referenced_block in freeblock_list):
            print_allocated_error(indir.block_n_referenced_block)

        # add to the dictionary
        if (indir.block_n_referenced_block not in used_block_dict):
            used_block_dict[indir.block_n_referenced_block] = []
        
        used_block_dict[indir.block_n_referenced_block].append([indirection_level, \
            indir.block_n_referenced_block, indir.owner_inode_n, indir.logical_block_offset])

    return used_block_dict

def print_invalid_error(indirection_string, block_num, inode_num, offset):
    print("INVALID " + indirection_string + "BLOCK " + str(block_num) + " IN INODE " \
        + str(inode_num) + " AT OFFSET " + str(offset))
    
    global errors
    errors += 1

def print_reserved_error(indirection_string, block_num, inode_num, offset):
    print("RESERVED " + indirection_string + "BLOCK " + str(block_num) + " IN INODE " \
        + str(inode_num) + " AT OFFSET " + str(offset))

    global errors
    errors += 1

def get_n_pointers_per_block(superblock_list):
    return int(superblock_list[0].block_size/4)

def get_indir_string(indir_level):
    if (indir_level == 1):
        return "INDIRECT "
    elif (indir_level == 2):
        return "DOUBLE INDIRECT "
    elif (indir_level == 3):
        return "TRIPLE INDIRECT "
    else:
        return ""
    
def print_unreferenced_error(block_num):
    print("UNREFERENCED BLOCK " + str(block_num))
    
    global errors
    errors += 1

def print_allocated_error(block_num):
    print("ALLOCATED BLOCK " + str(block_num) + " ON FREELIST")
    global errors
    errors += 1

def print_duplicate_error(indir_string, block_num, inode_num, offset):
    print("DUPLICATE " + indir_string + "BLOCK " + str(block_num) + " IN INODE " + str(inode_num) + \
        " AT OFFSET " + str(offset))
    global errors
    errors += 1

def inode_allocation_audits(freeinode_list, inode_list, superblock_list):
    # initial vars
    max_inode_num = superblock_list[0].total_num_inodes
    allocated_inode_nums = get_allocated_inode_nums(inode_list)
    first_nonreserved_inode = superblock_list[0].first_nonreserved_inode
    
    # cycle through all inodes in the group
    for inode_num in range(1, max_inode_num+1):
        if (inode_num in allocated_inode_nums and inode_num in freeinode_list):
            print_allocated_inode_error(inode_num)
        if (inode_num not in freeinode_list and inode_num >= first_nonreserved_inode \
            and inode_num not in allocated_inode_nums):
            print_unallocated_inode_error(inode_num)

# return a set containing the numbers of all allocated inodes, and a set with numbers of all unallocated
# inodes
def get_allocated_inode_nums(inode_list):
    # get allocated and 
    inode_nums_list = set()
    for inode in inode_list:
        if (inode.mode != 0):
            inode_nums_list.add(inode.inode_num)

    return inode_nums_list

def print_allocated_inode_error(inode_num):
    print("ALLOCATED INODE " + str(inode_num) + " ON FREELIST")
    global errors
    errors += 1

def print_unallocated_inode_error(inode_num):
    print("UNALLOCATED INODE " + str(inode_num) + " NOT ON FREELIST")
    global errors
    errors += 1

def directory_consistency_audits(superblock_list, inode_list, dirent_list, freeinode_list):
    # some initial vars for later
    max_inode_num = superblock_list[0].total_num_inodes
    file_ref_count = {}
    parent_tracker = {}
    parent_tracker[2] = 2
    double_dot_parent = {}

    for dirent in dirent_list:
        # extract some info
        file_inode = dirent.referenced_file_inode_num
        dir_inode = dirent.dir_inode_num
        file_name = dirent.name
        allocated_inode_nums = get_allocated_inode_nums(inode_list)

        # check for invalid inode number
        if (file_inode < 1 or file_inode > max_inode_num):
            print_invalid_dir_inode_error(dir_inode, file_name, file_inode)
    
        # check for unallocated inode
        if (file_inode in freeinode_list and file_inode not in allocated_inode_nums):
            print_unallocated_dir_inode_error(dir_inode, file_name, file_inode)

        # check for bad "." entry
        if (file_name == "'.'" and file_inode != dir_inode):
            print_self_or_parent_error(dir_inode, file_name, file_inode, dir_inode)

        # update the link tally for this inode
        if (file_inode not in file_ref_count):
            file_ref_count[file_inode] = 1
        else:
            file_ref_count[file_inode] += 1

        # record the parent of the inode
        if (file_name != "'..'" and file_name != "'.'"):
            parent_tracker[file_inode] = dir_inode
        elif (file_name == "'..'"):
            double_dot_parent[file_inode] = dir_inode

    # check for link count errors
    for inode in inode_list:
        if ((inode.inode_num not in file_ref_count) and inode.link_count != 0):
            print_bad_link_count_error(inode.inode_num, 0, inode.link_count)
        elif (inode.inode_num in file_ref_count and file_ref_count[inode.inode_num] != inode.link_count):
            print_bad_link_count_error(inode.inode_num, file_ref_count[inode.inode_num], inode.link_count)

    # verify the ".." dir entries are correct
    for dirent in dirent_list:
        # only consider relevant dirents
        if (dirent.name != "'..'"):
            continue

        # extract some vars
        file_name = dirent.name
        file_inode = dirent.referenced_file_inode_num
        dir_inode = dirent.dir_inode_num
        double_dot_dir_inode = double_dot_parent[file_inode]

        # check the ".." entries
        if (file_inode != parent_tracker[double_dot_dir_inode]):
            print_self_or_parent_error(dir_inode, file_name, file_inode, parent_tracker[double_dot_dir_inode])

def print_bad_link_count_error(inode_num, link_tally, link_count):
    print("INODE " + str(inode_num) + " HAS " + str(link_tally) + " LINKS BUT LINKCOUNT IS " + str(link_count))
    global errors
    errors += 1

def print_invalid_dir_inode_error(dir_inode, file_name, file_inode):
    print("DIRECTORY INODE " + str(dir_inode) + " NAME " + str(file_name) + " INVALID INODE " \
        + str(file_inode))
    global errors
    errors += 1

def print_unallocated_dir_inode_error(dir_inode, file_name, file_inode):
    print("DIRECTORY INODE " + str(dir_inode) + " NAME " + file_name + " UNALLOCATED INODE " \
        + str(file_inode))
    global errors
    errors += 1

def print_self_or_parent_error(dir_inode, file_name, file_inode, correct_inode):
    print("DIRECTORY INODE " + str(dir_inode) + " NAME " + file_name + " LINK TO INODE " + str(file_inode) \
        + " SHOULD BE " + str(correct_inode))
    global errors
    errors += 1

def exit_program():
    global errors
    if (errors != 0):
        exit(2)
    else:
        exit(0)

def main():
    # process command line args
    filename = process_args()

    # parse csv file
    superblock_list, group_list, freeblock_list, freeinode_list, inode_list, \
        dirent_list, indir_list = parse_csv_file(filename)

    # preform block consistency audits
    block_consistency_audits(superblock_list, freeblock_list, inode_list, group_list, indir_list)

    # perform inode allocation audits
    inode_allocation_audits(freeinode_list, inode_list, superblock_list)

    # perform directory consistency audits
    directory_consistency_audits(superblock_list, inode_list, dirent_list, freeinode_list)

    exit_program()


if __name__ == "__main__":
    main()