/*********************************************************************
 ** name: hannah baeq
 ** date: 10/31/2023
 ** desc: this file contains the function definitions for the pqueue
 **       class defined in pqueue.h
 ********************************************************************/
#include "pqueue.h"

// name: constructor
// pre: none
// post: pqueue member variables initialized
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure){
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
}

// name: destructor
// pre: memory allocated
// post: deallocated memory and re-initializes member variables
PQueue::~PQueue(){
  // recursive function that deletes the entire heap
  deleteHeap(m_heap);
  m_heap = nullptr;
  m_size = 0;
}

// name: clear
// pre: memory allocated
// post: queue is cleared, memory deallocated, and member variables
//       re-initialized
void PQueue::clear(){
  // make sure that the pqueue object is not already empty
  if(m_heap != nullptr){
    deleteHeap(m_heap);
  }
  m_heap = nullptr;
  m_size = 0;
}

// name: copy constructor
// pre: rhs memory allocated
// post: deep copy of rhs in *this
PQueue::PQueue(const PQueue& rhs){
  // copy member variables
  m_heap = equal(rhs.m_heap);
  // recursive function that creates a deep copy
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;
}

// name: assignment operator
// pre: rhs memory allocated
// post: deep copy of rhs in *this
// note: cannot call copy constructor
PQueue& PQueue::operator=(const PQueue& rhs){
  if(this != &rhs){
    // protect against self assignment
    if(m_heap != nullptr){
      // deallocate object if memory already exists
      clear();
    }
    // copy member variables
    m_heap = equal(rhs.m_heap);
    // recursive function that creates a deep copy
    m_size = rhs.m_size;
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
  }
  // return object
  return *this;
}

// name: merge
// pre: rhs & lhs member variables initialized
// post: merges rhs and lhs, rhs emptied after merged
// note:
//    - must have the same priority function & data structure
//    - if different, throw domain_error
//    - protection against self merging
//    - recursive function!
void PQueue::mergeWithQueue(PQueue& rhs){
  if((m_heap != nullptr) && (rhs.m_heap != nullptr)){
    // heap1 and heap2 are both non-empty
    if((rhs.getHeapType() == m_heapType)
       && (rhs.getStructure() == m_structure)
       && (rhs.getPriorityFn() == m_priorFunc)){
      // if the priority function & data structures all match
      int p1Prior = m_priorFunc(m_heap->m_patient);
      int p2Prior = m_priorFunc(rhs.m_heap->m_patient);
      // find out priority to switch m_heap pointers as
      // necessary
      m_size = m_size + rhs.m_size;
      if(m_heapType == MINHEAP){
        if(m_structure == SKEW){
          // min skew merge
          minMerge(m_heap, rhs.m_heap);
        }else{
          // min leftist merge
          minLeft(m_heap, rhs.m_heap);
        }
        if(p1Prior > p2Prior){
          Node *temp = m_heap;
          m_heap = rhs.m_heap;
          rhs.m_heap = temp;
        }
        // if the roots are swapped, change heap pointers
      }else{
        if(m_structure == SKEW){
          // max skew merge
          maxMerge(m_heap, rhs.m_heap);
        }else{
          // max leftist merge
          maxLeft(m_heap, rhs.m_heap);
        }
        if(p1Prior < p2Prior){
          Node *temp = m_heap;
          m_heap = rhs.m_heap;
          rhs.m_heap = temp;
        }
        // if the roots were swapped, change heap pointers
      }
    }else{
      throw domain_error("Incompatible heap/structure/priority types ():ERROR");
    }
  }else if((m_heap == nullptr) && (rhs.m_heap != nullptr)){
    // if heap1 is empty but heap2 is not
    m_heap = rhs.m_heap;
    m_size++;
  }
  rhs.m_heap = nullptr;
}

