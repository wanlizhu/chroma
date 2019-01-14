#ifndef CHROMA_RESC_ICONFIG_H
#define CHROMA_RESC_ICONFIG_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

namespace resc {

    class IConfig {
    public:
        enum class Mode {
            BINARY,
            SHADER,
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
        virtual std::shared_ptr<IInput> input() const noexcept = 0;
        virtual std::shared_ptr<IOutput> output() const noexcept = 0;
        virtual std::string to_string() const noexcept = 0;

        bool is_valid() const noexcept;
        bool mode() const noexcept;

    protected:
        bool m_valid = true;
        Mode m_mode = Mode::BINARY;
    };
}

#endif