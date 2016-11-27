const NBR_ROUTER: usize = 5;

pub struct LinkCost {
    link: u8,
    cost: u8
}

pub struct CircuitDB {
    nbr_link: u8,
    link_cost: [LinkCost; NBR_ROUTER]
}