const CC_INITIALIZING = "initalizing";
const CC_ERROR        = "error";
const CC_RUNNING      = "running";
const CC_AWAITING     = "awaiting";

String cc_state = CC_INITIALIZING;

void setupCruiseController(){

}

void loopCruiseController(){
    update_cc_status();
}

void update_cc_status(){
    if(cc_state == CC_INITIALIZING || cc_state == CC_ERROR){
        if(cc_trigger_pin == down){
            cc_state = CC_AWAITING;
        }
    }else if(cc_state == CC_RUNNING){
        if (obd_current_speed - cc_target_speed > 20){
            cc_state == CC_ERROR;
        }
    }
    log("CC", "cc_state = " + cc_state)
}