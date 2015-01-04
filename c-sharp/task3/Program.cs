using System.Collections;
using System.Text;
using System;
using System.Reflection;
using System.Diagnostics;

namespace RedBlackCS {
  public class Program {
    static void Main() {
      return;
    }
  }

  public class RedBlack : object {
    // the number of nodes contained in the tree
    private int intCount;

    // a simple randomized hash code. The hash code could be used as a key
    // if it is "unique" enough. Note: The IComparable interface would need to 
    // be replaced with int.
    private int intHashCode;

    // identifies the owner of the tree
    private string strIdentifier;

    // tree root
    private RedBlackNode rbTree;

    //  sentinelNode is convenient way of indicating a leaf node.
    public static RedBlackNode sentinelNode;

    // the node that was last found; used to optimize searches
    private RedBlackNode lastNodeFound;

    private Random rand = new Random();

    public RedBlack() {
      strIdentifier = base.ToString() + rand.Next();
      intHashCode = rand.Next();

      sentinelNode = new RedBlackNode();
      sentinelNode.Left = null;
      sentinelNode.Right = null;
      sentinelNode.Parent = null;
      sentinelNode.Color = RedBlackNode.BLACK;

      rbTree = sentinelNode;
      lastNodeFound = sentinelNode;
    }

    public RedBlack(string strIdentifier) {
      intHashCode = rand.Next();
      this.strIdentifier = strIdentifier;
    }

    public void Add(IComparable key, object data) {
      if (key == null || data == null)
        throw (new RedBlackException("RedBlackNode key and data must not be null"));

      int result = 0;

      RedBlackNode node = new RedBlackNode();
      RedBlackNode temp = rbTree;

      // find Parent
      while (temp != sentinelNode) {
        node.Parent = temp;
        result = key.CompareTo(temp.Key);
        if (result == 0)
          throw (new RedBlackException("A Node with the same key already exists"));
        if (result > 0)
          temp = temp.Right;
        else
          temp = temp.Left;
      }

      node.Key = key;
      node.Data = data;
      node.Left = sentinelNode;
      node.Right = sentinelNode;

      // insert node into tree starting at parent's location
      if (node.Parent != null) {
        result = node.Key.CompareTo(node.Parent.Key);
        if (result > 0)
          node.Parent.Right = node;
        else
          node.Parent.Left = node;
      } else
        rbTree = node;
      
      RestoreAfterInsert(node);
      lastNodeFound = node;
      intCount = intCount + 1;
    }

    // Additions to red-black trees usually destroy the red-black 
    // properties. Examine the tree and restore. Rotations are normally 
    // required to restore it.
    private void RestoreAfterInsert(RedBlackNode x) {
      // x and y are used as variable names for brevity, in a more formal
      // implementation, you should probably change the names
      RedBlackNode y;

      // maintain red-black tree properties after adding x
      while (x != rbTree && x.Parent.Color == RedBlackNode.RED) {
        // Parent node is .Colored red;
        // determine traversal path
        // is it on the Left or Right subtree?
        if (x.Parent == x.Parent.Parent.Left) {
          y = x.Parent.Parent.Right; // get uncle

          // // uncle is red; change x's Parent and uncle to black
          if (y != null && y.Color == RedBlackNode.RED) {
            x.Parent.Color = RedBlackNode.BLACK;
            y.Color = RedBlackNode.BLACK;

            // grandparent must be red. Why? Every red node that is not 
            // a leaf has only black children 
            x.Parent.Parent.Color = RedBlackNode.RED;
            x = x.Parent.Parent; // continue loop with grandparent
          } else {
            // uncle is black; determine if x is greater than Parent
            if (x == x.Parent.Right) {
              // yes, x is greater than Parent; rotate Left
              // make x a Left child
              x = x.Parent;
              RotateLeft(x);
            }

            // no, x is less than Parent
            x.Parent.Color = RedBlackNode.BLACK; // make Parent black
            x.Parent.Parent.Color = RedBlackNode.RED;	// make grandparent black
            RotateRight(x.Parent.Parent); // rotate right
          }
        } else { // x's Parent is on the Right subtree
          // this code is the same as above with "Left" and "Right" swapped
          y = x.Parent.Parent.Left;

          if (y != null && y.Color == RedBlackNode.RED) {
            x.Parent.Color = RedBlackNode.BLACK;
            y.Color = RedBlackNode.BLACK;
            x.Parent.Parent.Color = RedBlackNode.RED;
            x = x.Parent.Parent;
          } else {
            if (x == x.Parent.Left) {
              x = x.Parent;
              RotateRight(x);
            }

            x.Parent.Color = RedBlackNode.BLACK;
            x.Parent.Parent.Color = RedBlackNode.RED;
            RotateLeft(x.Parent.Parent);
          }
        }
      }

      rbTree.Color = RedBlackNode.BLACK; // rbTree should always be black
    }

