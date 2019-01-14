#include "commandline_input.h"

using namespace resc;

FileInput::FileInput(const char* path) 
    : m_path(path) {

}

FileInput::~FileInput() {

}

size_t FileInput::open() noexcept {
    m_file.open(m_path.c_str(), std::ifstream::binary | std::ios::ate);
    if (!m_file) {
        std::cerr << "Unable to open material source file '" << m_path << "'" << std::endl;
        return 0;
    }

    m_filesize = m_file.tellg();
    m_file.seekg(0, std::ios::beg);

    return m_filesize;
}

bool FileInput::close() noexcept {
    m_file.close();
    return m_file.fail();
}

std::unique_ptr<const char[]> FileInput::read() noexcept {
    auto buffer = std::unique_ptr<const char[]>(new char[m_filesize]);

    if (!m_file.read(const_cast<char*>(buffer.get()), m_filesize)) {
        std::cerr << "Unable to read material source file '" << m_path << "'" << std::endl;
        return nullptr;
    }

    return buffer;
}

const char* FileInput::name() const noexcept {
    return m_path.c_str();
}