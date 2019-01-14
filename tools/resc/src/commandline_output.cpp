#include "commandline_output.h"

using namespace resc;

FileOutput::FileOutput(const char* path) 
    : m_path(path) {
}

FileOutput::~FileOutput() {

}

bool FileOutput::open() noexcept {
    m_file.open(m_path.c_str(), std::ofstream::out | std::ofstream::binary);
    return !m_file.fail();
}

bool FileOutput::close() noexcept {
    m_file.close();
    return m_file.fail();
}

bool FileOutput::write(const void* data, size_t bytes) noexcept {
    m_file.write((char*)(data), bytes);
    return m_file.fail();
}

std::ostream& FileOutput::ostream() noexcept {
    return m_file;
}
