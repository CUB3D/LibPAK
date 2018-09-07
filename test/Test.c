//
// Created by cub3d on 06/09/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "LibPAK.h"

#define max(a, b) a < b ? b : a

void readPak(struct PAKFile* file, const char* filepath) {
    FILE* fileHandle = fopen(filepath, "rb");

    if(!fileHandle) {
        printf("Invalid file\n");
    }

    // Make sure starts with "PAK"
    char tmp[3];
    fread(tmp, sizeof(char), 3, fileHandle);
    if(strcmp(tmp, "PAK") != 0) {
        printf("File is not a pak file\n");
        return;
    }

    // Read the header
    fread(&file->header.version, sizeof(int), 1, fileHandle);
    fread(&file->header.fileCount, sizeof(int), 1, fileHandle);

    // Create buffers
    file->header.files = (struct PAKFileEntry*) malloc(sizeof(struct PAKFileEntry) * file->header.fileCount);
    file->header.paths = (struct PAKPathEntry*) malloc(sizeof(struct PAKPathEntry) * file->header.fileCount);
    
    // Read the file list
    for(int i = 0; i < file->header.fileCount; i++) {
        struct PAKFileEntry* entry = &file->header.files[i];

        fread(&entry->fileSize, sizeof(int), 1, fileHandle);
        fread(&entry->fileOffset, sizeof(int), 1, fileHandle);
        fread(&entry->hash, sizeof(int), 1, fileHandle);
        entry->index = i;
    }

    // Read the paths
    for (int i = 0; i < file->header.fileCount; i++) {
        struct PAKPathEntry* path = &file->header.paths[i];
        path->index = i;

        fread(&path->length, sizeof(int), 1, fileHandle);

        path->data = (char*) malloc(path->length * sizeof(char));

        fread(&path->data[0], sizeof(char), path->length, fileHandle);
    }

    // Find size of remaining file
    int currentPosition = ftell(fileHandle);
    fseek(fileHandle, 0, SEEK_END);
    int size = ftell(fileHandle) - currentPosition;
    fseek(fileHandle, currentPosition, SEEK_SET);

    file->data = (char*) malloc(sizeof(char) * size);
    fread(file->data, sizeof(char), size, fileHandle);
}

void writePak(struct PAKFile file) {
    FILE* fileHandle = fopen("Test.pak", "wb");

    // Write header
    fwrite("PAK", sizeof(char), 3, fileHandle);
    fwrite(&file.header.version, sizeof(int), 1, fileHandle);
    fwrite(&file.header.fileCount, sizeof(int), 1, fileHandle);

    int totalFileSize = 0;

    // Then write file list
    for(int i = 0; i < file.header.fileCount; i++) {
        fwrite(&file.header.files[i].fileSize, sizeof(int), 1, fileHandle);
        fwrite(&file.header.files[i].fileOffset, sizeof(int), 1, fileHandle);
        fwrite(&file.header.files[i].hash, sizeof(int), 1, fileHandle);

        totalFileSize += file.header.files[i].fileSize;
    }

    // Write the path data
    for (int i = 0; i < file.header.fileCount; ++i) {
        struct PAKPathEntry* path = &file.header.paths[i];

        fwrite(&path->length, sizeof(int), 1, fileHandle);
        fwrite(path->data, sizeof(char), path->length, fileHandle);
    }

    // Then write the file data
    fwrite(file.data, sizeof(char), totalFileSize, fileHandle);

    fclose(fileHandle);
}

void initPAK(struct PAKFile* file, int fileCount, int maxFileSize) {
    file->header.version = 1;
    file->header.fileCount = fileCount;

    file->header.files = (struct PAKFileEntry*) malloc(sizeof(struct PAKFileEntry) * file->header.fileCount);
    file->header.paths = (struct PAKPathEntry*) malloc(sizeof(struct PAKPathEntry) * file->header.fileCount);
    file->data = (char*) malloc(sizeof(char) * maxFileSize);
}

struct PAKFileEntry* createFile(struct PAKFile *file, int index, int size) {
    struct PAKFileEntry* entry = &file->header.files[index];
    entry->fileSize = size;

    //TODO: generate hashes
    entry->hash = 0;
    entry->index = index;

