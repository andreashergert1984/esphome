import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_FREQUENCY, CONF_ID

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@andreashergert1984"]
AUTO_LOAD = ["binary_sensor", "output"]
MULTI_CONF = True
CONF_MEGA2560FANSERVOCONTROLLER_ID = "mega2560fanservocontroller_id"

CONF_PWM_CHANNEL0 = 'pwm_channel0'
CONF_PWM_CHANNEL1 = 'pwm_channel1'
CONF_PWM_CHANNEL2 = 'pwm_channel2'
CONF_PWM_CHANNEL3 = 'pwm_channel3'
CONF_PWM_FREQUENCIES = {
    "0" : 0,
    "50Hz": 50,
    "25kHz": 25000,
}

mega2560fanservocontroller_ns = cg.esphome_ns.namespace("mega2560fanservocontroller")
Mega2560fanservocontrollerComponent = mega2560fanservocontroller_ns.class_("Mega2560FanServoController", cg.Component)


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Mega2560fanservocontrollerComponent),
            cv.Optional(CONF_PWM_CHANNEL0, default="0"): cv.enum(CONF_PWM_FREQUENCIES),
            cv.Optional(CONF_PWM_CHANNEL1, default="0"): cv.enum(CONF_PWM_FREQUENCIES),
            cv.Optional(CONF_PWM_CHANNEL2, default="0"): cv.enum(CONF_PWM_FREQUENCIES),
            cv.Optional(CONF_PWM_CHANNEL3, default="0"): cv.enum(CONF_PWM_FREQUENCIES),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x0a))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    cg.add(var.set_pwm_channel(0,config[CONF_PWM_CHANNEL0]))
    cg.add(var.set_pwm_channel(1,config[CONF_PWM_CHANNEL1]))
    cg.add(var.set_pwm_channel(2,config[CONF_PWM_CHANNEL2]))
    cg.add(var.set_pwm_channel(3,config[CONF_PWM_CHANNEL3]))

