from math import log10

class DoublyLinkedList:
    def __init__(self, num=0, prev=None, next=None):
        self.num = num
        self.prev = prev
        self.next = next
    
    def blink(self):
        if self.num == 0:
            self.num = 1
        else:
            num_of_digits = int(log10(self.num)) + 1
            if num_of_digits & 1:
                self.num *= 2024
            else:
                left_stone_num, right_stone_num = divmod(self.num, pow(10, num_of_digits >> 1))
                left_stone = DoublyLinkedList(left_stone_num, self.prev)
                self.prev.next = left_stone
                right_stone = DoublyLinkedList(right_stone_num, left_stone, self.next)
                self.next.prev = right_stone
                left_stone.next = right_stone

# Init
input_stones = [125, 17]
dummy_head = DoublyLinkedList()
dummy_tail = DoublyLinkedList(prev=dummy_head)
dummy_head.next = dummy_tail
prev = dummy_head
for stone in input_stones:
    curr = DoublyLinkedList(stone, prev, prev.next)
    prev.next = curr
    prev = curr

# Simulation
for i in range(25):
    curr = dummy_head.next
    while curr is not dummy_tail:
        curr.blink()
        curr = curr.next

# Counting stones
curr = dummy_head.next
stone_count = 0
while curr is not dummy_tail:
    curr = curr.next
    stone_count += 1
print(stone_count)
