/*

*/

#include <iostream>
#include <chrono>

#include "wam_redis_driver.hpp"

BarrettWamRedisDriver::BarrettWamRedisDriver(
    std::string redis_host, int redis_port,
    std::string sensors_key, std::string actuators_key
) : redis_host_(redis_host), redis_port_(redis_port),
    sensors_key_(sensors_key), actuators_key_(actuators_key)
{}

BarrettWamRedisDriver::~BarrettWamRedisDriver() {}

bool BarrettWamRedisDriver::connect() {

  // Connect to Redis
  if(pub_.connect(redis_host_, redis_port_) && (sub_.connect(redis_host_, redis_port_))) {
    std::cout << "Connected to the Redis server at "
        << redis_host_ << ":" << redis_port_ << "." << std::endl;
  } else {
    std::cerr << "Could not connect to the Redis server at "
        << redis_host_ << ":" << redis_port_ << "." << std::endl;
    return false;
  }

  // TODO connect to wam


  return true;
}

bool BarrettWamRedisDriver::actuate(std::vector<double> joint_torques) {
  return true;
}

const std::string& BarrettWamRedisDriver::get_sensor_command() {
  return sensor_command_;
}

void BarrettWamRedisDriver::run(int frequency) {

  sensor_command_ = "hello!";

  auto got_actuator_command = [&](const std::string& topic, const std::string& msg) {
    std::cout << "got actuator command: " << msg << std::endl;
  };

  sub_.subscribe(actuators_key_, got_actuator_command);

  while(true) {
    pub_.command({"PUBLISH", sensors_key_, get_sensor_command()});

    // TODO use frequency for sleep (or better yet, see if we have
    // event based capability for libbarrett
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}
