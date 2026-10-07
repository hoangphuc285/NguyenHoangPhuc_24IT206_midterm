# Custom UNIX `ls(1)` Implementation

**Student Name:** Nguyen Hoang Phuc  
**Student ID:** 24IT206  
**Repository URL:** https://github.com/hoangphuc285/NguyenHoangPhuc_24IT206_midterm.git

## Overview
This project implements a subset of the NetBSD `ls(1)` command line utility in C.

## Implemented Features & Flags
- `-A`, `-a`: Includes directory entries (ignoring `.` and `..` with `-A`)[cite: 1].
- `-c`, `-u`: Sort/display time by status change (`-c`) or last access time (`-u`)[cite: 1, 2].
- `-d`: List directories as plain files without recursively searching them[cite: 1].
- `-F`: Append file type indicators (`/`, `*`, `@`, `=`, `|`)[cite: 1].
- `-f`: Output directory contents unsorted[cite: 1].
- `-h`, `-k`: Block size and file size unit outputs[cite: 1].
- `-i`: Print file serial number (inode)[cite: 1].
- `-l`, `-n`: Output long format with symbolic names or numeric IDs[cite: 1, 2].
- `-q`, `-w`: Handle non-printable characters with `?` or print raw bytes[cite: 1, 2].
- `-R`: Recursively display subdirectories encountered[cite: 1].
- `-r`, `-S`, `-t`: Sort options by reverse order, file size, or timestamp[cite: 1, 2].
- `-s`: Display system blocks consumed per file[cite: 1].

## Compilation and Running
To build the project executable:
```bash
make

