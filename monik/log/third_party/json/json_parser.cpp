// json_parser.cpp
//
#include "monik/log/third_party/json/json_parser.h"
#include "monik/log/third_party/json/reader.h"
#include "monik/log/third_party/json/writer.h"  
#include <fstream>

namespace sdl { namespace log {

std::string json_parser::read_text_file(const char * const filename)
{
    MONIK_ASSERT(is_str_valid(filename));
    if (is_str_valid(filename)) {
        std::ifstream in(filename, std::ifstream::in | std::ifstream::binary);
        if (in.is_open()) {
            return std::string((std::istreambuf_iterator<char>(in)),
                std::istreambuf_iterator<char>());
        }
        MONIK_WARNING(0);
    }
    return{};
}

std::string json_parser::json_to_proto(std::string const & json, proto_syntax const ver)
{
    if (!json.empty()) {
        Json::Value root; // will contains the root value after parsing
        Json::Reader reader;
        if (reader.parse(json, root)) {
            const bool proto2 = (ver == proto_syntax::_2);
            static const char comma[] = ";\n";
            static const char tab[] = "  ";
            std::stringstream ss;
            ss << (proto2 ? R"(syntax = "proto2")" : R"(syntax = "proto3")") << comma;
            ss << "package " << root["package"].asString() << comma;
            ss << "option " << root["option"].asString() << comma;
            for (const auto & m : root["messages"]) {
                ss << "\nmessage " << m["message"].asString() << "\n{";
                for (const auto & f : m["enums"]) {
                    ss << "\n" << tab << "enum " << f["enum"].asString() << "\n" << tab << "{";
                    for (const auto & val : f["values"]) {
                        ss << "\n" << tab << tab << val["name"].asString() << " = " << val["tag"] << ";";
                        const auto comment = val["comment"].asString();
                        if (!comment.empty()) {
                            ss << R"( // )" << comment;
                        }
                    }
                    ss << "\n" << tab << "}";
                }
                for (const auto & f : m["fields"]) {
                    ss << "\n" << tab;
                    if (proto2) {
                        ss << "optional ";
                    }
                    ss << f["type"].asString() << " "
                       << f["name"].asString() << " = "
                       << f["tag"] << ";";
                    const auto comment = f["comment"].asString();
                    if (!comment.empty()) {
                        ss << R"( // )" << comment;
                    }
                    else {
                        size_t count = 0;
                        for (const auto & c : f["comments"]){
                            if (count++) {
                                ss << "\n" << tab;
                            }
                            ss << R"( // )" << c.asString();
                        }
                    }
                }
                ss << "\n}";
            }
            return ss.str();
        }
    }
    MONIK_ASSERT(0);
    return{};
}

}} // sdl

