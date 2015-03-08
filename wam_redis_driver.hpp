/*
*
*/

#pragma once

#include <string>
#include <vector>
#include <redox.hpp>

/**
*
*/
class BarrettWamRedisDriver {

public:

  /**
  *
  */
  BarrettWamRedisDriver(
      std::string redis_host = "localhost",
      int redis_port = 6379,
      std::string sensors_key = "barrett:wam:sensors",
      std::string actuators_key = "barrett:wam:actuators"
  );

  /**
  *
  */
  ~BarrettWamRedisDriver();

  /**
  *
  */
  bool connect();

  /**
  *
  */
  void run(int frequency = 1000);

private:

  bool actuate(std::vector<double> joint_torques);
  const std::string& get_sensor_command();

  const std::string redis_host_;
  const int redis_port_;

  const std::string sensors_key_;
  const std::string actuators_key_;

  redox::Redox pub_;
  redox::Subscriber sub_;

  std::string sensor_command_;

};
