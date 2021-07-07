# DEMO APPLICATION
In order to run this program, you need to create an AWS account, create a Thing, and created a named Shadow.
After that, you need to download the certificate key file and private key file to connect for publishing and subscribing.

Start the program com.hp.sms.wsuiotdevice.Application with the following command-line arguments:  
clientId clientEndpoint certificateFile privateKeyFile thingName shadowName

Example:
yourClientIdHere yourEndpointHere-ats.iot.yourRegionHere.amazonaws.com
yourCertificateNameHere-certificate.pem.crt yourCertificateNameHereAlso-private.pem.key
yourThingNameHere yourShadowNameHere

Your named shadow's policy must include permissions for the demo application's clientId to connect, as well as permission to subscribe/publish
to it's reserved MQTT topics /get, /get/accepted, /get/rejected, /update, /update/accepted, /update/rejected, and /update/delta.

This test application is intended to be run synchronously with the validator program
in order to demonstrate how the validator tests on whether the client is handling
out of order messages correctly or incorrectly.

To run:
- Launch the validator first.
- Once the validator has connected to AWS IoT, select option '1' from the validator menu.
- Follow validator prompts to disconnect and disconnect the demo application.
- When the demo application connects, the user is presented with the following menu:
	- SET UP - Enter command:
	- 0 - Connect to AWS IoT and subscribe to /get/accepted.
	- 1 - Connect to AWS IoT and don't subscribe to /get/accepted.
	- 2 - Handle out-of-order messages correctly.
	- 3 - Handle out-of-order messages incorrectly. (Default)
	- 4 - Show most recent document.
	- 5 - Disconnect from AWS IoT.
- Options 2 and 3 should be done first, before connecting the demo application to AWS IoT (options 0 and 1).
If 2 or 3 are not selected, default behavior is that the demo application will handle out-of-order messages incorrectly.