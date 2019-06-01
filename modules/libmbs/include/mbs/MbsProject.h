#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

#include "mbs/Module.h"

namespace mbs{
    constexpr unsigned MIN_MBS_FORMAT_VERSION = 1;
    constexpr unsigned MAX_MBS_FORMAT_VERSION = 2;

    class MbsProject{
    public:
        MbsProject(const std::string &mbsFile);
        ~MbsProject();

        void outputToCmake(const std::string &cmakeFile);
        const Module *getModule(const std::string &name) const;

        MbsProject(const MbsProject&) = delete;
        MbsProject &operator=(const MbsProject&) = delete;

    private:
        std::vector<Module*> m_modules;

        inline void verifyFormatVersion(const YAML::Node &rootNode);
        inline void loadModules(const YAML::Node &rootNode);
    };
};