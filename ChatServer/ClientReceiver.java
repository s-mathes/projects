import java.util.Scanner;

// Used by ClientMain. FromClient uses a scanner that constantly reads from the Server
// and prints it to the client's screen.
// Formerly FromClient

public class ClientReceiver extends Thread {
	Scanner sc; // Getting input from the socket that is connected to the Server.
	
	public void run()
	{
		while ( sc.hasNextLine() )
		{
			String buffer=sc.nextLine(); // Read in the next line from the scanner, which is connected to the Server.
			System.out.println(buffer);  // Print to the client's screen. The string includes the sender's username.
		}	
	}
	
	public void setScanner(Scanner sc)
	{
		this.sc=sc;
	}
	
}
