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
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <sysexits.h>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static const std::string VERSION = "2022.02.24 21:33";
static const int MAX_DAY = 1e5;

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
    bool busy = false;
};

using Contribs = std::vector<Contrib>;

struct Project {
    std::string name;
    int duration;
    int score;
    int deadline;
    Skills skills;
    bool done = false;
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
    int end_day;
};

struct AssignCmp {
    bool operator()(const Assign& lhs, const Assign& rhs) {
        return lhs.end_day > rhs.end_day; // for min_heap
    }
};

using Assigns = std::vector<Assign>;
using AssignsHeap = std::priority_queue<Assign, Assigns, AssignCmp>;

struct Output {
    Assigns assigns;
    int score = 0;
};

void PrintOutput(const Arguments& args, Output& data) {
    std::ofstream output(args.output_file);
    output << data.assigns.size() << std::endl;
    for (const Assign& assign: data.assigns) {
        output << assign.project_name << std::endl;
        bool first = true;
        for (const std::string& contrib_name: assign.contrib_names) {
            if (!first) {
                output << " ";
            }
            output << contrib_name;
            first = false;
        }
        output << std::endl;
    }
}

enum class Order {
    Shortest = 0,
    Largest = 1,
    Nearest = 2,
    NUM = 3,
};

void FreeContribs(const Strings& contrib_names, Contribs& contribs) {
    for (const std::string& contrib_name: contrib_names) {
        for (Contrib& contrib: contribs) {
            if (contrib_name != contrib.name) {
                continue;
            }
            contrib.busy = false;
            break;
        }
    }
}

void ImproveSkills(const Project& project, const Strings& contrib_names, Contribs& contribs) {
    for (int i = 0; i < project.skills.size(); i++) {
        const std::string& contrib_name = contrib_names[i];
        for (Contrib& contrib: contribs) {
            if (contrib_name != contrib.name) {
                continue;
            }

            bool found = false;
            const Skill& requirement = project.skills[i];
            for (Skill& skill: contrib.skills) {
                if (requirement.name != skill.name) {
                    continue;
                }

                found = true;
                if (requirement.level >= skill.level) {
                    skill.level++;
                }
                break;
            }
            if (!found) {
                Skill skill;
                skill.level = 1;
                skill.name = requirement.name;
                contrib.skills.push_back(skill);
            }
            break;
        }
    }
}

Strings AssignContribs(const Project& project, Contribs& contribs) {
    Strings result;
    for (const Skill& requirement: project.skills) {
        bool covered = false;
        for (Contrib& contrib: contribs) {
            if (contrib.busy) {
                continue;
            }

            for (const Skill& skill: contrib.skills) {
                if (skill.name != requirement.name) {
                    continue;
                }
                if (skill.level < requirement.level) {
                    continue;
                }
                covered = true;
                break;
            }
            if (!covered) {
                continue;
            }

            contrib.busy = true;
            result.push_back(contrib.name);
            break;
        }
        if (!covered) {
            break;
        }
    }
    if (result.size() < project.skills.size()) {
        FreeContribs(result, contribs);
        result.clear();
    }
    return result;
}

Output ExcetuteInProjectsOrder(Input& model) {
    Output output;

    int day = 0;
    int idx = 0;
    AssignsHeap assigns_heap;
    while (idx < model.projects.size() || !assigns_heap.empty()) {
        for (;idx < model.projects.size(); idx++) {
            Project& project = model.projects[idx];
            if (project.done) {
                continue;
            }
            Strings contribs = AssignContribs(project, model.contribs);
            if (contribs.empty()) {
                continue;
            }

            Assign assign;
            assign.project_name = project.name;
            assign.contrib_names = contribs;
            assign.end_day = day + project.duration;
            output.assigns.push_back(assign);
            assigns_heap.push(assign);

            ImproveSkills(project, contribs, model.contribs);
            output.score += project.score;
            project.done = true;
        }
        if (!assigns_heap.empty()) {
            Assign assign = assigns_heap.top();
            assigns_heap.pop();

            FreeContribs(assign.contrib_names, model.contribs);
            day = assign.end_day;
            idx = 0;
        }
        if (day > MAX_DAY) {
            break;
        }
    }
    return output;
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

        Input input;
        ReadInput(args, input);

        Output result;
        for (int i = 0; i < static_cast<int>(Order::NUM); i++) {
            Input model = input; // reset
            Output current;

            Order order = static_cast<Order>(i);
            switch (order) {
                case Order::Shortest:
                    std::sort(model.projects.begin(), model.projects.end(), [&](const Project& lhs, const Project& rhs) {
                        return lhs.duration < rhs.duration;
                    });
                    current = ExcetuteInProjectsOrder(model);
                    break;
                case Order::Largest:
                    std::sort(model.projects.begin(), model.projects.end(), [&](const Project& lhs, const Project& rhs) {
                        return lhs.score < rhs.score;
                    });
                    current = ExcetuteInProjectsOrder(model);
                    break;
                case Order::Nearest:
                    std::sort(model.projects.begin(), model.projects.end(), [&](const Project& lhs, const Project& rhs) {
                        return lhs.deadline < rhs.deadline;
                    });
                    current = ExcetuteInProjectsOrder(model);
                    break;
            }
            if (current.score > result.score) {
                result = current;
            }
        }

        PrintOutput(args, result);

    } catch(const std::exception& ex) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}