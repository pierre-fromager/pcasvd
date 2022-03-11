find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})
add_definitions (-DBOOST_TEST_DYN_LINK -pthread)
add_executable (
    pca_test 
    main.cpp 
    suites/pca.cpp
    suites/datafile.cpp
    suites/datatree.cpp
    #suites/gplot.cpp
    )
target_link_libraries (
    pca_test
    pca_lib
    pthread
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
    
add_test (NAME MyTest COMMAND pca_test)