#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <omp.h>

struct Options {
    Options(int argc, char** argv) {
        namespace bpo = boost::program_options;
        bpo::options_description desc("Google Hashcode. Options");

        // clang-format off
        desc.add_options()
            ("input_file", bpo::value<std::string>(&input_file)->default_value("input.txt"),
            "Path to input file with data for processing (or std::cin)")
            ("output_file", bpo::value<std::string>(&output_file)->default_value("output.txt"),
            "Path to output normalized text file (or std::cout)")
            ("threads", bpo::value<size_t>(&threads)->default_value(8),
            "Number of threads for OpenMP")
            ("help,h", "Show this help");
        // clang-format on

        bpo::variables_map map;
        bpo::store(bpo::parse_command_line(argc, argv, desc), map);
        if (map.count("help")) {
            std::cout << desc << std::endl;
            print_help_only = true;
        }
        bpo::notify(map);
    }

    std::string input_file;
    std::string output_file;
    size_t threads;

    bool print_help_only = false;
};

struct Pizza {
    size_t number = 0;
    std::unordered_set<size_t> ingredients;
};

struct Set {
    std::unordered_set<size_t> pizzas;
    std::unordered_set<size_t> ingredients;
};

struct SetCmpGreater {
    size_t operator()(const Set& lhs, const Set& rhs) const {
        return lhs.ingredients.size() > rhs.ingredients.size(); // for min_heap
    }
};

void Solve(const Options& options) {
    std::ifstream input_file(options.input_file);
    std::istream& input = (options.input_file.empty() ? std::cin : input_file);

    size_t pizzas_num;  // 100k
    size_t ingredients; // 10k
    std::string ingredient;
    input >> pizzas_num;
    std::vector<size_t> teams(5, 0); // 50k
    input >> teams[2] >> teams[3] >> teams[4];
    std::vector<Pizza> pizzas(pizzas_num);
    for (size_t number = 0; number < pizzas_num; number++) {
        Pizza& pizza = pizzas[number];
        pizza.number = number;
        input >> ingredients;
        for (size_t i = 0; i < ingredients; i++) {
            input >> ingredient;
            pizza.ingredients.insert(std::hash<std::string>{}(ingredient));
        }
    }

    size_t total_teams = 0;
    size_t total_persons = 0;
    for (size_t size = 0; size < teams.size(); size++) {
        total_persons += teams[size] * size;
        total_teams += teams[size];
    }

    std::priority_queue<Set, std::vector<Set>, SetCmpGreater> min_heap;
    std::vector<std::vector<Set>> sets(5);
    for (const Pizza& pizza : pizzas) {
        if (min_heap.size() < total_persons) {
            min_heap.push(Set{{pizza.number}, {pizza.ingredients}});
            continue;
        }
        if (pizza.ingredients.size() < min_heap.top().ingredients.size()) {
            continue;
        }
        min_heap.pop();
        min_heap.push(Set{{pizza.number}, {pizza.ingredients}});
    }
    while (!min_heap.empty()) {
        sets[1].push_back(min_heap.top());
        min_heap.pop();
    }

    std::vector<Set> result_sets;
    for (size_t merge_size = 1; merge_size < 3; merge_size++) {
        std::vector<Set>& merging_sets = sets[merge_size];
        size_t heap_size = total_persons * (merge_size + 1);

#pragma omp parallel num_threads(options.threads)
#pragma omp single
        for (auto it = merging_sets.begin(); it != merging_sets.end(); ++it) {
#pragma omp task default(none) firstprivate(it) shared(pizzas, min_heap, heap_size)
            {
                const Set& set = *it;
                for (const Pizza& pizza : pizzas) {
                    if (set.pizzas.find(pizza.number) != set.pizzas.end()) {
                        continue;
                    }

                    Set merged;
                    merged.pizzas.insert(set.pizzas.begin(), set.pizzas.end());
                    merged.pizzas.insert(pizza.number);
                    merged.ingredients.insert(set.ingredients.begin(), set.ingredients.end());
                    merged.ingredients.insert(pizza.ingredients.begin(), pizza.ingredients.end());

                    if (min_heap.size() < heap_size) {
#pragma omp critical(min_heap)
                        {
                            min_heap.push(merged);
                        }
                        continue;
                    }

                    if (merged.ingredients.size() < min_heap.top().ingredients.size()) {
                        continue;
                    }

#pragma omp critical(min_heap)
                    {
                        min_heap.pop();
                        min_heap.push(merged);
                    }
                } // for (pizzas)
            } // #pragma omp task
        } // #pragma omp parallel

        merging_sets.clear();
        std::vector<Set>& merged_sets = sets[merge_size + 1];
        while (!min_heap.empty()) {
            merged_sets.push_back(min_heap.top());
            result_sets.push_back(min_heap.top());
            min_heap.pop();
        }
    }

    for (std::vector<Set>& set : sets) {
        set.clear();
    }

    std::sort(result_sets.begin(), result_sets.end(),
        [&](const Set& lhs, const Set& rhs) {
            return lhs.ingredients.size() > rhs.ingredients.size();
        }
    );

    std::vector<size_t> result_indexes;
    std::unordered_set<size_t> used_pizzas;
    for (size_t index = 0; total_teams && index < result_sets.size(); index++) {
        const Set& set = result_sets[index];
        if (teams[set.pizzas.size()] == 0) {
            continue;
        }
        bool valid = true;
        for (const size_t& pizza : set.pizzas) {
            if (used_pizzas.find(pizza) != used_pizzas.end()) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            continue;
        }

        result_indexes.push_back(index);
        for (const size_t& pizza : set.pizzas) {
            used_pizzas.insert(pizza);
        }

        teams[set.pizzas.size()]--;
        total_teams--;
    }

    std::ofstream output_file(options.output_file.c_str());
    std::ostream& output = (options.output_file.empty() ? std::cout : output_file);

    output << result_indexes.size() << std::endl;
    for (const size_t& index : result_indexes) {
        const Set& set = result_sets[index];
        output << set.pizzas.size();
        for (const size_t& pizza : set.pizzas) {
            output << " " << pizza;
        }
        output << std::endl;
    }
}

int main(int argc, char* argv[]) {
    try {
        Options options(argc, argv);
        if (options.print_help_only) {
            return EXIT_SUCCESS;
        }

        if (!options.input_file.empty() && !boost::filesystem::exists(options.input_file)) {
            std::cerr << "Input file with data for processing doesn't exist: " << options.input_file << std::endl;
            exit(EXIT_FAILURE);
        }
        if (!options.output_file.empty() && boost::filesystem::exists(options.output_file)) {
            boost::filesystem::remove(options.output_file);
        }

        Solve(options);
        return EXIT_SUCCESS;
    }
    catch (std::exception& ex) {
        std::cerr << "FATAL ERROR: " << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}