// name: insert
// pre: member variables initialized
// post: patient inserted into queue
// note: must maintain heap property types (min, max, leftist)
void PQueue::insertPatient(const Patient& patient){
  if(patient.m_patient == ""){
    // if patient object is empty, return
    return;
  }else{
    // call merge
    PQueue aqueue(m_priorFunc, m_heapType, m_structure);
    Node *newNode = new Node(patient);
    aqueue.m_heap = newNode;
    aqueue.m_size++;
    mergeWithQueue(aqueue);
  }
}

// name: num
// pre: memory allocated
// post: returns the number of patients in the queue
int PQueue::numPatients()const{ return m_size; }

// name: get priority
// pre: object created and priority set
// post: returns the current priority function
prifn_t PQueue::getPriorityFn()const{ return m_priorFunc; }

// name: get next
// pre: memory allocated
// post: extracts and returns highest priority patient
// note: extract = remove, throws out_of_range exception if
//       queue is empty when called
Patient PQueue::getNextPatient(){
  if(m_heap == nullptr){
    // if heap is empty, throw error
    throw out_of_range("Queue is empty ():ERROR");
  }else{
    Node *left = m_heap->m_left;
    Node *right = m_heap->m_right;
    // hold on to patient object that needs returning
    Patient returnVal =  m_heap->m_patient;
    delete m_heap;
    // delete / extract
    m_heap = left;
    PQueue aqueue(m_priorFunc, m_heapType, m_structure);
    aqueue.m_heap = right;
    mergeWithQueue(aqueue);
    // merge the two subheaps together
    m_size--;
    // decrement size
    return returnVal;
  }
}

// name: set priority
// pre: memory allocated
// post: sets a new priority function and heaptype
// note: rebuilds the heap
void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType){
  if(m_heap != nullptr){
    if((priFn == m_priorFunc) && (heapType == m_heapType)){
      // protect againt changing into same priFn/heap
      return;
    }
    // hold onto heap in temp heap
    PQueue aqueue(m_priorFunc, m_heapType, m_structure);
    aqueue = *this;
    // clear out heap
    clear();
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_structure = aqueue.m_structure;
    // call helper function
    resetHelper(aqueue.m_heap);
  }
}

// name: set structure
// pre: memory allocated
// post: rebuilds heap based on new structure type given
//       using current data structure
void PQueue::setStructure(STRUCTURE structure){
  if(m_heap != nullptr){
    if(structure == m_structure){
      // protect against changing into same structure
      return;
    }
    PQueue aqueue(m_priorFunc, m_heapType, m_structure);
    aqueue = *this;
    clear();
    // hold onto heap in temp heap and clear
    m_priorFunc = aqueue.m_priorFunc;
    m_heapType = aqueue.m_heapType;
    m_structure = structure;
    // call helper function
    resetHelper(aqueue.m_heap);
  }
}

// name: get structure
// pre: memory allocated
// post: returns the structure type
STRUCTURE PQueue::getStructure()const{ return m_structure; }

// name: get heap
// pre: memory allocated
// post: returns heap type
HEAPTYPE PQueue::getHeapType()const{ return m_heapType; }

// name: print queue
// pre: memory allocated
// post: prints patients using pre-order traversal
void PQueue::printPatientQueue()const{
  if(m_heap != nullptr){
    // if heap is empty, cannot print
    print(m_heap);
  }
}

///////////////////helper functions//////////////////////
// name: reset helper
// pre: set structure called
// post: recursively reconstructs the heap based on the
//       new structure type
void PQueue::resetHelper(Node *root){
  if(root != nullptr){
    // reconstruct using inorder traversal
    resetHelper(root->m_left);
    // call insert
    insertPatient(root->m_patient);
    resetHelper(root->m_right);
  }
}

// name: print(Node *root)
// pre: print patient queue is called
// post: prints patients using pre-order traversal
void PQueue::print(Node *root)const{
  if(root != nullptr){
    // calculate priority function
    int num = m_priorFunc(root->m_patient);
    cout << "[" << num << "] " << root->m_patient.getPatient()
         << ", temperature: " << root->m_patient.getTemperature()
         << ", oxygen: " << root->m_patient.getOxygen()
         << ", respiratory: " << root->m_patient.getRR()
         << ", blood pressure: " << root->m_patient.getBP()
         << ", nurse opinion: " << root->m_patient.getOpinion()
         << endl;
    // pre order traversal
    print(root->m_left);
    print(root->m_right);
  }
}

