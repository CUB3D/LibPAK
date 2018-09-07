#ifndef LIBPAK_LIBRARY_H
#define LIBPAK_LIBRARY_H

// Define structs used globally

struct PAKFileEntry {
    int fileSize;
    int fileOffset;
    int hash;
    int index;
};

struct PAKPathEntry {
    int length;
    char* data;
    int index;
};

struct PAKHeader {
    int version;
    int fileCount;
    struct PAKFileEntry* files;
    struct PAKPathEntry* paths;
};

struct PAKFile {
    struct PAKHeader header;
    char* data;
};

void hello();

#endif