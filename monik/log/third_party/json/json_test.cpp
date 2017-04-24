// json_test.cpp
//
#include "monik/log/third_party/json/json_parser.h"
#include "monik/log/third_party/json/reader.h"
#include "monik/log/third_party/json/writer.h"  

#if defined(MONIK_OS_WIN32) && MONIK_DEBUG
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
                    MONIK_ASSERT(test_read_file(path + "pass1.json"));
                    MONIK_ASSERT(test_read_file(path + "pass2.json"));
                    MONIK_ASSERT(test_read_file(path + "pass3.json"));
                    MONIK_ASSERT(test_read_file(path + "messaging.json"));
                    MONIK_ASSERT(test_proto(path + "messaging.json"));
                    MONIK_ASSERT(test_read_file(std::string(protobuf) + "logger.json"));
                }
                catch (const std::exception& e) {
                    MONIK_TRACE(e.what());
                    MONIK_ASSERT(0);
                }
            }
        }
    private:
        bool test_read_file(std::string const &, bool trace = false);
        bool test_proto(std::string const &);
    };
    
    bool unit_test::test_read_file(std::string const & filename, const bool trace)
    {
        MONIK_TRACE("test_read_file: ", filename);
        const std::string str = json_parser::read_text_file(filename.c_str());
        if (!str.empty()) {
            Json::Value root; // will contains the root value after parsing
            Json::Reader reader;
            if (reader.parse(str, root)) {
                if (trace) {
                    MONIK_TRACE(root);
                }
                return true;
            }
        }
        MONIK_ASSERT(0);
        return false;
    }

    bool unit_test::test_proto(std::string const & filename)
    {
        const std::string str = json_parser::read_text_file(filename.c_str());
        if (!str.empty()) {
            const auto s2 = json_parser::json_to_proto(str, json_parser::proto_syntax::_2);
            const auto s3 = json_parser::json_to_proto(str, json_parser::proto_syntax::_3);
            MONIK_TRACE(s2);
            MONIK_TRACE(s3);
            return !(s2.empty() || s3.empty());
        }
        MONIK_ASSERT(0);
        return false;
    }

    static unit_test s_test;

}}} // sdl
#endif //#if SV_DEBUG
