#include "mbs/ApplicationModule.h"

using namespace mbs;

void ApplicationModule::outputToCmake(std::ostream &out, const MbsProject *moduleData) const {
    outputFileList(out);
    out << "add_executable(module-" << name() << " ${MBS_MODULE_" << name() << "_FILES})" << std::endl;
    outputIncludeList(out, "module-" + name(), moduleData);
    out << "set_target_properties(module-" << name() << " PROPERTIES OUTPUT_NAME " << name() << ")" << std::endl;
}

void ApplicationModule::outputLinkingToCmake(std::ostream &out) const {
    for(const std::string &dependency : dependencies()){
        out << "target_link_libraries(module-" << name() << " module-" << dependency << ")" << std::endl;
    }
}
