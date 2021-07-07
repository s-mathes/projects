package com.hp.sms.wsuiotdevice;

import com.amazonaws.services.iot.client.AWSIotDevice;
import com.amazonaws.services.iot.client.AWSIotException;
import com.amazonaws.services.iot.client.AWSIotMqttClient;
import com.amazonaws.services.iot.client.AWSIotQos;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.hp.sms.wsuiotdevice.TopicTest;
import com.hp.sms.wsuiotdevice.sampleUtil.SampleUtil;

import java.util.Scanner;

public class Application {
    public static void main(String[] arguments) throws AWSIotException, InterruptedException, JsonProcessingException {
        String clientId =""; // = "my-thing"; // Name of the client attempting to connect
        String clientEndpoint= ""; // = "a2yvcbrfibr991-ats.iot.us-west-2.amazonaws.com"; // c&p endpoint from console IoT settings
        String certificateFile=""; // = "C:\\Users\\srobi\\Documents\\Spring 2021\\CS 401 - Capstone\\my-thing\\my-thing.cert.pem"; // X.509 based certificate file
        String privateKeyFile=""; // = "C:\\Users\\srobi\\Documents\\Spring 2021\\CS 401 - Capstone\\my-thing\\my-thing.private.key"; // PKCS#1 or PKCS#8 PEM encoded private key file
        String thingName=""; // = "my-thing"; // The name of the Thing (often the same as clientId).
        String shadowName=""; // = "test-shadow"; // The name of the Shadow.

        if (arguments.length == 6 || arguments.length == 0) {
            // User-provided overrides
            if (arguments.length == 4) {
                clientId = arguments[0];
                clientEndpoint = arguments[1];
                certificateFile = arguments[2];
                privateKeyFile = arguments[3];
                thingName = arguments[4];
                shadowName = arguments[5];
            }
            ObjectMapper mapper = new ObjectMapper();
            SampleUtil.KeyStorePasswordPair pair = SampleUtil.getKeyStorePasswordPair(certificateFile, privateKeyFile);
            AWSIotMqttClient client = new AWSIotMqttClient(clientEndpoint, clientId, pair.keyStore, pair.keyPassword);
            TopicTest topicGetAccepted = new TopicTest("$aws/things/" + thingName + "/shadow/name/" + shadowName + "/get/accepted", AWSIotQos.QOS0);
            AWSIotDevice awsIotDevice = new AWSIotDevice(thingName);
            String payload = "{ \"Message\": \"Request\" }";

            boolean handleOoOMessages = false;
            boolean subscribed = false;
            boolean connected = false;
            while (true) {
                Scanner myObj = new Scanner(System.in);  // Create a Scanner object
                System.out.println("SET UP - Enter command: ");
                System.out.println("0 - Connect to AWS IoT and subscribe to /get/accepted.");
                System.out.println("1 - Connect to AWS IoT and don't subscribe to /get/accepted.");
                System.out.println("2 - Handle out-of-order messages correctly.");
                System.out.println("3 - Handle out-of-order messages incorrectly. (Default)");
                System.out.println("4 - Show most recent document. ");
                System.out.println("5 - Disconnect from AWS IoT.");
                String input = myObj.nextLine();  // Read user input
                if (input.equals("0")) {
                    try {
                        client.connect();
                    } catch (AWSIotException e) {
                        e.printStackTrace();
                    }
                    System.out.println("Connection made.");
                    client.subscribe(topicGetAccepted, false);
                    System.out.println(">>>>>Service is subscribed to reserved MQTT topic /get/accepted.<<<<<");
                    //client.attach(awsIotDevice);
                    subscribed = true;
                    connected = true;
                    Thread.sleep(1000);
                } // 0
                else if (input.equals("1")) {
                    try {
                        client.connect();
                    } catch (AWSIotException e) {
                        e.printStackTrace();
                    }
                    System.out.println("Connection made.");
                    client.attach(awsIotDevice);
                    System.out.println(">>>>>Service is not subscribed to MQTT topic /get/accepted.<<<<<");
                    subscribed = false;
                    connected = true;
                    Thread.sleep(1000);
                } // 1
                else if (input.equals("2")) {
                    handleOoOMessages = true;
                    System.out.println(">>>>>Service is now handling out-of-order messages correctly.<<<<<");
                }  // 2
                else if (input.equals("3")) {
                    handleOoOMessages = false;
                    System.out.println(">>>>>Service is now handling out-of-order messages incorrectly.<<<<<");
                } // 3
                else if (input.equals("4")) {
                    TopicTest getAccepted = new TopicTest("$aws/things/" + thingName + "/shadow/name/" + shadowName + "/get/accepted", AWSIotQos.QOS0);
                    if (subscribed == false) {
                        client.subscribe(getAccepted);
                    }
                    client.publish("$aws/things/" + thingName + "/shadow/name/" + shadowName + "/get", "{ \"Message\": \"Request\" }");
                    System.out.println("Latest Shadow Document:");
                    Thread.sleep(1000);
                    System.out.println(getAccepted.getLatestMessage());
                    if (subscribed == false) {
                        client.unsubscribe(getAccepted);
                    }
                } // 4
                else if (input.equals("5")) {
                    client.disconnect();
                    client.detach(awsIotDevice);
                    connected = false;
                    subscribed = false;
                } // 5

                String versionString = "";
                if ( connected == true) {
                    //System.out.println(">>>>>Listening on /get/accepted...<<<<<");
                    while(true) {
                        Thread.sleep(500);
                        if ( topicGetAccepted.getLatestMessage() != null ) {
                            System.out.println("RECEIVED: " + topicGetAccepted.getLatestMessage());
                            JsonNode response = mapper.readTree(topicGetAccepted.getLatestMessage());
                            if (response != null) {
                                JsonNode versionNode = response.findValue("version");
                                if (versionNode != null ) {
                                    versionString = versionNode.toPrettyString();
                                    System.out.println("Version received on /get/accepted: " + versionString);
                                    String topicStringGet = "$aws/things/" + thingName + "/shadow/name/" + shadowName + "/get";
                                    client.publish(topicStringGet, payload);
                                    Thread.sleep(200);
                                    JsonNode shadowjson = mapper.readTree(topicGetAccepted.getLatestMessage()); // CHECK W/ SHADOW
                                    JsonNode shadowVersion = shadowjson.findValue("version");
                                    System.out.println("Version on shadow: " + shadowVersion.toPrettyString());
                                    if ( handleOoOMessages == true ) { // CHECK VERSIONING
                                        if (Integer.parseInt(versionString) <= Integer.parseInt(shadowVersion.toPrettyString())) { // it was out of order
                                            System.out.println("Message was out of order, do not update.");
                                            //System.out.println(response.findValue("state").toPrettyString());
                                        } else {
                                            System.out.println("Updating the shadow.");
                                            JsonNode update = response.findValue("state");
                                            System.out.println(update.toPrettyString());
                                        }
                                    } else {
                                        String topicStringUpdate = "$aws/things/" + thingName + "/shadow/name/" + shadowName + "/update";

                                        if ( response.findValue("desired") != null ){
                                            System.out.println("UPDATING: {\n" + "\"state\": { \"desired\": " + response.findValue("desired").toPrettyString() + "\n}\n}");
                                            client.publish(topicStringUpdate,"{\n" + "\"state\": { \"desired\": " + response.findValue("desired").toPrettyString() + "\n}\n}");
                                        }
                                        if ( response.findValue("reported") != null ){
                                            System.out.println("UPDATING: {\n" + "\"state\": { \"reported\": " + response.findValue("reported").toPrettyString() + "\n}\n}");
                                            client.publish(topicStringUpdate,"{\n" + "\"state\": { \"reported\": " + response.findValue("reported").toPrettyString() + "\n}\n}");
                                        }

                                    }
                                }
                            }
                            topicGetAccepted.clearLatestMessage();
                            //System.out.println("Listening on /get/accepted...");
                        }
                    }
                }
            } // end program while

                /*
                ObjectMapper mapper = new ObjectMapper();
                JsonNode response, shadow;
                String versionString, shadowVersion;
                response = null;
                if (subscribed == true) { // The service is subscribed to /get/accepted
                    client.attach(awsIotDevice);
                    while (true) { // wait for response on /get/accepted
                        if ( mapper.readTree(topicGetAccepted.getLatestMessage()) != null )
                            response = mapper.readTree(topicGetAccepted.getLatestMessage()); // listen to update on /get/accepted
                            if (handleOoOMessages == true) { // check version number and update values
                                if (response != null) {
                                    JsonNode versionNode = response.findValue("version");
                                    if (versionNode != null) {
                                        versionString = versionNode.toPrettyString();
                                        System.out.println("VERSION: " + versionString);
                                    }
                                }
                                shadow = mapper.readTree(awsIotDevice.get()); // check shadow
                                if (shadow != null) {
                                    JsonNode shadowVersionNode = shadow.findValue("version");
                                    System.out.println("SHADOW VERSION: " + shadowVersionNode.toPrettyString());
                                }
                                break;
                            }
                            else if (handleOoOMessages == false) { // update values
                                continue;
                            }
                        }
                    }
                } // End of if subscribed == true */

        } else {
                System.out.println("Usage: clientId clientEndpoint certificateFile privateKeyFile");
                System.out.println("Example: " + clientId + " " + clientEndpoint + " " + certificateFile + " " + privateKeyFile);
                System.out.println("The certificate file and private key file must be complete paths or in current working directory.");
        }
    }

}