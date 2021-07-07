package com.hp.sms.wsuiotdevice;

import com.amazonaws.services.iot.client.*;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.util.Iterator;
import java.util.Map;
import java.util.Scanner;

public class MessageOrder {
    private final AWSIotMqttClient client;
    private final String thingName;  // name of the thing (my-thing)
    private final String shadowName; // it's named shadow (test-shadow)
    private static final AWSIotQos TestTopicQos = AWSIotQos.QOS0;
    private final String clientid;
    private int getAccepted=0;
    private int updateAccepted=0;
    private int updateDelta=0;
    private TopicTest topicDisconnected;
    private boolean bypass = false;

    public MessageOrder(AWSIotMqttClient client, String thingName, String shadowName, String clientid) throws AWSIotException {
        this.client = client;
        this.thingName = thingName;
        this.shadowName = shadowName;
        this.clientid = clientid;
        this.topicDisconnected = new TopicTest("$aws/events/presence/disconnected/" + this.clientid, AWSIotQos.QOS0);
        this.client.subscribe(topicDisconnected);
    }

    public void setBypass(boolean value) {
        this.bypass = value;
    }

    // Use getVersion to get the most recent version of the thing's named shadow.
    public int getVersion() throws AWSIotException, InterruptedException, JsonProcessingException {
        String payload = "{ \"Message\": \"Request\" }";
        ObjectMapper mapper = new ObjectMapper();
        JsonNode response;
        String versionString = "";
        String topicStringGet = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get";
        String topicStringGetAccepted = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted";
        TopicTest topicGet = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get", AWSIotQos.QOS0);
        TopicTest topicGetAccepted = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted", AWSIotQos.QOS0);
        TopicTest topicUpdateAccepted = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/update/accepted", AWSIotQos.QOS0);
        this.client.subscribe(topicUpdateAccepted);
        this.client.subscribe(topicGetAccepted, true);
        this.client.publish(topicStringGet, payload);
        this.client.subscribe(topicUpdateAccepted);
        Thread.sleep(1000);
        response = mapper.readTree(topicGetAccepted.getLatestMessage());
        if (response != null) {
            JsonNode versionNode = response.findValue("version");
            if (versionNode != null ){
                versionString = versionNode.toPrettyString();
            }
        }

        return Integer.parseInt(versionString);
    }
    // Gather the ServiceValidator field
    public boolean getServiceValidator(String field, String fieldValue) throws AWSIotException, InterruptedException, JsonProcessingException {
        String payload = "{ \"Message\": \"Request\" }";
        ObjectMapper mapper = new ObjectMapper();
        JsonNode response;
        String versionString = "";
        String topicStringGet = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get";
        String topicStringGetAccepted = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted";
        TopicTest topicGet = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get", AWSIotQos.QOS0);
        TopicTest topicGetAccepted = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted", AWSIotQos.QOS0);
        this.client.subscribe(topicGetAccepted, true);
        this.client.publish(topicStringGet, payload);
        Thread.sleep(3000);
        response = mapper.readTree(topicGetAccepted.getLatestMessage());
        JsonNode secondres = mapper.readTree(response.findValue("desired").toPrettyString());

        if (secondres != null) {
            JsonNode serviceValidatorNode = secondres.findValue(field);
            //System.out.println("CHECKING SERVICE: desired: " + secondres.toPrettyString());
            if( serviceValidatorNode != null ){
                if ( serviceValidatorNode.toPrettyString().equals("\""+fieldValue+"\"") ){
                    System.out.println(field + " is 'true'.");
                    return true;
                }
                else {
                    System.out.println(field + " is 'false'.");
                    return false;
                }
            }


        }
        System.out.println("Error.");
        return false;
    }
    //Gather the ServiceValidator field
    public boolean getDeviceValidator(String field, String fieldValue) throws AWSIotException, InterruptedException, JsonProcessingException {
        String payload = "{ \"Message\": \"Request\" }";
        ObjectMapper mapper = new ObjectMapper();
        String topicStringGet = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get";
        String topicStringGetAccepted = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted";
        TopicTest topicGet = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get", AWSIotQos.QOS0);
        TopicTest topicGetAccepted = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted", AWSIotQos.QOS0);
        this.client.subscribe(topicGetAccepted, true);
        this.client.publish(topicStringGet, payload);
        Thread.sleep(3000);
        JsonNode response;
        response = mapper.readTree(topicGetAccepted.getLatestMessage());
        JsonNode secondres = mapper.readTree(response.findValue("reported").toPrettyString());

        if (secondres != null) {
            JsonNode serviceValidatorNode = secondres.findValue(field);
            System.out.println("CHECKING DEVICE: reported: " + secondres.toPrettyString());
            if( serviceValidatorNode != null ){
                if ( serviceValidatorNode.toPrettyString().equals("\""+fieldValue+"\"") ){
                    System.out.println("field is 'true'.");
                    return true;
                }
                else {
                    System.out.println("field is 'false'.");
                    return false;
                }
            }


        }
        System.out.println("Error.");
        return false;
    }
    // MessageOrder.checkSubscriptions() establishes a client connect
    // to an AWS IoT thing. It subscribes to the lifecycle topic '$aws/events/subscriptions/subscribed/[clientid]
    // to track what topics a client is subscribed to.
    // Returns 'true' if subscribed to /get/accepted, and 'false' if not.
    public boolean checkSubscriptions() throws AWSIotException, JsonProcessingException, InterruptedException {
        String topicStringGetAccepted = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted";
        TopicTest topicUpdateAccepted = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/update/accepted", AWSIotQos.QOS0);
        TopicTest topicUpdateRejected = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/update/rejected", AWSIotQos.QOS0);
        TopicTest topicGetAccepted = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted", AWSIotQos.QOS0);
        TopicTest topicEvent = new TopicTest("$aws/events/subscriptions/subscribed/" + this.clientid, AWSIotQos.QOS0);
        this.client.subscribe(topicEvent, false);

        System.out.println(topicEvent.subscribedTo()); // Check the responses received in the lifecycle topic.
        if (topicEvent.subscribedTo().contains(" " + topicStringGetAccepted + " ")){
            System.out.println("Client is subscribed to reserved MQTT topic /get/accepted.");
            System.out.println("Version: " + this.getVersion());
            return true;
        }
        else {
            System.out.println("Client is not subscribed to reserved MQTT topic /get/accepted, cannot confirm that out of order messages are being handled correctly through versioning.");
            return false;
        }
    }

