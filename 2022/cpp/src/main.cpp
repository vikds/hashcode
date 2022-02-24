#include <algorithm>
#include <chrono>
#include <deque>
#include <fstream>
#include <functional>
#include <getopt.h>
#include <iostream>
#include <iterator>
#include <limits>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <sysexits.h>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static const std::string VERSION = "2022.02.24 21:33";

void Usage(const char* binary_name) {
    std::cerr << "Usage: " << binary_name << " [-io] [-i input][-o output]" << std::endl;
    std::cerr << "\t-i|--input (required): input file" << std::endl;
    std::cerr << "\t-o|--output (required): output file" << std::endl;
    std::cerr << "\t-s|--seed: random number generator seed to improve result (default: 0)" << std::endl;
    std::cerr << "\t-h|--help: show this usage (ver. " << VERSION << ")" << std::endl;
    std::exit(EX_USAGE);
}

struct Arguments {
    std::string input_file;
    std::string output_file;
    size_t seed = 0;

    std::string ToString(const char* binary_name) const {
        std::ostringstream oss;
        oss << binary_name << " arguments:" << std::endl;
        oss << "--input: " << input_file << std::endl;
        oss << "--output: " << output_file;
        return oss.str();
    }
};

Arguments ParseArguments(int argc, char* argv[]) {
    Arguments arguments;
    static struct option long_options[] = {
        { "input", required_argument, nullptr, 'i' },
        { "output", required_argument, nullptr, 'o' },
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
    std::cout << arguments.ToString(binary_name) << std::endl;
    return arguments;
}

bool file_exist(const std::string& file_name) {
    return access(file_name.c_str(), F_OK) != -1;
}

bool delete_file(const std::string& file_name) {
    return unlink(file_name.c_str()) != -1;
}

using Strings = std::vector<std::string>;

struct Skill {
    std::string name;
    int level;
};

using Skills = std::vector<Skill>;

struct Contrib {
    std::string name;
    Skills skills;
};

using Contribs = std::vector<Contrib>;

struct Project {
    std::string name;
    int duration;
    int score;
    int deadline;
    Skills skills;
};

using Projects = std::vector<Project>;

struct Input {
    Contribs contribs;
    Projects projects;
};

void ReadInput(const Arguments& args, Input& data) {
    std::ifstream input(args.input_file);
    int contrib_num, project_num;
    input >> contrib_num >> project_num;

    int skill_num;
    for (int i = 0; i < contrib_num; i++) {
        Contrib contrib;
        input >> contrib.name;
        input >> skill_num;
        for (int j = 0; j < skill_num; j++) {
            Skill skill;
            input >> skill.name;
            input >> skill.level;
            contrib.skills.push_back(skill);
        }
        data.contribs.push_back(contrib);
    }

    int role_num;
    for (int i = 0; i < project_num; i++) {
        Project project;
        input >> project.name;
        input >> project.duration;
        input >> project.score;
        input >> project.deadline;
        input >> role_num;
        for (int j = 0; j < role_num; j++) {
            Skill skill;
            input >> skill.name;
            input >> skill.level;
            project.skills.push_back(skill);
        }
        data.projects.push_back(project);
    }
}

struct Assign {
    std::string project_name;
    Strings contrib_names;
};

using Assigns = std::vector<Assign>;

struct Output {
    Assigns assigns;
};

void PrintOutput(const Arguments& args, Output& data) {
    std::ofstream output(args.output_file);
    output << data.assigns.size() << std::endl;
    for (const Assign& assign: data.assigns) {
        output << assign.project_name << std::endl;
        bool first = false;
        for (const std::string& contrib_name: assign.contrib_names) {
            if (!first) {
                output << " ";
            }
            output << contrib_name;
        }
        output << std::endl;
    }
}

strcut Model {

};



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

        Input input;
        ReadInput(args, input);

        Output output;
        PrintOutput(args, output);

    } catch(const std::exception& ex) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}