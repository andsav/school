pub trait Packet {
    type X;
    fn encode(&self) -> Self::X;
    fn log_message(&self) -> String;
}

pub struct Hello {
    pub router_id: u8,
    pub link_id: u8
}

impl Packet for Hello {
    type X = [u8; 2];
    fn encode(&self) -> Self::X {
        [self.router_id, self.link_id]
    }
    fn log_message(&self) -> String {
        String::from("HELLO link_id: ") + &*self.link_id.to_string()
    }
}

pub struct PSU {
    pub sender: u8,
    pub router_id: u8,
    pub link_id: u8,
    pub cost: u8,
    pub via: u8
}

impl Packet for PSU {
    type X = [u8; 5];
    fn encode(&self) -> Self::X  {
        [self.sender, self.router_id, self.link_id, self.cost, self.via]
    }
    fn log_message(&self) -> String {
        String::from("LSPDU link_id: ") + &*self.link_id.to_string()
    }
}

pub struct Init {
    pub router_id: u8
}

impl Packet for Init {
    type X = [u8; 1];
    fn encode(&self) -> Self::X  {
        [self.router_id]
    }
    fn log_message(&self) -> String {
        String::from("INIT")
    }
}
