import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, uart
from esphome.const import CONF_ID, CONF_NAME

DEPENDENCIES = ["uart"]

dp_ns = cg.esphome_ns.namespace("dp_component")
DpComponent = dp_ns.class_("DpComponent", cg.PollingComponent, uart.UARTDevice)

CONF_COMPRESSOR = "compressor"
CONF_FAN_LOW = "fan_low"
CONF_FAN_HIGH = "fan_high"
CONF_CIRCULATION_PUMP_HP = "circulation_pump_hp"
CONF_SUPPLEMENTARY_HEATING = "supplementary_heating"
CONF_ALARM_LED = "alarm_led"

CONFIG_SCHEMA = cv.nameable(
    binary_sensor.BINARY_SENSOR_PLATFORM_SCHEMA.extend({
        cv.GenerateID(): cv.declare_id(DpComponent),
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
        cv.Optional(CONF_COMPRESSOR): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_FAN_LOW): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_FAN_HIGH): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_CIRCULATION_PUMP_HP): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_SUPPLEMENTARY_HEATING): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_ALARM_LED): binary_sensor.binary_sensor_schema(),
    }).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], await cg.get_variable(config["uart_id"]))
    await cg.register_component(var, config)

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
