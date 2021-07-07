package com.hp.sms.wsuiotdevice;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import com.amazonaws.services.iot.client.AWSIotException;
import com.amazonaws.services.iot.client.AWSIotMqttClient;
import com.amazonaws.services.iot.client.sample.sampleUtil.SampleUtil;
import com.fasterxml.jackson.core.JsonProcessingException;

import java.io.IOException;
import java.util.Scanner;

public class Application {
    public static void main(String[] arguments) throws AWSIotException, IOException {
        // "my-thing";  Name of the client attempting to connect
        String clientId = "sdk-java";
        // c&p endpoint from console IoT settings
        String clientEndpoint = "a2yvcbrfibr991-ats.iot.us-west-2.amazonaws.com";
        // X.509 based certificate file
        String certificateFile = "C:\\Users\\srobi\\Documents\\Spring 2021\\CS 401 - Capstone\\my-thing\\my-thing.cert.pem";
        // PKCS#1 or PKCS#8 PEM encoded private key file
        String privateKeyFile = "C:\\Users\\srobi\\Documents\\Spring 2021\\CS 401 - Capstone\\my-thing\\my-thing.private.key";
        // The name of the Thing (often the same as clientId).
        String thingName = "my-thing";
        // The name of the Shadow.
        String shadowName = "test-shadow";

        if (arguments.length == 6) {
            // User-provided overrides
            clientId = arguments[0];
            clientEndpoint = arguments[1];
            certificateFile = arguments[2];
            privateKeyFile = arguments[3];
            thingName = arguments[4];
            shadowName = arguments[5];
        } else {
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

            System.out.println("Enter your client ID: ");
            clientId = reader.readLine();

            System.out.println("Enter your client endpoint: ");
            clientEndpoint = reader.readLine();

            System.out.println("Enter the file path to your certificate file (xxx.pem.crt):");
            certificateFile = reader.readLine();

            System.out.println("Enter the file path to your private key file (xxx.private.key):");
            privateKeyFile = reader.readLine();

            System.out.println("Enter your thing name:");
            thingName = reader.readLine();

            System.out.println("Enter your shadow name:");
            shadowName = reader.readLine();
        }
        // Sarah - Connection for the MessageOrder is made outside in Application, and the client is passed to the function.
        SampleUtil.KeyStorePasswordPair pair = SampleUtil.getKeyStorePasswordPair(certificateFile,privateKeyFile);
        AWSIotMqttClient client = new AWSIotMqttClient(clientEndpoint, "wsu-validator", pair.keyStore, pair.keyPassword);

        System.out.println("");
        System.out.println("Client ID: " + clientId +
                "\nClient Endpoint: " + clientEndpoint +
                "\nCertificate File: " + certificateFile +
                "\nPrivate Key File: " + privateKeyFile +
                "\nThing Name: " + thingName +
                "\nShadow Name: " + shadowName);


            Scanner myObj = new Scanner(System.in);  // Create a Scanner object
            System.out.println(">>> VALIDATOR MENU <<< ");
            System.out.println("0 - Connect to AWS IoT and validate named shadow JSON document.");
            System.out.println("1 - Connect to application and check MQTT subscriptions and out-of-order message handling.");
            String input = myObj.nextLine();  // Read user input

            if ( input.equals("0") ) {
                System.out.println("Enter the template path:");
                input = myObj.nextLine();
                System.out.println("\n>>>>> VALIDATING THE NAMED SHADOW<<<<<");
                ShadowValidator shadowValidator = new ShadowValidator("wsu-validator", clientEndpoint, certificateFile, privateKeyFile);
                try {
                    shadowValidator.shadowTest(thingName, shadowName, input);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            } else if ( input.equals("1") ) {
                try {
                    client.connect();
                } catch (AWSIotException e) {
                    e.printStackTrace();
                }
                System.out.println(">>>>> CHECKING MQTT SUBSCRIPTIONS <<<<<");
                MessageOrder checkMessageOrder = new MessageOrder(client, thingName, shadowName, clientId);
                try {
                    checkMessageOrder.checkMQTTTopics(); // Checking for /get/accepted, /update/accepted, and /update/delta
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                System.out.println("\n>>>>> CHECKING OUT-OF-ORDER MESSAGES <<<<<");
                try {
                    checkMessageOrder.checkHandling();
                } catch (JsonProcessingException | InterruptedException e) {
                    e.printStackTrace();
                }
                client.disconnect();
            } else {
                System.out.println("Invalid option.");
            }
    }
}