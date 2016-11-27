use std::process;

pub fn abort(msg: &'static str) {
    println!("{}\nRouter aborted!", msg);
    process::exit(1);
}