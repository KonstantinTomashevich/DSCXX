find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(TestJSON TestModule.cpp json.cpp)
target_link_libraries(TestJSON JSON Boost::unit_test_framework)
