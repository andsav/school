import java.io.FileInputStream;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;
import java.net.InetAddress;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;


public class sender {

    // Constants
    private final static int MAX_READ_SIZE = 500;
    private final static int WINDOW_SIZE = 10;

    // Attributes
    private static InetAddress nEmulatorAddress;
    private static int nEmulatorPort;

    private static DatagramSocket socket;

    private static FileInputStream file;

    private static PrintWriter seqNumWriter;
    private static PrintWriter ackWriter;

    private static ArrayList<byte[]> chunks = new ArrayList<byte[]>();

    private static int baseNum = 0;
    private static int nextSeqNum = 0;

    /**
     * Point of entry
     *
     * @param args
     * @throws Exception
     */
    public static void main(String args[]) throws Exception {
        try {
            init(args);
            chunkFile();

            sendPackets();

        } catch (Exception e) {
            System.err.println(e);
            System.exit(1);
        }
    }

    /**
     * Validate and parse arguments, initialize
     *
     * @param args
     */
    public static void init(String args[]) throws SocketException, FileNotFoundException, UnsupportedEncodingException, Exception, IOException {
        if (args.length != 4) {
            throw new Exception("Sender excepts 4 arguments");
        }

        nEmulatorAddress = InetAddress.getByName(args[0]);
        nEmulatorPort = Integer.parseInt(args[1]);
        int senderPort = Integer.parseInt(args[2]);

        if (nEmulatorPort > '\uffff') {
            throw new Exception("Invalid port " + nEmulatorPort);
        }

        if (senderPort > '\uffff') {
            throw new Exception("Invalid port " + senderPort);
        }

        socket = new DatagramSocket(Integer.parseInt(args[2]));

        file = new FileInputStream(args[3]);

        seqNumWriter = new PrintWriter("segnum.log", "UTF-8");
        ackWriter = new PrintWriter("ack.log", "UTF-8");
    }

    /**
     * Chunk file into packets
     */
    public static void chunkFile() throws IOException, Exception {
        byte buffer[] = new byte[MAX_READ_SIZE];
        int rc = file.read(buffer);
        int i = 0;
        packet p;

        while(rc != -1) {
            p = packet.createPacket(i++, buffer.toString());
            chunks.add(p.getUDPdata());
            rc = file.read(buffer);
        }

        System.out.println(chunks.toString());
    }

    /**
     * Send packages using the Go-Back-N protocol
     */
    public static void sendPackets() throws IOException, Exception {
        while(baseNum < chunks.size()) {
            for(nextSeqNum = baseNum;
                nextSeqNum < Math.min(baseNum + WINDOW_SIZE, chunks.size());
                nextSeqNum++) {
                byte sendPacket[] = chunks.get(nextSeqNum);

                socket.send(new DatagramPacket(sendPacket, sendPacket.length, nEmulatorAddress, nEmulatorPort));

                if (packet.parseUDPdata(sendPacket).getType() == 1) {
                    seqNumWriter.println(packet.parseUDPdata(sendPacket).getSeqNum());
                }
            }
        }
    }

    public class receiverThread extends Thread {
        public void run() {
            System.out.println("Hello from a thread!");
        }

        public static void main(String args[]) {
            (new receiverThread()).start();
        }
    }

}