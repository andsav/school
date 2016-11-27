pub trait Encode {
    type X;
    fn encode(&self) -> Self::X;
}

pub struct Hello {
    pub router_id: u8,
    pub link_id: u8
}

impl Encode for Hello {
    type X = [u8; 2];
    fn encode(&self) -> Self::X {
        [self.router_id, self.link_id]
    }
}

pub struct PSU {
    pub sender: u8,
    pub router_id: u8,
    pub link_id: u8,
    pub cost: u8,
    pub via: u8
}

impl Encode for PSU {
    type X = [u8; 5];
    fn encode(&self) -> Self::X  {
        [self.sender, self.router_id, self.link_id, self.cost, self.via]
    }
}

pub struct Init {
    pub router_id: u8
}

impl Encode for Init {
    type X = [u8; 1];
    fn encode(&self) -> Self::X  {
        [self.router_id]
    }
}
