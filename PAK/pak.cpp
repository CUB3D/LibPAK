//
// Created by cub3d on 03/04/19.
//

#include <argp.h>
#include <LibPAK.h>
#include <filesystem>
#include <fstream>

const static char doc[] = "";
const static char args_doc[] = "-i [IN] -o [OUT]";

const struct argp_option options[] = {
        {"input", 'i', "dir", 0, "Select the input dir."},
        {"output", 'o', "file", 0, "Select the output file."},
        {nullptr}
};

struct arguments {
    char* inputDir;
    char* outputFile;
};

static error_t parse_opt(int key, char* arg, struct argp_state* state) {
    auto* args = static_cast<arguments *>(state->input);
    switch(key) {
        case 'i': args->inputDir = arg;
        case 'o': args->outputFile = arg;
        case ARGP_KEY_ARG: return 0;
        default: return ARGP_ERR_UNKNOWN;
    }
}

static struct argp argp = {options, parse_opt, args_doc, doc, nullptr, nullptr, nullptr};


void buildPAK(struct arguments args) {
    std::filesystem::path inputPath(args.inputDir);
    std::filesystem::recursive_directory_iterator srcItr(inputPath);

    int size = 0;
    int fileSize = 0;

    for(auto& child: srcItr) {
        size++;
        fileSize += child.file_size();
    }

    srcItr = std::filesystem::recursive_directory_iterator(inputPath);

    PAKFile pak{};
    initPAK(&pak, size, fileSize); // Add buffer space just in case file size changes next loop

    for(auto& child : srcItr) {
        auto f = addFile(&pak, child.file_size(), child.path().lexically_relative(inputPath).c_str());

        char* filedata = static_cast<char *>(malloc(child.file_size()));

        std::ifstream childReader(child.path());
        childReader.read(filedata, child.file_size());

        setFileData(&pak, f, filedata, child.file_size());
    }
    finallizePAK(&pak);
    writePak(pak);
}



int main(int argc, char** argv) {
    // Handle user input
    struct arguments args{};
    args.inputDir = ".";
    args.outputFile = "out.pak";
    argp_parse(&argp, argc, argv, 0, nullptr, &args);

    printf("Building PAK from '%s'\n", args.inputDir);
    printf("Writing output to '%s'\n", args.outputFile);

    buildPAK(args);

    return 0;
}