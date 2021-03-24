import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, time
from esphome.const import CONF_ID, CONF_TIME_ID

DEPENDENCIES = ["time"]

CONF_POWER_ID = "power_id"

total_energy_ns = cg.esphome_ns.namespace("total_energy")
TotalEnergy = total_energy_ns.class_(
    "TotalEnergy", sensor.Sensor, cg.Component
)

CONFIG_SCHEMA = sensor.SENSOR_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(TotalEnergy),
        cv.GenerateID(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
        cv.Required(CONF_POWER_ID): cv.use_id(sensor.Sensor),
    }
).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)

    sens = yield cg.get_variable(config[CONF_POWER_ID])
    cg.add(var.set_parent(sens))
    time_ = yield cg.get_variable(config[CONF_TIME_ID])
    cg.add(var.set_time(time_))

@automation.register_action(
    "total_energy.reset",
    TotalEnergyResetAction,
    maybe_simple_id(
        {
            cv.Required(CONF_ID): cv.use_id(TotalEnergy),
        }
    ),
)
def total_energy_reset_code(config, action_id, template_arg, args):
    paren = yield cg.get_variable(config[CONF_ID])
    yield cg.new_Pvariable(action_id, template_arg, paren)
