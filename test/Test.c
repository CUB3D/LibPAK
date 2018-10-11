//
// Created by cub3d on 06/09/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "LibPAK.h"

char* strs[5] = {
    "This is a test",
    "This is also a test",
    "Realllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllly long string",
    "s", // smol string
    "Hello, World!"
};

int main() {
    // Create a pak file
    struct PAKFile file;

    initPAK(&file, 3001, 1024 * 1024 * 256);

    // Add the first file
    for(int i = 0; i < 3000; i++) {
        char* str = strs[i % 5];
        struct PAKFileEntry *entry = addFile(&file, strlen(str) + 1, str);
        setFileData(&file, entry, str, strlen(str) + 1);
    }

    finallizePAK(&file);
    writePak(file);

    struct PAKFile inFile;
    readPak(&inFile, "Test.pak");

    for (int i = 0; i < inFile.header.fileCount; ++i) {
        printf("%s - ", inFile.header.paths[i].data);
        printf("String of length: %d - ", inFile.header.files[i].fileSize);
        char dat[inFile.header.files[i].fileSize * sizeof(char)];
        getFileData(&inFile, &inFile.header.files[i], dat);
        printf("%s\n", dat);
    }
}