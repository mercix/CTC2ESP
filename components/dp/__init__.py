import esphome.codegen as cg
import esphome.components.uart as uart
from esphome.components import binary_sensor
from esphome.const import CONF_ID

CODEOWNERS = ["@your_github_username"]

dp_ns = cg.esphome_ns.namespace("dp")
DP = dp_ns.class_("DP", cg.PollingComponent, uart.UARTDevice)

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema()
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend({cg.required(CONF_ID): cg.declare_id(DP)})
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
