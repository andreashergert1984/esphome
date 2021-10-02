import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import uart

DEPENDENCIES = ["uart"]
CODEOWNERS = ["@andreashergert1984"]
AUTO_LOAD = ["binary_sensor", "sensor"]
MULTI_CONF = True

CONF_VAILLANT_X6_ID = "vaillant_x6_id"

vaillant_x6_ns = cg.esphome_ns.namespace("vaillant_x6")
Vaillant_x6Component = vaillant_x6_ns.class_("Vaillant_x6", cg.Component)

VAILLANT_X6_COMPONENT_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.Required(CONF_VAILLANT_X6_ID): cv.use_id(Vaillant_x6Component),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.Schema({cv.GenerateID(): cv.declare_id(Vaillant_x6Component)})
    .extend(cv.polling_component_schema("1s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
