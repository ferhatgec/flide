// MIT License
//
// Copyright (c) 2021 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
//

#include <sstream>
#include <chrono>
#include <thread>

extern "C" {
    #include <unistd.h>
}

#include "../include/flide_parse.hpp"
#include "../include/flide_helpers.hpp"

#if defined(__linux__)
    #include <sys/ioctl.h>
#endif

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
    #if defined(__linux__)
        #if defined(TIOCGSIZE)
            struct ttysize ts;
            if(ioctl(STDIN_FILENO, TIOCGSIZE, &ts) == 0) {
                this->h = ts.ts_lines         ;
                this->w = ts.ts_cols          ;
            }
        #elif defined(TIOCGWINSZ)
            struct winsize ts;
            if(ioctl(STDIN_FILENO, TIOCGWINSZ, &ts) == 0) {
                this->h = ts.ws_row         ;
                this->w = (ts.ws_col / 1.50);
            }
            #endif // TIOCGWINSZ
    #endif // defined(__linux__)

    for(auto& token : this->tokens) {
        if(this->is_label || this->is_left) {
            if(token.length() <= 1) { continue; }
            if(is_data) {
                if(token.back() == '"') {
                    this->label_data.append(token); this->label_data.pop_back();

                    if(this->is_left) {
                        this->Left(this->label_data);
                    } else { println(this->label_data); }
                    this->label_data.erase();
                    this->is_label = this->is_data = this->is_left = false; continue;
                }
            }

            if(token.front() == '"') {
                if(token.back() == '"') {
                    token.pop_back(); token.erase(0, 1);

                    if(this->is_left) {
                        this->Left(token);
                    } else { println(token); }
                    this->is_label = this->is_left = false; continue;
                } this->is_data = true;

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

            case FlideTokens::Left: {
                this->is_left = true; break;
            }

            default: {
                break;
            }
        }
    }
}

void Flide_Parser::Left(const std::string& data) noexcept {
    println(data);
    for(auto i = 0; i < this->w; i++) { print(' '); }
}