cmake_minimum_required(VERSION 3.8)
project(bt_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(behaviortree_cpp REQUIRED)

add_executable(bt_sample src/00_bt_sample.cpp)
ament_target_dependencies(bt_sample PUBLIC behaviortree_cpp)
target_include_directories(bt_sample PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(first_bt src/01_first_bt.cpp)
ament_target_dependencies(first_bt PUBLIC behaviortree_cpp)
target_include_directories(first_bt PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(blackboard src/02_blackboard.cpp)
ament_target_dependencies(blackboard PUBLIC behaviortree_cpp)
target_include_directories(blackboard PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(generic_ports src/03_generic_ports.cpp)
ament_target_dependencies(generic_ports PUBLIC behaviortree_cpp)
target_include_directories(generic_ports PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(async_bt src/04_async_bt.cpp)
ament_target_dependencies(async_bt PUBLIC behaviortree_cpp)
target_include_directories(async_bt PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(pass_through_door src/05_pass_through_door.cpp)
ament_target_dependencies(pass_through_door PUBLIC behaviortree_cpp)
target_include_directories(pass_through_door PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(port_remapping src/06_port_remapping.cpp)
ament_target_dependencies(port_remapping PUBLIC behaviortree_cpp)
target_include_directories(port_remapping PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(scripting src/09_scripting.cpp)
ament_target_dependencies(scripting PUBLIC behaviortree_cpp)
target_include_directories(scripting PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS
  bt_sample
  first_bt
  blackboard
  generic_ports
  async_bt
  pass_through_door
  port_remapping
  scripting
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
