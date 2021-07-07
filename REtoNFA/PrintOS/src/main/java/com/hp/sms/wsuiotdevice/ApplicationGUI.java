package com.hp.sms.wsuiotdevice;

import com.amazonaws.services.iot.client.AWSIotException;
import com.amazonaws.services.iot.client.AWSIotMqttClient;
import com.amazonaws.services.iot.client.sample.sampleUtil.SampleUtil;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
//import com.hp.sms.wsuiotdevice.sampleUtil.SampleUtil;

import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.List;

/**
 * Main class for the GUI version of the HP AWS IoT Validation Suite.
 */
public class ApplicationGUI extends JFrame {
    private JPanel panel1;
    private JTextField textFieldClientId;
    private JTextField textFieldClientEndpoint;
    private JTextField textFieldCertificateFile;
    private JTextField textFieldPrivateKeyFile;
    private JTextField textFieldThingName;
    private JButton certificateFileBrowseButton;
    private JButton privateFileBrowseButton;
    private JTextField textFieldShadowName;
    private JButton oooButton;
    private JButton clearAllButton;
    private JTextPane textPane;
    private JTextField textFieldTemplateJSON;
    private JButton templateJSONBrowseButton;
    private JButton enterButton;
    private JButton validateTemplateButton;
    private JTextPane textPaneTemplate;
    private JButton stopButton;
    private ShadowValidator shadowValidator;
    private MessageOrder checkMessageOrder;
    private String templateString;
    private ArrayList<String> templateStrings;
    private ArrayList<String> templateColorStrings;
    private Thread templateValidationThread;
    private Thread oooThread;
    private Map.Entry<String, JsonNode> lastEntry;

    public static void main(String[] args) {
        ApplicationGUI dialog = new ApplicationGUI();
        dialog.redirectSystemStreams();
        dialog.pack();
        dialog.loadPreviousSettings();
        Dimension dimension = new Dimension();
        dimension.height = 800;
        dimension.width = 1150;
        dialog.setMinimumSize(dimension);
        dialog.setContentPane(dialog.panel1);
        dialog.setTitle("HP AWS IoT Validation Suite");
        dialog.getRootPane().setDefaultButton(dialog.enterButton);
        dialog.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        dialog.setVisible(true);
    }

    public ApplicationGUI() {
        // Certificate file browse button actions.
        certificateFileBrowseButton.addActionListener(e -> {
            String filePath = getFilePath();
            textFieldCertificateFile.setText(filePath);
        });

        // private key file browse button actions.
        privateFileBrowseButton.addActionListener(e -> {
            String filePath = getFilePath();
            textFieldPrivateKeyFile.setText(filePath);
        });

        // Out of Order Message button actions.
        oooButton.addActionListener(e -> {
            oooThread = new Thread(() -> {
                try {
                    disableButtons();
                    outOfOrderMessages();
                } catch (AWSIotException | InterruptedException awsIotException) {
                    awsIotException.printStackTrace();
                }
            });
            oooThread.start();
        });

        // Clear All button actions.
        clearAllButton.addActionListener(e -> {
            textFieldClientId.setText("");
            textFieldClientEndpoint.setText("");
            textFieldCertificateFile.setText("");
            textFieldPrivateKeyFile.setText("");
            textFieldThingName.setText("");
            textFieldShadowName.setText("");
            textFieldTemplateJSON.setText("");
        });

        // JSON Template browse button actions.
        templateJSONBrowseButton.addActionListener(e -> {
            String filePath = getFilePath();
            textFieldTemplateJSON.setText(filePath);
            setTemplateArrayFromFilePath(filePath);
        });

        // Enter button actions.
        enterButton.addActionListener(e -> {
            if (shadowValidator != null) {
                shadowValidator.setBypass(false);
            }

            if (checkMessageOrder != null) {
                checkMessageOrder.setBypass(false);
            }
        });

        // Validate Template button actions.
        validateTemplateButton.addActionListener(e -> {
            disableButtons();
            templateValidationThread = new Thread(this::validateTemplate);
            templateValidationThread.start();
        });

        // Stop button actions.
        stopButton.addActionListener(e -> {
            System.out.println("Attempting to stop process...");
            if (templateValidationThread != null && templateValidationThread.isAlive()) {
                try {
                    shadowValidator.interrupt();
                } catch (AWSIotException awsIotException) {
                    awsIotException.printStackTrace();
                }

                templateValidationThread.interrupt();
            }

            if (oooThread != null && oooThread.isAlive()) {
                try {
                    checkMessageOrder.interrupt();
                } catch (AWSIotException awsIotException) {
                    awsIotException.printStackTrace();
                }

                oooThread.interrupt();
            }

            enableButtons();
            System.out.println("Process successfully stopped.");
        });

        // Action for when the JSON Template text field is changed.
        textFieldTemplateJSON.addActionListener(e -> setTemplateArrayFromFilePath(textFieldTemplateJSON.getText()));
    }

