#include "shader_compiler.h"

using namespace resc;

ShaderCompiler::ShaderCompiler() {
    m_processors[IConfig::Mode::SHADER] = &ShaderCompiler::process_shader;
}

bool ShaderCompiler::run(const IConfig* config) {
    if (m_processors.find(config->mode()) == m_processors.end())
        return false;

    (*this.*m_processors[config->mode()])(config);

    return true;
}

bool ShaderCompiler::check(const IConfig* config) {
    if (config->mode() != IConfig::Mode::SHADER) {
        std::cerr << "Wrong working mode, [shader] mode required." << std::endl;
        return false;
    }

    if (config->input() == nullptr) {
        std::cerr << "Missing input filename." << std::endl;
        return false;
    }

    return true;
}

bool ShaderCompiler::process_shader(const IConfig* config) const noexcept {
    size_t size = config->input()->open();
    if (size == 0) return false;
    std::unique_ptr<const char[]> in = config->input()->read();

    return true;
}
