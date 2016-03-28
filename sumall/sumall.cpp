#include <message.h>
#include <cassert>
#include <cstdio>
#include "sumall.h"
#include <chrono>

// This is a sample solution to the "Sum all integers" problem. Each node sums
// the elements that belong to it (that is, the ones with position equal to
// MyNodeId() modulo NumberOfNodes()).
//
// To showcase the communication a bit better, instead of sending all the
// results to a "master" node, each node sends its result to the next one,
// accumulating the result from the previous node. The last node prints the
// final result.
int main() {
  using namespace std::chrono;

  high_resolution_clock::time_point startTime = high_resolution_clock::now();

  long long sum = 0LL;

  for (long long pos = MyNodeId(); pos < GetN(); pos += NumberOfNodes()) {
    sum += GetNumber(pos);
  }

  int masterNodeId = 0;

  if (MyNodeId() == masterNodeId) {
      // masterNodeId is working as master
      // All nodes will send sum to master
      for (int nodeId = 0; nodeId < NumberOfNodes(); nodeId++) {
          if(nodeId != masterNodeId) {
            Receive(nodeId);
            sum += GetLL(nodeId);
          }
      }
  }

  if (MyNodeId() != masterNodeId) {
    PutLL(masterNodeId, sum);
    Send(masterNodeId);
  } else {
    printf("%lld\n", sum);
  }

  high_resolution_clock::time_point endTime = high_resolution_clock::now();
  printf("Node: %d, StartTime: %d, endTime: %d, TimeTaken: %f ms\n", MyNodeId(), startTime, endTime, 1000*std::chrono::duration_cast<duration<double>>(endTime - startTime));

  return 0;
}
