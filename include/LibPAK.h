#ifndef LIBPAK_LIBRARY_H
#define LIBPAK_LIBRARY_H

#include "Definitions.h"

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


EXPORT void readPak(struct PAKFile* file, const char* filepath);
EXPORT void writePak(struct PAKFile file);

EXPORT void initPAK(struct PAKFile* file, int fileCount, int maxFileSize);
EXPORT struct PAKFileEntry* createFile(struct PAKFile *file, int index, int size);
EXPORT void setPathData(struct PAKFile* file, struct PAKFileEntry* entry, const char* str);
EXPORT void finallizePAK(struct PAKFile* file);
EXPORT void setFileData(struct PAKFile* file, struct PAKFileEntry* entry, char* data, int size);
EXPORT struct PAKFileEntry* addFile(struct PAKFile* file, int size, const char* path);
EXPORT struct PAKFileEntry* getFileFromPath(struct PAKFile* file, const char* path);
EXPORT void setFileDataP(struct PAKFile* file, const char* path, char* data, int size);
EXPORT void getFileData(struct PAKFile* file, struct PAKFileEntry* entry, char* outbuffer);

#endif