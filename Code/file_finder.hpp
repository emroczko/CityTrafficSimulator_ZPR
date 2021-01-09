#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif
#include "slots_enum.hpp"
#include <iostream>

namespace ZPR {
    class FileFinder{
    public:
        FileFinder();
        std::string CheckIfFileExist(const std::string& name, int number);
        bool CheckIfFileExist(const std::string& name);
    private:
    };
}