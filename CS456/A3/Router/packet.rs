pub struct Hello {
    router_id: u8,
    link_id: u8
}

pub struct PSU {
    sender: u8,
    router_id: u8,
    link_id: u8,
    cost: u8,
    via: u8
}

pub struct Init {
    router_id: u8
}
