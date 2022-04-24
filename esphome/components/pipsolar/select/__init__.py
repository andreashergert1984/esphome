import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import CONF_ID

# from esphome.jsonschema import jschema_composite

from .. import CONF_PIPSOLAR_ID, PIPSOLAR_COMPONENT_SCHEMA, pipsolar_ns

DEPENDENCIES = ["uart"]

CODEOWNERS = ["@andreashergert1984"]
CONF_OPTIONSMAP = "optionsmap"
CONF_STATUSMAP = "statusmap"

CONF_OUTPUT_SOURCE_PRIORITY = "output_source_priority"
CONF_CHARGER_SOURCE_PRIORITY = "charger_source_priority"

PipsolarSelect = pipsolar_ns.class_("PipsolarSelect", cg.Component, select.Select)


# @jschema_composite
def ensure_option_map():
    def validator(value):
        cv.check_not_templatable(value)
        option = cv.All(cv.string_strict)
        mapping = cv.All(cv.string_strict)
        options_map_schema = cv.Schema({option: mapping})
        value = options_map_schema(value)

        all_values = list(value.values())
        unique_values = set(value.values())
        if len(all_values) != len(unique_values):
            raise cv.Invalid("Mapping values must be unique.")

        return value

    return validator


TYPES = {
    CONF_OUTPUT_SOURCE_PRIORITY: ("POP00", None),
    CONF_CHARGER_SOURCE_PRIORITY: ("PCP00", None),
}


PIPSELECT_SCHEMA = select.SELECT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(PipsolarSelect),
        cv.Optional(CONF_OPTIONSMAP): ensure_option_map(),
        cv.Optional(CONF_STATUSMAP): ensure_option_map(),
    }
).extend(cv.COMPONENT_SCHEMA)


CONFIG_SCHEMA = PIPSOLAR_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): PIPSELECT_SCHEMA for type in TYPES}
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_PIPSOLAR_ID])

    for type, (on, off) in TYPES.items():
        if type in config:
            conf = config[type]
            options_map = conf[CONF_OPTIONSMAP]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await select.register_select(var, conf, options=list(options_map.keys()))
            cg.add(getattr(paren, f"set_{type}_select")(var))
            cg.add(var.set_parent(paren))
            for mappingkey in options_map.keys():
                cg.add(var.add_mapping(mappingkey, options_map[mappingkey]))
            if CONF_STATUSMAP in conf:
                status_map = conf[CONF_STATUSMAP]
                for mappingkey in status_map.keys():
                    cg.add(var.add_status_mapping(mappingkey, status_map[mappingkey]))
