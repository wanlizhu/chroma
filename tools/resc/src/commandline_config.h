#ifndef CHROMA_RESC_COMMANDLINE_CONFIG_H
#define CHROMA_RESC_COMMANDLINE_CONFIG_H

#include "iconfig.h"
#include "commandline_input.h"
#include "commandline_output.h"
#include <vector>
#include <string>

namespace resc {

class CommandLineConfig : public IConfig {
public:
    CommandLineConfig(int argc, char** argv);
    virtual ~CommandLineConfig();

    virtual std::shared_ptr<IInput> input() const noexcept override;
    virtual std::shared_ptr<IOutput> output() const noexcept override;
    virtual std::string to_string() const noexcept override;

private:
    bool parse();

private:
    int m_argc = 0;
    char** m_argv = nullptr;
    std::shared_ptr<FileInput> m_input;
    std::shared_ptr<FileOutput> m_output;
};

}

#endif