//*****************************
// Jacob Saunders
//*****************************

#include <iostream>
#include "json.hpp"

#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()),
	max_size_(max_size),
	size_(0) {
}

void PriorityQueue::insert(Key k) {
	insert(std::make_pair(k, std::make_pair(0, 0)));
}

void PriorityQueue::insert(KeyValuePair kv) {
    //checks if queue is full
    if (size_ != max_size_)
    {
    //increases size
    size_++;
    //adds new node
    nodes_[size_] = kv;
    //calls heapify up function
    heapifyUp(size_);
    }
}

KeyValuePair PriorityQueue::min() {
	//Returns the root node
    return nodes_[1];
}

KeyValuePair PriorityQueue::removeMin() {
	//Saves root node in Min
    KeyValuePair Min = min();
    //calls remove on root
    removeNode(1);
    //returns the Min node
    return Min;
}

bool PriorityQueue::isEmpty() const {
	// Returns true if size is zero
    return (size_ == 0);
}

size_t PriorityQueue::size() const {
	// Returns the size
    return size_;
}

nlohmann::json PriorityQueue::JSON() const {
	nlohmann::json result;
  for (size_t i = 1; i <= size_; i++) {
      nlohmann::json node;
      KeyValuePair kv = nodes_[i];
      node["key"] = kv.first;
      node["value"] = kv.second;
      if (i != ROOT) {
          node["parent"] = std::to_string(i / 2);
      }
      if (2 * i <= size_) {
          node["leftChild"] = std::to_string(2 * i);
      }
      if (2 * i + 1 <= size_) {
          node["rightChild"] = std::to_string(2 * i + 1);
      }
      result[std::to_string(i)] = node;
  }
  result["metadata"]["max_size"] = max_size_;
  result["metadata"]["size"] = size_;
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {
	// Creates temp
    KeyValuePair temp;
    //exits function if target is root
    if (i == 1)
    {
        return;
    }
    //compares node to parent
    if(getKey(i) < getKey(i/2))
    {
        //swaps if parents is greater then child node
        temp = nodes_[i/2];
        nodes_[i/2] = nodes_[i];
        nodes_[i] = temp;
        //calls heapify up on parent node
        heapifyUp(i/2);
    }
    return;
}

void PriorityQueue::heapifyDown(size_t i) {
	// creates temp
    KeyValuePair temp;
    //checks if there are 2 children
    if(2*i <= size_-1)
    {
        //finds smaller child
        if (getKey(2*i) < getKey((2*i)+1))
        {
            //compares child to parent
            if(getKey(i) > getKey(2*i))
            {
                //swaps child and parent
                temp = nodes_[i];
                nodes_[i] = nodes_[2*i];
                nodes_[2*i] = temp;
                //calls heapify down on child
                heapifyDown(2*i);
            }
        }else 
        {
            //compares child to parent
            if(getKey(i) > getKey((2*i)+1))
            {
                //swaps child and parent
                temp = nodes_[i];
                nodes_[i] = nodes_[(2*i)+1];
                nodes_[(2*i)+1] = temp;
                //calls heapify down on child
                heapifyDown((2*i)+1);
            }
        }
    }else if(2*i == size_) // checks if there is one child
    {
        //compares child to parent
        if(getKey(i) > getKey(2*i))
        {
            //swaps child and parent
            temp = nodes_[i];
            nodes_[i] = nodes_[2*i];
            nodes_[2*i] = temp;
            //calls heapify down on child
            heapifyDown(2*i);
        }
    }
    
    
    return;
}

void PriorityQueue::removeNode(size_t i) {
	// sets target node to last node
    nodes_[i] = nodes_[size_];
    //decreases size
    size_--;
    //calls heapifty down on target node
    heapifyDown(i);
}

Key PriorityQueue::getKey(size_t i) {
	// returns the key of the target node
    return nodes_[i].first;
}
