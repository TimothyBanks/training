pub mod bubble {
  pub fn execute<T: PartialOrd>(slice : &mut Vec<T>) {
    if slice.is_empty() {
      return;
    }

    let mut sorted = false;
    
    while !sorted {
      sorted = true;
      for i in 0..slice.len() - 1 {
        if slice[i] > slice[i+1] {
          slice.swap(i, i+1);
          sorted = false;
        }
      }
    }
  }
}
pub mod insertion {
  pub fn execute<T: PartialOrd>(slice : &mut Vec<T>) {
    if slice.is_empty() {
      return;
    }

    for i in 1..slice.len() {
      for j in (1..i + 1).rev() {
        if slice[j] < slice[j-1] {
          slice.swap(j, j-1);
        }
      }
    }
  }
}
pub mod selection {
  pub fn execute<T: PartialOrd>(slice : &mut Vec<T>) {
    if slice.is_empty() {
      return;
    }

    for i in 0..slice.len() - 1 {
      let mut min = i;

      for j in i+1..slice.len() {
        if slice[j] < slice[min] {
          min = j;
        }
      }

      slice.swap(i, min);
    }
  }
}
pub mod merge {
  pub fn execute<T: PartialOrd + Clone + Copy>(slice : &mut Vec<T>) {
    if slice.len() < 2 {
      return;
    }

    let mid = slice.len() / 2;
    let left = &mut slice[0..mid].to_vec();
    let right = &mut slice[mid..].to_vec();

    execute(left);
    execute(right);

    let mut i = 0;
    let mut li = 0;
    let mut ri = 0;

    while i < slice.len() && li < left.len() && ri < right.len() {
      if left[li] < right[ri] {
        slice[i] = left[li];
        li += 1;
      } else {
        slice[i] = right[ri];
        ri += 1;
      }
      i += 1;
    }   

    let mut drain = |v: &Vec<T>, mut vi| {
      while vi < v.len() {
        slice[i] = v[vi];
        i += 1;
        vi += 1;
      }
    };
    drain(left, li);
    drain(right, ri);
  }
}
pub mod quick {
  pub mod details {
    pub fn partition<T: PartialOrd + Clone + Copy>(slice : &mut Vec<T>, start : usize, end : usize) -> usize {
      let mut begin = start;
      for i in start..end {
        if slice[i] < slice[end] {
          slice.swap(i, begin);
          begin += 1;
        }
      }

      slice.swap(begin, end);
      begin
    }

    pub fn sort<T: PartialOrd + Clone + Copy>(slice : &mut Vec<T>, start : usize, end : usize) {
      if end <= start {
        return;
      }

      let pindex = partition(slice, start, end);
      sort(slice, start, pindex - 1);
      sort(slice, pindex + 1, end);
    }
  }

  pub fn execute<T: PartialOrd + Clone + Copy>(slice : &mut Vec<T>) {
    details::sort(slice, 0, slice.len() - 1);
  }
}