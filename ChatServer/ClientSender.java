import java.util.Scanner;
import java.io.*;

// Used by ClientMain.
// Extension of the thread class. Continuous while loop that checks for input.
// Takes input from the user/client from their keyboard, and then flushes it
// out to the server with the PrintWriter. 
// Formerly ToClient

public class ClientSender extends Thread {
	PrintWriter pw; // sending output to the socket. PW is connected to socket.getOutputStream
	String username;
	
		public void run() 
		{
		Scanner sc=new Scanner(System.in); // Read input from the user/client keyboard
			while ( sc.hasNextLine() ) // As long as there is a next line, keep reading it into a buffer.
			{
				String buffer=sc.nextLine(); 
				if ( buffer.equals("logout") ) {
					System.out.println("<<< " + username + " has left the chat. >>>");
					sc.close();
					pw.close();
					return;
				}
				pw.println(username + ": " + buffer); // Send it out the socket. Introduce yourself! Send your username!
				pw.flush(); // flush it out
			}
			sc.close();
		}
		
		public void setPrintWriter(PrintWriter pw,String username) // Constructor. Set up the connection to the output stream and 
		{														   // identify the user.
			this.pw=pw;
			this.username=username;
		}

}
