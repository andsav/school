mod helpers;
mod packet;
mod state;

use std::{env, net, thread};
use std::fs::File;
use packet::Encode;

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
    let nse_hostname: &str = &*args[2].to_string();
    let router_port: u16 = args[3].parse::<u16>().expect("Invalid router_port");
    let nse_port: u16 = args[4].parse::<u16>().expect("Invalid nse_port");

    /*
    Initialization
    */
    let socket = net::UdpSocket::bind(("localhost", router_port)).expect("Error binding to socket");

    let log_file_name = "router".to_string() + s_router_id + ".log";
    let log = File::create(&log_file_name).expect("Unable to create log file");

    /*
    Main Program
    */

    // 1) Send Init

    let init = packet::Init{router_id: router_id}.encode();
    socket.send_to(&init, (nse_hostname, nse_port));


    // 2) Send Hello to neighbours


    // 3) Receive Hello and LSPDU and flood LSPDU


}