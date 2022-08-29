mod fibonacci;
mod sort;
mod tree;

fn test_sort(sort : fn(&mut Vec<i32>)) {
  let mut v = vec![10, 8, 6, 4, 2, 0, 9, 7, 5, 3, 1];
  sort(&mut v);
  println!("{:?}", v);
}

fn test_fibonacci(n : u128, fib : fn(u128) -> u128) {
  println!("{}", fib(n));
}

fn test_tree(traversal: tree::Traversal) {
  let data = vec![Some("F"), Some("B"), Some("G"), Some("A"), Some("D"), None, Some("I"), None, None, Some("C"), Some("E"), Some("H"), None];
  let predicate = |node: &tree::Node<&str>| {
    match node.data {
      Some(x) => {
        print!("{}", x);
      },
      None => {}
    }

    return false;
  };

  let tree = tree::new(&data);
  match tree {
    Some(x) => {
      tree::traverse(&x, traversal, predicate);
      print!("\n");
    },
    None => {
      println!("Empty tree!!!");
    }
  };
}

fn main() {
    test_sort(sort::bubble::execute);
    test_sort(sort::selection::execute);
    test_sort(sort::insertion::execute);
    test_sort(sort::merge::execute);
    test_sort(sort::quick::execute);

    test_fibonacci(10, fibonacci::recursive::execute);
    test_fibonacci(10, fibonacci::dynamicv1::execute);
    test_fibonacci(10, fibonacci::dynamicv2::execute);
    // test_fibonacci(100, fibonacci::recursive::execute);
    test_fibonacci(100, fibonacci::dynamicv1::execute);
    test_fibonacci(100, fibonacci::dynamicv2::execute);

    test_tree(tree::Traversal::Breadth);
    test_tree(tree::Traversal::Preorder);
    test_tree(tree::Traversal::Inorder);
    test_tree(tree::Traversal::Postorder);
    test_tree(tree::Traversal::Preorder_recursive);
    test_tree(tree::Traversal::Inorder_recursive);
    test_tree(tree::Traversal::Postorder_recursive);
}
