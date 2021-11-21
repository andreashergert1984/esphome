import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output

from esphome.const import (
    CONF_ID,
    CONF_CHANNEL,
    CONF_PORT
)

from .. import (
    mega2560fanservocontroller_ns,
    Mega2560fanservocontrollerComponent,
    CONF_MEGA2560FANSERVOCONTROLLER_ID,
)

DEPENDENCIES = ["mega2560fanservocontroller"]
CODEOWNERS = ["@andreashergert1984"]


Mega2560fanservocontrollerOutput = mega2560fanservocontroller_ns.class_(
    "Mega2560FanServoControllerOutput", cg.Component, output.FloatOutput
)

CONFIG_SCHEMA = cv.All(
    output.FLOAT_OUTPUT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_MEGA2560FANSERVOCONTROLLER_ID): cv.use_id(Mega2560fanservocontrollerComponent),
            cv.GenerateID(): cv.declare_id(Mega2560fanservocontrollerOutput),
            cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=3),
            cv.Required(CONF_PORT): cv.int_range(min=0, max=2),
        }
    ),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await output.register_output(var, config)
    parent = await cg.get_variable(config[CONF_MEGA2560FANSERVOCONTROLLER_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_port(config[CONF_PORT]))
    cg.add(parent.enable_pwm_port(config[CONF_CHANNEL],config[CONF_PORT]))
