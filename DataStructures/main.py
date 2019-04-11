from LinkedList import LinkedList
from BinarySearchTree import BinarySearchTree
from collections import deque
import numpy as np

arrival_times = []

with open("/Users/zacharygolla/dataStructures/arrival_times.txt", "r") as f:
  for line in f:
    arrival_times.append(int(line.strip()))

print(arrival_times)

"""
Array and Stack practice

array = []
print(array)
array.append(1)
array.append(2)
print(array)

array.pop()
print(array)
array.append(6)
print(array)
array.pop(1)
print(array)

append() - Add an element to the end of the list
extend() - Add all elements of a list to the another list
insert() - Insert an item at the defined index
remove() - Removes an item from the list
pop() - Removes and returns an element at the given index
clear() - Removes all items from the list
index() - Returns the index of the first matched item
count() - Returns the count of number of items passed as an argument
sort() - Sort items in a list in ascending order
reverse() - Reverse the order of items in the list
copy() - Returns a shallow copy of the list

"""
"""
Linked List Testing

#Create Linked List
my_list = LinkedList()

#Add elements to linked list
my_list.append(1)
my_list.append(2)
my_list.append(3)
my_list.append(4)

#Show linked list
my_list.display()

#Show the data in the 3rd node
print(my_list.get(2))

#erase the 2nd node
my_list.erase(1)

#display linked list again
my_list.display()
"""

"""
dequeue Practice

#NOTE A QUEUE CAN HAVE INSERTION IN ONE END AND EXTRACTION FROM ANOTHER
#A QUEUE CAN HAVE INSERTION AND EXTRACTION FROM BOTH ENDS

que = deque('ghi')

print(que)

que.append('z')
que.append('y')

print(que)

que.appendleft('u')

print(que)

que.pop
"""

"""
#Fill tree with 100 elements whos number is randomly between 0 and 1000
def fill_tree(tree, num_elems = 100, max_int = 1000):
    from random import randint
    for _ in range(num_elems):
        current_element = randint(0,max_int)
        tree.insert(current_element)
    return tree


tree = BinarySearchTree()
tree = fill_tree(tree)

tree.print_tree()

tree.insert(5)
tree.insert(1)

tree.print_tree()

print(tree.height())

print(tree.search(5))
print(tree.search(3))
print(tree.search(1))
"""


