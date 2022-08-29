#[derive(Debug, Clone)]
pub struct Node<T: Copy> {
  pub left: Option<usize>,  // Each child node will just be an index back into the arena where the actual node is held.
  pub right: Option<usize>, // Each child node will just be an index back into the arena where the actual node is held.
  pub data: Option<T>       // The data held by this node.
}

pub struct Tree<T: Copy> {
  pub nodes: Vec<Node<T>>,  // Rust prefers the idea of a "memory arena".
  pub root: Option<usize>   // The root of the binary tree.
}

pub fn new<T: Copy>(v: &[Option<T>]) -> Option<Tree<T>> {
  if v.is_empty() {
    return None;
  }

  let mut tree = Tree{nodes: vec![Node{left: None, right: None, data: v[0]}], root: Some(0)};
  let mut nodes_index = 0;

  let create_node = |node_data: &Option<T>, tree: &mut Tree<T>| -> Option<usize> {
    match node_data {
      Some(_x) => {
        tree.nodes.push(Node{left: None, right: None, data : *node_data});
        return Some(tree.nodes.len() - 1);
      },
      None => { /* No child */ }
    }
    return None;
  };

  let mut i = 1;
  while i < v.len() {
    let left = create_node(&v[i], &mut tree);
    i += 1;
    let right = create_node(&v[i], &mut tree);
    i += 1;

    let current = &mut tree.nodes[nodes_index];
    nodes_index += 1;
    current.left = left;
    current.right = right;
  }
  
  Some(tree)
}

pub enum Traversal {
  Breadth,
  Preorder, // Parent, Left, Right
  Inorder,  // Left, Parent, Right
  Postorder, // Left, Right, Parent
  Preorder_recursive, // Parent, Left, Right
  Inorder_recursive,  // Left, Parent, Right
  Postorder_recursive // Left, Right, Parent
}

pub fn traverse<T: Copy>(tree: &Tree<T>, traversal: Traversal, predicate: fn(&Node<T>) -> bool) {
  match traversal {
    Traversal::Breadth => {
      traverse_breadth(&tree, predicate);
    },
    Traversal::Preorder => {
      traverse_preorder(&tree, predicate);
    },
    Traversal::Inorder => { 
      traverse_inorder(&tree, predicate);
    },
    Traversal::Postorder => {
      traverse_postorder(&tree, predicate);
    },
    Traversal::Preorder_recursive => {
      traverse_preorder_recursive(&tree, Some(0 as usize), predicate);
    },
    Traversal::Inorder_recursive => { 
      traverse_inorder_recursive(&tree, Some(0 as usize), predicate);
    },
    Traversal::Postorder_recursive => {
      traverse_postorder_recursive(&tree, Some(0 as usize), predicate);
    }
  }
}

pub fn traverse_breadth<T: Copy>(tree: &Tree<T>, predicate: fn(&Node<T>) -> bool) {
  let mut queue = vec![tree.root];

  while !queue.is_empty() {
    let current = queue.pop().unwrap();
    match current {
      Some(x) => {
        let node = &tree.nodes[x];
        if predicate(&node) {
          return;
        }
        match node.left {
          Some(_x) => {
            queue.insert(0, node.left);
          },
          None => {}
        }
        match node.right {
          Some(_x) => {
            queue.insert(0, node.right);
          },
          None => {}
        }
      }
      None => {}
    }
  }
}

pub fn traverse_preorder<T: Copy>(tree: &Tree<T>, predicate: fn(&Node<T>) -> bool) {
  let mut stack: Vec<usize> = Vec::new();
  let mut current = tree.root.unwrap();

  while !stack.is_empty() || current != usize::MAX {
    if current == usize::MAX {
      current = stack.pop().unwrap();
    } else {
      let node = &tree.nodes[current];
      if predicate(&node) {
        return;
      }
      match node.right {
        Some(x) => { stack.push(x); },
        None => {}
      }
      match node.left {
        Some(x) => { current = x; },
        None => { current = usize::MAX; }
      }
    }
  }
}

pub fn traverse_preorder_recursive<T: Copy>(tree: &Tree<T>, node: Option<usize>, predicate: fn(&Node<T>) -> bool) {
  match node {
    Some(x) => {
      let node = &tree.nodes[x];
      if predicate(&node) {
        return;
      }

      traverse_preorder_recursive(&tree, node.left, predicate);
      traverse_preorder_recursive(&tree, node.right, predicate);
    },
    None => {}
  }
}

pub fn traverse_inorder<T: Copy>(tree: &Tree<T>, predicate: fn(&Node<T>) -> bool) {
  let mut stack: Vec<usize> = Vec::new();
  let mut current = tree.root.unwrap();

  while !stack.is_empty() || current != usize::MAX {
    if current == usize::MAX {
      current = stack.pop().unwrap();
      let node = &tree.nodes[current];
      if predicate(&node) {
        return;
      }
      match node.right {
        Some(x) => { current = x; },
        None => { current = usize::MAX; }
      }
    } else {
      stack.push(current);
      let node = &tree.nodes[current];
      match node.left {
        Some(x) => { current = x; },
        None => { current = usize::MAX; }
      }
    }
  }
}

pub fn traverse_inorder_recursive<T: Copy>(tree: &Tree<T>, node: Option<usize>, predicate: fn(&Node<T>) -> bool) {
  match node {
    Some(x) => {
      let node = &tree.nodes[x];
      traverse_inorder_recursive(&tree, node.left, predicate);
      if predicate(&node) {
        return;
      }
      traverse_inorder_recursive(&tree, node.right, predicate);
    },
    None => {}
  }
}

pub fn traverse_postorder<T: Copy>(tree: &Tree<T>, predicate: fn(&Node<T>) -> bool) {
  let mut stack: Vec<usize> = Vec::new();
  let mut current = tree.root.unwrap();
  let mut last = usize::MAX;

  while !stack.is_empty() || current != usize::MAX {
    if current == usize::MAX {
      let peek = stack.last().unwrap();
      let node = &tree.nodes[*peek];
      let mut right = usize::MAX;
      match node.right {
        Some(x) => { right = x; },
        None => {}
      };
      if right != usize::MAX && right != last {
        current = right;
      } else {
        if predicate(&node) {
          return;
        }
        last = *peek;
        current = usize::MAX;
        stack.pop();
      }
    } else {
      stack.push(current);
      let node = &tree.nodes[current];
      match node.left {
        Some(x) => { current = x; }
        None => { current = usize::MAX; }
      }
    }
  }
}

pub fn traverse_postorder_recursive<T: Copy>(tree: &Tree<T>, node: Option<usize>, predicate: fn(&Node<T>) -> bool) {
  match node {
    Some(x) => {
      let node = &tree.nodes[x];
      traverse_postorder_recursive(&tree, node.left, predicate);
      traverse_postorder_recursive(&tree, node.right, predicate);
      if predicate(&node) {
        return;
      }
    },
    None => {}
  }
}