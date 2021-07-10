//
// Created by fly0ut on 6/22/21.
//

#include <cstdlib>
#include <iostream>
#include <limits>
#include <QApplication>
#include "../include/commands.h"
#include "../include/typeAlias.h"
#include "../include/pageFaultSim.h"
#include "../qtTemplates/PageFaultDisplay.h"


class shellWrap{
private:
    const char *promptPreFix = "[";
    const char *promptPostFix = "]&";
    const char *pageSimHelp = "1-Read reference string\n2-Generate reference string\n3-Display current reference string\n4-Simulate FIFO\n5-Simulate OPT\n6-simulate LRU\n7-Simulate LFU\n";
    const char *fileSimHelp =  "0-Exit\n1-Select directory\n2-List directory content (first level)\n3-List directory content (all levels)\n4-Delete file\n5-Display file (hexadecimal view)\n6-Encrypt file (XOR with password)\n7-Decrypt file (XOR with password)\n";
    const char *algoPreAmble = "Starting to run demand page simulation using the algorithm ";
    std::string args, args2, password;
    std::string cliPrompt = "[]&";
    si intArgs = -1;
    si argc=0;
    char** argv;
    pageFaultSim pageSim;
    shell shellFuncs;
public:
    shellWrap(si setArgc, char* setArgv[]) : argc(setArgc), argv(setArgv){
        //this->setDirHome();
        this->mainLoop();
    }
private:
    si mainLoop(){
        while(intArgs != 0){
            std::cout << this->pageSimHelp;
            std::cin >> intArgs;
            this->pageFaultMap(intArgs);
            args.clear();
            args2.clear();
            password.clear();
        }
        std::cout << "Bye, bye!" << std::endl;
        return 0;
    }

    si pageFaultMap(si commandIndex) {
        switch (commandIndex) {
            case 0:
                break;
            case 1:
                std::cout << "Input reference string with [0-9], up to 20 digits\n";
                std::cin >> args;
                if(args.length() > std::to_string(std::numeric_limits<u16>::max()).length() || std::stol(args) > (int)(std::numeric_limits<u16>::max())){
                    std::cout << "Error: unsigned short can only hold: " << std::numeric_limits<u16>::max() << " and input was: " << args << "!\nWhat are you doing?.....\n";
                    break;
                }
                std::cout << "\n";
                if(!this->shellFuncs.sanatizeToNums(args)){
                    std::cout << "Error: bad input : " << args;
                } else{
                    this->pageSim.setReferenceString(std::vector<u16>(args.begin(), args.end()));
                    break;
                }
                break;
            case 2:
                std::cout << "Enter reference string length: ";
                std::cin >> args;

                if( args.length() > std::to_string(std::numeric_limits<u16>::max()).length() || std::stol(args) > (int)(std::numeric_limits<u16>::max())){
                    std::cout << "Error: unsigned short can only hold: " << std::numeric_limits<u16>::max() << " and input was: " << args << "!\nWhat are you doing?.....\n";
                    break;
                }
                std::cout << std::endl;
                if(!this->shellFuncs.sanatizeToNums(args)){
                    std::cout << "Error: bad input : " << args;
                } else {
                    std::cout << "Reference String updated to: " << this->pageSim.generateReferenceString(std::stoi(args)) << "\n";
                }
                break;
            case 3:
                std::cout << "current Reference String is: " << this->pageSim.getReferenceString() << "\n";
                break;
            case 4:
                std::cout << this->algoPreAmble << "FIFO." << std::endl;
                this->pageSim.pageFaultAlgoStepAPI(1);
                break;
            case 5:
                std::cout << this->algoPreAmble << "OPT." << std::endl;
                this->pageSim.pageFaultAlgoStepAPI(2);
                break;
            case 6:
                std::cout << this->algoPreAmble << "LRU." << std::endl;
                this->pageSim.pageFaultAlgoStepAPI(3);
                break;
            case 7:
                std::cout << this->algoPreAmble << "LFU." << std::endl;
                this->pageSim.pageFaultAlgoStepAPI(4);
                break;
            case 10:
                this->startPageFaultGui();
                break;
            default:
                std::cout << "Not a valid command!" << std::endl;
        }
        return 1;
    }

