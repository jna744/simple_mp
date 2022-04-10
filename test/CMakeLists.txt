add_executable(${PROJECT_NAME}_tests smp_tests.cpp)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC ${PROJECT_NAME})
