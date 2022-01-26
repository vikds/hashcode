#include <getopt.h>
#include <sysexits.h>
#include <unistd.h>

#include "fwd.hpp"
#include "model.hpp"
#include "signaling.hpp"
#include "solution.hpp"

using namespace hashcode;

void Usage(const char* binary_name) {
    std::cerr << "Usage: " << binary_name << " [-io] [-i input][-o output]" << std::endl;
    std::cerr << "\t-i|--input (required): input file" << std::endl;
    std::cerr << "\t-o|--output (required): output file" << std::endl;
    std::cerr << "\t-a|--attempts: attempts to improve result (default: 10)" << std::endl;
    std::cerr << "\t-r|--rotations: schedule rotations to improve result (default: 10)" << std::endl;
    std::cerr << "\t-s|--seed: random number generator seed to improve result (default: 0)" << std::endl;
    std::cerr << "\t-h|--help: show this usage" << std::endl;
    std::exit(EX_USAGE);
}

Arguments ParseArguments(int argc, char* argv[]) {
    Arguments arguments;
    static struct option long_options[] = {
        { "input", required_argument, nullptr, 'i' },
        { "output", required_argument, nullptr, 'o' },
        { "attempts", required_argument, nullptr, 'a' },
        { "rotations", required_argument, nullptr, 'r' },
        { "seed", required_argument, nullptr, 's' },
        { "help", no_argument, nullptr, 'h' },
        { nullptr, 0, nullptr, 0 }
    };
    int ch;
    const char* binary_name = argv[0];
    while ((ch = getopt_long(argc, argv, "i:o:a:r:s:h", long_options, nullptr )) != -1) {
        switch (ch) {
            case 'i':
                arguments.input_file = optarg;
                break;
            case 'o':
                arguments.output_file = optarg;
                break;
            case 'a':
                arguments.attempts = std::atoi(optarg);
                break;
            case 'r':
                arguments.rotations = std::atoi(optarg);
                break;
            case 's':
                arguments.seed = std::atoi(optarg);
                break;
            case 'h':
                Usage(binary_name);
        }
    }
    bool is_valid = true;
    std::ostringstream oss;
    if (arguments.input_file.empty()) {
        oss << "\t-i|--input option is required" << std::endl;
        is_valid = false;
    }
    if (arguments.output_file.empty()) {
        oss << "\t-o|--output option is required" << std::endl;
        is_valid = false;
    }
    if (!is_valid) {
        std::cerr << "Invalid options:" << std::endl;
        std::cerr << oss.str();
        Usage(binary_name);
    }
    return arguments;
}

bool file_exist(const std::string& file_name) {
    return access(file_name.c_str(), F_OK) != -1;
}

bool delete_file(const std::string& file_name) {
    return unlink(file_name.c_str()) != -1;
}

int main(int argc, char* argv[]) {
    try {
        Arguments args = ParseArguments(argc, argv);

        if (!file_exist(args.input_file)) {
            std::cerr << "Input file doesn't exist: " << args.input_file << std::endl;
            return EXIT_FAILURE;
        }
        if (file_exist(args.output_file)) {
            delete_file(args.output_file);
        }

        InputData input_data(args);
        Solution solution(input_data);
        Signaling best_signaling = solution.GetBestSignaling();
        best_signaling.SaveToFile(input_data);

    } catch(const std::exception& ex) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}