#ifndef CHROMA_RESC_COMMANDLINE_INPUT_H
#define CHROMA_RESC_COMMANDLINE_INPUT_H

#include "iconfig.h"

namespace resc {

class FileInput : public IConfig::IInput {
public:
    explicit FileInput(const char* path);
    virtual ~FileInput() {}

    virtual size_t open() noexcept override;
    virtual bool close() noexcept override;
    virtual std::unique_ptr<const char[]> read() noexcept override;
    virtual const char* name() const noexcept override;

private:
    const std::string m_path;
    std::ifstream m_file;
    size_t m_filesize = 0;
};

}

#endif