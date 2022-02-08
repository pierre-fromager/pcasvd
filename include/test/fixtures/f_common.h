
#ifndef TEST_COMMON_FIXTURE_H
#define TEST_COMMON_FIXTURE_H

struct TestBaseFixt
{
    const std::string ctcname = boost::unit_test::framework::current_test_case().p_name;
    const std::string m_setup = "Setup " + ctcname;
    const std::string m_teardown = "Teardown " + ctcname;
};

#endif