Import("env")


def skip_unused_tflite_drivers(node):
    source = node.srcnode().get_abspath()
    if "/TensorFlowLite_ESP32/src/bus/" in source or "/TensorFlowLite_ESP32/src/screen/" in source:
        return None
    return node


env.AddBuildMiddleware(skip_unused_tflite_drivers)

# After Arduino configures the linker, keep the TensorFlow Lite version used by
# anomaly_detector.cpp and remove the conflicting framework archive.
if "espressif__esp-tflite-micro" in env.get("LIBS", ()):
    env.Replace(LIBS=[lib for lib in env["LIBS"] if lib != "espressif__esp-tflite-micro"])
