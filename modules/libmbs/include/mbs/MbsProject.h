#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

#include "mbs/Module.h"

namespace mbs{
    constexpr unsigned MBS_FORMAT_VERSION = 0;

    class MbsProject{
    public:
        MbsProject(const std::string &mbsFile);
        ~MbsProject();

        void outputToCmake(const std::string &cmakeFile);

        MbsProject(const MbsProject&) = delete;
        MbsProject &operator=(const MbsProject&) = delete;

    private:
        std::vector<Module*> m_modules;

        inline void verifyFormatVersion(const YAML::Node &rootNode);
        inline void loadModules(const YAML::Node &rootNode);
    };
};