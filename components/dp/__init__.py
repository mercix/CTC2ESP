import esphome.codegen as cg
import esphome.components.uart as uart
from esphome.components import binary_sensor
from esphome.const import CONF_ID

CODEOWNERS = ["@your_github_username"]

dp_ns = cg.esphome_ns.namespace("dp")
DP = dp_ns.class_("DP", cg.PollingComponent, uart.UARTDevice)

CONF_DP_ID = "dp_id"

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema()
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend({cg.Required(CONF_ID): cg.declare_id(DP)})
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    # Create the binary sensors and assign them to the DP instance
    compressor = cg.new_Pvariable(binary_sensor.BinarySensor.new())
    fan_low = cg.new_Pvariable(binary_sensor.BinarySensor.new())
    fan_high = cg.new_Pvariable(binary_sensor.BinarySensor.new())
    circulation_pump_hp = cg.new_Pvariable(binary_sensor.BinarySensor.new())
    supplementary_heating = cg.new_Pvariable(binary_sensor.BinarySensor.new())
    alarm_led = cg.new_Pvariable(binary_sensor.BinarySensor.new())

    cg.add(var.compressor, compressor)
    cg.add(var.fan_low, fan_low)
    cg.add(var.fan_high, fan_high)
    cg.add(var.circulation_pump_hp, circulation_pump_hp)
    cg.add(var.supplementary_heating, supplementary_heating)
    cg.add(var.alarm_led, alarm_led)
