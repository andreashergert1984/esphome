import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    DEVICE_CLASS_POWER,
    ICON_EMPTY,
    CONF_VAILLANT_X6_ID,
)
from . import Vaillant_X6Component

DEPENDENCIES = ["uart"]

vaillant_x6_sensor_ns = cg.esphome_ns.namespace("vaillant_x6sensor")

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(vaillant_x6_sensor_ns),
        cv.Required(CONF_VAILLANT_X6_ID): cv.use_id(Vaillant_X6Component),
    }
)


def to_code(config):
    paren = yield cg.get_variable(config[CONF_VAILLANT_X6_ID])
