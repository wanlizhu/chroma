#ifndef CHROMA_RESC_SHADER_H
#define CHROMA_RESC_SHADER_H

#include "icompiler.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace resc {

class ShaderCompiler final : public ICompiler {
    using ShaderProcessor = bool (ShaderCompiler::*)(const IConfig*) const;
public:
    ShaderCompiler();

    virtual bool run(const IConfig* config) override;
    virtual bool check(const IConfig* config) override;

private:
    bool process_shader(const IConfig* config) const noexcept;

private:
    std::unordered_map<IConfig::Mode, ShaderProcessor> m_processors;
};

}

#endif