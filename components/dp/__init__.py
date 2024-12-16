import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, binary_sensor
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]

dp_ns = cg.esphome_ns.namespace("dp")
DP = dp_ns.class_(
    "DP", cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema({cv.GenerateID(): cv.declare_id(DP)})
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    # Create the DP component instance
    var = cg.new_Pvariable(config[CONF_ID])
    
    # Register the component and the UART device
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

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
        sens = binary_sensor.BinarySensor.new()  # Create the binary sensor
        cg.add(sens.set_name(sensor_name))
        cg.add(getattr(var, attr_name))  # Attach the sensor to the component
