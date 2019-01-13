#ifndef CHROMA_RESC_COMMANDLINE_CONFIG_H
#define CHROMA_RESC_COMMANDLINE_CONFIG_H

#include "iconfig.h"
#include "commandline_input.h"
#include "commandline_output.h"

namespace resc {

class CommandLineConfig : public IConfig {
public:
    CommandLineConfig(int argc, char** argv);
    virtual ~CommandLineConfig();

    virtual IInput* input() const noexcept override;
    virtual IOutput* output() const noexcept override;
    virtual std::string to_string() const noexcept override;

private:
    bool parse();

private:
    int m_argc = 0;
    char** argv = nullptr;
    FileInput* m_input = nullptr;
    FileOutput* m_output = nullptr;
};

}

#endif