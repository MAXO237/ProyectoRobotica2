#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

class WheelVelocitySubscriber : public rclcpp::Node
{
public:
    WheelVelocitySubscriber() : Node("wheel_velocity_subscriber")
    {
        subscription_ = this->create_subscription<std_msgs::msg::Float64MultiArray>(
            "wheel_velocities", 10,
            std::bind(&WheelVelocitySubscriber::callback, this, std::placeholders::_1));
        
        RCLCPP_INFO(this->get_logger(), "Wheel velocity subscriber started");
    }

private:
    void callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
    {
        if (msg->data.size() == 4) {
            RCLCPP_INFO(this->get_logger(),
                "Velocidades recibidas: FL=%.3f, RL=%.3f, FR=%.3f, RR=%.3f",
                msg->data[0], msg->data[1], msg->data[2], msg->data[3]);
        } else {
            RCLCPP_WARN(this->get_logger(), "Mensaje con tamaño inesperado: %zu", msg->data.size());
        }
    }

    rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<WheelVelocitySubscriber>());
    rclcpp::shutdown();
    return 0;
}
