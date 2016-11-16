#include <iostream>
#include "router.h"
#include "Socket.h"
#include "Packet.h"
#include "LinkState.h"

using namespace std;

int main(int argc, char **argv) {

    try {
        if(argc != 5) {
            cerr << "Router excepts 4 arguments: <router_id>, <nse_hostname>, <router_port>, <nse_port>" << endl;
            throw exception();
        }

        /*
         * Parsing arguments
         */
        unsigned int router_id = atoi(argv[1]);
        string nse_hostname = argv[2];
        unsigned int router_port = atoi(argv[3]);
        unsigned int nse_port = atoi(argv[4]);

        /*
         * Initialization
         */

        // File
        stringstream log_file_name;
        log_file_name << "router" << argv[1] << ".log";
        ofstream log;
        log.open(log_file_name.str(), ofstream::out | ofstream::trunc);

        // Socket
        Socket socket;
        if(socket.fd < 0) {
            cerr << "Socket creation failed" << endl;
            throw exception();
        }
        if(!socket.init(router_port, nse_hostname, nse_port)) {
            cerr << "Socket init failed" << endl;
            throw exception();
        }

        cout << socket.fd << endl;

        cout << router_id << endl;

        /*
         * Cleanup
         */
        log.close();

    } catch(...) {
        cerr << "Program aborted" << endl;
    }

    return 0;
}