
#ifndef TEST_COMMON_FIXTURE_H
#define TEST_COMMON_FIXTURE_H

struct TestBaseFixt
{
    const std::string m_setup = "setup";
    const std::string m_teardown = "teardown";
};

#endif