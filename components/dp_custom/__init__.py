import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

# Define the custom namespace for your component
dp_ns = cg.esphome_ns.namespace("dp_custom")
DP = dp_ns.class_("DP", cg.PollingComponent, uart.UARTDevice)

# Define the configuration schema for dp_custom
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DP),
        # The uart_id must be an actual UARTComponent, so we use cv.use_id() to reference it
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    }
).extend(uart.UART_DEVICE_SCHEMA)

# Generate the code and register the component
async def to_code(config):
    # Step 1: Resolve uart_id to an actual UARTComponent object
    uart_component = await uart.register_uart_device(config["uart_id"], config)

    # Step 2: Create the DP component and pass the resolved uart_component
    var = cg.new_Pvariable(config[CONF_ID], uart_component)

    # Step 3: Register the DP component
    await cg.register_component(var, config)