    // Find the next empty space, travel over the size of each file before this one
    for(int i = 0; i < index; i++) {
        entry->fileOffset += file->header.files[i].fileSize;
    }

    file->header.paths[index].index = index;

    return entry;
}

int getUsedFileCount(struct PAKFile* file) {
    int count = 0;
    for (int i = 0; i < file->header.fileCount; ++i) {
        if(file->header.files[i].fileSize > 0)
            count++;
    }

    return count;
}

void setPathData(struct PAKFile* file, struct PAKFileEntry* entry, const char* str) {
    struct PAKPathEntry* path = &file->header.paths[entry->index];
    path->length = strlen(str);
    path->data = (char*) malloc(path->length * sizeof(char));
    memcpy(&path->data[0], str, path->length);
    path->index = entry->index;
}

void finallizePAK(struct PAKFile* file) {
    // First remove any empty files

    struct PAKFileEntry* oldFileList = file->header.files;
    struct PAKPathEntry* oldPathList = file->header.paths;
    int oldFileCount = file->header.fileCount;
    char* oldData = file->data;

    // Get the number of real files as well as total size
    int notEmptyFileCount = 0;
    int totalSize = 0;

    for (int i = 0; i < file->header.fileCount; ++i) {
        if(file->header.files[i].fileSize > 0) {
            notEmptyFileCount++;
            totalSize += file->header.files[i].fileSize;
        }
    }

    // Generate new file list and file count
    initPAK(file, notEmptyFileCount, totalSize);

    // fill in files
    for (int i = 0, j=0; i < oldFileCount; ++i) {
        if(oldFileList[i].fileSize > 0) {
            struct PAKFileEntry oldFile = oldFileList[i];

            // Add the new file
            struct PAKFileEntry* newFile = createFile(file, j, oldFile.fileSize);
            struct PAKPathEntry* newPath = &file->header.paths[j];

            // Set its path data
            setPathData(file, newFile, oldPathList[i].data);
            newPath->index = j;

            // Move its data to the new data buffer
            memcpy(&file->data[newFile->fileOffset], &oldData[oldFile.fileOffset], oldFile.fileSize);

            j++;
        }
    }

    // Get rid of old file list
    free(oldFileList);
    free(oldPathList);
    free(oldData);
}

void setFileData(struct PAKFile* file, struct PAKFileEntry* entry, char* data, int size) {
    memcpy(&file->data[entry->fileOffset], data, size);
}

struct PAKFileEntry* addFile(struct PAKFile* file, int size, const char* path) {
    struct PAKFileEntry* newFile = createFile(file, getUsedFileCount(file), size);
    setPathData(file, newFile, path);
    return newFile;
}

struct PAKFileEntry* getFileFromPath(struct PAKFile* file, const char* path) {
    for (int i = 0; i < file->header.fileCount; ++i) {
        int length = max(strlen(path), file->header.paths[i].length);
        if(memcmp(file->header.paths[i].data, path, length) == 0) {
            return &file->header.files[i];
        }
    }
}

void setFileDataP(struct PAKFile* file, const char* path, char* data, int size) {
    setFileData(file, getFileFromPath(file, path), data, size);
}

void getFileData(struct PAKFile* file, struct PAKFileEntry* entry, char* outbuffer) {
    memcpy(outbuffer, &file->data[entry->fileOffset], entry->fileSize);
}

int main() {
    // Create a pak file

    struct PAKFile file;

    initPAK(&file, 25000, 1024 * 1024 * 256);

    // Add the first file
    char* str = "Hello, World!\0";

    for(int i = 0; i < 3000; i++) {
        struct PAKFileEntry *entry = addFile(&file, 14 * sizeof(char), "/test/path/here\0");
        setFileData(&file, entry, str, strlen(str));
    }

    finallizePAK(&file);
    writePak(file);

    struct PAKFile inFile;
    readPak(&inFile, "Test.pak");

    for (int i = 0; i < inFile.header.fileCount; ++i) {
        printf("%s - ", inFile.header.paths[i].data);
        char dat[100];
        getFileData(&inFile, &inFile.header.files[i], dat);
        printf("%s\n", dat);
    }
}