    // Rebalance the tree by rotating the nodes to the left
    public void RotateLeft(RedBlackNode x) {
      // pushing node x down and to the Left to balance the tree. x's Right child (y)
      // replaces x (since y > x), and y's Left child becomes x's Right child 
      // (since it's < y but > x).
      
      RedBlackNode y = x.Right; // get x's Right node, this becomes y

      // set x's Right link
      x.Right = y.Left; // y's Left child's becomes x's Right child
      
      // modify parents
      if (y.Left != sentinelNode)
        y.Left.Parent = x; // sets y's Left Parent to x

      if (y != sentinelNode)
        y.Parent = x.Parent; // set y's Parent to x's Parent

      if (x.Parent != null) {	// determine which side of it's Parent x was on
        if (x == x.Parent.Left)
          x.Parent.Left = y; // set Left Parent to y
        else
          x.Parent.Right = y; // set Right Parent to y
      } else
        rbTree = y; // at rbTree, set it to y

      // link x and y 
      y.Left = x; // put x on y's Left 
      if (x != sentinelNode) // set y as x's Parent
        x.Parent = y;
    }

    // Rebalance the tree by rotating the nodes to the right
    public void RotateRight(RedBlackNode x) {
      // pushing node x down and to the Right to balance the tree. x's Left child (y)
      // replaces x (since x < y), and y's Right child becomes x's Left child 
      // (since it's < x but > y).
      RedBlackNode y = x.Left; // get x's Left node, this becomes y

      // set x's Right link
      x.Left = y.Right; // y's Right child becomes x's Left child

      // modify parents
      if (y.Right != sentinelNode)
        y.Right.Parent = x; // sets y's Right Parent to x

      if (y != sentinelNode)
        y.Parent = x.Parent; // set y's Parent to x's Parent

      if (x.Parent != null) // null=rbTree, could also have used rbTree			{	// determine which side of it's Parent x was on
        if (x == x.Parent.Right)
          x.Parent.Right = y; // set Right Parent to y
        else
          x.Parent.Left = y; // set Left Parent to y
      } else
        rbTree = y; // at rbTree, set it to y

      // link x and y 
      y.Right = x; // put x on y's Right
      if (x != sentinelNode) // set y as x's Parent
        x.Parent = y;
    }

    // Gets the data object associated with the specified key
    public object GetData(IComparable key) {
      int result;

      RedBlackNode treeNode = rbTree; // begin at root

      // traverse tree until node is found
      while (treeNode != sentinelNode) {
        result = key.CompareTo(treeNode.Key);
        if (result == 0) {
          lastNodeFound = treeNode;
          return treeNode.Data;
        }
        if (result < 0)
          treeNode = treeNode.Left;
        else
          treeNode = treeNode.Right;
      }

      throw (new RedBlackException("RedBlackNode key was not found"));

    }

    // return an enumerator that returns the tree nodes in order
    public RedBlackEnumerator GetEnumerator() {
      // elements is simply a generic name to refer to the 
      // data objects the nodes contain
      return Elements(true);

    }

    // if(ascending is true, the keys will be returned in ascending order, else
    // the keys will be returned in descending order.
    public RedBlackEnumerator Keys() {
      return Keys(true);
    }

    public RedBlackEnumerator Keys(bool ascending) {
      return new RedBlackEnumerator(rbTree, true, ascending);
    }

    public RedBlackEnumerator Values() {
      return Elements(true);
    }

    // Returns an enumeration of the data objects.
    // if(ascending is true, the objects will be returned in ascending order,
    // else the objects will be returned in descending order.
    public RedBlackEnumerator Elements() {
      return Elements(true);
    }

    public RedBlackEnumerator Elements(bool ascending) {
      return new RedBlackEnumerator(rbTree, false, ascending);
    }

    public bool IsEmpty() {
      return (rbTree == null);
    }

    public void Remove(IComparable key) {
      if (key == null)
        throw (new RedBlackException("RedBlackNode key is null"));

      // find node
      int result;
      RedBlackNode node;

      // see if node to be deleted was the last one found
      result = key.CompareTo(lastNodeFound.Key);

      if (result == 0)
        node = lastNodeFound;
      else { // not found, must search		
        node = rbTree;

        while (node != sentinelNode) {
          result = key.CompareTo(node.Key);
          if (result == 0)
            break;
          if (result < 0)
            node = node.Left;
          else
            node = node.Right;
        }

        if (node == sentinelNode)
          return; // key not found
      }

      Delete(node);
      intCount = intCount - 1;
    }

