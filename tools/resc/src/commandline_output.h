#ifndef CHROMA_RESC_COMMANDLINE_OUTPUT_H
#define CHROMA_RESC_COMMANDLINE_OUTPUT_H

#include "iconfig.h"

namespace resc {

class FileOutput : public IConfig::IOutput {
public:
    explicit FileOutput(const char* path);
    virtual ~FileOutput();

    virtual bool open() noexcept override;
    virtual bool close() noexcept override;
    virtual bool write(const void* data, size_t bytes) noexcept override;
    virtual std::ostream& ostream() noexcept override;
    
private:
    const std::string m_path;
    std::ofstream m_file;
};

}

#endif