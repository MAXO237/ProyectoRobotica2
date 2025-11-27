cmake_minimum_required(VERSION 3.8)
project(viz_package_cpp2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 1. ENCONTRAR PAQUETES (Solo los necesarios)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# 2. DEFINICIÓN DEL EJECUTABLE PWMConverter
# Asegúrate de que tu archivo esté en la carpeta 'src'
add_executable(pwm_converter src/PWMConverter.cpp)

target_include_directories(pwm_converter PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

target_compile_features(pwm_converter PUBLIC c_std_99 cxx_std_17)

# Dependencias del nodo
ament_target_dependencies(pwm_converter 
  rclcpp 
  std_msgs
)

# 3. INSTALACIÓN
install(TARGETS
  pwm_converter
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()