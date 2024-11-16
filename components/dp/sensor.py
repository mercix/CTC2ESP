import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, binary_sensor
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]

# Define the namespace and component
dp_ns = cg.esphome_ns.namespace("dp")
DPComponent = dp_ns.class_("DP", cg.PollingComponent, uart.UARTDevice)

# Configuration schema
CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(DPComponent),
            cv.Optional("compressor"): binary_sensor.binary_sensor_schema(),
            cv.Optional("fan_low"): binary_sensor.binary_sensor_schema(),
            cv.Optional("fan_high"): binary_sensor.binary_sensor_schema(),
            cv.Optional("circulation_pump_hp"): binary_sensor.binary_sensor_schema(),
            cv.Optional("supplementary_heating"): binary_sensor.binary_sensor_schema(),
            cv.Optional("alarm_led"): binary_sensor.binary_sensor_schema(),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("5s"))
)

# Component generation
def to_code(config):
    # Create the component
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

    # Attach binary sensors
    if "compressor" in config:
        sens = yield binary_sensor.new_binary_sensor(config["compressor"])
        cg.add(var.set_compressor_sensor(sens))
    if "fan_low" in config:
        sens = yield binary_sensor.new_binary_sensor(config["fan_low"])
        cg.add(var.set_fan_low_sensor(sens))
    if "fan_high" in config:
        sens = yield binary_sensor.new_binary_sensor(config["fan_high"])
        cg.add(var.set_fan_high_sensor(sens))
    if "circulation_pump_hp" in config:
        sens = yield binary_sensor.new_binary_sensor(config["circulation_pump_hp"])
        cg.add(var.set_circulation_pump_hp_sensor(sens))
    if "supplementary_heating" in config:
        sens = yield binary_sensor.new_binary_sensor(config["supplementary_heating"])
        cg.add(var.set_supplementary_heating_sensor(sens))
    if "alarm_led" in config:
        sens = yield binary_sensor.new_binary_sensor(config["alarm_led"])
        cg.add(var.set_alarm_led_sensor(sens))
