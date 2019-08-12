//server side

import java.net.*;
import java.io.*;

public class Server{
	private Socket socket =null;
	private ServerSocket server = null;
	private DataInputStream input = null;

	public Server(int port)
	{
		try
		{
			server = new ServerSocket(port);
			System.out.println("server started");
			System.out.println("Waiting for connection");
			socket=server.accept();
			System.out.println("connection accepted");

			input = new DataInputStream(new BufferedInputStream(socket.getInputStream()));

			String line ="";
			while(!line.equals("over")){
				try{
					line = input.readUTF();
					System.out.println(line);
				}
				catch(IOException i)
				{
					System.out.println(i);
				}
			}
			System.out.println("closing connection");
			socket.close();
			input.close();
		}
		catch(IOException i)
		{
			System.out.println(i);
		}
	}

	public static void main(String args[])
	{
		Server server = new Server(8888);
	}
}

//-----------------------------------------------------------------------------------------------------------------------------
//client
import java.net.*;
import java.io.*;

public class Client
{
	private Socket socket=null;
	private DataInputStream input = null;
	private DataOutputStream out = null;

	public Client(String address ,int port)
	{
		try
		{
			socket = new Socket(address,port);
			input = new DataInputStream(System.in);
			out = new DataOutputStream(socket.getOutputStream());
		}
		catch(UnknownHostException u)
		{
			System.out.println(u);
		}
		catch(IOException i)
		{
			System.out.println(i);
		}
		
		String line ="";
		while(!line.equals("over"))
		{
			try
			{
				
			line=input.readLine();
			out.writeUTF(line);
			}
		catch(IOException i)
		{
			System.out.println(i);
		}
		}
	try
	{
		input.close();
		out.close();
		socket.close();
	}
	catch(IOException i){
		System.out.println(i);
	}
}
public static void main(String args[])
{
	Client client = new Client("127.0.0.1",8888);
}
}