// name: equal(Node *rhs)
// pre: equal operator is called
// post: a recursive function that will create an exact
//       deep copy
Node* PQueue::equal(Node *rhs){
  if(rhs != nullptr){
    // creates new node to make a deep copy
    Node *newNode = new Node(rhs->getPatient());
    // copy member variables
    newNode->setNPL(rhs->getNPL());
    // preorder traversal (really cant work any other way)
    newNode->m_left = equal(rhs->m_left);
    newNode->m_right = equal(rhs->m_right);
    return newNode;
  }
  return nullptr;
}

// name: delete heap
// pre: memory allocated
// post: a recurive function that deallocated memory
void PQueue::deleteHeap(Node *pos){
  if(pos != nullptr){
    // deletes heap bottom up
    deleteHeap(pos->m_left);
    deleteHeap(pos->m_right);
    // delete the node
    delete pos;
    pos = nullptr;
  }
}

// name: min merge
// pre: merge function called
// post: will recursively merge two nodes/heaps of
//       skew structure
void PQueue::minMerge(Node *p1, Node *p2){
  if(p1 != nullptr){
    bool flag = true;
    int p1Prior = m_priorFunc(p1->m_patient);
    int p2Prior = m_priorFunc(p2->m_patient);
    // find out priority
    if(p1Prior > p2Prior){
      Node *curr = p1;
      p1 = p2;
      p2 = curr;
    }
    Node *temp = p1->m_left;
    p1->m_left = p1->m_right;
    p1->m_right = temp;
    // swapping by hand :3
    if(p1->m_left != nullptr){
      // this will stop seg faults in the case that left
      // == nullptr
      if(m_priorFunc(p1->m_left->m_patient)
         > m_priorFunc(p2->m_patient)){
        // if p1 and p2 are going to swap on the next
        // recursive call, flag
        flag = false;
      }
      minMerge(p1->m_left, p2);
    }
    // recursive call
    if(flag == false){
      // if p1 and p2 swapped in the recursive call, change
      // the pointers of the 'parent'
      p1->m_left = p2;
    }
    if(p1->m_left == nullptr){
      // if the left was nullptr, swap
      p1->m_left = p2;
    }
  }
}

// name: min leftist
// pre: merge function called
// post: will recursively merge two nodes/heaps of
//       leftist strucure
void PQueue::minLeft(Node *p1, Node *p2){
  if(p1 != nullptr){
    bool flag = true;
    // calculate priority function
    int p1Prior = m_priorFunc(p1->m_patient);
    int p2Prior = m_priorFunc(p2->m_patient);
    // if p1 does not have greater priority, swap
    if(p1Prior > p2Prior){
      Node *curr = p1;
      p1 = p2;
      p2 = curr;
    }
    if(p1->m_right != nullptr){
      if(m_priorFunc(p1->m_right->m_patient)
         > m_priorFunc(p2->m_patient)){
        // flag if children will swap
        flag = false;
      }
      // recursive call
      minLeft(p1->m_right, p2);
    }
    if(flag == false){
      // if flag, then swap children
      Node *temp = p1->m_right;
      p1->m_right = p2;
      p2 = temp;
    }
    if(p1->m_right == nullptr){
      // if right is empty, p2 becomes right child
      p1->m_right = p2;
    }
    if((p1->m_left != nullptr) && (p1->m_right != nullptr)){
      // npl only changes if both left and right are not empty
      p1->m_npl = min(p1->m_left->m_npl, p1->m_right->m_npl) + 1;
      // if the left npl value is < right npl, swap children
      if(p1->m_left->m_npl < p1->m_right->m_npl){
        Node *temp = p1->m_left;
        p1->m_left = p1->m_right;
        p1->m_right = temp;
      }
    }else if((p1->m_left == nullptr) && (p1->m_right != nullptr)){
      // if there is a right child but not a left child, swap
      Node *temp = p1->m_left;
      p1->m_left = p1->m_right;
      p1->m_right = temp;
    }
  }
}

