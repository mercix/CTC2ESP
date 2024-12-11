import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

# Define the custom namespace for the dp_custom component
dp_ns = cg.esphome_ns.namespace("dp_custom")
DP = dp_ns.class_("DP", cg.PollingComponent, uart.UARTDevice)

# Define the configuration schema for dp_custom
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DP),
        # Ensure uart_id references an existing UARTComponent
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    }
).extend(uart.UART_DEVICE_SCHEMA)

# Function to generate code from the configuration and register the component
async def to_code(config):
    # Step 1: Get the actual UARTComponent by resolving uart_id
    uart_component = await uart.register_uart_device(config["uart_id"], config)

    # Step 2: Create the DP component with the resolved UARTComponent
    dp_component = cg.new_Pvariable(config[CONF_ID], uart_component)

    # Step 3: Register the DP component
    await cg.register_component(dp_component, config)
