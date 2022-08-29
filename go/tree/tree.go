package tree

type TreeNode struct {
	Data string
	Left *TreeNode
	Right *TreeNode
}

func NewTreeNode(data string, left *TreeNode, right *TreeNode) *TreeNode {
	return &TreeNode{Data: data, Left: left, Right: right}
}

func DefaultTreeNode() *TreeNode {
	return &TreeNode{Data: "", Left: nil, Right: nil}
}

type Tree struct {
	Root *TreeNode
}

func NewTree(data []string) *Tree {
  l := len(data)
	t := &Tree{Root: nil}

	if l > 0 {
		(*t).Root = DefaultTreeNode()
		(*t).Root.Data = data[0]

		var queue []*TreeNode
		queue = append(queue, t.Root)

		for i := 1; i < l;  {
			current := queue[0]
			queue = queue[1:]

			if data[i] != "\000" {
				(*current).Left = DefaultTreeNode()
				(*current).Left.Data = data[i]
				queue = append(queue, (*current).Left)
			}
			i++

			if data[i] != "\000" {
				(*current).Right = DefaultTreeNode()
				(*current).Right.Data = data[i]
				queue = append(queue, (*current).Right)
			}
			i++
		}
	}

	return t
}

func DefaultTree() *Tree {
	return &Tree{Root: nil}
}

type TraversalFunctor func(*TreeNode)bool
type Traversal int8
const (
	Breadth 				Traversal = 0
	DepthPreOrder 						= 1	// Parent, Left, Right
	DepthInOrder							= 2 // Left, Parent, Right
	DepthPostOrder						= 3 // Left, Right, Parent
)

func (t Tree) BreadthFirstTraversal(functor TraversalFunctor) {
	if t.Root == nil {
		return
	}

	var queue []*TreeNode
	queue = append(queue, t.Root)

	for {
		if len(queue) == 0 {
			break
		}

		current := queue[0]
		queue = queue[1:]

		if functor(current) {
			return
		}

		if (*current).Left != nil {
			queue = append(queue, (*current).Left)
		}

		if (*current).Right != nil {
			queue = append(queue, (*current).Right)
		}
	}
}

func (t Tree) DepthFirstPreOrderTraversal(functor TraversalFunctor) {
	if t.Root == nil {
		return
	}
	
	var stack []*TreeNode
	var current *TreeNode = t.Root

	for {
		if len(stack) == 0 && current == nil {
			break
		}

		if current == nil {
			current = stack[len(stack) - 1]
			stack = stack[:len(stack) - 1]
		} else {
			if functor(current) {
				return
			}

			if (*current).Right != nil {
				stack = append(stack, (*current).Right)
			}

			current = (*current).Left
		}
	}
}

func (t Tree) DepthFirstInOrderTraversal(functor TraversalFunctor) {
	if t.Root == nil {
		return
	}
	
	var stack []*TreeNode
	var current *TreeNode = t.Root

	for {
		if len(stack) == 0 && current == nil {
			break
		}

		if current == nil {
			current = stack[len(stack) - 1]
			stack = stack[:len(stack) - 1]

			if functor(current) {
				return
			}

			current = (*current).Right
		} else {
			stack = append(stack, current)
			current = (*current).Left
		}
	}
}

func (t Tree) DepthFirstPostOrderTraversal(functor TraversalFunctor) {
	if t.Root == nil {
		return
	}
	
	var stack []*TreeNode
	var current *TreeNode = t.Root
	var lastVisited *TreeNode = nil

	for {
		if len(stack) == 0 && current == nil {
			break
		}

		if current == nil {
			peek := stack[len(stack) - 1]
			if (*peek).Right != nil && (*peek).Right != lastVisited {
				current = (*peek).Right
			} else {
				if (functor(peek)) {
					return
				}
				lastVisited = peek
				current = nil
				stack = stack[:len(stack) - 1]
			}
		} else {
			stack = append(stack, current)
			current = (*current).Left
		}
	}
}

func (t Tree) Traverse(traversal Traversal, functor TraversalFunctor) {
	if t.Root == nil {
		return
	}

	switch traversal {
	case Breadth:
		t.BreadthFirstTraversal(functor)
		break
	case DepthPreOrder:
		t.DepthFirstPreOrderTraversal(functor)
		break
	case DepthInOrder:
		t.DepthFirstInOrderTraversal(functor)
		break
	case DepthPostOrder:
		t.DepthFirstPostOrderTraversal(functor)
		break
	}
}