    public void checkMQTTTopics(boolean isGui) throws AWSIotException, InterruptedException {
        this.bypass = isGui;
        this.checkMQTTTopics();
    }

    public void checkMQTTTopics() throws AWSIotException, InterruptedException {
        String topicStringGetAccepted = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted";
        String topicStringUpdateAccepted = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/update/accepted";
        String topicStringUpdateDelta = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/update/delta";
        String classicShadowGet = "$aws/things/" + this.thingName + "/shadow/get/accepted";
        String classicShadowUpdate = "$aws/things/" + this.thingName + "/shadow/update/accepted";
        TopicTest topicUpdateAccepted = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/update/accepted", AWSIotQos.QOS0);
        Scanner myObj = new Scanner(System.in);  // Create a Scanner object
        TopicTest topicEvent = new TopicTest("$aws/events/subscriptions/subscribed/" + this.clientid, AWSIotQos.QOS0);
        TopicTest topicConnected = new TopicTest("$aws/events/presence/connected/" + this.clientid, AWSIotQos.QOS0);
        this.client.subscribe(topicConnected);
        this.client.subscribe(topicEvent, false);

        System.out.println("Disconnect your application from AWS IoT.");
        if (bypass) {
            System.out.println("Click Enter when disconnected.");
            while (bypass) {
                Thread.sleep(100);
            }
        } else {
            System.out.println("Hit ENTER when disconnected.");
            myObj.nextLine();
        }
        // Subscribe to life events of their service
        System.out.println("Connect your application to AWS IoT.");
        Thread.sleep(5000);

        while(true) {
            if (topicConnected.getLatestMessage() != null) {
                System.out.println("Your application has connected. Checking MQTT subscriptions...");
                //this.client.subscribe(topicEvent, true);
                break;
            } else {
                System.out.println("No connection made, waiting to connect...");
                Thread.sleep(5000);
            }
        }

        int i = 0;
        if ( topicEvent.subscribedTo().isEmpty() ){
            System.out.println("Waiting for MQTT subscriptions...");
            while ( i <= 10 ) {
                Thread.sleep(3000);
                System.out.println("No subscriptions found. Waiting for MQTT subscriptions...");
                if ( !topicEvent.subscribedTo().isEmpty() ){
                    System.out.println("Subscriptions found.");
                    break;
                }
                i++; // Give them 10 tries to make a subscription.
                if ( i == 10 ){ System.out.println("Client is not subscribed to any MQTT topics."); return; }
            }
        }

        // Check MQTT subscriptions.
        Thread.sleep(5000); // Wait for a moment to allow the client to subscribe to everything.
        if (topicEvent.subscribedTo().contains(" " + classicShadowGet + " ") || topicEvent.subscribedTo().contains(" " + classicShadowUpdate + " ")){
            System.out.println("*** Client is subscribed, and may be one-to-one mapped, to the CLASSIC shadow. ***");
        }
        if (topicEvent.subscribedTo().contains(" " + topicStringGetAccepted + " ")){
            System.out.println("Client is subscribed to reserved MQTT topic /get/accepted.");
            this.getAccepted=1;
        }
        else {
            System.out.println("Client is not subscribed to reserved MQTT topic /get/accepted.");
        }
        if (topicEvent.subscribedTo().contains(" " + topicStringUpdateAccepted + " ")){
            System.out.println("Client is subscribed to reserved MQTT topic /update/accepted.");
            this.updateAccepted=1;
        }
        else {
            System.out.println("Client is not subscribed to reserved MQTT topic /update/accepted.");
        }
        if (topicEvent.subscribedTo().contains(" " + topicStringUpdateDelta + " ")){
            System.out.println("Client is subscribed to reserved MQTT topic /update/delta.");
            this.updateDelta=1;
        }
        else {
            System.out.println("Client is not subscribed to reserved MQTT topic /update/delta.");
        }
        if ( updateAccepted == 0 && updateDelta == 0 & getAccepted == 0){
            System.out.println("Client is not subscribed to a /get/accepted, /update/delta, or /update/accepted and is likely not using versioning to control-out-of order messages to a named shadow.");
            this.client.unsubscribe(topicEvent);
            return;
        }
        //this.client.unsubscribe(topicEvent);
        System.out.println(">>>>> MQTT Subscription Check Complete <<<<<");
        System.out.println(">>>>> Please leave your application connected during the rest of the validation. <<<<<");
        return;
}

