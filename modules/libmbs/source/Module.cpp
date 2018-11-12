#include "mbs/Module.h"

#include <string>
#include <algorithm>
#include <filesystem>
#include <functional>

#include "mbs/MbsException.h"
#include "mbs/ApplicationModule.h"
#include "mbs/LibraryModule.h"
#include "mbs/MbsProject.h"

using namespace mbs;

Module* Module::loadModule(YAML::const_iterator module) {
    std::string name = module->first.as<std::string>();
    YAML::Node node = module->second;

    if(!node["type"]) throw MbsException("The module " + name + " does not have a type property.");

    std::string type = node["type"].as<std::string>();

    if(type == "library"){
        return new LibraryModule(name, node);
    }else if(type == "application"){
        return new ApplicationModule(name, node);
    }else{
        throw MbsException("Unknown module type \"" + type + "\" in module " + name + ".");
    }
}

Module::Module(const std::string &name, const YAML::Node &node)
        : m_name(name), m_modulePath(std::filesystem::path("./modules") / std::filesystem::path(name))
{
    std::filesystem::path includePath = m_modulePath / std::filesystem::path("include");
    std::filesystem::path sourcePath = m_modulePath / std::filesystem::path("source");
    loadFileList(&m_headers, includePath);
    loadFileList(&m_sourceFiles, sourcePath);

    if(node["dependencies"]){
        const YAML::Node dependenciesNode = node["dependencies"];
        if(!dependenciesNode.IsSequence()) throw MbsException("The dependencies node of the module " + name + " is not a sequence.");
        for(YAML::const_iterator it=dependenciesNode.begin();it!=dependenciesNode.end();++it){
            m_dependencies.push_back(it->as<std::string>());
        }
    }
}

void Module::loadFileList(std::vector<std::filesystem::path> *vector, const std::filesystem::path &path) {
    for(const std::filesystem::directory_entry &entry : std::filesystem::recursive_directory_iterator(path)){
        if(entry.is_regular_file()){
            vector->push_back(entry.path());
        }
    }
}

void Module::outputFileList(std::ostream &out) const {
    out << "set(MBS_MODULE_" << m_name << "_FILES" << std::endl;

    std::for_each(m_headers.begin(), m_headers.end(), std::bind(&Module::printPathInSourceSet, std::placeholders::_1, std::ref(out)));
    std::for_each(m_sourceFiles.begin(), m_sourceFiles.end(), std::bind(&Module::printPathInSourceSet, std::placeholders::_1, std::ref(out)));

    out << ")" << std::endl;
}

void Module::outputIncludeList(std::ostream &out, const std::string &targetName, const MbsProject *moduleData) const {
    out << "target_include_directories(" << targetName << " PRIVATE modules/" << m_name << "/include)" << std::endl;

    std::set<std::string> dependencies;
    buildDependencyList(&dependencies, moduleData, this);
    for(const std::string &dependency : dependencies){
        out << "target_include_directories(" << targetName << " PRIVATE modules/" << dependency << "/include)" << std::endl;
    }
}

void Module::printPathInSourceSet(const std::filesystem::path &path, std::ostream &out) {
    out << "    " << path.string() << std::endl;
}

std::pair<std::string, std::string> Module::parseDependencyDeclaration(const std::string &dependencyDeclaration) {
    auto it = std::find(dependencyDeclaration.begin(), dependencyDeclaration.end(), '-');
    std::string moduleName(dependencyDeclaration.begin(), it);
    std::string dependencyType(it+1,dependencyDeclaration.end());

    if(dependencyType != "static" && dependencyType != "dynamic"){
        throw MbsException("Improper library type specification: " + dependencyType);
    }

    return {moduleName, dependencyType};
}

void Module::buildDependencyList(std::set<std::string> *dependencies, const MbsProject *moduleData, const mbs::Module *module) const {
    for(const std::string &dependencyName : module->m_dependencies){
        auto parsedDependencyDeclaration = parseDependencyDeclaration(dependencyName);
        if(dependencies->count(parsedDependencyDeclaration.first) == 0){ //if this is the first time that this module is being processed in this context
            dependencies->insert(parsedDependencyDeclaration.first);
            const Module *dependency = moduleData->getModule(parsedDependencyDeclaration.first);
            if(dependency == nullptr) throw MbsException("Unknown module " + parsedDependencyDeclaration.first + " listed as dependency.");
            if(dependency != this) { //allow for circular dependencies
                buildDependencyList(dependencies, moduleData, dependency);
            }
        }
    }
}