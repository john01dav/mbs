#pragma once

#include "Module.h"

namespace mbs{
    class ApplicationModule : public Module{
    public:
        ApplicationModule(const std::string &name, const YAML::Node &node) : Module(name, node) {}

        void outputToCmake(std::ostream &out, const MbsProject *moduleData) const override;
        void outputLinkingToCmake(std::ostream &out) const override;

        ApplicationModule(const ApplicationModule &) = delete;
        ApplicationModule &operator=(const ApplicationModule &) = delete;
    };
}
