#pragma once
#include "esphome.h"

namespace esphome {
namespace dp_component {

class DpComponent : public PollingComponent, public UARTDevice {
 public:
  DpComponent(UARTComponent *parent) : PollingComponent(5000), UARTDevice(parent) {}

  void set_compressor(binary_sensor::BinarySensor *sensor) { this->compressor = sensor; }
  void set_fan_low(binary_sensor::BinarySensor *sensor) { this->fan_low = sensor; }
  void set_fan_high(binary_sensor::BinarySensor *sensor) { this->fan_high = sensor; }
  void set_circulation_pump_hp(binary_sensor::BinarySensor *sensor) { this->circulation_pump_hp = sensor; }
  void set_supplementary_heating(binary_sensor::BinarySensor *sensor) { this->supplementary_heating = sensor; }
  void set_alarm_led(binary_sensor::BinarySensor *sensor) { this->alarm_led = sensor; }

  void setup() override;
  void update() override;

 private:
  binary_sensor::BinarySensor *compressor{nullptr};
  binary_sensor::BinarySensor *fan_low{nullptr};
  binary_sensor::BinarySensor *fan_high{nullptr};
  binary_sensor::BinarySensor *circulation_pump_hp{nullptr};
  binary_sensor::BinarySensor *supplementary_heating{nullptr};
  binary_sensor::BinarySensor *alarm_led{nullptr};
};

}  // namespace dp_component
}  // namespace esphome
