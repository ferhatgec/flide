// MIT License
//
// Copyright (c) 2021 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
//

#ifndef FLIDE_FLIDE_HPP
#define FLIDE_FLIDE_HPP

#include <iostream>
#include <string>

class Flide {
public:
    std::string file, file_data;
public:
    Flide () = default;
    ~Flide() = default;

    void Init(std::string const filename) noexcept;
};

#endif // FLIDE_FLIDE_HPP
