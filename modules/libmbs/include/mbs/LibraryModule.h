#pragma once

#include <ostream>
#include <string>
#include <yaml-cpp/yaml.h>

#include "mbs/Module.h"

namespace mbs {
    class LibraryModule : public Module{
    public:
        LibraryModule(const std::string &name, const YAML::Node &node);

        void outputToCmake(std::ostream &out) const override;
        void outputLinkingToCmake(std::ostream &out) const override;

        LibraryModule(const LibraryModule &) = delete;
        LibraryModule &operator=(const LibraryModule &) = delete;

    private:
        bool m_isStatic, m_isDynamic;

    };
}
