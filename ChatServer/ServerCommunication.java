import java.util.LinkedList;
import java.util.Scanner;
import java.io.*;

// Used by ServerMain.
// The server passes a LinkedList of the PrintWriters associated with the output streams
// of all the clients. It takes input and sends it out to every client saved in the LinkedList.

public class ServerCommunication extends Thread {
	String username;
	LinkedList<PrintWriter> users; // stores p
	Scanner sc; // taking input lines from clients
	int ID;
	
		public void run() 
		{
			int i=0;
			while ( sc.hasNextLine() ) // sc is connected to the socket.InputStream()
			{
				String buffer=sc.nextLine(); // Read input 
				while ( users.size() > i )   // Go through and send to all the users
				{
					System.out.println("# of users in LL: " + users.size() + " Thread ID: " + ID + " " + buffer); // Aside from buffer, these are extra quality control statements.
					PrintWriter pw=(PrintWriter)users.get(i);
					pw.println(buffer); // send it to the client's socket
					pw.flush(); 		
					i++;
				}
				i=0; // Reset i
			}
			sc.close();
		}
		
		public void setPrintWriter(String username,LinkedList<PrintWriter> users,Scanner sc,int ID) // Constructor
		{
			this.username=username;
			this.users=users;
			this.sc=sc;
			this.ID=ID;/////////////////////////////////////////
		}

}

