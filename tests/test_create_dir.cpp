/*
 * This content is released under the MIT License as specified in https://raw.githubusercontent.com/gabime/spdlog/master/LICENSE
 */
#include "includes.h"

using spdlog::details::os::create_dir;
using spdlog::details::os::file_exists;

void test_create_dir(const char *path, const char *normalized_path)
{
    auto rv = create_dir(path);
    REQUIRE(rv == true);
    REQUIRE(file_exists(normalized_path));
}

#include "spdlog/sinks/stdout_color_sinks.h"

TEST_CASE("create_dir", "[create_dir]")
{
    prepare_logdir();
    test_create_dir("test_logs/dir1/dir1", "test_logs/dir1/dir1");
    test_create_dir("test_logs/dir1///dir2", "test_logs/dir1/dir2");
    test_create_dir("./test_logs/dir1/dir3", "test_logs/dir1/dir3");
    test_create_dir("test_logs/../test_logs/dir1/dir4", "test_logs/dir1/dir4");
    test_create_dir("./test_logs/dir1/dir2/dir99/../dir23", "test_logs/dir1/dir2/dir23");
}

TEST_CASE("dir_name", "[create_dir]")
{
    using spdlog::details::os::dir_name;
    REQUIRE(dir_name("").empty());
    REQUIRE(dir_name("dir").empty());
    REQUIRE(dir_name("dir/") == "dir");
    REQUIRE(dir_name("dir///") == "dir//");
    REQUIRE(dir_name("dir/file") == "dir");
    REQUIRE(dir_name("dir/file.txt") == "dir");
    REQUIRE(dir_name("dir/file.txt/") == "dir/file.txt");
    REQUIRE(dir_name("/dir/file.txt") == "/dir");
    REQUIRE(dir_name("//dir/file.txt") == "//dir");
    REQUIRE(dir_name("//dir/file.txt") == "//dir");
    REQUIRE(dir_name("../file.txt") == "..");
    REQUIRE(dir_name("./file.txt") == ".");
#ifdef WIN32
    REQUIRE(dir_name(R"(dir\)") == "dir");
    REQUIRE(dir_name(R"(dir\\\)") == "dir//");
    REQUIRE(dir_name(R"(dir\file)") == "dir");
    REQUIRE(dir_name(R"(dir\file.txt)") == "dir");
    REQUIRE(dir_name(R"(dir\file.txt\)") == "dir/file.txt");
    REQUIRE(dir_name(R"(\dir\file.txt)") == "/dir");
    REQUIRE(dir_name(R"(\\dir\file.txt)") == "//dir");
    REQUIRE(dir_name(R"(\\dir\file.txt)") == "//dir");
    REQUIRE(dir_name(R"(..\file.txt)") == "..");
    REQUIRE(dir_name(R"(.\file.txt)") == ".");
    REQUIRE(dir_name(R"(c:\\a\b\c\d\file.txt)") == "c://a/b/c/d");
    //REQUIRE(dir_name(R"(c:\\a)") == "c://");
#endif

}
