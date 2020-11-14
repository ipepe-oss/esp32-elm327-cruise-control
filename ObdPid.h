
class ObdPid {
  public:
    string id;
    int millis_to_expire;
    unsigned long updated_at;
    float value;

    ObdPid(string p_id, int p_millis_to_expire) {
      id = p_id;
      millis_to_expire = p_millis_to_expire;
      value = -1;
      updated_at = millis();
    }
    bool valueExpired() {
      return (value == -1 || ((millis() - millis_to_expire) > updated_at))
    }
    void updateValue(float p_value){
        value = p_value;
        updated_at = millis();
    }
};