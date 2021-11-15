import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_TIMESTAMP,
    DEVICE_CLASS_VOLTAGE,
    ICON_EMPTY,
    ICON_THERMOMETER,
    ICON_TIMER,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_HERTZ,
    UNIT_MINUTE,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_EMPTY,
    UNIT_VOLT_AMPS,
    UNIT_WATT,
    CONF_BUS_VOLTAGE,
    CONF_BATTERY_VOLTAGE,
)
from .. import VAILLANT_X6_COMPONENT_SCHEMA, CONF_VAILLANT_X6_ID

# known sensors
CONF_STORAGE_TEMPERATURE = "storage_temperature"
CONF_FORERUN_TEMPERATURE = "forerun_temperature"
CONF_BURNER_LOCK_TIME_LEFT = "burner_lock_time_left"

COMMANDS = {
    CONF_STORAGE_TEMPERATURE: {
        'command': 0x17,
        'data_type': 2,
        'response_length' : 6,
        'data_length': 2,
        'has_status': True
    },
    CONF_FORERUN_TEMPERATURE: {
        'command': 0x18,
        'data_type': 2,
        'response_length' : 6,
        'data_length': 2,
        'has_status': True
    },
    CONF_BURNER_LOCK_TIME_LEFT: {
        'command': 0x38,
        'data_type': 1,
        'response_length' : 4,
        'data_length': 1,
        'has_status': False
    },
}


TYPES = {
    CONF_STORAGE_TEMPERATURE: sensor.sensor_schema(
        UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE
    ),
    CONF_FORERUN_TEMPERATURE: sensor.sensor_schema(
        UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE 
    ),
    CONF_BURNER_LOCK_TIME_LEFT: sensor.sensor_schema(
        UNIT_MINUTE, ICON_TIMER, 1, DEVICE_CLASS_EMPTY
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
            cg.add(paren.addSensor(sens,COMMANDS[type]['command'],COMMANDS[type]['data_type'],COMMANDS[type]['response_length'],COMMANDS[type]['data_length'],COMMANDS[type]['has_status'],type))
