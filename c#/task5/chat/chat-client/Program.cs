using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace chat {
  class Program {
    static void Main(string[] args) {
      if (args.Length != 4) {
        printUsage();
        return;
      }

      int remotePort;
      if (!Int32.TryParse(args[1], out remotePort)) {
        printUsage();
        return;
      }

      int localPort;
      if (!Int32.TryParse(args[2], out localPort)) {
        printUsage();
        return;
      }

      bool isServer;
      if (!Boolean.TryParse(args[3], out isServer)) {
        printUsage();
        return;
      }

      Client client = new Client();
      Server server = new Server();

      if (isServer) {
        if (!server.Accept(localPort)) {
          return;
        }
        if (!client.Connect(args[0], remotePort)) {
          return;
        }
      } else {
        if (!client.Connect(args[0], remotePort)) {
          return;
        }
        if (!server.Accept(localPort)) {
          return;
        }
      }

      Thread thread = new Thread(client.Start);
      thread.Start();

      server.Start();
    }

    static void printUsage() {
      Console.WriteLine("Usage: <this app> <remote ip> <remote port> <local port> <is server>");
    }
  }

  public class Server {
    private Socket handler;

    public Server() {
    }

    public bool Accept(int port) {
      try {
        Console.WriteLine("Waiting for connection...");

        IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Any, port);

        Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        listener.Bind(localEndPoint);
        listener.Listen(10);
        handler = listener.Accept();

        Console.WriteLine("Connection accepted from {0}!", handler.RemoteEndPoint.ToString());
      } catch (Exception e) {
        Console.WriteLine(e.ToString());
        return false;
      }
      return true;
    }

    public void Start() {
      try {
        byte[] bytes = new byte[1024];
        while (true) {
          int bytesReceived = handler.Receive(bytes);
          string message = Encoding.ASCII.GetString(bytes, 0, bytesReceived);
          if (message.Equals("/exit\n")) {
            break;
          }
          Console.Write("Received: " + message);
        }

        Console.WriteLine("Server disconnecting...");

        handler.Shutdown(SocketShutdown.Both);
        handler.Close();

        Console.WriteLine("Server disconnected!");
      } catch (Exception e) {
        Console.WriteLine(e.ToString());
        return;
      }
    }
  }

  public class Client {
    private Socket sender;

    public Client() {
    }

    public bool Connect(string ip, int port) {
      try {
        Console.WriteLine("Connecting...");

        IPAddress ipAddress = IPAddress.Parse(ip);
        IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

        sender = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        sender.Connect(remoteEP);

        Console.WriteLine("Connected to {0}!", sender.RemoteEndPoint.ToString());
      } catch (Exception e) {
        Console.WriteLine(e.ToString());
        return false;
      }
      return true;
    }

    public void Start() {
      try {
        while (true) {
          string command = Console.ReadLine() + "\n";
          byte[] message = Encoding.ASCII.GetBytes(command);

          int bytesSent = 0;
          while (bytesSent < message.Length) {
            bytesSent += sender.Send(message);
          }

          if (command.Equals("/exit\n")) {
            break;
          }
        }

        Console.WriteLine("Client disconnecting...");

        sender.Shutdown(SocketShutdown.Both);
        sender.Close();

        Console.WriteLine("Client disconnected!");
      } catch (Exception e) {
        Console.WriteLine(e.ToString());
        return;
      }
    }
  }
}
