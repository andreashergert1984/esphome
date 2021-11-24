import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from esphome.const import (
    CONF_ID,
    CONF_CHANNEL,
    CONF_PORT,
    CONF_UPDATE_INTERVAL,
    STATE_CLASS_NONE
    
)
from esphome.cpp_types import PollingComponent

from .. import (
    mega2560fanservocontroller_ns,
    Mega2560fanservocontrollerComponent,
    CONF_MEGA2560FANSERVOCONTROLLER_ID,
)

DEPENDENCIES = ["mega2560fanservocontroller"]
CODEOWNERS = ["@andreashergert1984"]

Mega2560fanservocontrollerSensor = mega2560fanservocontroller_ns.class_(
    "Mega2560FanServoControllerSensor", sensor.Sensor, cg.PollingComponent
)

CONFIG_SCHEMA = sensor.sensor_schema(
    state_class=STATE_CLASS_NONE,
).extend(
    {
            cv.GenerateID(CONF_MEGA2560FANSERVOCONTROLLER_ID): cv.use_id(Mega2560fanservocontrollerComponent),
            cv.GenerateID(): cv.declare_id(Mega2560fanservocontrollerSensor),
            cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=3),
            cv.Required(CONF_PORT): cv.int_range(min=0, max=2),
            cv.Optional(CONF_UPDATE_INTERVAL,default = 60): cv.positive_time_period_seconds,
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    parent = await cg.get_variable(config[CONF_MEGA2560FANSERVOCONTROLLER_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_port(config[CONF_PORT]))
    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))
    cg.add(parent.register_rpm_sensor(config[CONF_CHANNEL],config[CONF_PORT],var))

