// json_test.cpp
//
#include "monik/log/third_party/json/json_parser.h"
#include "monik/log/third_party/json/reader.h"
#include "monik/log/third_party/json/writer.h"  

#if defined(SDL_OS_WIN32) && SDL_DEBUG
namespace sdl { namespace log { namespace {
    class unit_test {
    public:
        unit_test()
        {
            if (0) {
                const char json_test[] = CMAKE_CURRENT_SOURCE_DIR R"(/dataserver/log/third_party/json/test/)";
                const char protobuf[] = CMAKE_CURRENT_SOURCE_DIR R"(/dataserver/log/protobuf/)";
                const std::string path(json_test);
                try {
                    SDL_ASSERT(test_read_file(path + "pass1.json"));
                    SDL_ASSERT(test_read_file(path + "pass2.json"));
                    SDL_ASSERT(test_read_file(path + "pass3.json"));
                    SDL_ASSERT(test_read_file(path + "messaging.json"));
                    SDL_ASSERT(test_proto(path + "messaging.json"));
                    SDL_ASSERT(test_read_file(std::string(protobuf) + "logger.json"));
                }
                catch (const std::exception& e) {
                    SDL_TRACE(e.what());
                    SDL_ASSERT(0);
                }
            }
        }
    private:
        bool test_read_file(std::string const &, bool trace = false);
        bool test_proto(std::string const &);
    };
    
    bool unit_test::test_read_file(std::string const & filename, const bool trace)
    {
        SDL_TRACE("test_read_file: ", filename);
        const std::string str = json_parser::read_text_file(filename.c_str());
        if (!str.empty()) {
            Json::Value root; // will contains the root value after parsing
            Json::Reader reader;
            if (reader.parse(str, root)) {
                if (trace) {
                    SDL_TRACE(root);
                }
                return true;
            }
        }
        SDL_ASSERT(0);
        return false;
    }

    bool unit_test::test_proto(std::string const & filename)
    {
        const std::string str = json_parser::read_text_file(filename.c_str());
        if (!str.empty()) {
            const auto s2 = json_parser::json_to_proto(str, json_parser::proto_syntax::_2);
            const auto s3 = json_parser::json_to_proto(str, json_parser::proto_syntax::_3);
            SDL_TRACE(s2);
            SDL_TRACE(s3);
            return !(s2.empty() || s3.empty());
        }
        SDL_ASSERT(0);
        return false;
    }

    static unit_test s_test;

}}} // sdl
#endif //#if SV_DEBUG
