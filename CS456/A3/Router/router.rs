mod helpers;
mod packet;
mod state;

use std::{env, net, thread};
use std::fs::File;
use std::io::Write;
use packet::Packet;

fn main() {
    /*
    Argument validation and parsing
    */

    let args: Vec<_> = env::args().collect();

    if args.len() != 5 {
        helpers::abort("Expects arguments: <router_id>, <nse_hostname>, <router_port>, <nse_port>");
    }

    let router_id: u8 = args[1].parse::<u8>().expect("Invalid router_id");
    let s_router_id: &str = &*args[1].to_string();
    let s_router_name: String = "R".to_string() + s_router_id;

    let nse_hostname: &str = &*args[2].to_string();
    let router_port: u16 = args[3].parse::<u16>().expect("Invalid router_port");
    let nse_port: u16 = args[4].parse::<u16>().expect("Invalid nse_port");

    /*
    Initialization
    */
    let socket = net::UdpSocket::bind(("localhost", router_port)).expect("Error binding to socket");

    let log_file_name = "router".to_string() + s_router_id + ".log";
    let mut log = File::create(&log_file_name).expect("Unable to create log file");

    /*
    Main Program
    */
    macro_rules! udp_send {
        ($packet: expr) => {
            let pkt = $packet;
            let pkt_bits = pkt.encode();
            let log_msg = s_router_name + " " + &*pkt.log_message();

            socket.send_to(&pkt_bits, (nse_hostname, nse_port)).expect("Failed to send packet");

            log.write_all(&*log_msg.as_bytes()).expect("Failed to write log");
        }
    }

    // 1) Send Init

    udp_send!(packet::Init{router_id: router_id});


    // 2) Send Hello to neighbours


    // 3) Receive Hello and LSPDU and flood LSPDU


}