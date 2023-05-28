
enum State
{
  MODE_ANALYSIS,
  MODE_CONFIG,
  MODE_CONNECT,
  MODE_RUN,
  MODE_OTA,
  MODE_WAIT_UPDATE,
  MODE_WAIT_CONFIG,
  MODE_RESET,

  MODE_MAX_VALUE
};

#if defined(KUR_DEBUG)
const char *StateStr[MODE_MAX_VALUE + 1] = {
    "ANALYSIS",
    "CONFIG",
    "CONNECTING",
    "RUNNING",
    "OTA",
    "WAIT_UPDATE",
    "WAIT_CONFIG",
    "RESET",

    "INIT"};
#endif

namespace KURState
{
  volatile State state = MODE_MAX_VALUE;
  State get() { return state; }
  bool is(State m) { return (state == m); }
  void set(State m);
};