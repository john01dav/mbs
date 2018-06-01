#include "mbs/MbsProject.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

#include "mbs/MbsException.h"
#include "mbs/Module.h"

using namespace mbs;

MbsProject::MbsProject(const std::string &mbsFile) {
    YAML::Node rootNode = YAML::LoadFile(mbsFile);
    verifyFormatVersion(rootNode);
    loadModules(rootNode);
}


MbsProject::~MbsProject() {
    for(Module *module : m_modules){
        delete module;
    }
}

void MbsProject::outputToCmake(const std::string &cmakeFile) {
    try {
        std::ofstream fout;
        fout.exceptions(std::ios::failbit | std::ios::badbit);
        fout.open(cmakeFile);

        for(Module *module : m_modules){
            module->outputToCmake(fout);
            fout << std::endl;
        }


        for(Module *module : m_modules){
            module->outputLinkingToCmake(fout);
        }

    }catch(std::exception &e){
        throw MbsException("Failed to write the cmake file " + cmakeFile + " with reason " + e.what() + ".");
    }
}

void MbsProject::verifyFormatVersion(const YAML::Node &rootNode) {
    if(!rootNode["mbs-format-version"]) throw MbsException("The mbs-format-version property must exist in mbs.yml");

    unsigned version = rootNode["mbs-format-version"].as<unsigned>();

    if(version > MBS_FORMAT_VERSION){
        throw MbsException("The mbs version " + std::to_string(version) + " is not supported by this version of mbs.");
    }
}

void MbsProject::loadModules(const YAML::Node &rootNode) {
    if(!rootNode["modules"]) throw MbsException("The modules node must exist.");

    const YAML::Node moduleNode = rootNode["modules"];

    if(!moduleNode.IsMap()) throw MbsException("The modules node must be a map");

    for(YAML::const_iterator it=moduleNode.begin();it!=moduleNode.end();++it){
        m_modules.push_back(Module::loadModule(it));
    }
}