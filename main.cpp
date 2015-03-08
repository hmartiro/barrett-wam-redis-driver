/*

*/

#include <chrono>

#include "wam_redis_driver.hpp"

int main(int argc, char* argv[]) {

  std::string redis_host = "localhost";
  int redis_port = 6379;

  std::string sensors_key = "barrett:wam:sensors";
  std::string actuators_key = "barrett:wam:actuators";

  int frequency = 1000;

  BarrettWamRedisDriver driver(redis_host, redis_port, sensors_key, actuators_key);

  if(!driver.connect()) return 1;

  driver.run(frequency);

  return 0;
}