    // Delete a node from the tree and restore red black properties
    private void Delete(RedBlackNode z) {
      // A node to be deleted will be: 
      //  1. a leaf with no children
      //  2. have one child
      //  3. have two children
      // If the deleted node is red, the red black properties still hold.
      // If the deleted node is black, the tree needs rebalancing

      RedBlackNode x = new RedBlackNode(); // work node to contain the replacement node
      RedBlackNode y; // work node 

      // find the replacement node (the successor to x) - the node one with 
      // at *most* one child. 
      if (z.Left == sentinelNode || z.Right == sentinelNode)
        y = z; // node has sentinel as a child
      else {
        // z has two children, find replacement node which will 
        // be the leftmost node greater than z
        y = z.Right; // traverse right subtree	
        while (y.Left != sentinelNode) // to find next node in sequence
          y = y.Left;
      }

      // at this point, y contains the replacement node. it's content will be copied 
      // to the valules in the node to be deleted

      // x (y's only child) is the node that will be linked to y's old parent. 
      if (y.Left != sentinelNode)
        x = y.Left;
      else
        x = y.Right;

      // replace x's parent with y's parent and
      // link x to proper subtree in parent
      // this removes y from the chain
      x.Parent = y.Parent;

      if (y.Parent != null)
        if (y == y.Parent.Left)
          y.Parent.Left = x;
        else
          y.Parent.Right = x;
      else
        rbTree = x; // make x the root node

      // copy the values from y (the replacement node) to the node being deleted.
      // note: this effectively deletes the node. 
      if (y != z) {
        z.Key = y.Key;
        z.Data = y.Data;
      }

      if (y.Color == RedBlackNode.BLACK)
        RestoreAfterDelete(x);

      lastNodeFound = sentinelNode;
    }