    // checkHandling checks to see if a client is subscribed to /get/accepted.
    // returns 0 if the client isn't subscribed to /get/accepted
    // returns 1 if the client is subscribed but not using /get/accepted or the 'version' field to handle out of order messages.
    // returns 2 if the client is subscribed to /get/accepted and using the 'version' field to handle out of order messages.
    public int checkHandling() throws InterruptedException, AWSIotException, JsonProcessingException {
        String topicStringGetAccepted = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted";
        String topicStringUpdateAccepted = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/update/accepted";
        String topicStringGet = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get";
        TopicTest topicEvent = new TopicTest("$aws/events/subscriptions/subscribed/" + this.clientid, AWSIotQos.QOS0);
        TopicTest topicConnected = new TopicTest("$aws/events/presence/connected/" + this.clientid, AWSIotQos.QOS0);
        TopicTest topicUpdateAccepted = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/update/accepted", AWSIotQos.QOS0);
        TopicTest topicGetAccepted = new TopicTest("$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/get/accepted", AWSIotQos.QOS0);
        String topicStringUpdate = "$aws/things/" + this.thingName + "/shadow/name/" + this.shadowName + "/update";

        ObjectMapper mapper = new ObjectMapper();
        JsonNode realShadowJSON,realReported = null,realDesired = null;

        this.client.subscribe(topicGetAccepted, true);
        this.client.publish(topicStringGet, "{ \"Message\": \"Request\" }");
        System.out.println("Saving shadow state...");
        Thread.sleep(1000);
        realShadowJSON = mapper.readTree(topicGetAccepted.getLatestMessage());
        if (realShadowJSON != null) {
             realReported = realShadowJSON.findValue("reported");
             realDesired =  realShadowJSON.findValue("desired");
             /*System.out.println("{\n " +
                                    "\"state\": {" +
                                        "\n \"desired\": " +
                                            realDesired.toPrettyString() + ",\n" +
                                       "\"reported\": " +
                                            realReported.toPrettyString() +
                                    "\n}\n}\n");*/
        }


        this.client.subscribe(topicUpdateAccepted, true);
        this.client.subscribe(topicConnected);
        this.client.subscribe(topicEvent, false);

        JsonNode response;
        // Check to see if the client initiated a disconnect.
        if ( this.topicDisconnected.getLatestMessage() != null) {
            response = mapper.readTree(this.topicDisconnected.getLatestMessage());
            JsonNode clientInit = mapper.readTree(response.findValue("clientInitiatedDisconnect").toPrettyString());
            if ( clientInit.toPrettyString().equals("true") ){
                System.out.println(">>>>> Your application has disconnected. Please reconnect. <<<<<");
                while(true) {
                    if (topicConnected.getLatestMessage() != null) {
                        System.out.println("Your application has connected. Checking out-of-order message handling...");
                        //this.client.subscribe(topicEvent, true);
                        break;
                    } else {
                        System.out.println("No connection made, waiting to connect...");
                        Thread.sleep(5000);
                    }
                }
            }
        }
        int realVersion;
        int fakeVersion = this.getVersion() - 5; // Get the current version and decrement it.
        // devices should write to 'reported'
        // services should write to 'desired'
        String fieldName = "";
        // Next, publish a fake /get/accepted document to all subscribed clients. This includes /get/accepted.
        if ( this.getAccepted == 1 ) {
            this.client.subscribe(topicGetAccepted,true);
            this.client.publish(topicStringGet,"{ \"message\": \"Shadow Request \" }" );
            Thread.sleep(500);
            int i = 0;
            JsonNode secondres=null;
            response = mapper.readTree(topicGetAccepted.getLatestMessage());
            JsonNode reported = response.findValue("reported");
            realVersion = this.getVersion();
            if ( reported != null ){ secondres = mapper.readTree(reported.toPrettyString()); }
            if ( secondres != null) {
                Iterator<Map.Entry<String, JsonNode>> fields = secondres.fields();
                while (fields.hasNext() ) { // Grab the fields in /get/accepted reported and attempt to force it to "false"
                    Map.Entry<String, JsonNode> field = fields.next();
                    fieldName = field.getKey();
                    String fakeUpdateAcceptedReportedString = "{ " +
                            "\"state\": {" +
                            "\"reported\": { \"" + fieldName + "\" : -1" +
                            "}" +
                            "}," +
                            "\"version\":" + fakeVersion + "}";
                    System.out.println("Publishing to /get/accepted: " + fakeUpdateAcceptedReportedString);
                    this.client.publish(topicStringGetAccepted, fakeUpdateAcceptedReportedString);
                }
                System.out.println("Waiting for response...");
                Thread.sleep(3000);
                if ( realVersion == this.getVersion() ) {
                    System.out.println("No 'reported' update was pushed to the named shadow, versioning is being handled correctly by client's device.");
                } else if (realVersion != this.getVersion()) {
                   System.out.println("Version control is not being used on the client's device.");
                }
                // Do the same for desired.
                realVersion = this.getVersion();
                fields = secondres.fields();
                while (fields.hasNext() ) { // Grab the fields in /get/accepted reported and attempt to force it to "false"
                    Map.Entry<String, JsonNode> field = fields.next();
                    fieldName = field.getKey();
                    String fakeUpdateAcceptedDesiredString = "{ " +
                            "\"state\": {" +
                            "\"desired\": { \"" + fieldName + "\" : -1" +
                            "}" +
                            "}," +
                            "\"version\":" + fakeVersion + "}";
                    System.out.println("Publishing to /get/accepted: " + fakeUpdateAcceptedDesiredString);
                    this.client.publish(topicStringGetAccepted, fakeUpdateAcceptedDesiredString);
                }
                System.out.println("Waiting for response...");
                Thread.sleep(3000);
                if ( realVersion == this.getVersion() ) {
                    System.out.println("No 'desired' update was pushed to the named shadow, versioning is being handled correctly by client's service.");
                } else if (realVersion != this.getVersion()) {
                    System.out.println("Version control is not being used on the client's service.");
                }
            }
        }
        if ( this.updateAccepted == 1){  /////////////////////////// Publish to /update/accepted
            this.client.subscribe(topicUpdateAccepted, true);
            if ( topicUpdateAccepted.getLatestMessage() == null ){
                System.out.println("Please publish an update through your application.");
                while ( true ){
                    Thread.sleep(3000);
                    System.out.println("Waiting for application to publish an update...");
                    if ( topicUpdateAccepted.getLatestMessage() != null ){
                        System.out.println("Update received on /update/accepted. Checking version control...");
                        break;
                    }
                }
            }
            int i = 0;
            /// Reported first
            JsonNode secondres=null;
            response = mapper.readTree(topicUpdateAccepted.getLatestMessage());
            JsonNode reported = response.findValue("reported");
            realVersion = this.getVersion();
            if ( reported != null ){ secondres = mapper.readTree(reported.toPrettyString()); }
            if ( secondres != null) {
                Iterator<Map.Entry<String, JsonNode>> fields = secondres.fields();
                while (fields.hasNext() && i == 0 ) { // Grab the first field in /update/accepted reported and attempt to force it to "false"
                    Map.Entry<String, JsonNode> field = fields.next();
                    fieldName = field.getKey();
                    String fakeUpdateAcceptedReportedString = "{ " +
                            "\"state\": {" +
                            "\"reported\": { \"" + fieldName + "\" : -1" +
                            "}" +
                            "}," +
                            "\"version\":" + fakeVersion + "}";
                    System.out.println("Publishing to /update/accepted: " + fakeUpdateAcceptedReportedString);
                    this.client.publish(topicStringUpdateAccepted, fakeUpdateAcceptedReportedString);
                    System.out.println("Waiting for response...");
                    Thread.sleep(3000);
                    if ( realVersion == this.getVersion() ) {
                        System.out.println("No 'reported' update was pushed to the named shadow, versioning is being handled correctly by client's device.");
                    } else if (realVersion != this.getVersion()) {
                        System.out.println("Version control is not being used on the client's device.");
                    }
                    // Do the same for desired.
                    realVersion = this.getVersion();
                    String fakeUpdateAcceptedDesiredString = "{ " +
                            "\"state\": {" +
                            "\"desired\": { \"" + fieldName + "\" : -1" +
                            "}" +
                            "}," +
                            "\"version\":" + fakeVersion + "}";
                    System.out.println("Publishing to /update/accepted: " + fakeUpdateAcceptedDesiredString);
                    this.client.publish(topicStringUpdateAccepted, fakeUpdateAcceptedDesiredString);
                    System.out.println("Waiting for response...");
                    Thread.sleep(3000);
                    if ( realVersion == this.getVersion() ) {
                        System.out.println("No 'desired' update was pushed to the named shadow, versioning is being handled correctly by client's service.");
                    } else if (realVersion != this.getVersion()) {
                        System.out.println("Version control is not being used on the client's service.");
                    }
                    i++;
                }
                }
            }

        System.out.println("Restoring original shadow state.");
        client.publish(topicStringUpdate,"{\n " +
                "\"state\": {" +
                "\n \"desired\": " +
                realDesired.toPrettyString() + ",\n" +
                "\"reported\": " +
                realReported.toPrettyString() +
                "\n}\n}\n");
        System.out.println(">>>>> Out-Of-Order Message Handling Check Complete. <<<<<");


        return 1;
    }

    public void findJSONValue(String jsonString, String key) throws JsonProcessingException {
        ObjectMapper mapper = new ObjectMapper();
        JsonNode object = mapper.readTree(jsonString);

        if (object != null) {
            for (JsonNode node : object.findValues(key)) {
                System.out.println("JSON VALUE: " + node.toPrettyString());
            }
        }
    }

    public void interrupt() throws AWSIotException {
        if (client != null) {
            client.disconnect();
        }
    }
}
