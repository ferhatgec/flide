// MIT License
//
// Copyright (c) 2021 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
//

#include <fstream>
#include "../include/flide.hpp"
#include "../include/flide_parse.hpp"

void Flide::Init(std::string const filename) noexcept {
    std::ifstream read(filename);

    for(std::string temp; std::getline(read, temp);
        this->file_data.append(temp + "\n"))
        ; read.close();
}

int main(int argc, char** argv) noexcept {
    if(argc < 2) { return 1; }

    Flide        init; init.Init(std::string(argv[argc - 1]));
    Flide_Parser parser;
    parser.Tokenize(init);
    parser.Parse();
}