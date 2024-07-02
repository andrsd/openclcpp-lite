#include "gmock/gmock.h"
#include "openclcpp-lite/template.h"

namespace ocl = openclcpp_lite;

TEST(TemplateTest, build)
{
    ocl::Template::Params params;
    params.set("type", std::string("int"));
    params.set("i", 1234);
    params.set("d", 0.123456789);
    std::string t = "void fn({{ type }} type) { {{ i }}, {{ d }} }";
    auto s = ocl::Template::build(t, params);
    EXPECT_EQ(s, "void fn(int type) { 1234, 0.123457 }");
}

TEST(TemplateTest, non_existing_var)
{
    ocl::Template::Params params;
    params.set("type", std::string("int"));
    std::string t = "void fn({{ type-asdf }} type);";
    try {
        ocl::Template::build(t, params);
        FAIL();
    }
    catch (ocl::Exception & e) {
        EXPECT_STREQ(e.what(), "No variable named 'type-asdf'");
    }
}

TEST(TemplateTest, no_var_name)
{
    ocl::Template::Params params;
    params.set("type", std::string("int"));
    std::string t = "void fn({{ }} type);";
    try {
        ocl::Template::build(t, params);
        FAIL();
    }
    catch (ocl::Exception & e) {
        EXPECT_STREQ(e.what(), "Malformed template");
    }
}

TEST(TemplateTest, not_closed)
{
    ocl::Template::Params params;
    params.set("type", std::string("int"));
    std::string t = "void fn({{";
    try {
        ocl::Template::build(t, params);
        FAIL();
    }
    catch (ocl::Exception & e) {
        EXPECT_STREQ(e.what(), "Malformed template");
    }
}

TEST(TemplateTest, empty)
{
    ocl::Template::Params params;
    EXPECT_EQ(ocl::Template::build("", params), "");
}
