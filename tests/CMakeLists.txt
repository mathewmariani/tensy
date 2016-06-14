# thanks to anax https://github.com/miguelmartin75/anax
set(TESTING_LIBRARY_NAME "tensy_tests")

add_library(${TESTING_LIBRARY_NAME} ${PROJECT_FILES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lest)

# neat little macro for creating tests
macro(create_test TARGET_NAME SOURCE)
  add_executable(${TARGET_NAME} ${SOURCE})
  target_link_libraries(
    ${TARGET_NAME}
    ${TESTING_LIBRARY_NAME}
  )
  add_test(${TARGET_NAME} ${TARGET_NAME})
endmacro()

# create the tests
create_test(test_entities test_entities.cpp)
create_test(test_processes test_processes.cpp)
