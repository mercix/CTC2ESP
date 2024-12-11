import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

# Define the custom namespace for your component
dp_ns = cg.esphome_ns.namespace("dp_custom")
DP = dp_ns.class_("DP", cg.PollingComponent, uart.UARTDevice)

# Define the configuration schema
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DP),
        # Make sure uart_id is passed properly to the DP component
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    }
).extend(uart.UART_DEVICE_SCHEMA)

# Generate code from the configuration and register the component
async def to_code(config):
    # Register the UART component by resolving the uart_id to the actual UARTComponent object
    uart_component = await uart.register_uart_device(config["uart_id"], config)
    
    # Create the DP component and pass the resolved uart_component
    var = cg.new_Pvariable(config[CONF_ID], uart_component)
    
    # Register the DP component with the ESPHome system
    await cg.register_component(var, config)
