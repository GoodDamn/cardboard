VERS_1.0 {
  global:
    # Export Cardboard Symbols.
    Cardboard*;
    *nativeIncrementDeviceParamsChangedCount;
    *JNIEnv*;
    *JavaVM*;

  # Hide everything else.
  local:
    *;
};
