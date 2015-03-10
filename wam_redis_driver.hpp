/*
*
*/

#pragma once

#include <string>
#include <vector>
#include <redox.hpp>
#include <jsoncpp/json/json.h>

#include <barrett/products/product_manager.h>

// Only support 7 DOF WAM right now
static const int DOF = 7;

// The macro below makes a number of typedefs that allow convenient access
// to commonly used barrett::units. For example, the typedefs establish
// "jp_type" as a synonym for "units::JointPositions<DOF>::type". This macro
// (along with a few others) is defined in barrett/units.h.
BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);

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

  Json::Reader reader_;
  Json::FastWriter writer_;
  Json::StyledWriter styled_writer_;

  std::string sensor_command_;

  barrett::ProductManager pm_;

  barrett::systems::Wam<DOF>* wam_;

  jp_type jpos_;  // jp is a DOFx1 column vector of joint positions
};
