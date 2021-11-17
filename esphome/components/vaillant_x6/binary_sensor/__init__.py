import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    DEVICE_CLASS_EMPTY,
    ICON_EMPTY,
    UNIT_EMPTY,
    CONF_ID,
)
from .. import VAILLANT_X6_COMPONENT_SCHEMA, CONF_VAILLANT_X6_ID

# known sensors
CONF_INTERNAL_PUMP = "internal_pump"
CONF_EXTERNAL_PUMP = "external_pump"
CONF_FLAME_SIGNAL = "flame_signal"
CONF_PRIORITYSWITCHINGVALVE = "priorityswitchingvalve"

COMMANDS = {
    CONF_INTERNAL_PUMP: {
        'command': 0x44,
        'data_type': 0,
        'response_length' : 4,
        'data_length': 1,
        'has_status': 0,
    },
    CONF_EXTERNAL_PUMP: {
        'command': 0x3F,
        'data_type': 0,
        'response_length' : 4,
        'data_length': 1,
        'has_status': 0,
    },
    CONF_FLAME_SIGNAL: {
        'command': 0x05,
        'data_type': 0,
        'response_length' : 4,
        'data_length': 2,
        'has_status': 1,
    },
    CONF_PRIORITYSWITCHINGVALVE: {
        'command': 0x42,
        'data_type': 0,
        'response_length' : 4,
        'data_length': 2,
        'has_status': 1,
    }

}


TYPES = {
    CONF_INTERNAL_PUMP,
    CONF_EXTERNAL_PUMP,
    CONF_FLAME_SIGNAL,
    CONF_PRIORITYSWITCHINGVALVE,
}

CONFIG_SCHEMA = VAILLANT_X6_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): binary_sensor.BINARY_SENSOR_SCHEMA for type in TYPES}
)

async def to_code(config):
    paren = await cg.get_variable(config[CONF_VAILLANT_X6_ID])
    for type in TYPES:
        if type in config:
            conf = config[type]
            sens = cg.new_Pvariable(conf[CONF_ID])
            await binary_sensor.register_binary_sensor(sens, conf)
            cg.add(paren.addBinarySensor(sens,COMMANDS[type]['command'],COMMANDS[type]['data_type'],COMMANDS[type]['response_length'],COMMANDS[type]['data_length'],COMMANDS[type]['has_status'],type))
