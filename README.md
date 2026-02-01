Mini File System Simulator in C

This project is a simple in-memory file system simulator written in C. It mimics basic file system behavior such as directories, files, permissions, and disk limits. The goal of this project was to understand how file systems organize and manage data internally.

Overview

The simulator provides a command-line interface where users can create directories and files, navigate through folders, and perform read/write operations. All data is managed using dynamic memory and linked data structures.

Features

- Hierarchical directory structure
- Create, read, write, and delete files
- Directory navigation (mkdir, cd, pwd)
- File permissions (read/write)
- Disk usage limit simulation
- Tree view of directory structure
- Save and load disk usage

Concepts Demonstrated

- Dynamic memory allocation in C
- Linked lists and tree structures
- Basic file system design concepts
- Command-line interface handling
- Safe string handling

Build and Run

Compile:
```bash
gcc -Wall mini_fs_max.c -o mini_fs

Run:

./mini_fs
```
Example Usage

```bash
mkdir docs  
cd docs  
create file1  
write file1 HelloWorld  
read file1  
ls  
tree  
```
Author

Vignesh S
