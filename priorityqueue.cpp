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
	// TODO: complete this function
    if (size_ != max_size_)
    {
    size_++;
    nodes_[size_] = kv;
    heapifyUp(size_);
    }
}

KeyValuePair PriorityQueue::min() {
	// TODO: complete this function
    return nodes_[1];
}

KeyValuePair PriorityQueue::removeMin() {
	// TODO: complete this function
    KeyValuePair Min = min();
    removeNode(1);
    return Min;
}

bool PriorityQueue::isEmpty() const {
	// TODO: complete this function
    return (size_ == 1);
}

size_t PriorityQueue::size() const {
	// TODO: complete this function
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
	// TODO: complete this function
    KeyValuePair temp;
    if (i == 1)
    {
        return;
    }
    if(getKey(i) < getKey(i/2))
    {
        temp = nodes_[i/2];
        nodes_[i/2] = nodes_[i];
        nodes_[i] = temp;
        heapifyUp(i/2);
    }
    return;
}

void PriorityQueue::heapifyDown(size_t i) {
	// TODO: complete this function
    KeyValuePair temp;

    if(2*i <= size_-1)
    {
        if (getKey(2*i) < getKey((2*i)+1))
        {
            if(getKey(i) > getKey(2*i))
            {
                temp = nodes_[i];
                nodes_[i] = nodes_[2*i];
                nodes_[2*i] = temp;
                heapifyDown(2*i);
            }
        }else 
        {
            if(getKey(i) > getKey((2*i)+1))
            {
                temp = nodes_[i];
                nodes_[i] = nodes_[(2*i)+1];
                nodes_[(2*i)+1] = temp;
                heapifyDown((2*i)+1);
            }
        }
    }else if(2*i == size_)
    {
        if(getKey(i) > getKey(2*i))
        {
            temp = nodes_[i];
            nodes_[i] = nodes_[2*i];
            nodes_[2*i] = temp;
            heapifyDown(2*i);
        }
    }
    
    
    return;
}

void PriorityQueue::removeNode(size_t i) {
	// TODO: complete this function
    nodes_[i] = nodes_[size_];
    size_--;
    heapifyDown(i);
}

Key PriorityQueue::getKey(size_t i) {
	// TODO: complete this function
    return nodes_[i].first;
}
