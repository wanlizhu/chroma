#ifndef CHROMA_RESC_RESOURCE_H
#define CHROMA_RESC_RESOURCE_H

#include "icompiler.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace resc {
    
class ResourceCompiler final : public ICompiler {
    using ResourceProcessor = bool (ResourceCompiler::*)(const IConfig*) const;
public:
    ResourceCompiler();

    virtual bool run(const IConfig* config) override;
    virtual bool check(const IConfig* config) override;

private:
    bool process_bin(const IConfig* config) const noexcept;

private:
    std::unordered_map<IConfig::Mode, ResourceProcessor> m_processors; 
};

}

#endif