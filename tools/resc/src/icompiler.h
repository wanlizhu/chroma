#ifndef CHROMA_RESC_COMPILER_H
#define CHROMA_RESC_COMPILER_H

#include "iconfig.h"

namespace resc {

class ICompiler {
public:
    virtual ~ICompiler() = default;

    bool start(const IConfig* config);

protected:
    virtual bool run(const IConfig* config) = 0;
    virtual bool check(const IConfig* config) = 0;
    bool write();
};

}

#endif