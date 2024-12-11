#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/log.h"

class DP : public esphome::PollingComponent, public esphome::uart::UARTDevice {
public:
  DP(esphome::uart::UARTComponent *parent);

  esphome::binary_sensor::BinarySensor *compressor;
  esphome::binary_sensor::BinarySensor *fan_low;
  esphome::binary_sensor::BinarySensor *fan_high;
  esphome::binary_sensor::BinarySensor *circulation_pump_hp;
  esphome::binary_sensor::BinarySensor *supplementary_heating;
  esphome::binary_sensor::BinarySensor *alarm_led;

  void setup() override;
  void update() override;
  void readData();
  void processHexString(char* hexString);
  std::vector<bool> getBits(uint8_t value);
  void publishSensorStates();
};
