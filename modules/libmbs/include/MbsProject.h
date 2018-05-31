#pragma once

#include <string>

class MbsProject{
public:
    MbsProject(const std::string &mbsFile);

    MbsProject(const MbsProject&) = delete;
    MbsProject &operator=(const MbsProject&) = delete;

private:

};