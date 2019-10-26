import java.net.*;
import java.util.Scanner;
import java.io.*;

// Connects a client/user to the Server. The client generates a string from their keyboard
// and sends it out through the socket output stream to the Server. The Server
// collects the string, and then distributes it to every client's input stream.

public class ClientMain {

public static void main(String[] args) {
	Socket socket = null;
	Scanner inputSocket = null;
	PrintWriter toSocket = null;
	String username;
            
	try {  // Connect the client to the server.
		socket=new Socket("localhost",2466);
	} catch (UnknownHostException e1) {
		e1.printStackTrace();
	} catch (IOException e1) {
		e1.printStackTrace();
	} 
	
	ClientSender TC=new ClientSender();  
    ClientReceiver FC=new ClientReceiver(); 
    
		try {
			System.out.println("<<< You have joined the chat. >>>");
			
			/////////////// generating a username
			Scanner usersc=new Scanner(System.in);  	
			System.out.println("<<< What is your username? >>>");
			username=usersc.nextLine();					// Establish a username
			System.out.println("<<< Your username is " + username + " >>>"); // The username is sent out with your input string.
			usersc.close();
			
			inputSocket=new Scanner(socket.getInputStream()); // Wire up the input stream from the Server.
			toSocket=new PrintWriter(socket.getOutputStream()); // Wire up the output stream to the Server.
			
			toSocket.println("<<< " + username + " has joined the chat. >>>"); // introduce yourself! 
			toSocket.flush(); 
			
			TC.setPrintWriter(toSocket,username); // Connect the output stream to a new thread to read from the keyboard and send out to the Server.
	        FC.setScanner(inputSocket);  // Connect the input stream to a new thread to read input from the Server.
	        TC.start();
	        FC.start();
		} catch ( Exception e ) {
			System.out.println("Error: " + e);
		}
		
		if ( TC.isAlive() == false )
		{
			return;
		}
                                       	
	} // end of the main program

} // end of the class

	

