import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart
from esphome.const import CONF_ID

# Define the custom namespace for your component
dp_ns = cg.esphome_ns.namespace("dp_custom")
DP = dp_ns.class_("DP", cg.PollingComponent, uart.UARTDevice)

# Define the configuration schema
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DP),
        # Ensure the uart_id is passed properly
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    }
).extend(uart.UART_DEVICE_SCHEMA)

# Generate code from the configuration and register the component
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config["uart_id"])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
