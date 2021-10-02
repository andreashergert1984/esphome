import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ICON_EMPTY,
    ICON_THERMOMETER,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_HERTZ,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_EMPTY,
    UNIT_VOLT_AMPS,
    UNIT_WATT,
    CONF_BUS_VOLTAGE,
    CONF_BATTERY_VOLTAGE,
)
from .. import VAILLANT_X6_COMPONENT_SCHEMA, CONF_VAILLANT_X6_ID

DEPENDENCIES = ["uart"]

# known sensors
CONF_STORAGE_TEMPERATURE = "storage_temperature"

COMMANDS = {
    CONF_STORAGE_TEMPERATURE: {
        'command': 0x17,
        'data_type': 'float',
        'response_length' : 6,
        'data_length': 2,
        'has_status': True
    }
}


TYPES = {
    CONF_STORAGE_TEMPERATURE: sensor.sensor_schema(
        UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE
    ),
}

CONFIG_SCHEMA = VAILLANT_X6_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): schema for type, schema in TYPES.items()}
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_VAILLANT_X6_ID])

    for type, _ in TYPES.items():
        if type in config:
            conf = config[type]
            sens = await sensor.new_sensor(conf)
            cg.add(paren.addSensor(sens,COMMANDS[type]['command'],COMMANDS[type]['data_type'],COMMANDS[type]['response_length'],COMMANDS[type]['data_length'],COMMANDS[type]['has_status']))
