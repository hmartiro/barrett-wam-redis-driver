/*

*/

#include "wam_redis_driver.hpp"

using namespace barrett;

int main(int argc, char* argv[]) {

  std::string redis_host = "172.24.68.224";
  int redis_port = 6379;

  std::string sensors_key = "barrett:wam:sensors";
  std::string actuators_key = "barrett:wam:actuators";

  BarrettWamRedisDriver driver(redis_host, redis_port, sensors_key, actuators_key);

  if(!driver.connect()) return 1;

  driver.run();

  return 0;
}