// name: max merge
// pre: merge function called
// post: will recursively merge two max skew heaps
void PQueue::maxMerge(Node *p1, Node *p2){
  // same as minMerge except conditionals change!!
  if(p1 != nullptr){
    bool flag = true;
    int p1Prior = m_priorFunc(p1->m_patient);
    int p2Prior = m_priorFunc(p2->m_patient);
    // find out priority
    if(p1Prior < p2Prior){
      Node *curr = p1;
      p1 = p2;
      p2 = curr;
    }
    Node *temp = p1->m_left;
    p1->m_left = p1->m_right;
    p1->m_right = temp;
    // swapping by hand just in case swap doesn't change
    // the pointers too :3
    if(p1->m_left != nullptr){
      // this will stop seg faults in the case that left
      // == nullptr
      if(m_priorFunc(p1->m_left->m_patient)
         < m_priorFunc(p2->m_patient)){
        // if p1 and p2 are going to swap on the next
        // recursive call, flag
        flag = false;
      }
      maxMerge(p1->m_left, p2);
    }
    // recursive call
    if(flag == false){
      // if p1 and p2 swapped in the recursive call, change
      // the pointers of the 'parent'
      p1->m_left = p2;
    }
    if(p1->m_left == nullptr){
      // if the left was nullptr, swap
      p1->m_left = p2;
    }
  }
}

// name: max leftist
// pre: merge function called
// post: will recursively merge two nodes/heaps of
//       leftist strucure
void PQueue::maxLeft(Node *p1, Node *p2){
  // the exact same comments at min left but they got
  // repetitive
  if(p1 != nullptr){
    bool flag = true;
    int p1Prior = m_priorFunc(p1->m_patient);
    int p2Prior = m_priorFunc(p2->m_patient);
    if(p1Prior < p2Prior){
      Node *curr = p1;
      p1 = p2;
      p2 = curr;
    }
    if(p1->m_right != nullptr){
      if(m_priorFunc(p1->m_right->m_patient)
         < m_priorFunc(p2->m_patient)){
        flag = false;
      }
      maxLeft(p1->m_right, p2);
    }
    if(flag == false){
      Node *temp = p1->m_right;
      p1->m_right = p2;
      p2 = temp;
    }
    if(p1->m_right == nullptr){
      p1->m_right = p2;
    }
    if((p1->m_left != nullptr) && (p1->m_right != nullptr)){
     p1->m_npl = min(p1->m_left->m_npl, p1->m_right->m_npl) + 1;
      if(p1->m_left->m_npl < p1->m_right->m_npl){
        Node *temp = p1->m_left;
        p1->m_left = p1->m_right;
        p1->m_right = temp;
      }
    }else if((p1->m_left == nullptr) && (p1->m_right != nullptr)){
      Node *temp = p1->m_left;
      p1->m_left = p1->m_right;
      p1->m_right = temp;
    }
  }
}

////////////////////given functions//////////////////////
void PQueue::dump()const{
  if(m_size == 0){
    cout << "Empty heap.\n" ;
  }else{
    dump(m_heap);
  }
  cout << endl;
}
void PQueue::dump(Node *pos) const {
  if(pos != nullptr){
    cout << "(";
    dump(pos->m_left);
    if(m_structure == SKEW)
      cout << m_priorFunc(pos->m_patient) << ":"
           << pos->m_patient.getPatient();
    else
      cout << m_priorFunc(pos->m_patient) << ":"
           << pos->m_patient.getPatient() << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}
ostream& operator<<(ostream& sout, const Patient& patient){
  sout << patient.getPatient()
       << ", temperature: " << patient.getTemperature()
       << ", oxygen: " << patient.getOxygen()
       << ", respiratory: " << patient.getRR()
       << ", blood pressure: " << patient.getBP()
       << ", nurse opinion: " << patient.getOpinion();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node){
  sout << node.getPatient();
  return sout;
}
