//
// Created by fly0ut on 6/22/21.
//

#include "../include/commands.h"


    si shell::cd(std::string newPath) {
        fs::path dirPath(newPath);

        if (!fs::is_directory(dirPath)) {
            std::cout << "Not a valid path: " << newPath << "\n";
            return 1;
        }

        fs::current_path(dirPath);
        this->wd = dirPath;

        return 0;
    }

    std::string  shell::getWD(){
        return this->wd;
    }

    int shell::ls() {

        if(this->wd.empty()){std::cout << "Error: director notset!\n";return 1;}

        std::stringstream  fileStream;
        std::stringstream  dirStream;
        //Programme hangs if the buffer stream is read while empty.
        fileStream << " ";
        dirStream << " ";
        for(auto& e : fs::directory_iterator(fs::current_path())) {
            if(e.is_regular_file() && !e.is_directory())
                fileStream << e.path().string().erase(0, fs::current_path().string().size()) << ", ";
            if(e.is_directory())
                dirStream << e.path().string().erase(0, fs::current_path().string().size()) << ", ";
        }

        std::cout << "files: " << fileStream.rdbuf() << "\n" << "Directories: " << dirStream.rdbuf() << "\n\n";

        return 0;
    }

    int shell::ls_r() {

        if(this->wd.empty()){std::cout << "Error: director notset!\n";return 1;}
        std::stringstream  fileStream;
        std::stringstream  dirStream;
        //Programme hangs if the buffer stream is read while empty.
        fileStream << " ";
        dirStream << " ";
        for(auto& e : fs::recursive_directory_iterator(fs::current_path())) {
            if(e.is_regular_file())
                fileStream << e.path().string().erase(0, fs::current_path().string().size()) << ", ";
            if(e.is_directory())
                dirStream << e.path().string().erase(0, fs::current_path().string().size()) << ", ";
        }
        std::cout << "files: " << fileStream.rdbuf() << "\n" << "Directories: " << dirStream.rdbuf() << "\n\n";


        return 0;
    }

    int shell::rm(std::string fileToRemove){
        fs::path file(fs::current_path() / fileToRemove);

        if(this->wd.empty()){
            std::cout << "Error: working director is not set!\n";
            return 1;
        }
        if(!is_regular_file(file)){
            std::cout << "Error finding file: " << file << "\n";
            return 1;
        }
        fs::remove(file);

        return 0;
    }

    int shell::hexdump(fs::path inputPath){
        std::ifstream inputStream((fs::current_path() / inputPath).string(), std::ios::in | std::ios::binary);

        if(this->wd.empty()){
            std::cout << "Error: working director is not set!\n";
            return 0;
        }

        if(!inputStream || !inputStream.is_open()){
            std::cout << "Error opening input file: " << inputPath;
            return 1;
        }

        std::cout << "Hexdump of file " << inputPath << "\n*************************************\n";


        std::vector<si> bufferHex;
        std::vector<si> bufferOffset;
        std::vector<char> bufferACII;
        //You don't want to be calling COUT all the time since it will need the kernel to respond.
        std::stringstream hexDumpDisplayBuffer;

        inputStream.seekg(0, std::ios::end);
        const size_t fileSize = inputStream.tellg();
        inputStream.seekg(0, std::ios::beg);

        const static size_t chunckSize = 500000;
        std::vector<char> buffer(chunckSize);
        si ii = 0;
        const si chunkNum = fileSize/chunckSize;
        const size_t tail = fileSize%chunckSize;


        if(chunkNum>0)
            for(si i=0; inputStream.good() && i<chunkNum;i++){
             inputStream.read(buffer.data(), chunckSize);
             std::transform(buffer.begin(), buffer.end(), std::back_inserter(bufferHex), [](char &c){return (int)c;});
             std::for_each(bufferHex.begin(), bufferHex.end(),
                           [&ii, &hexDumpDisplayBuffer](si &e)
                          {   ii++;
                              hexDumpDisplayBuffer << std::hex<< e;
                              if(ii%8==0 && ii%32 != 0){hexDumpDisplayBuffer << "    ";}
                              else if(ii%32==0){hexDumpDisplayBuffer << "\n";}
                          });
              ii=0;
              std::cout  << hexDumpDisplayBuffer.rdbuf();
              hexDumpDisplayBuffer.str(std::string());
              bufferHex.clear();
            }

        if(tail != 0) {
            buffer.resize(tail);
            inputStream.read(buffer.data(), tail);
            std::transform(buffer.begin(), buffer.end(), std::back_inserter(bufferHex), [](char &c){return (int)c;});
            std::for_each(bufferHex.begin(), bufferHex.end(),
                          [&ii, &hexDumpDisplayBuffer](si &e)
                          {   ii++;
                              hexDumpDisplayBuffer << std::hex << e;
                              if(ii%8==0 && ii%32 != 0){hexDumpDisplayBuffer << "    ";}

                              if(ii%32==0){hexDumpDisplayBuffer << "\n";}
                          });
            ii=0;

            std::cout  << hexDumpDisplayBuffer.rdbuf();
            hexDumpDisplayBuffer.str(std::string());
            buffer.clear();
            bufferHex.clear();
        }

        std::cout << "\n****************************\nEnd of hexdump\n\n";

        return 0;
    };

    int shell::xorCrypt(fs::path inputPath, fs::path outputPath, std::string keySeed){
        std::ifstream inputFile((fs::current_path() / inputPath).string(), std::ios::in | std::ios::binary);
        std::ofstream outputFile((fs::current_path() /  outputPath).string(), std::ios::out | std::ios::binary | std::ios::trunc);

        if(this->wd.empty()){
            std::cout << "Error: working director is not set!\n";
            return 0;
        }

        if(!inputFile && !inputFile.is_open()) {
            std::cout << "Error opening input file: " << inputPath << "\n";
            return 1;
        }

        if(!outputFile && !outputFile.is_open()) {
            std::cout << "Error opening output file: " << outputPath << "\n";
            return 1;
        }

        std::string  keyString = (this->genKey(keySeed));
        char *keyBuffer = (char*)(malloc(sizeof(*keyBuffer)*keyString.size()));
        memcpy(keyBuffer, keyString.c_str(), keyString.size());
        char *buffer = (char*)malloc(sizeof(*buffer)*this->keyBufSize);

        inputFile.seekg(0, std::ios::end);
        const size_t fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        outputFile.seekp(0, std::ios::beg);

        const ui rep = fileSize/this->keyBufSize;
        const size_t tail = fileSize%this->keyBufSize;

        if(keyBufSize <= fileSize) {
            for (si i = 0; i < rep && inputFile.good(); i++) {
                inputFile.read(buffer, this->keyBufSize);
                outputFile.write(this->char_prt_xor_256_bytes(buffer, keyBuffer, this->keyBufSize), this->keyBufSize);
            }
        }

        if(tail != 0) {
            inputFile.read(buffer, tail);
            outputFile.write(this->char_prt_xor_256_bytes(buffer, keyBuffer, tail), tail);
        }

        inputFile.close();
        outputFile.close();

        std::cout << "\nContent of un-ciphered file: \n**************************************\n";
        this->cat(inputPath);
        std::cout << "\n*****************************\nEnd of contents.\n\n";

        std::cout << "\nContent of xor ciphered file: \n**************************************\n";
        this->cat(outputPath);
        std::cout << "\n*****************************\nEnd of contents.\n\n";

        std::free(keyBuffer);
        std::free(buffer);

        return 0;
    };

    int shell::xorDecrypt(const fs::path& inputPath, const fs::path& outputPath, std::string keySeed) {
        std::ifstream inputFile((fs::current_path() / inputPath).string(), std::ios::in | std::ios::binary);
        std::ofstream outputFile((fs::current_path() / outputPath).string(), std::ios::out | std::ios::binary | std::ios::trunc);

        if(this->wd.empty()){
            std::cout << "Error: working director is not set!\n";
            return 0;
        }

        if(!inputFile && !inputFile.is_open()) {
            std::cout << "Error opening input file: " << inputPath << "\n";
            return 1;
        }

        if(!outputFile && !outputFile.is_open()) {
            std::cout << "Error opening output file: " << outputPath << "\n";
            return 1;
        }



        std::string  keyString = (this->genKey(keySeed));
        char *keyBuffer = (char*)(malloc(sizeof(*keyBuffer)*keyString.size()));
        memcpy(keyBuffer, keyString.c_str(), keyString.size());
        char *buffer = (char*)malloc(sizeof(*buffer)*this->keyBufSize);

        inputFile.seekg(0, std::ios::end);
        const size_t fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);
        outputFile.seekp(0, std::ios::beg);

        const ui rep = fileSize/this->keyBufSize;
        const size_t tail = fileSize%this->keyBufSize;


        if(keyBufSize < fileSize) {
            for (si i = 0; i < rep && inputFile.good(); i++) {
                inputFile.read(buffer, this->keyBufSize);
                outputFile.write(this->char_prt_xor_256_bytes(keyBuffer, buffer, this->keyBufSize), this->keyBufSize);
            }
        }

        //ToDo need to input part of the XOR
        if(tail != 0) {
            inputFile.read(buffer, tail);
            outputFile.write(this->char_prt_xor_256_bytes(keyBuffer, buffer, tail), tail);
        }

        inputFile.close();
        outputFile.close();

        std::free(keyBuffer);
        std::free(buffer);

        std::cout << "\nContent of xor ciphered file: \n********************************************\n";
        this->cat(inputPath);
        std::cout << "\n************************************\nEnd of contents.\n\n";
        std::cout << "\nContent of deciphered file: \n********************************************\n";
        this->cat(outputPath);
        std::cout << "\n************************************\nEnd of contents.\n\n";

        return 0;
    };

    std::string shell::genKey(std::string keySeed) {
        const char *keySeed_c_str=keySeed.c_str();
        char *key = (char*)malloc(sizeof(*key)*this->keyBufSize);
        size_t seedSize= keySeed.size();

        if(seedSize > this->keyBufSize){
            std::cout << "password needs to be shorter then 256 bytes\n";
            return "";
        }

        ui rep = this->keyBufSize/seedSize;
        size_t tail = this->keyBufSize-seedSize*rep;

        for(si i=0; i < rep; i++) {
            std::memcpy(key+(seedSize*i), keySeed_c_str, seedSize);
        }
        if(tail != 0){
            std::memcpy(key+(rep*seedSize), keySeed_c_str, tail);
        }

        std::string stringKey(key);
        std::free(key);

        return stringKey;
    }

    //Can't use int for this. The size of a int changes from 64bit to 32bit systems. 32bits to 16bits respectively.
    char *shell::char_prt_xor_256_bytes(char *lhs, char *rhs, size_t charSize){
        u64 outu64=0;
        char *outCharPoint = (char*)malloc(256);

        for(int i=0;i<(256/sizeof(u64)); i++){
            outu64 = *reinterpret_cast<unsigned long long int *>(rhs+i*sizeof(u64)) ^ *reinterpret_cast<unsigned long long int *>(lhs+i*sizeof(u64));
            memcpy(outCharPoint+sizeof(u64)*i, (reinterpret_cast<char *>(&outu64)), sizeof(u64));
        }

        char* outPut = (char *)malloc(charSize);
        memcpy(outPut, outCharPoint, charSize);
        std::free(outCharPoint);
        return outPut;
    }

    std::string shell::prepLength(std::string key, size_t fileSize){
        std::string  prepKey;
        for(si i=0;i<(si)(fileSize/key.size());i++){
            prepKey.append(key);
        }
        prepKey.append(key.substr(0, fileSize%key.size()));

        return prepKey;
    }

    //Code found from this line to this line+19 is from the user "Anshul Jain" here https://stackoverflow.com/questions/12671510/xor-on-two-hexadeicmal-values-stored-as-string-in-c .
    //It is not being used to xor files normal, just to compare my method of XOR to, so I can time my way vs a more high level cpp way.
    std::string  shell::char_prt_xor_256_bytes_normal(const std::string &lhs,const std::string &rhs){
        std::string xorOut;

        assert(lhs.size()==rhs.size());

        std::transform(lhs.begin(), lhs.end(),
                       rhs.begin(), std::back_inserter(xorOut),
                       Xor<std::string::value_type>());

        return xorOut;
    }
    //end to code taken from stackoverflow used for comparison.

    //ToDo: Stub
    si shell::vaildater(fs::path og, fs::path dr){
        return 0;
        std::string ogHash, drHash;
        ogHash = std::system(("sha256 "+og.string()).c_str());
        drHash = std::system(("sha256sum "+dr.string()).c_str());
        return 1;
    };

    si shell::touch(fs::path writePath){

        if(this->wd.empty()){
            std::cout << "Error: working director is not set!\n";
            return 0;
        }

        if(exists((fs::current_path() / writePath))) {
            std::cout << "Error: File already exist!\n";
            return 0;
        }

        std::ofstream fileCreateStream((fs::current_path() / writePath).string(), std::ios::out);

        if(fileCreateStream.bad()){
            std::cout << "Error: Failed opening file: " << writePath << "\n";
            return 0;
        }

        fileCreateStream << "";

        fileCreateStream.close();

        return 1;
    }

    si shell::cat(fs::path readPath){
        std::ifstream outStream((fs::current_path() / readPath).string(), std::ios::in);

        if(this->wd.string().size() <= 0){
            std::cout << "Error: working director is not set!\n";
            return 0;
        }

        if(outStream.fail() || !outStream.is_open()){
            std::cout << "Failed opening file for reading :" << readPath << "\n";
            return 1;
        }

        std::string readBuffer;

        while(outStream.good()){
            std::getline(outStream, readBuffer);
            std::cout << readBuffer;
        }

        0;
    }

    si shell::cyptTimer() {
        std::string fakeFile = "fafa9rnuq09c-3wjxn4q3oexaiurcenufad98jxnq9sjxqj0q9ruewaxirpn32  0xudpq c9rxqnuzqnosfd9un09328rc42rqnrcoewc\nqnuc9w8ec9rqc8yyrcn982qcyr28c\n ucd209n832409284qn09c9w8uq8xyr948pmxi32q[-ndsajdaoihd roiewopinrcucpnqomxanocmxioeinucamrxopqinmxqoieuroewflkdjsafdoieawjroiejda;ofdjewafowfiwc0n";
        for(si i=0;i<10;i++)
            fakeFile.append(fakeFile);
        char *fakeFileLowLevel = (char *)malloc(fakeFile.size());
        const char* constToNot = fakeFile.c_str();
        memcpy(fakeFileLowLevel, constToNot, fakeFile.size());
        std::string key =this->genKey("gottaGoFast12343242");
        std::string keyPrep = this->prepLength(key, fakeFile.size());

        auto f1start = std::chrono::high_resolution_clock::now();
        //Low level xor
        size_t rep = fakeFile.size()/this->keyBufSize;
        size_t tail = fakeFile.size()%this->keyBufSize;
        si i=0;
         for (; i < rep; i++) {
             this->char_prt_xor_256_bytes(key.data(), fakeFileLowLevel+i*256, 256);
         }
        if(tail != 0) {
            this->char_prt_xor_256_bytes(key.data(), fakeFileLowLevel+i*256+tail, tail);
        }
        auto f1stop = std::chrono::high_resolution_clock::now();
        auto f1duration = duration_cast<std::chrono::nanoseconds>(f1stop - f1start);

        //Running this here to make sure the compile of the template function doesn't slow it down.
        this->char_prt_xor_256_bytes_normal(keyPrep, fakeFile);
        auto f2start = std::chrono::high_resolution_clock::now();
        //Need to include data chunking/parsing for each algorithm since it is indicative of compute time i use.
        //i.e rep and tail subsetting for the low level way.
        key = this->prepLength(key, fakeFile.size());
        //High level xor
        this->char_prt_xor_256_bytes_normal(key, fakeFile);
        auto f2stop = std::chrono::high_resolution_clock::now();
        auto f2duration = duration_cast<std::chrono::nanoseconds>(f2stop - f2start);

        std::string howMuchFaster  = (f1duration.count() < f2duration.count()) ? "Low level(my) way is "+std::to_string(f2duration.count()/f1duration.count())+" times faster then the high level.\n" :
                "High level way is " +std::to_string(f1duration.count()/f2duration.count())+" times faster then low level.\n";

        std::cout << "Low level(my) way is " << f1duration.count() <<" nanoseconds long and High level(recommend) way is " << f2duration.count() << " nanoseconds long\n"
        << "Data size used for the fake file is " << fakeFile.size() <<" charters long.\n" << howMuchFaster;

        return 0;
    }


    std::string shell::getEnvVar( const std::string & varName ) {
        const char * envVar = std::getenv(varName.data());
        if ( envVar == nullptr ) { // invalid to assign nullptr to std::string
            return "";
        }
        return envVar;
    }

    si shell::sanatizeToNums(std::string dirtySting) {
        return !dirtySting.empty() && std::find_if(dirtySting.begin(), dirtySting.end(), [](char &e){return !std::isdigit(e);}) == dirtySting.end();
    }