    /**
     * Enables the Validate Template button and Out of Order Messaging button.
     */
    private void enableButtons() {
        validateTemplateButton.setEnabled(true);
        oooButton.setEnabled(true);
    }

    /**
     * Disables the Validate Template button and the Out of Order Messaging button.
     */
    private void disableButtons() {
        validateTemplateButton.setEnabled(false);
        oooButton.setEnabled(false);
    }

    /**
     * Opens a file browser.
     * @return
     *      Returns the absolute path to whatever was selected,
     *      unless cancelled, in which case it will return an empty string.
     */
    private String getFilePath() {
        JFileChooser fileChooser = new JFileChooser();
        int response = fileChooser.showOpenDialog(null);
        if (response != JFileChooser.APPROVE_OPTION) {
            return "";
        }

        return fileChooser.getSelectedFile().getAbsolutePath();
    }

    /**
     * Sets a private array of strings to each line of the template file pointed to by the Template JSON text field.
     * @param filePath
     *      The file path of the template file. It's just the text within the Template JSON text field.
     */
    private void setTemplateArrayFromFilePath(String filePath) {
        try {
            textPaneTemplate.setText("");
            if (Files.exists(Paths.get(filePath))) {
                String s = new String(Files.readAllBytes(Paths.get(filePath)));
                ObjectMapper mapper = new ObjectMapper();
                JsonNode templateObject = mapper.readTree(s);
                templateString = templateObject.toString();
                String[] stringArr = templateObject.toPrettyString().split(System.getProperty("line.separator"));
                templateStrings = new ArrayList<>();
                templateStrings.addAll(Arrays.asList(stringArr));
                templateColorStrings = new ArrayList<>();
                for (int i = 0; i < templateStrings.size(); i++) {
                    templateColorStrings.add("black");
                }

                refreshTemplatePane();
            }
        } catch (IOException exception) {
            exception.printStackTrace();
        }
    }

    /**
     * Takes a private array of strings and an array of colors and sets each line in the Template JSON preview pane.
     */
    private void refreshTemplatePane() {
        StringBuilder formattedString = new StringBuilder("<html><pre>");
        for (int i = 0; i < templateStrings.size(); i++) {
            formattedString
                    .append("<font color=\"")
                    .append(templateColorStrings.get(i))
                    .append("\">")
                    .append(templateStrings.get(i))
                    .append("</font><br>");
        }
        formattedString.append("</pre></html>");
        textPaneTemplate.setText(formattedString.toString());
    }

