// MIT License
//
// Copyright (c) 2021 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
//

#ifndef FLIDE_FLIDE_PARSE_HPP
#define FLIDE_FLIDE_PARSE_HPP

#include <vector>
#include "flide.hpp"
#include "defs/flide_defs.hpp"

enum class FlideTokens : const u8 {
    Begin,
    End,

    Label,
    Wait ,

    New  ,

    Left ,

    Undef
};

static std::vector<std::string> keywords = {
    "Begin",
    "End"  ,

    "Label",
    "Wait" ,

    "New"  ,

    "Left"
};

class Flide_Parser {
    bool is_begin = false,

         is_label = false,
         is_data  = false,

         is_wait  = false,

         is_left  = false;

    std::string label_data;

    Flide init;
    std::vector<std::string> tokens;

    u32 w = 0, h = 0;
public:
    Flide_Parser () = default;

    ~Flide_Parser() = default;



    FlideTokens
         Match(std::string& token)
                    noexcept;
    void Tokenize(Flide init)
                    noexcept;
    void Parse   () noexcept;

    void Left    (const std::string& data)
                    noexcept;
};

#endif // FLIDE_FLIDE_PARSE_HPP
