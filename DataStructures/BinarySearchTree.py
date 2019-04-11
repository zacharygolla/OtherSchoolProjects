class Node:
    def __init__(self, data=None):
        self.data = data
        self.left_child = None
        self.right_child = None


class BinarySearchTree:
    def __init__(self):
        self.root = None

    def insert(self, data):
        if self.root is None:
            self.root = Node(data)
        else:
            self._insert(data, self.root)

    def _insert(self, data, cur_node):
        if data < cur_node.data:
            if cur_node.left_child is None:
                cur_node.left_child = Node(data)
            else:
                self._insert(data, cur_node.left_child)
        else:
            if cur_node.right_child is None:
                cur_node.right_child = Node(data)
            else:
                self._insert(data, cur_node.right_child)

    def print_tree(self):
        if self.root is not None:
            self._print_tree(self.root)

    def _print_tree(self, current_node):
        if current_node is not None:
            self._print_tree(current_node.left_child)
            print(current_node.data)
            self._print_tree(current_node.right_child)

    def height(self):
        if self.root is not None:
            return self._height(self.root, 0)
        else:
            return 0

    def _height(self, current_node, current_height):
        if current_node is None:
            return current_height
        left_height = self._height(current_node.left_child, current_height + 1)
        right_height = self._height(current_node.right_child, current_height + 1)
        return max(left_height, right_height)

    def search(self, data):
        if self.root != None:
            return self._search(data, self.root)
        else:
            return False

    def _search(self, data, cur_node):
        if data == cur_node.data:
            return True
        elif data < cur_node.data and cur_node.left_child != None:
            return self._search(data, cur_node.left_child)
        elif data > cur_node.data and cur_node.right_child != None:
            return self._search(data, cur_node.right_child)
        return False

    def find(self, value):
        if self.root is not None:
            return self._find(value, self.root)
        else:
            return None

    def _find(self, value, cur_node):
        if value == cur_node.value:
            return cur_node
        elif value < cur_node.value and cur_node.left_child is not None:
            return self._find(value, cur_node.left_child)
        elif value > cur_node.value and cur_node.right_child is not None:
            return self._find(value, cur_node.right_child)

    def delete_value(self, value):
        return self.delete_node(self.find(value))

    def delete_node(self, node):
        # Protect against deleting a node not found in the tree
        if node is None or self.find(node.value) is None:
            print("Node to be deleted not found in the tree!")
            return None

        # returns the node with min value in tree rooted at input node
        def min_value_node(n):
            current = n
            while current.left_child is not None:
                current = current.left_child
            return current

        # returns the number of children for the specified node
        def num_children(n):
            number_of_children = 0
            if n.left_child is not None: number_of_children += 1
            if n.right_child is not None: number_of_children += 1
            return number_of_children

        # get the parent of the node to be deleted
        node_parent = node.parent

        # get the number of children of the node to be deleted
        node_children = num_children(node)

        # break operation into different cases based on the
        # structure of the tree & node to be deleted

        # CASE 1 (node has no children)
        if node_children == 0:

            # Added this if statement post-video, previously if you
            # deleted the root node it would delete entire tree.
            if node_parent is not None:
                # remove reference to the node from the parent
                if node_parent.left_child is node:
                    node_parent.left_child = None
                else:
                    node_parent.right_child = None
            else:
                self.root = None

        # CASE 2 (node has a single child)
        if node_children is 1:

            # get the single child node
            if node.left_child is not None:
                child = node.left_child
            else:
                child = node.right_child

            # Added this if statement post-video, previously if you
            # deleted the root node it would delete entire tree.
            if node_parent is not None:
                # replace the node to be deleted with its child
                if node_parent.left_child is node:
                    node_parent.left_child = child
                else:
                    node_parent.right_child = child
            else:
                self.root = child

            # correct the parent pointer in node
            child.parent = node_parent

        # CASE 3 (node has two children)
        if node_children is 2:
            # get the in order successor of the deleted node
            successor = min_value_node(node.right_child)

            # copy the in order successor's value to the node formerly
            # holding the value we wished to delete
            node.value = successor.value

            # delete the in order successor now that it's value was
            # copied into the other node
            self.delete_node(successor)