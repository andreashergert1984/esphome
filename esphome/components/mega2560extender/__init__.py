import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import uart

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@andreashergert1984"]
AUTO_LOAD = ["binary_sensor", "sensor", "switch", "output"]
MULTI_CONF = True

CONF_MEGA2560EXTENDER_ID = "mega2560extender_id"

mega2560extender_ns = cg.esphome_ns.namespace("mega2560extender")
Mega2560extenderComponent = pipsolar_ns.class_("Mega2560Extender", cg.Component)

MEGA2560EXTENDER_COMPONENT_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.Required(CONF_MEGA2560EXTENDER_ID): cv.use_id(Mega2560extenderComponent),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.Schema({cv.GenerateID(): cv.declare_id(PipsolarComponent)})
    .extend(cv.polling_component_schema("1s"))
    .extend(uart.I2C_DEVICE_SCHEMA)
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)
