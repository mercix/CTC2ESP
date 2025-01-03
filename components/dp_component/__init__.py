import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, binary_sensor
from esphome.const import CONF_ID, CONF_UART_ID

DEPENDENCIES = ["uart"]

# Namespace and class definition
dp_ns = cg.esphome_ns.namespace("dp_component")
DpComponent = dp_ns.class_("DpComponent", cg.PollingComponent, uart.UARTDevice)

# Configuration keys
CONF_COMPRESSOR = "compressor"
CONF_FAN_LOW = "fan_low"
CONF_FAN_HIGH = "fan_high"
CONF_CIRCULATION_PUMP_HP = "circulation_pump_hp"
CONF_SUPPLEMENTARY_HEATING = "supplementary_heating"
CONF_ALARM_LED = "alarm_led"

# Main dp_component schema
DP_COMPONENT_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DpComponent),
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
}).extend(cv.COMPONENT_SCHEMA)

# Binary sensor schema for `platform: dp_component`
BINARY_SENSOR_PLATFORM_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_ID): cv.use_id(DpComponent),
    cv.Optional(CONF_COMPRESSOR): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_FAN_LOW): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_FAN_HIGH): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_CIRCULATION_PUMP_HP): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_SUPPLEMENTARY_HEATING): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_ALARM_LED): binary_sensor.binary_sensor_schema(),
})

CONFIG_SCHEMA = DP_COMPONENT_SCHEMA


async def to_code(config):
    # Initialize the dp_component
    uart_component = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[CONF_ID], uart_component)
    await cg.register_component(var, config)


async def to_code_binary_sensor(config):
    # Link binary sensors to dp_component
    var = await cg.get_variable(config[CONF_ID])

    if CONF_COMPRESSOR in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_COMPRESSOR])
        cg.add(var.set_compressor(sens))
    if CONF_FAN_LOW in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_FAN_LOW])
        cg.add(var.set_fan_low(sens))
    if CONF_FAN_HIGH in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_FAN_HIGH])
        cg.add(var.set_fan_high(sens))
    if CONF_CIRCULATION_PUMP_HP in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_CIRCULATION_PUMP_HP])
        cg.add(var.set_circulation_pump_hp(sens))
    if CONF_SUPPLEMENTARY_HEATING in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_SUPPLEMENTARY_HEATING])
        cg.add(var.set_supplementary_heating(sens))
    if CONF_ALARM_LED in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_ALARM_LED])
        cg.add(var.set_alarm_led(sens))
