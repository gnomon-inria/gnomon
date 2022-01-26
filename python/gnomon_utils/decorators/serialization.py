import pickle
from base64 import b64decode, b64encode


def serialize(attr):
    def decorator(cls: type):
        def serialize_func(self: object) -> str:
            return b64encode(pickle.dumps(getattr(self, attr))).decode("ascii")

        def deserialize_func(self: object, serialization: str):
            setattr(self, attr, pickle.loads(b64decode(serialization.encode("ascii"))))

        setattr(cls, "serialize", serialize_func)
        setattr(cls, "deserialize", deserialize_func)
        return cls
    return decorator
