import java.net.*;
import java.util.LinkedList;
import java.util.Scanner;
import java.io.*;

public class ServerMain {
public static void main(String[] args)
{
	int ID=0;
	Socket socket = null;
	ServerSocket serverSocket = null;
	Scanner sc = null; // receives from the socket
	PrintWriter pw = null; // writes to the socket
	String username="";
	LinkedList<PrintWriter> users=new LinkedList<PrintWriter>();
	
    try {
		serverSocket=new ServerSocket(2466);
	} catch (IOException e1) {
		e1.printStackTrace();
	}
	System.out.println("Server started");
	System.out.println("Waiting for a client...");


	while ( true ) {
		try {
			socket=serverSocket.accept();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("Client accepted.");
		
		try {
			sc=new Scanner(socket.getInputStream()); // A new scanner gets input from the client.
			pw=new PrintWriter(socket.getOutputStream()); // A new printer sends output to the client.
			users.add(pw); 				// Add the new client's output stream in the LinkedList of users. 
			ID++; /////////////////////////////////////////////
		} catch ( Exception h ) {
			System.out.println("Error");
		}	
		ServerCommunication TS=new ServerCommunication();  			// Start a new thread
        TS.setPrintWriter(username,users,sc,ID);  // Send the new scanner to the new thread
	    TS.start();	
	} //end while loop

       
    } // end of the main program
} // end of the class
