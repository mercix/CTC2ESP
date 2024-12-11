#pragma once

#include "esphome.h"
#include <vector>

class DP : public PollingComponent, public UARTDevice {
public:
  DP(UARTComponent *parent);

  binary_sensor::BinarySensor *compressor;
  binary_sensor::BinarySensor *fan_low;
  binary_sensor::BinarySensor *fan_high;
  binary_sensor::BinarySensor *circulation_pump_hp;
  binary_sensor::BinarySensor *supplementary_heating;
  binary_sensor::BinarySensor *alarm_led;

  void setup() override;
  void loop() override;
  void update() override;

private:
  void readData();
  void processHexString(char* hexString);
  std::vector<bool> getBits(uint8_t value);
  void publishSensorStates();
};
