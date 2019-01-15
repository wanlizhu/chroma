#include <iostream>
#include <memory>
#include "commandline_config.h"
#include "shader_compiler.h"
#include "resource_compiler.h"

using namespace resc;

int main(int argc, char** argv) {
    CommandLineConfig config(argc, argv);
    if (!config.is_valid()) {
        std::cerr << "Invalid parameters." << std::endl;
        return 1;
    }

    std::unique_ptr<ICompiler> compiler = nullptr;
    switch (config.mode()) {
        case IConfig::Mode::BINARY:
            compiler.reset(new ResourceCompiler());
            break;

        case IConfig::Mode::SHADER:
            compiler.reset(new ShaderCompiler());
            break;

        default:
            std::cerr << "Unsupported working mode." << std::endl;
            return 1;
    }

    if (!compiler->start(&config)) {
        return 1;
    }

	return 0;
}