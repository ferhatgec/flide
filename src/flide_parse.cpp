// MIT License
//
// Copyright (c) 2021 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
//

#include <sstream>
#include <chrono>
#include <thread>
#include "../include/flide_parse.hpp"
#include "../include/flide_helpers.hpp"

FlideTokens Flide_Parser::Match(std::string& token) noexcept {
    u8 i = 0; for(auto& keyword : keywords) {
        if(keyword == token) {
            return static_cast<FlideTokens>(i);
        } ++i;
    } return FlideTokens::Undef;
}

void Flide_Parser::Tokenize(Flide init) noexcept {
    std::istringstream stream(init.file_data);

    for(std::string temp;
        stream >> temp; this->tokens.push_back(temp))
        ;
}

void Flide_Parser::Parse() noexcept {
    for(auto& token : this->tokens) {
        if(is_label) {
            if(token.length() <= 1) { continue; }
            if(is_data) {
                if(token.back() == '"') {
                    this->label_data.append(token); this->label_data.pop_back();
                    println(this->label_data); this->label_data.erase();
                    is_label = is_data = false; continue;
                }
            }

            if(token.front() == '"') {
                if(token.back() == '"') {
                    token.pop_back(); token.erase(0, 1);
                    println(token);
                    is_label = false; continue;
                } is_data = true;

                this->label_data
                    .append(token.erase(0, 1) + " "); continue;
            }

            this->label_data.append(token + " "); continue;
        }

        if(is_wait) {
            if(numeric(token)) {
                std::this_thread::sleep_for(std::chrono::seconds(
                        std::stoi(token)
                        ));

                is_wait = false; continue;
            }
        }

        auto type = this->Match(token);

        switch(type) {
            case FlideTokens::Begin: {
                this->is_begin = true;
                clear(); refresh(); to_up(); break;
            }

            case FlideTokens::End: {
                std::exit(0);
            }

            case FlideTokens::Label: {
                this->is_label = true; break;
            }

            case FlideTokens::Wait : {
                this->is_wait = true; break;
            }

            case FlideTokens::New: {
                clear(); refresh(); break;
            }

            default: {
                break;
            }
        }
    }
}