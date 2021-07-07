package com.hp.sms.wsuiotdevice;

import com.amazonaws.services.iot.client.*;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.hp.sms.wsuiotdevice.sampleUtil.SampleUtil;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class ShadowValidator {
    private final String clientId;
    private final String clientEndpoint;
    private final String certificateFile;
    private final String privateKeyFile;
    private String lastReceivedMessage = "";
    private boolean bypass = false;
    private AWSIotMqttClient client;

    /**
     * Creates an instance of the ShadowValidator class.
     * @param clientId
     *      The client ID / Thing name
     *      (i.e., laser-printer).
     * @param clientEndpoint
     *      C&P endpoint from console IoT settings
     *      (i.e., {some-value}.iot.{region}.amazonaws.com).
     * @param certificateFile
     *      X.509 based certificate file
     *      (i.e., {certificate-id}-certificate.pem.crt).
     * @param privateKeyFile
     *      PKCS#1 or PKCS#8 PEM encoded private key file
     *      (i.e., {clientId}.private.key).
     */
    public ShadowValidator(String clientId, String clientEndpoint, String certificateFile, String privateKeyFile) {
        this.clientId = clientId;
        this.clientEndpoint = clientEndpoint;
        this.certificateFile = certificateFile;
        this.privateKeyFile = privateKeyFile;
    }

    public void setBypass(boolean bypass) {
        this.bypass = bypass;
    }

    public String getLastReceivedMessage() {
        return this.lastReceivedMessage;
    }

    /**
     * This currently subscribes to a topic named "$aws/things/{thingName}/shadow/name/{shadowName}/get/accepted".
     * It busy-loops after subscribing, and once it receives a message, it prints out the entire message, then parses
     * the JSON string to print out whatever is under the "state" -> "reported" node.
     * @param thingName
     *      The name of the Thing we're subscribing to.
     * @param shadowName
     *      The shadow name that we're subscribing to. Use an empty string for a classic shadow.
     * @param templatePath
     *      The path to the template file that we'll be comparing against.
     * @param isGui
     *      Set this to true to indicate that this is being ran by the GUI instead of the console.
     * @throws Exception
     *      Throws Exception.
     */
    public boolean shadowTest(String thingName, String shadowName, String templatePath, boolean isGui) throws Exception {
        bypass = isGui;
        return shadowTest(thingName, shadowName, templatePath);
    }

    /**
     * This currently subscribes to a topic named "$aws/things/{thingName}/shadow/name/{shadowName}/get/accepted".
     * It busy-loops after subscribing, and once it receives a message, it prints out the entire message, then parses
     * the JSON string to print out whatever is under the "state" -> "reported" node.
     * @param thingName
     *      The name of the Thing we're subscribing to.
     * @param shadowName
     *      The shadow name that we're subscribing to. Use an empty string for a classic shadow.
     * @param templatePath
     *      The path to the template file that we'll be comparing against.
     */
    public boolean shadowTest(String thingName, String shadowName, String templatePath) throws Exception {
        System.out.println("Subscribing to topic.");
        SampleUtil.KeyStorePasswordPair pair = SampleUtil.getKeyStorePasswordPair(this.certificateFile, this.privateKeyFile);
        client = new AWSIotMqttClient(this.clientEndpoint, this.clientId, pair.keyStore, pair.keyPassword);

        AWSIotDevice device = new AWSIotDevice(thingName);
        try {
            client.attach(device);
            client.connect();
            TopicTest topicGetAccepted;
            if (shadowName == null || shadowName.isEmpty()) {
                topicGetAccepted = new TopicTest(
                        "$aws/things/" + thingName + "/shadow/get/accepted",
                        AWSIotQos.QOS0);
            } else {
                topicGetAccepted = new TopicTest(
                        "$aws/things/" + thingName + "/shadow/name/" + shadowName + "/get/accepted",
                        AWSIotQos.QOS0);
            }

            System.out.println("Subscribing to " + topicGetAccepted.getTopic());
            client.subscribe(topicGetAccepted);
            Thread.sleep(100);
            if (bypass) {
                System.out.println("Click Enter to publish an empty message.");
                while (bypass) {
                    Thread.sleep(100);
                }
            } else {
                System.out.println("Press Enter to publish an empty message.");
                System.in.read();
            }

            client.publish(topicGetAccepted.getTopic().replace("/accepted", ""), "");
            System.out.println("Waiting for response from topic...");
            while (!topicGetAccepted.isMessageReceived()) Thread.sleep(100);

            System.out.println("Received message:");
            this.lastReceivedMessage = topicGetAccepted.getLatestMessage();
            try {
                ObjectMapper mapper = new ObjectMapper();
                JsonNode node = mapper.readTree(topicGetAccepted.getLatestMessage());
                System.out.println(node.toPrettyString());
            } catch (Exception e) {
                System.out.println(topicGetAccepted.getLatestMessage());
            }

            String templateString = new String(Files.readAllBytes(Paths.get(templatePath)));
            boolean test = this.compareJson(topicGetAccepted.getLatestMessage(), templateString);
            topicGetAccepted.setMessageReceived(false);
            client.disconnect();
            if (test) {
                System.out.println("Successfully validated.");
                return true;
            } else {
                System.out.println("Validation failed.");
                return false;
            }
        } catch (IOException | AWSIotException | InterruptedException e) {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * Compare a JSON string with a template. This will initially check each base node of the template JSON,
     * find any matching fields in the comparison JSON, and then recursively checks each node to find out if
     * the exact template exists within the JSON string. This will check the path in order to get to the JSON node,
     * so if your template has a path like "state -> desired -> configuration -> version", then that exact
     * path must exist in the JSON string. However, if your template simply has "version", it will only care
     * if that field exists anywhere within the JSON string.
     *
     * This function does not care about the value for each JSON node, only the keys.
     * @param jsonString
     * The JSON string that we are validating.
     * @param template
     * The template that we are comparing against the JSON string.
     * @return
     * Returns true if the JSON string contains matching key paths from the template, otherwise returns false.
     */
    public boolean compareJson(String jsonString, String template) throws Exception {
        if (jsonString == null) {
            throw new Exception("JSON string is null.");
        } else if (template == null) {
            throw new Exception("Template JSON string is null.");
        } else if (jsonString.trim().isEmpty()) {
            throw new Exception("JSON string is empty or whitespace.");
        } else if (template.trim().isEmpty()) {
            throw new Exception("Template string is empty or whitespace.");
        }

        try {
            ObjectMapper templateMapper = new ObjectMapper();
            ObjectMapper comparisonMapper = new ObjectMapper();
            JsonNode templateObject = templateMapper.readTree(template);
            JsonNode comparisonObject = comparisonMapper.readTree(jsonString);
            if (templateObject != null && comparisonObject != null) {
                for (Iterator<Map.Entry<String, JsonNode>> iterator = templateObject.fields(); iterator.hasNext();) {
                    boolean subValidated = false;
                    Map.Entry<String, JsonNode> entry = iterator.next();
                    List<JsonNode> comparisons = comparisonObject.findValues(entry.getKey());
                    for (JsonNode comparison : comparisons) {
                        if (recurseNode(comparison, entry.getValue())) {
                            subValidated = true;
                        }
                    }

                    if (!subValidated) {
                        return false;
                    }
                }

                // Made it through the for-loop without failing a sub-validation.
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public void interrupt() throws AWSIotException {
        if (client != null) {
            client.disconnect();
        }
    }

    private boolean recurseNode(JsonNode jsonNode, JsonNode templateNode) {
        if (jsonNode == null && templateNode == null) {
            // We reached the end of the recursion successfully and both nodes are null.
            return true;
        } else if (jsonNode == null || templateNode == null) {
            // We reached the end of the recursion for one of the trees but not the other, so they don't match.
            return false;
        } else {
            boolean value = true;
            for (Iterator<Map.Entry<String, JsonNode>> iterator = templateNode.fields(); iterator.hasNext();) {
                Map.Entry<String, JsonNode> entry = iterator.next();
                if (!recurseNode(jsonNode.get(entry.getKey()), entry.getValue())) {
                    value = false;
                }
            }

            return value;
        }
    }
}
