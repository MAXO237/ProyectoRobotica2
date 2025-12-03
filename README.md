# ProyectoRobotica2
## Requisitos
- Tener el driver de joystick instalado. Se instala con:
  ```bash
  sudo apt install ros-jazzy-joy
  ```
## Instrucciones de Ejecución
- Conectar el joystick a la computadora local.
- Crear un espacio de trabajo en la raspberry pi de Paquito, y poner allí el paquete [**paquito_package_cpp**](./paquito_package_cpp).
- Crear un espacio de trabajo en la computadora local, y poner allí el paquete [**viz_package_cpp**](./viz_package_cpp2).
- Cargar el archivo [**Esclavo.ino**](./Codigo_Arduino/Esclavo) al Arduino de Paquito.
- Compilar (**colcon build**) y activar el underlay y overlay (hacer los dos **source**) en los dos espacio de trabajo (el de la computadora local y el del raspberry pi).
### Dentro de la computadora local
- En terminal 1, escribir:
  ```bash
  ros2 run joy joy_node
  ```
- En terminal 2, escribir:
  ```bash
  ros2 run viz_package_cpp2 ps2_control_node
  ```
- En terminal 3, escribir:
  ```bash
  ros2 run viz_package_cpp2 wheel_publisher
  ```
- (Opcional: Para ver los rad/s que se envían) En otra terminal, escribir:
  ```bash
  ros2 run viz_package_cpp2 wheel_subscriber
  ```
- (Opcional: Para enviar un mensaje Twist sin el joystick) En otra terminal, escribir:
  ```bash
  ros2 topic pub --once /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.0, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.0}}"
  ```
  Se pueden modificar los valores (x, y, z) a su consideración.
### Dentro de la raspberry pi de Paquito
- Dentro del espacio de trabajo, escribir:
  ```bash
  ros2 run paquito_package_cpp pwm_converter
  ```
## Uso
Una vez terminados los pasos anteriores, usar el joystick para controlar a Paquito.
