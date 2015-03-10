/*

*/

#include <iostream>
#include <chrono>
#include <thread>

#include <barrett/exception.h>
#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>

#include "wam_redis_driver.hpp"

using namespace barrett;
using namespace redox;

# define BARRETT_SMF_WAIT_FOR_SHIFT_ACTIVATE true
# define BARRETT_SMF_PROMPT_ON_ZEROING true

#ifndef BARRETT_SMF_WAM_CONFIG_PATH
#  define BARRETT_SMF_WAM_CONFIG_PATH NULL
#endif

BarrettWamRedisDriver::BarrettWamRedisDriver(
    std::string redis_host, int redis_port,
    std::string sensors_key, std::string actuators_key
) : redis_host_(redis_host), redis_port_(redis_port),
    sensors_key_(sensors_key), actuators_key_(actuators_key)
{
  // Give us pretty stack-traces when things die
  barrett::installExceptionHandler();
}

BarrettWamRedisDriver::~BarrettWamRedisDriver() {

//  wam_->idle();

  // Wait for the user to press Shift-idle
  pm_.getSafetyModule()->waitForMode(SafetyModule::IDLE);

}

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

  // TODO separate into method

  pm_.waitForWam(BARRETT_SMF_PROMPT_ON_ZEROING);
  pm_.wakeAllPucks();

  if (pm_.foundWam3()) {
    std::cerr << "Only 7 DOF WAM supported, found 3 DOF!" << std::endl;
    return false;
  } else if (pm_.foundWam4()) {
    std::cerr << "Only 7 DOF WAM supported, found 4 DOF!" << std::endl;
    return false;
  } else if (pm_.foundWam7()) {
    wam_ = pm_.getWam7(BARRETT_SMF_WAIT_FOR_SHIFT_ACTIVATE, BARRETT_SMF_WAM_CONFIG_PATH);
  } else {
    std::cerr << "No WAM was found. Perhaps you have found a bug in ProductManager::waitForWam()" << std::endl;
    return false;
  }

  // The WAM is now Shift-activated and applying zero torque with a loop-rate
  // of 500Hz.
  
  return true;
}

const std::string& BarrettWamRedisDriver::get_sensor_command() {

  // TODO read from Barrett
  return sensor_command_;
}

void BarrettWamRedisDriver::run(int frequency) {

  sensor_command_ = "hello!";

  sub_.subscribe(actuators_key_, [&](const std::string& topic, const std::string& msg) {

      // TODO store value as member
      Json::Value value;

      reader_.parse(msg, value);

      std::cout << "Got message: " << styled_writer_.write(value) << std::endl;

      if(!value.isObject()) {
        std::cerr << "Expecting top-level JSON object!" << std::endl;
        return;
      }

      if(value.isMember("q") && value["q"].isArray()) {

        if(value["q"].size() != DOF) {
          std::cerr << "Expecting " << DOF << " values!" << std::endl;
          return;
        }

        for(int i = 0; i < DOF; i++) {

          if(!value["q"][i].isConvertibleTo(Json::ValueType::realValue)) {
            std::cerr << "Expecting doubles!" << std::endl;
            return;
          }

          jpos_[i] = value["q"][i].asDouble();
        }

        std::cout << "jpos_ = " << jpos_.transpose() << std::endl;

        wam_->moveTo(jpos_);
      }

      return;
  });

  while(true) {

    // TODO write as JSON

    pub_.command({"PUBLISH", sensors_key_, get_sensor_command()});

    // TODO use frequency for sleep (or better yet, see if we have
    // event based capability for libbarrett
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frequency));
  }
}
