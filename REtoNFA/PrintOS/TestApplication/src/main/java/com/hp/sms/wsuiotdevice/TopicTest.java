package com.hp.sms.wsuiotdevice;

import com.amazonaws.services.iot.client.AWSIotMessage;
import com.amazonaws.services.iot.client.AWSIotMqttClient;
import com.amazonaws.services.iot.client.AWSIotQos;
import com.amazonaws.services.iot.client.AWSIotTopic;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.util.ArrayList;
import java.util.List;

public class TopicTest extends AWSIotTopic {
    private boolean messageReceived;
    private String latestMessage;
    private List subscribedTopics = new ArrayList();

    /**
     * Extends the AWSIotTopic class for added benefits.
     * @param topic
     *      The topic we want to subscribe to
     *      (i.e., $aws/things/{thingName}/shadow/name/{shadowName}/get/accepted).
     * @param qot
     *      The MQTT Quality of Service.
     *      AWSIotQos.QOS0: This client will not acknowledge to the Device Gateway that messages are received.
     *      AWSIotQos.QOS1: This client will acknowledge to the Device Gateway that messages are received.
     */
    public TopicTest(String topic, AWSIotQos qot) {
        super(topic, qot);
        this.messageReceived = false;
    }

    /**
     * This is false by default and only is true if a message was received.
     * @return
     *      Returns a value indicating whether or not a message was received.
     */
    public boolean isMessageReceived() { return this.messageReceived; }

    /**
     * Sets the messageReceived value. Useful for clearing the message flag after processing the latest message.
     * @param value
     *      A boolean value to set the messageReceived value.
     */
    public void setMessageReceived(boolean value) { this.messageReceived = value; }

    /**
     * This gets the latest message received by the system.
     * WARNING: this can be null, so check isMessageReceived() first.
     * @return
     *      Returns a string containing the latest message received. More likely than not, it's JSON-formatted. Or null.
     */
    public String getLatestMessage() { return this.latestMessage; }

    public List subscribedTo() {
        return subscribedTopics;
    }

    public void clearLatestMessage(){ this.latestMessage = null; }

    @Override
    public void onMessage(AWSIotMessage message) {
        if (message == null) return;
        this.messageReceived = true;
        this.latestMessage = message.getStringPayload();
        String validatorMessage = "";

        ObjectMapper mapper = new ObjectMapper();
        JsonNode object = null;
        try {
            object = mapper.readTree(this.latestMessage);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
        if (object != null) {
            JsonNode clientIDnode = object.findValue("clientId");
            if( clientIDnode != null){
                JsonNode timestampNode = object.findValue(("timestamp"));
                if( timestampNode != null ){
                    JsonNode eventTypeNode = object.findValue(("eventType"));
                    if( eventTypeNode.toPrettyString().equals("\"subscribed\"") ){
                        JsonNode topicNode = object.findValue("topics");
                        if (topicNode != null ){
                            String topic = topicNode.toPrettyString().replace("[","");
                            topic = topic.replace("]","");
                            topic = topic.replace("\"","");

                            System.out.println(this.latestMessage);
                            //System.out.println(clientIDnode + " " + eventTypeNode + " to: " + topic);
                            subscribedTopics.add(topic);
                        }
                    } else if ( eventTypeNode.toPrettyString().equals("\"unsubscribed\"")) {
                        JsonNode topicNode = object.findValue("topics");
                        subscribedTopics.remove(topicNode);
                    }
                }
            }
            else {
                //System.out.println("---- MESSAGE RECEIVED ----");
                //System.out.println(this.latestMessage);
            }
        }

       // System.out.println("---- MESSAGE RECEIVED ----");
        this.latestMessage = message.getStringPayload();
    }

    @Override
    public void onSuccess() {

    }

    @Override
    public void onFailure() {

    }

    @Override
    public void onTimeout() {

    }
}