    /**
     * Validates all of the fields within the GUI before running either the Template Validator or the
     * Out of Order Messaging function.
     * @param template
     *      Whether or not this is the Template Validator so we can decide
     *      on if we need to check the JSON Template field.
     * @return
     *      Returns True if the fields are valid, otherwise False.
     */
    private boolean validateFields(boolean template) {
        String errors = "";
        if (textFieldClientId.getText().isEmpty()) errors = errors.concat("Client ID field is empty\n");
        if (textFieldClientEndpoint.getText().isEmpty()) errors = errors.concat("Client Endpoint field is empty\n");
        if (textFieldCertificateFile.getText().isEmpty()) errors = errors.concat("Certificate File field is empty\n");
        if (textFieldPrivateKeyFile.getText().isEmpty()) errors = errors.concat("Private Key File field is empty\n");
        if (textFieldThingName.getText().isEmpty()) errors = errors.concat("Thing Name field is empty\n");
        if (template && textFieldTemplateJSON.getText().isEmpty()) errors = errors.concat("Template JSON field is empty\n");

        if (!errors.isEmpty()) {
            textPane.setText(textPane.getText() + "Can't run validation suite. The following errors occurred:\n");
            textPane.setText(textPane.getText() + errors);
            return true;
        }

        File tempFile = new File("." + File.separator + "last_settings.txt");
        try (FileWriter fw = new FileWriter(tempFile)) {
            fw.write(textFieldClientId.getText() + "|" +
                    textFieldClientEndpoint.getText() + "|" +
                    textFieldCertificateFile.getText() + "|" +
                    textFieldPrivateKeyFile.getText() + "|" +
                    textFieldThingName.getText() + "|" +
                    textFieldShadowName.getText() + "|" +
                    textFieldTemplateJSON.getText());
            fw.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return false;
    }

    /**
     * Runs the Template Validator. See the ShadowValidator class for details on how this works.
     */
    private void validateTemplate() {
        if (validateFields(true)) {
            enableButtons();
            return;
        }

        shadowValidator = new ShadowValidator(
                textFieldClientId.getText(),
                textFieldClientEndpoint.getText(),
                textFieldCertificateFile.getText(),
                textFieldPrivateKeyFile.getText());

        try {
            boolean result = shadowValidator.shadowTest(
                    textFieldThingName.getText(),
                    textFieldShadowName.getText(),
                    textFieldTemplateJSON.getText(),
                    true);
            if (result) {
                for (int i = 0; i < templateColorStrings.size(); i++) {
                    templateColorStrings.set(i, "green");
                }
            } else {
                for (int i = 0; i < templateColorStrings.size(); i++) {
                    templateColorStrings.set(i, "red");
                }
            }

            refreshTemplatePane();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            enableButtons();
        }
    }

    /**
     * Runs the Out of Order Messaging test. See the MessageOrder class for details on how this works.
     * @throws AWSIotException
     * @throws InterruptedException
     */
    private void outOfOrderMessages() throws AWSIotException, InterruptedException {
        if (validateFields(false)) {
            enableButtons();
            return;
        }

        SampleUtil.KeyStorePasswordPair pair = SampleUtil.getKeyStorePasswordPair(
                textFieldCertificateFile.getText(),
                textFieldPrivateKeyFile.getText());

        AWSIotMqttClient client = new AWSIotMqttClient(
                textFieldClientEndpoint.getText(),
                "wsu-validator",
                pair.keyStore,
                pair.keyPassword);

        try{
            client.connect();
        } catch (AWSIotException e) {
            e.printStackTrace();
        }
        System.out.println(">>>>> CHECKING MQTT SUBSCRIPTIONS <<<<<");
        checkMessageOrder = new MessageOrder(
                client,
                textFieldThingName.getText(),
                textFieldShadowName.getText(),
                textFieldClientId.getText());

        checkMessageOrder.checkMQTTTopics(true); // Checking for /get/accepted, /update/accepted, and /update/delta

        System.out.println("\n>>>>> CHECKING OUT-OF-ORDER MESSAGES <<<<<");
        try {
            checkMessageOrder.checkHandling();
        } catch (JsonProcessingException | InterruptedException e) {
            e.printStackTrace();
        }

        client.disconnect();
        enableButtons();
    }

    /**
     * Loads previous settings found in a local file named "last_settings.txt" and puts them into the GUI fields.
     */
    private void loadPreviousSettings() {
        File settingsFile = new File("." + File.separator + "last_settings.txt");
        try (Scanner scanner = new Scanner(settingsFile)) {
            String scannedString = "";
            while (scanner.hasNextLine()) {
                scannedString = scannedString.concat(scanner.nextLine());
            }

            String[] strings = scannedString.split("\\|", -1);
            textFieldClientId.setText(strings[0]);
            textFieldClientEndpoint.setText(strings[1]);
            textFieldCertificateFile.setText(strings[2]);
            textFieldPrivateKeyFile.setText(strings[3]);
            textFieldThingName.setText(strings[4]);
            textFieldShadowName.setText(strings[5]);
            textFieldTemplateJSON.setText(strings[6]);
            if (!textFieldTemplateJSON.getText().isEmpty()) {
                setTemplateArrayFromFilePath(textFieldTemplateJSON.getText());
            }
        } catch (IOException | NullPointerException e) {
            e.printStackTrace();
        }
    }

    /*
    The following two functions redirect the console output to the text pane. Credit goes to:
    http://unserializableone.blogspot.com/2009/01/redirecting-systemout-and-systemerr-to.html
     */
    private void updateTextPane(final String text) {
        SwingUtilities.invokeLater(() -> {
            Document doc = textPane.getDocument();
            try {
                doc.insertString(doc.getLength(), text, null);
            } catch (BadLocationException e) {
                throw new RuntimeException(e);
            }
            textPane.setCaretPosition(doc.getLength() - 1);
        });
    }

    private void redirectSystemStreams() {
        OutputStream out = new OutputStream() {
            @Override
            public void write(final int b) {
                updateTextPane(String.valueOf((char) b));
            }

            @Override
            public void write(byte[] b, int off, int len) {
                updateTextPane(new String(b, off, len));
            }

            @Override
            public void write(byte[] b) {
                write(b, 0, b.length);
            }
        };

        System.setOut(new PrintStream(out, true));
        System.setErr(new PrintStream(out, true));
    }
}
