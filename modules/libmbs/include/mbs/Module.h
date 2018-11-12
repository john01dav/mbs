#pragma once

#include <filesystem>
#include <ostream>
#include <string>
#include <set>
#include <yaml-cpp/yaml.h>

namespace mbs{

    class MbsProject;

    class Module{
    public:
        virtual ~Module(){}

        const std::string &name() const noexcept { return m_name; }
        const std::vector<std::string> &dependencies() const noexcept { return m_dependencies; }

        virtual void outputToCmake(std::ostream &out, const MbsProject *moduleData) const = 0;
        virtual void outputLinkingToCmake(std::ostream &out) const = 0;

        static Module* loadModule(YAML::const_iterator module);

        Module(const Module &) = delete;
        Module &operator=(const Module&) = delete;

    protected:
        Module(const std::string &name, const YAML::Node &node);

        void outputFileList(std::ostream &out) const;
        void outputIncludeList(std::ostream &out, const std::string &targetName, const MbsProject *moduleData) const;

    private:
        std::string m_name;
        std::filesystem::path m_modulePath;
        std::vector<std::string> m_dependencies;
        std::vector<std::filesystem::path> m_headers;
        std::vector<std::filesystem::path> m_sourceFiles;

        static void loadFileList(std::vector<std::filesystem::path> *vector, const std::filesystem::path &path);
        static void printPathInSourceSet(const std::filesystem::path &path, std::ostream &out);
        static std::pair<std::string, std::string> parseDependencyDeclaration(const std::string &dependencyDeclaration);
        void buildDependencyList(std::set<std::string> *dependencies, const MbsProject *moduleData, const Module *module) const;
    };
}
