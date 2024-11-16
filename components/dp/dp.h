#include "esphome.h"

class DP : public PollingComponent, public UARTDevice {
public:
  DP() : PollingComponent(5000) {}

  void set_compressor_sensor(binary_sensor::BinarySensor *sensor) { compressor_ = sensor; }
  void set_fan_low_sensor(binary_sensor::BinarySensor *sensor) { fan_low_ = sensor; }
  void set_fan_high_sensor(binary_sensor::BinarySensor *sensor) { fan_high_ = sensor; }
  void set_circulation_pump_hp_sensor(binary_sensor::BinarySensor *sensor) { circulation_pump_hp_ = sensor; }
  void set_supplementary_heating_sensor(binary_sensor::BinarySensor *sensor) { supplementary_heating_ = sensor; }
  void set_alarm_led_sensor(binary_sensor::BinarySensor *sensor) { alarm_led_ = sensor; }

  void setup() override {}
  void update() override;

private:
  binary_sensor::BinarySensor *compressor_;
  binary_sensor::BinarySensor *fan_low_;
  binary_sensor::BinarySensor *fan_high_;
  binary_sensor::BinarySensor *circulation_pump_hp_;
  binary_sensor::BinarySensor *supplementary_heating_;
  binary_sensor::BinarySensor *alarm_led_;

  void read_data();
  void process_data(const char *hex_data);
};