    // Deletions from red-black trees may destroy the red-black 
    // properties. Examine the tree and restore. Rotations are normally 
    // required to restore it
    private void RestoreAfterDelete(RedBlackNode x) {
      RedBlackNode y;
      while (x != rbTree && x.Color == RedBlackNode.BLACK) {
        if (x == x.Parent.Left) // determine sub tree from parent				{
          y = x.Parent.Right; // y is x's sibling 
          if (y.Color == RedBlackNode.RED) { // x is black, y is red - make both black and rotate
            y.Color = RedBlackNode.BLACK;
            x.Parent.Color = RedBlackNode.RED;
            RotateLeft(x.Parent);
            y = x.Parent.Right;
          }
          if (y.Left.Color == RedBlackNode.BLACK &&
            y.Right.Color == RedBlackNode.BLACK) { // children are both black
            y.Color = RedBlackNode.RED; // change parent to red
            x = x.Parent; // move up the tree
          } else {
            if (y.Right.Color == RedBlackNode.BLACK) {
              y.Left.Color = RedBlackNode.BLACK;
              y.Color = RedBlackNode.RED;
              RotateRight(y);
              y = x.Parent.Right;
            }
            y.Color = x.Parent.Color;
            x.Parent.Color = RedBlackNode.BLACK;
            y.Right.Color = RedBlackNode.BLACK;
            RotateLeft(x.Parent);
            x = rbTree;
          }
        } else { // right subtree - same as code above with right and left swapped
          y = x.Parent.Left;
          if (y.Color == RedBlackNode.RED) {
            y.Color = RedBlackNode.BLACK;
            x.Parent.Color = RedBlackNode.RED;
            RotateRight(x.Parent);
            y = x.Parent.Left;
          }
          if (y.Right.Color == RedBlackNode.BLACK &&
            y.Left.Color == RedBlackNode.BLACK) {
            y.Color = RedBlackNode.RED;
            x = x.Parent;
          } else {
            if (y.Left.Color == RedBlackNode.BLACK) {
              y.Right.Color = RedBlackNode.BLACK;
              y.Color = RedBlackNode.RED;
              RotateLeft(y);
              y = x.Parent.Left;
            }
            y.Color = x.Parent.Color;
            x.Parent.Color = RedBlackNode.BLACK;
            y.Left.Color = RedBlackNode.BLACK;
            RotateRight(x.Parent);
            x = rbTree;
          }
        }
      }
      x.Color = RedBlackNode.BLACK;
    }

    public void Clear() {
      rbTree = sentinelNode;
      intCount = 0;
    }

    public int Size() {
      return intCount;
    }

    public override int GetHashCode() {
      return intHashCode;
    }

    public override string ToString() {
      return strIdentifier.ToString();
    }
  }

  public class RedBlackNode {
    public static int RED = 0;
    public static int BLACK = 1;

    // key provided by the calling class
    private IComparable ordKey;

    // the data or value associated with the key
    private object objData;

    // color - used to balance the tree
    private int intColor;

    private RedBlackNode rbnLeft;
    private RedBlackNode rbnRight;
    private RedBlackNode rbnParent;

    public IComparable Key {
      get {
        return ordKey;
      }
      set {
        ordKey = value;
      }
    }

    public object Data {
      get {
        return objData;
      }
      set {
        objData = value;
      }
    }

    public int Color {
      get {
        return intColor;
      }
      set {
        intColor = value;
      }
    }

    public RedBlackNode Left {
      get {
        return rbnLeft;
      }
      set {
        rbnLeft = value;
      }
    }

    public RedBlackNode Right {
      get {
        return rbnRight;
      }
      set {
        rbnRight = value;
      }
    }

    public RedBlackNode Parent {
      get {
        return rbnParent;
      }
      set {
        rbnParent = value;
      }
    }

    public RedBlackNode() {
      Color = RED;
    }
  }

  public class RedBlackException : Exception {
    public RedBlackException() { }
    public RedBlackException(string msg) : base(msg) { }
  }

  public class RedBlackEnumerator {
    // the treap uses the stack to order the nodes
    private Stack stack;

    // return the keys
    private bool keys;

    // return in ascending order (true) or descending (false)
    private bool ascending;

    // key
    private IComparable ordKey;

    // the data or value associated with the key
    private object objValue;

    public string Color; // testing only, don't use in live system
    public IComparable parentKey; // testing only, don't use in live system

    public IComparable Key {
      get {
        return ordKey;
      }
      set {
        ordKey = value;
      }
    }

    public object Value {
      get {
        return objValue;
      }
      set {
        objValue = value;
      }
    }

    public RedBlackEnumerator() {
    }

    // Determine order, walk the tree and push the nodes onto the stack
    public RedBlackEnumerator(RedBlackNode tnode, bool keys, bool ascending) {
      stack = new Stack();
      this.keys = keys;
      this.ascending = ascending;

      // use depth-first traversal to push nodes into stack
      // the lowest node will be at the top of the stack
      if (ascending) { // find the lowest node
        while (tnode != RedBlack.sentinelNode) {
          stack.Push(tnode);
          tnode = tnode.Left;
        }
      } else {
        // the highest node will be at top of stack
        while (tnode != RedBlack.sentinelNode) {
          stack.Push(tnode);
          tnode = tnode.Right;
        }
      }
    }

    public bool HasMoreElements() {
      return (stack.Count > 0);
    }

    public object NextElement() {
      if (stack.Count == 0)
        throw (new RedBlackException("Element not found"));

      // the top of stack will always have the next item
      // get top of stack but don't remove it as the next nodes in sequence
      // may be pushed onto the top
      // the stack will be popped after all the nodes have been returned
      RedBlackNode node = (RedBlackNode)stack.Peek();	//next node in sequence

      if (ascending) {
        if (node.Right == RedBlack.sentinelNode) {
          // yes, top node is lowest node in subtree - pop node off stack 
          RedBlackNode tn = (RedBlackNode)stack.Pop();

          // peek at right node's parent 
          // get rid of it if it has already been used
          while (HasMoreElements() && ((RedBlackNode)stack.Peek()).Right == tn)
            tn = (RedBlackNode)stack.Pop();
        } else {
          // find the next items in the sequence
          // traverse to left; find lowest and push onto stack
          RedBlackNode tn = node.Right;

          while (tn != RedBlack.sentinelNode) {
            stack.Push(tn);
            tn = tn.Left;
          }
        }
      } else { // descending, same comments as above apply
        if (node.Left == RedBlack.sentinelNode) {
          // walk the tree
          RedBlackNode tn = (RedBlackNode)stack.Pop();
          while (HasMoreElements() && ((RedBlackNode)stack.Peek()).Left == tn)
            tn = (RedBlackNode)stack.Pop();
        } else {
          // determine next node in sequence
          // traverse to left subtree and find greatest node - push onto stack
          RedBlackNode tn = node.Left;
          while (tn != RedBlack.sentinelNode) {
            stack.Push(tn);
            tn = tn.Right;
          }
        }
      }

      Key = node.Key;
      Value = node.Data;

      return keys == true ? node.Key : node.Data;
    }

    public bool MoveNext() {
      if (HasMoreElements()) {
        NextElement();
        return true;
      }
      return false;
    }
  }
}