    si shellCallMap(si commandIndex){
        switch (commandIndex) {
            case 0:
                std::cout << "Bye, bye!\n";
                exit(1);
                break;
            case 1:
                std::cout << "Give director to select\n";
                std::cout << args << "\n";
                this->shellFuncs.cd(args);
                this->promptConstr();
                break;
            case 2:
                this->shellFuncs.ls();
                break;
            case 3:
                this->shellFuncs.ls_r();
                break;
            case 4:
                std::cout << "Give file to remove\n";
                std::cout << args << "\n";
                this->shellFuncs.rm(args);
                break;
            case 5:
                std::cout << "Give file to hexdump\n";
                std::cout << args << "\n";
                this->shellFuncs.hexdump(args);
                break;
            case 6:
                std::cout << "Give file to encrypt.\n";
                std::cout << args << "\n";
                std::cout << "Give file to output to.\n";
                std::cout << args2 << "\n";
                std::cout << "Enter password.\n";
                std::cout << password << "\n";
                this->shellFuncs.xorCrypt(args, args2, password);
                break;
            case 7:
                std::cout << "Give file to encrypt.\n";
                std::cout << args << "\n";
                std::cout << "Give file to output to.\n";
                std::cout << args2<< "\n";
                std::cout << "Enter password.\n";
                std::cout << password << "\n";
                this->shellFuncs.xorDecrypt(args, args2, password);
                break;
            case 7133:
                std::cout << "Running timer\n";
                this->cryptTimer();
                std::cout << "Timer done\n";
                break;
            case 61337:
                std::cout << "Running Hex debug\n";
                this->hexDebug();
                std::cout << "Ending hex debug\n";
            default:
                std::cout << "Error, not a valid command\n";
        }

        return 1;
    }

    si demoFINALPROJECT(){
        args = "/home/fly0ut/Desktop";
        this->shellCallMap(1);

        this->shellCallMap(2);

        args = "testing";
        args2 = "testing2";
        password = "4v5525";
        this->shellCallMap(6);

        args = "testing2";
        args2 = "testingDe";
        password = "4v5525";
        this->shellCallMap(7);

        args = "testingBig";
        args2 = "testingBig2";
        password = "d2d032-i90efw09ufa";
        this->shellCallMap(6);

        args = "testingBig2";
        args2 = "testingBigDe";
        password = "d2d032-i90efw09ufa";
        this->shellCallMap(7);

        args = "testingBig";
        this->shellCallMap(5);
        args = "testingBigDe";
        this->shellCallMap(5);

        args = "/home/fly0ut/Documents/Books";
        this->shellCallMap(1);

        this->shellCallMap(3);

        args = "removeMe!";
        this->shellFuncs.touch(args);
        this->shellCallMap(2);
        this->shellCallMap(4);

        args = "/home/fly0ut/Desktop";
        this->shellCallMap(1);

        args = "testing2";
        this->shellCallMap(4);
        args = "testingDe";
        this->shellCallMap(4);
        args = "testingBig2";
        this->shellCallMap(4);
        args = "testingBigDe";
        this->shellCallMap(4);

        return 0;
    }

    si hexDebug(){
        this->shellFuncs.hexdump("/home/fly0ut/Desktop/testingBig");
        return 0;
    }

    si cryptTimer(){
        this->shellFuncs.cyptTimer();
        return 0;
    }

    std::string promptConstr() {
        this->cliPrompt = shellWrap::promptPreFix+this->shellFuncs.getWD()+shellWrap::promptPostFix;
        return cliPrompt;
    }

    si setDirHome(){
        if(this->shellFuncs.getEnvVar("HOME").size() <= 0) {
            return 0;
        }


        const std::string  dirHomePrompt = "Do you want to try and set your HOME environmental variable as the working director? [Y/n]: ";
        std::string response;
        std::cout << dirHomePrompt;
        std::cin >> response;
        std::cout << "\n";

        while(response != "Y" && response != "n" && response != "y" && response != "N") {
            std::cout << response << " is not a valid answer\n" << dirHomePrompt;
            std::cin >> response;
        }

        if(response == "n" || response == "N"){
            return 1;
        }

        this->shellFuncs.cd(this->shellFuncs.getEnvVar("HOME"));
        this->promptConstr();

        return 1;
    }

    si startPageFaultGui(){
        QApplication a(argc, argv);
        PageFaultDisplay pageGUI;
        pageGUI.setPageSim(&this->pageSim);

        pageGUI.show();

        return QApplication::exec();
    }

};