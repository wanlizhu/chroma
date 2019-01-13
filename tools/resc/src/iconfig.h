#ifndef CHROMA_RESC_ICONFIG_H
#define CHROMA_RESC_ICONFIG_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace resc {

    class IConfig {
    public:
        enum class Mode {
            RESOURCE,
        };

        enum class OutputFormat {
            C_HDR_SRC,
            BLOB,
        };

        class IInput {
        public:
            virtual ~IInput() = default;
            virtual size_t open() noexcept = 0;
            virtual std::unique_ptr<const char[]> read() noexcept = 0;
            virtual bool close() noexcept = 0;
            virtual const char* name() const noexcept = 0;
        };

        class IOutput {
        public:
            virtual ~IOutput() = default;
            virtual bool open() noexcept = 0;
            virtual bool close() noexcept = 0;
            virtual bool write(const void* data, size_t bytes) noexcept = 0;
            virtual std::ostream& ostream() noexcept = 0;
        };

    public:

        virtual ~IConfig() = default;
        virtual IInput* input() const noexcept = 0;
        virtual IOutput* output() const noexcept = 0;
        virtual std::string to_string() const noexcept = 0;

        bool is_valid() const noexcept;
        bool mode() const noexcept;
        OutputFormat output_format() const noexcept;

    protected:
        bool m_valid = true;
        Mode m_mode = Mode::RESOURCE;
        OutputFormat m_outformat = OutputFormat::C_HDR_SRC;
    };
}

#endif