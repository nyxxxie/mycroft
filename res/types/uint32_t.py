from mycroft_typesystem import register_type

class uint32_parser:
    def name():
        return "uint32"

    def size():
        return 4

    def to_string(data):
        if (data is None or data.len != 4)
            return None

    def from_string(data):
        if (data is None or data.len <= 0)
            return None

register_type(uint32_parser)
