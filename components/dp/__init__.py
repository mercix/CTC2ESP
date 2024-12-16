import esphome.codegen as cg
import esphome.components.binary_sensor as binary_sensor
import esphome.components.uart as uart
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_UART_ID

DEPENDENCIES = ['uart']

# Define the namespace and class for the DP component
dp_ns = cg.esphome_ns.namespace('dp')
DP = dp_ns.class_('DP', cg.PollingComponent, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DP),
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
}).extend(cv.polling_component_schema('5s'))

def to_code(config):
    # Retrieve the UART component using the correct type
    uart_component = yield cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[CONF_ID], uart_component)
    
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, uart_component)  # Register UART device correctly

    # Define and attach binary sensors
    sensors = {
        "compressor": "Compressor",
        "fan_low": "Fan Low",
        "fan_high": "Fan High",
        "circulation_pump_hp": "Circulation Pump HP",
        "supplementary_heating": "Supplementary Heating",
        "alarm_led": "Alarm",
    }

    for attr_name, sensor_name in sensors.items():
        sens = cg.new_Pvariable(binary_sensor.BinarySensor())
        cg.add(sens.set_name(sensor_name))
        setattr(var, attr_name, sens)  # Attach the sensor to the DP instance
