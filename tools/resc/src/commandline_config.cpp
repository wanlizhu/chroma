#include "commandline_config.h"
#include <getopt/getopt.h>

using namespace resc;

namespace details {
static void usage() {
    std::cout << "RESC is a command-line tool to compile resources.\n"
        << "Usages:\n"
        << "    RESC [options] <input-file or dir>\n"
        << "\n"
        << "Supported input formats:\n"
        << "    All the files (.*)\n"
        << "\n"
        << "Options:\n"
        << "   --help, -h\n"
        << "       Print this message\n\n"
        << "   --output, -o\n"
        << "       Specify path to output file\n\n"
        << "   --mode, -o\n"
        << "       Specify the compile mode [binary|shader]";

}

} // namespace details

CommandLineConfig::CommandLineConfig(int argc, char** argv) 
    : IConfig()
    , m_argc(argc)
    , m_argv(argv) {
    m_valid = parse();
}

CommandLineConfig::~CommandLineConfig() {
}

std::shared_ptr<IConfig::IInput> CommandLineConfig::input() const noexcept {
    return m_input;
}

std::shared_ptr<IConfig::IOutput> CommandLineConfig::output() const noexcept {
    return m_output;
}

std::string CommandLineConfig::to_string() const noexcept {
    std::string parameters;
    for (size_t i = 0; i < m_argc; i++) {
        parameters += m_argv[i];
        parameters += " ";
    }
    return parameters;
}

bool CommandLineConfig::parse() {
    static constexpr const char* OPTSTR = "ho:m:";
    static const struct option OPTIONS[] = {
        {"help", no_argument, nullptr, 'h'},
        {"output", required_argument, nullptr, 'o'},
        {"mode", required_argument, nullptr, 'm'},
        { nullptr, 0, nullptr, 0 }
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(m_argc, m_argv, OPTSTR, OPTIONS, &option_index)) >= 0) {
        std::string arg(optarg ? optarg : "");
        switch (opt) {
            default:
            case 'h':
            details::usage();
            exit(0);
            break;

            case 'm':
            if (arg == "binary") {
                m_mode = Mode::BINARY;
            } 
            else if (arg == "shader") {
                m_mode = Mode::SHADER;
            }
            else {
                std::cerr << "Unrecognized mode option. Must be 'binary'|'shader'." << std::endl;
                return false;
            }
            break;

            case 'o':
            m_output = std::make_shared<FileOutput>(arg.c_str());
            break;
        }
    }

    if (m_argc - optind > 1) {
        std::cerr << "Only one input file should be specified on the command line." << std::endl;
        return false;
    }

    if (m_argc - optind > 0) {
        m_input = std::make_shared<FileInput>(m_argv[optind]);
    }

    return true;
}