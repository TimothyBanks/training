pub mod recursive {
  pub fn execute(n : u128) -> u128 {
    if n < 2 {
      return n;
    }
    execute(n - 1) + execute(n - 2)
  }
}
pub mod dynamicv1 {
  pub mod details {
    pub fn execute(n : u128, lookup : &mut Vec<u128>) -> u128 {
      if n < 2 {
        return n;
      }

      if lookup[(n - 1) as usize] == 0 {
        lookup[(n - 1) as usize] = execute(n - 1, lookup);
      }

      if lookup[(n - 2) as usize] == 0 {
        lookup[(n - 2) as usize] = execute(n - 2, lookup);
      }

      lookup[(n - 1) as usize] + lookup[(n - 2) as usize]
    }
  }
  pub fn execute(n : u128) -> u128 {
    let mut lookup = vec![0; (n + 1) as usize];
    return details::execute(n, &mut lookup);
  }
}
pub mod dynamicv2 {
  pub fn execute(n : u128) -> u128 {
    if n < 2 {
      return n;
    }

    let mut n1 = 1 as u128;
    let mut n2 = 0 as u128;

    for _i in 2..n+1 {
      let n = n1 + n2;
      n2 = n1;
      n1 = n;
    }

    n1
  }
}