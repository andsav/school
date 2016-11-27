mod helpers;
mod packet;
mod state;

use std::{env, net};
use std::fs::File;

fn main() {
    /*
    Argument validation and parsing
    */

    let args: Vec<_> = env::args().collect();

    if args.len() != 5 {
        helpers::abort("Expected 4 arguments: <router_id>, <nse_hostname>, <router_port>, <nse_port>");
    }

    let router_id: u8 = args[1].parse::<u8>().expect("Invalid router_id");
    let nse_hostname: &str = &*args[2].to_string();
    let router_port: u16 = args[3].parse::<u16>().expect("Invalid router_port");
    let nse_port: u16 = args[4].parse::<u16>().expect("Invalid nse_port");

    /*
    Initialization
    */
    let socket = net::UdpSocket::bind((nse_hostname, nse_port)).expect("Error binding to socket");

    let log_file_name = "router".to_string() + &*args[1] + ".log";
    let log = File::create(&log_file_name).expect("Unable to create log file");

    /*
    Main Program
    */
    let mut buf = [0; 10];
    loop {
        socket.recv_from(&mut buf);
    }

    println!("{} {} {} {}", router_id, nse_hostname, router_port, nse_port);
}