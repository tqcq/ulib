#include <json/json.h>
#include <ulib/log/log.h>

int
main(int argc, char *argv[])
{
    ULOG_INFO("main", "hello world");
    Json::Value root;
    root["hello"] = "world";
    // to string
    std::string str = root.toStyledString();
    ULOG_INFO("main", "json: {}", str.c_str());
    return 0;
}
