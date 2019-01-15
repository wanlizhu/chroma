#include "icompiler.h"

using namespace resc;

bool ICompiler::start(const IConfig* config) {
    if (!check(config)) {
        return false;
    }

    return run(config);
}

bool ICompiler::write(const IConfig* config, const void* data, size_t size) {
    std::shared_ptr<IConfig::IOutput> out = config->output();
    if (!out->open()) {
        std::cerr << "Unable to create output file." << std::endl;
        return false;
    }

    out->write(data, size);
    out->close();

    return true;
}