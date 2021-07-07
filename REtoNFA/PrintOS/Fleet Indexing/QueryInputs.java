package com.hp.sms.wsuiotdevice;

public class QueryInputs {
    private String sn;
    private String model;
    private String type;
    public String getModel(){
        return model;
    }
    public String getType(){
        return type;
    }
    public String getSN(){
        return sn;
    }

    public void setModel(String model) {
        model = model;
    }
    public void setType(String type) {
        type = type;
    }

    public void setSN(String SN) {
        this.sn = SN;
    }
}