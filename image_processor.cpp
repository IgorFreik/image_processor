#pragma once
#include "filters.h"
#include "io.h"

struct Parser {
    struct Filter {
        std::string name;
        std::vector<std::string> filter_args = {};
    };

    struct Result {
        const char* input_path;
        const char* output_path;
        std::vector<Filter> filters = {};
    };

    static Result Apply(int argc, const char* argv[]) {
        Result result;
        result.input_path = argv[1];
        result.output_path = argv[2];
        Filter curr_filter;
        for (int i = 3; i < argc; ++i) {
            if (argv[i][0] == '-') {
                result.filters.push_back(curr_filter);
                curr_filter.name = argv[i];
                curr_filter.filter_args.clear();
            } else {
                curr_filter.filter_args.push_back(argv[i]);
            }
        }
        result.filters.push_back(curr_filter);
        return result;
    }
};

int main(int argc, const char* argv[]) {
    Parser::Result ParsedInput = Parser::Apply(argc, argv);
    Image im = BMP::read(ParsedInput.input_path);
    for (auto filter : ParsedInput.filters) {
        if (filter.name == "-crop") {
            Crop::Apply(im, std::stoi(filter.filter_args[0]), std::stoi(filter.filter_args[1]));
        }
        if (filter.name == "-gs") {
            Grayscale::Apply(im);
        }
        if (filter.name == "-neg") {
            Negative::Apply(im);
        }
        if (filter.name == "-sharp") {
            Sharpening::Apply(im);
        }
        if (filter.name == "-edge") {
            Grayscale::Apply(im);
            EdgeDetection::Apply(im, std::stod(filter.filter_args[0]));
        }
    }
    BMP::write(im, ParsedInput.output_path);
    return 0;
}
