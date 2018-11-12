#include "mbs/LibraryModule.h"

#include "mbs/MbsException.h"

using namespace mbs;

LibraryModule::LibraryModule(const std::string &name, const YAML::Node &node) : Module(name, node) {
    if(!node["library-type"]) throw MbsException("The library-type property is required for modules of type library.");
    std::string libraryType = node["library-type"].as<std::string>();

    if(libraryType == "static") {
        m_isStatic = true;
        m_isDynamic = false;
    }else if(libraryType == "dynamic"){
        m_isStatic = false;
        m_isDynamic = true;
    }else if(libraryType == "both"){
        m_isStatic = true;
        m_isDynamic = true;
    }else{
        throw MbsException("The library-type property must be \"static\". \"dynamic\", or \"both\".");
    }
}

void LibraryModule::outputToCmake(std::ostream &out, const MbsProject *moduleData) const {
    outputFileList(out);
    if(m_isStatic){
        out << "add_library(module-" << name() << "-static STATIC ${MBS_MODULE_" << name() << "_FILES})" << std::endl;
        outputIncludeList(out, "module-" + name() + "-static", moduleData);
        out << "set_target_properties(module-" << name() << "-static PROPERTIES OUTPUT_NAME " << name() << ")" << std::endl;
    }
    if(m_isDynamic){
        out << "add_library(module-" << name() << "-dynamic SHARED ${MBS_MODULE_" << name() << "_FILES})" << std::endl;
        outputIncludeList(out, "module-" + name() + "-dynamic", moduleData);
        out << "set_target_properties(module-" << name() << "-dynamic PROPERTIES OUTPUT_NAME " << name() << ")" << std::endl;
    }
}

void LibraryModule::outputLinkingToCmake(std::ostream &out) const {
    for(const std::string &dependency : dependencies()){
        if(m_isStatic) out << "target_link_libraries(module-" << name() << "-static module-" << dependency << ")" << std::endl;
        if(m_isStatic) out << "target_link_libraries(module-" << name() << "-dynamic module-" << dependency << ")" << std::endl;
    }
}
