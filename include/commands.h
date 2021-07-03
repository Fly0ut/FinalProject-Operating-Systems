//
// Created by fly0ut on 6/22/21.
//

#ifndef FINALPROJECT_COMMANDS_H
#define FINALPROJECT_COMMANDS_H
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cstring>
#include <chrono>
#include "typeAlias.h"

namespace fs = std::filesystem;

class shell {

private:
    const static size_t keyBufSize = 256;
    fs::path wd;

public:

    shell()= default;

    si cd(std::string newPath);

    std::string  getWD();

    int ls();

    int ls_r();

    int rm(std::string fileToRemove);

    int hexdump(fs::path inputPath);

    int xorCrypt(fs::path inputPath, fs::path outputPath, std::string keySeed);

    int xorDecrypt(const fs::path& inputPath, const fs::path& outputPath, std::string keySeed);

    std::string genKey(std::string keySeed);

    //Can't use int for this. The size of a int changes from 64bit to 32bit systems. 32bits to 16bits respectively.
    char *char_prt_xor_256_bytes(char *lhs, char *rhs, size_t charSize);

    std::string prepLength(std::string key, size_t fileSize);

    //Code found from this line to this line+19 is from the user "Anshul Jain" here https://stackoverflow.com/questions/12671510/xor-on-two-hexadeicmal-values-stored-as-string-in-c .
    //It is not being used to xor files normal, just to compare my method of XOR to, so I can time my way vs a more high level cpp way.
    template <typename T>
    struct Xor : std::binary_function<T, T, T> {
        T operator() (T a, T b) {
            return a ^ b;
        }
    };

    std::string  char_prt_xor_256_bytes_normal(const std::string &lhs,const std::string &rhs);
    //end to code taken from stackoverflow used for comparison.

    //ToDo: Stub
    si vaildater(fs::path og, fs::path dr);

    si touch(fs::path writePath);

    si cat(fs::path readPath);

    si cyptTimer();

    std::string getEnvVar( const std::string & varName );

    si sanatizeToNums(std::string dirtySting);


};
#endif //FINALPROJECT_COMMANDS_H
