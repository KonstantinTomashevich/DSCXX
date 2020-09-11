find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(TestUniqueWords TestModule.cpp TestTrie.cpp)
target_link_libraries(TestUniqueWords UniqueWords Boost::unit_test_framework)
