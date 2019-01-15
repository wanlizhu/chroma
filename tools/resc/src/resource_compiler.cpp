#include "resource_compiler.h"

using namespace resc;

ResourceCompiler::ResourceCompiler() {
    m_processors[IConfig::Mode::BINARY] = &ResourceCompiler::process_bin;
}

bool ResourceCompiler::run(const IConfig* config) {
    if (m_processors.find(config->mode()) == m_processors.end())
        return false;

    (*this.*m_processors[config->mode()])(config);

    return true;
}

bool ResourceCompiler::check(const IConfig* config) {
    if (config->mode() != IConfig::Mode::BINARY) {
        std::cerr << "Wrong working mode, [binary] mode required." << std::endl;
        return false;
    }

    if (config->input() == nullptr) {
        std::cerr << "Missing input filename." << std::endl;
        return false;
    }

    return true;
}

bool ResourceCompiler::process_bin(const IConfig* config) const noexcept {
    size_t size = config->input()->open();
    if (size == 0) return false;
    std::unique_ptr<const char[]> in = config->input()->read();

    return true;
}
