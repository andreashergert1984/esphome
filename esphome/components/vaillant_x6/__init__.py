import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.const import CONF_ID, CONF_VALUE
from esphome.components import uart

DEPENDENCIES = ["uart"]
CODEOWNERS = ["@andreashergert1984"]
AUTO_LOAD = ["sensor"]
MULTI_CONF = True

vaillant_x6_ns = cg.esphome_ns.namespace("vaillant_x6")
Vaillant_x6Component = vaillant_x6_ns.class_("Vaillant_x6", cg.Component)

Device = vaillant_x6_ns.enum("Device")

SetOutputAction = vaillant_x6_ns.class_("SetOutputAction", automation.Action)

CONFIG_SCHEMA = cv.All(
    cv.Schema({cv.GenerateID(): cv.declare_id(Vaillant_x6Component)})
    .extend(cv.polling_component_schema("1s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

