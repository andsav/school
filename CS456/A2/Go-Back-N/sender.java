import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class sender {

    // Constants
    private final int MAX_READ_SIZE = 500;
    private final int WINDOW_SIZE = 10;

    // Attributes
    private static String nEmulatorAddress;
    private static int nEmulatorPort;
    private static int senderPort;

    private static DatagramSocket senderReceiveSocket;
    private static File file;

    private static PrintWriter seqnumWriter;
    private static PrintWriter ackWriter;

    private static ArrayList<byte[]> chunks = new ArrayList<byte[]>();

    /**
     * Point of entry
     *
     * @param args
     * @throws Exception
     */
    public static void main(String args[]) throws Exception {
        try {
            parseArgs(args);
            init();
            chunkFile();


        } catch(Exception e) {
            System.err.println(e);
            System.exit(1);
        }
    }

    /**
     * Validate and parse arguments
     *
     * @param args
     */
    public static void parseArgs(String args[]) throws Exception {
        if(args.length != 4) {
            throw new Exception("Sender excepts 4 arguments");
        }

        nEmulatorAddress = args[0];

        nEmulatorPort = Integer.parseInt(args[1]);
        senderPort = Integer.parseInt(args[2]);

        if(nEmulatorPort > '\uffff') {
            throw new Exception("Invalid port " + nEmulatorPort);
        }

        if(senderPort > '\uffff') {
            throw new Exception("Invalid port " + senderPort);
        }

        file = new File(args[3]);
        if(!file.exists()) {
            throw new Exception("File does not exist");
        }
    }

    /**
     * Initialize
     */
    public static void init() throws SocketException, FileNotFoundException, UnsupportedEncodingException {
        senderReceiveSocket = new DatagramSocket(senderPort);

        seqnumWriter = new PrintWriter("segnum.log", "UTF-8");
        ackWriter = new PrintWriter("ack.log", "UTF-8");
    }

    public static void chunkFile() {

        
    }

}