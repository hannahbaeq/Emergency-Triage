/****************************************************************
 ** name: hannah baeq
 ** sect: 4
 ** date: 2023 november 9
 ** mail: hbaeq1@umbc.edu
 ** desc: this file contains the tester class and implementation.
 ****************************************************************/
#include "pqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;
// common strings made global constants for easier call
const string ERR =  " Error Case - ";
const string EDGE = "  Edge Case - ";
const string NORM = "Normal Case - ";
const string FAIL = "\033[1;31mFAILED\033[0m";
const string PASS = "\033[1;32mPASSED\033[0m";
const string MSG = "at(): ";
// priority function definitions
int priorityFn1(const Patient & patient);
int priorityFn2(const Patient & patient);
// a name database for testing purposes
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
    "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
    "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green",
    "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong",
    "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
    "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};

// imported random class
enum RANDOM{UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random{
public:
  Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type){
    if(type == NORMAL){
      m_generator = std::mt19937(m_device());
      m_normdist = std::normal_distribution<>(mean,stdev);
    }else if(type == UNIFORMINT){
      m_generator = std::mt19937(10);
      m_unidist = std::uniform_int_distribution<>(min,max);
    }else if(type == UNIFORMREAL){
      m_generator = std::mt19937(10);
      m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
    }else{
      m_generator = std::mt19937(m_device());
    }
  }
  void setSeed(int seedNum){
    m_generator = std::mt19937(seedNum);
  }
  void getShuffle(vector<int> & array){
    for(int i = m_min; i<=m_max; i++){
      array.push_back(i);
    }
    shuffle(array.begin(),array.end(),m_generator);
  }
  void getShuffle(int array[]){
    vector<int> temp;
    for(int i = m_min; i<=m_max; i++){
      temp.push_back(i);
    }
    std::shuffle(temp.begin(), temp.end(), m_generator);
    vector<int>::iterator it;
    int i = 0;
    for(it=temp.begin(); it != temp.end(); it++){
      array[i] = *it;
      i++;
    }
  }
  int getRandNum(){
    int result = 0;
    if(m_type == NORMAL){
      result = m_min - 1;
      while(result < m_min || result > m_max)
        result = m_normdist(m_generator);
    }else if (m_type == UNIFORMINT){
      result = m_unidist(m_generator);
    }
    return result;
  }
  double getRealRandNum(){
    double result = m_uniReal(m_generator);
    result = std::floor(result*100.0)/100.0;
    return result;
  }
private:
  int m_min;
  int m_max;
  RANDOM m_type;
  std::random_device m_device;
  std::mt19937 m_generator;
  std::normal_distribution<> m_normdist;
  std::uniform_int_distribution<> m_unidist;
  std::uniform_real_distribution<double> m_uniReal;
};

class Tester{
public:
  ///////////////////helper functions////////////////////
  // name: checkSkew
  // desc: recursively checks the min/max heap properties
  bool checkHeap(PQueue &heap, Node *node){
    bool result = true;
    if(node != nullptr){
      int prior = heap.m_priorFunc(node->m_patient);
      int priorL, priorR;
      result = result && ((checkHeap(heap, node->m_left)) && (checkHeap(heap, node->m_right)));
      // calculates priority functions --> conditionals prevent seg faults
      if((node->m_left != nullptr) && (node->m_right != nullptr)){
        priorL = heap.m_priorFunc(node->m_left->m_patient);
        priorR = heap.m_priorFunc(node->m_right->m_patient);
      }else if((node->m_left != nullptr) && (node->m_right == nullptr)){
        priorL = heap.m_priorFunc(node->m_left->m_patient);
      }else if((node->m_left == nullptr) && (node->m_right != nullptr)){
        priorR = heap.m_priorFunc(node->m_right->m_patient);
      }
      if(heap.m_heapType == MINHEAP){
        // checks for minheap properties
        if((node->m_left != nullptr) && (node->m_right != nullptr)){
          result = result && ((prior <= priorL) && (prior <= priorR));
        }else if((node->m_left != nullptr) && (node->m_right == nullptr)){
          result = result && (prior <= priorL);
        }else if((node->m_left == nullptr) && (node->m_right != nullptr)){
          result = result && (prior <= priorR);
        }
      }else{
        // checks for maxheap properties
        if((node->m_left != nullptr) && (node->m_right != nullptr)){
          result = result && ((prior >= priorL) && (prior >= priorR));
        }else if((node->m_left != nullptr) && (node->m_right == nullptr)){
          result = result && (prior >= priorL);
        }else if((node->m_left == nullptr) && (node->m_right != nullptr)){
          result = result && (prior >= priorR);
        }
      }
    }
    return result;
  }

  // name: checkLeft
  // desc: recursively checks leftist heap properties
  bool checkLeft(PQueue &heap, Node *node){
    bool result = true;
    if(node != nullptr){
      int npl = node->m_npl;
      int nplL = 0;
      int nplR = 0;
      result = result && ((checkLeft(heap, node->m_left)) && (checkLeft(heap, node->m_right)));
      // pulling npl values --> conditionals ensure no seg faults occur
      if((node->m_left != nullptr) && (node->m_right != nullptr)){
        nplL = node->m_left->m_npl;
        nplR = node->m_right->m_npl;
      }else if((node->m_left != nullptr) && (node->m_right == nullptr)){
        nplL = node->m_left->m_npl;
      }else if((node->m_left == nullptr) && (node->m_right != nullptr)){
        nplR = node->m_right->m_npl;
      }
      // checks whether or not the npl value is correct
      if((node->m_left != nullptr) && (node->m_right != nullptr)){
        result = result && (npl == (min(nplL, nplR) + 1));
      }else if((node->m_left == nullptr) || (node->m_right == nullptr)){
        result = result && (npl == 0);
      }
      // checks whether ot not the leftnpl >= rightnpl
      result = result && (nplL >= nplR);
    }
    return result;
  }


  // name: equal
  // desc: recursively checks whether or not each node is
  //       equal
  bool equal(Node *lhs, Node *rhs){
    bool result = true;
    if((lhs != nullptr) && (rhs != nullptr)){
      // call on left and right --> dont need to check if left and right
      // children are the same (the recursive call does that)
      result = result && equal(lhs->m_left, rhs->m_left);
      result = result && equal(lhs->m_right, rhs->m_right);
      // same patient
      result = result && (lhs->m_patient == rhs->m_patient);
    }else if(((lhs == nullptr) && (rhs != nullptr)) || ((lhs != nullptr) && (rhs == nullptr)))
      result = false;
    // if only one of the nodes are nullptr
    return result;
  }

  // name: insert min skew
  // case: norm || inserting 300 nodes into a min skew heap
  // desc: should maintain min heap properties
  bool insertMinSkew(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, SKEW);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    result = checkHeap(aQueue, aQueue.m_heap);
    return result;
  }

  // name: insert min skew edge
  // case: edge || inserting patient with borderline stats into
  //       a min skew heap
  // desc: should insert node
  bool insertMinSkewEdge(){
    bool result = true;
    PQueue aQueue(priorityFn2, MINHEAP, SKEW);
    for(int i = 0; i < 1; i++){
      // all patient stats are on the edge
      Patient patient("Hannah Baeq", MAXTEMP, MINOX, MINRR, MAXBP, 1);
      aQueue.insertPatient(patient);
    }
    if(aQueue.m_heap == nullptr){
      result = false;
    }
    return result;
  }

  // name: insert min skew error
  // case: error || inserting an empty patient
  // desc: should not insert node
  bool insertMinSkewErr(){
    bool result = true;
    PQueue aQueue(priorityFn2, MINHEAP, SKEW);
    for(int i = 0; i < 1; i++){
      // patient stat is past the allowed range (probably dead)
      Patient patient("Hannah Baeq", MAXTEMP+1, MINOX, MINRR, MAXBP, 1);
      aQueue.insertPatient(patient);
    }
    if(aQueue.m_heap != nullptr){
      result = false;
    }
    return result;
  }

  // name: insert max skew
  // case: norm || inserting 300 nodes into a max skew heap
  // desc: should maintain max heap properties
  bool insertMaxSkew(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, SKEW);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    result = checkHeap(aQueue, aQueue.m_heap);
    return result;
  }

  // name: insert max skew edge
  // case: edge || inserting patient with borderline stats into
  //       a max skew heap
  // desc: should insert node
  bool insertMaxSkewEdge(){
    bool result = true;
    PQueue aQueue(priorityFn1, MAXHEAP, SKEW);
    for(int i = 0; i < 1; i++){
      // all patient stats are on the edge
      Patient patient("Hannah Baeq", MINTEMP, MAXOX, MAXRR, MINBP, 10);
      aQueue.insertPatient(patient);
    }
    if(aQueue.m_heap == nullptr){
      result = false;
    }
    return result;
  }

  // name: insert max skew error
  // case: error || inserting an empty patient
  // desc: should not insert node
  bool insertMaxSkewErr(){
    bool result = true;
    PQueue aQueue(priorityFn1, MAXHEAP, SKEW);
    for(int i = 0; i < 1; i++){
      // patient stats are past allowed values (probably dead again)
      Patient patient("Hannah Baeq", MINTEMP-1, MAXOX, MAXRR, MINBP, 10);
      aQueue.insertPatient(patient);
    }
    if(aQueue.m_heap != nullptr){
      result = false;
    }
    return result;
  }

  // name: insert min leftist
  // case: norm || inserting 300 nodes into a leftist min heap
  // desc: should maintain min heap & leftist heap properties
  bool insertMinLeft(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    // checking that min/max heap values apply & leftist properties
    result = checkHeap(aQueue, aQueue.m_heap);
    result = result && checkLeft(aQueue, aQueue.m_heap);
    return result;
  }

  // name: insert leftist min edge
  // case: edge || inserting patient with borderline stats into
  //       a leftist min heap
  // desc: should insert node
  bool insertMinLeftEdge(){
    bool result = true;
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for(int i = 0; i < 1; i++){
      Patient patient("Hannah Baeq", MAXTEMP, MINOX, MINRR, MAXBP, 1);
      aQueue.insertPatient(patient);
    }
    if(aQueue.m_heap == nullptr){
      result = false;
    }
    return result;
  }

  // name: insert leftist min error
  // case: error || inserting an empty patient
  // desc: should not insert node
  bool insertMinLeftErr(){
    bool result = true;
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for(int i = 0; i < 1; i++){
      Patient patient("Hannah Baeq", MAXTEMP+1, MINOX, MINRR, MAXBP, 1);
      aQueue.insertPatient(patient);
    }
    if(aQueue.m_heap != nullptr){
      result = false;
    }
    return result;
  }

  // name: insert max leftist
  // case: norm || inserting 300 nodes into a leftist max heap
  // desc: should maintain max heap & leftist heap properties
  bool insertMaxLeft(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    result = checkHeap(aQueue, aQueue.m_heap);
    result = result && checkLeft(aQueue, aQueue.m_heap);
    return result;
  }

  // name: insert max left edge
  // case: edge || inserting patient with borderline stats into
  //       a max leftist heap
  // desc: should insert node
  bool insertMaxLeftEdge(){
    bool result = true;
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 1; i++){
      Patient patient("Hannah Baeq", MINTEMP, MAXOX, MAXRR, MINBP, 10);
      aQueue.insertPatient(patient);
    }
    if(aQueue.m_heap == nullptr){
      result = false;
    }
    return result;
  }

  // name: insert max left error
  // case: error || inserting an empty patient
  // desc: should not insert node
  bool insertMaxLeftErr(){
    bool result = true;
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 1; i++){
      Patient patient("Hannah Baeq", MINTEMP-1, MAXOX, MAXRR, MINBP, 10);
      aQueue.insertPatient(patient);
    }
    if(aQueue.m_heap != nullptr){
      result = false;
    }
    return result;
  }

  // name: remove min skew
  // case: norm || removing 3 nodes from a min skew heap
  // desc: should maintain min heap properties || nodes with
  //       the highest priority should be removed first.
  bool removeMinSkew(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, SKEW);
    for(int i = 0; i < 10; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    // removing the 3 highest priority nodes
    Patient patient1 = aQueue.m_heap->m_patient;
    Patient patient1removed = aQueue.getNextPatient();
    result = result && (patient1 == patient1removed);
    Patient patient2 = aQueue.m_heap->m_patient;
    Patient patient2removed = aQueue.getNextPatient();
    result = result && (patient2 == patient2removed);
    Patient patient3 = aQueue.m_heap->m_patient;
    Patient patient3removed = aQueue.getNextPatient();
    int prior1 = aQueue.m_priorFunc(patient1);
    int prior2 = aQueue.m_priorFunc(patient2);
    int prior3 = aQueue.m_priorFunc(patient3);
    result = result && (patient3 == patient3removed);
    result = result && (prior1 <= prior2) && (prior2 <= prior3);
    result = result && checkHeap(aQueue, aQueue.m_heap);
    return result;
  }

  // name: remove min skew edge
  // case: edge || removing 300 nodes from a min skew heap
  // desc: heap should be empty at the end
  bool removeMinSkewEdge(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, SKEW);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    for(int i = 0; i < 300; i++){
      Patient patient = aQueue.getNextPatient();
      int prior = aQueue.m_priorFunc(patient);
      if(aQueue.m_heap != nullptr){
        int priorNext = aQueue.m_priorFunc(aQueue.m_heap->m_patient);
        result = result && (prior <= priorNext);
      }
    }
    if(aQueue.m_heap != nullptr){
      result = false;
    }
    return result;
  }

  // name: remove max skew
  // case: norm || removing 3 nodes from a max skew heap
  // desc: should maintain max heap properties || nodes with
  //       the highest priority should be removed first.
  bool removeMaxSkew(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, SKEW);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    // removing the 3 highest priority nodes
    Patient patient1 = aQueue.m_heap->m_patient;
    int prior1 = aQueue.m_priorFunc(aQueue.m_heap->m_patient);
    Patient patient1removed = aQueue.getNextPatient();
    result = result && (patient1 == patient1removed);
    Patient patient2 = aQueue.m_heap->m_patient;
    int prior2 = aQueue.m_priorFunc(aQueue.m_heap->m_patient);
    Patient patient2removed = aQueue.getNextPatient();
    result = result && (patient2 == patient2removed);
    Patient patient3 = aQueue.m_heap->m_patient;
    int prior3 = aQueue.m_priorFunc(aQueue.m_heap->m_patient);
    Patient patient3removed = aQueue.getNextPatient();
    result = result && (patient3 == patient3removed);
    result = result && (prior1 >= prior2) && (prior2 >= prior3);
    result = result && checkHeap(aQueue, aQueue.m_heap);
    return result;
  }

  // name: remove max skew edge
  // case: edge || removing 300 nodes from a max skew heap
  // desc: heap should be empty at the end
  bool removeMaxSkewEdge(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, SKEW);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    for(int i = 0; i < 300; i++){
      Patient patient = aQueue.getNextPatient();
      int prior = aQueue.m_priorFunc(patient);
      if(aQueue.m_heap != nullptr){
        int priorNext = aQueue.m_priorFunc(aQueue.m_heap->m_patient);
        result = result && (prior >= priorNext);
      }
    }
    if(aQueue.m_heap != nullptr){
      result = false;
    }
    return result;
  }

  // name: remove min left
  // case: norm || removing 3 nodes from a min left heap
  // desc: should maintain min heap properties || nodes with
  //       the highest priority should be removed first.
  bool removeMinLeft(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    // removing the 3 highest priority nodes
    Patient patient1 = aQueue.m_heap->m_patient;
    Patient patient1removed = aQueue.getNextPatient();
    result = result && (patient1 == patient1removed);
    Patient patient2 = aQueue.m_heap->m_patient;
    Patient patient2removed = aQueue.getNextPatient();
    result = result && (patient2 == patient2removed);
    Patient patient3 = aQueue.m_heap->m_patient;
    Patient patient3removed = aQueue.getNextPatient();
    result = result && (patient3 == patient3removed);
    int prior1 = aQueue.m_priorFunc(patient1);
    int prior2 = aQueue.m_priorFunc(patient2);
    int prior3 = aQueue.m_priorFunc(patient3);
    result = result && (prior1 <= prior2) && (prior2 <= prior3);
    result = result && checkHeap(aQueue, aQueue.m_heap);
    result = result && checkLeft(aQueue, aQueue.m_heap);
    return result;
  }

  // name: remove min left edge
  // case: edge || removing 300 nodes from a min left heap
  // desc: heap should be empty at the end
  bool removeMinLeftEdge(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    for(int i = 0; i < 300; i++){
      Patient patient = aQueue.getNextPatient();
      int prior = aQueue.m_priorFunc(patient);
      if(aQueue.m_heap != nullptr){
        int priorNext = aQueue.m_priorFunc(aQueue.m_heap->m_patient);
        result = result && (prior <= priorNext);
      }
    }
    if(aQueue.m_heap != nullptr){
      result = false;
    }
    return result;
  }

  // name: remove max left
  // case: norm || removing 3 nodes from a max left heap
  // desc: should maintain max heap properties || nodes with
  //       the highest priority should be removed first.
  bool removeMaxLeft(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    // removing the 3 highest priority nodes
    Patient patient1 = aQueue.m_heap->m_patient;
    Patient patient1removed = aQueue.getNextPatient();
    result = result && (patient1 == patient1removed);
    Patient patient2 = aQueue.m_heap->m_patient;
    Patient patient2removed = aQueue.getNextPatient();
    result = result && (patient2 == patient2removed);
    Patient patient3 = aQueue.m_heap->m_patient;
    Patient patient3removed = aQueue.getNextPatient();
    result = result && (patient3 == patient3removed);
    int prior1 = aQueue.m_priorFunc(patient1);
    int prior2 = aQueue.m_priorFunc(patient2);
    int prior3 = aQueue.m_priorFunc(patient3);
    result = result && (prior1 >= prior2) && (prior2 >= prior3);
    result = result && checkHeap(aQueue, aQueue.m_heap);
    result = result && checkLeft(aQueue, aQueue.m_heap);
    return result;
  }

  // name: remove max left edge
  // case: edge || removing 300 nodes from a max left heap
  // desc: heap should be empty at the end
  bool removeMaxLeftEdge(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 300; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    for(int i = 0; i < 300; i++){
      Patient patient = aQueue.getNextPatient();
      int prior = aQueue.m_priorFunc(patient);
      if(aQueue.m_heap != nullptr){
        int priorNext = aQueue.m_priorFunc(aQueue.m_heap->m_patient);
        result = result && (prior >= priorNext);
      }
    }
    if(aQueue.m_heap != nullptr){
      result = false;
    }
    return result;
  }

  // name: remove error
  // case: error || removing from an empty queue
  // desc: out of range exception should be caught
  bool removeErr(){
    bool result = false;
    PQueue aQueue(priorityFn1, MAXHEAP, SKEW);
    try{
      aQueue.getNextPatient();
    }catch(const out_of_range& e){
      // if exception is caught
      result = true;
    }
    return result;
  }

  // name: min to max (priority function change)
  // case: norm || changing a min heap to max heap
  // desc: heap should change and pass tests
  bool minToMaxSkew(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, SKEW);
    for(int i = 0; i < 10; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    aQueue.setPriorityFn(priorityFn1, MAXHEAP);
    result = checkHeap(aQueue, aQueue.m_heap);
    return result;
  }

  // name: max to min (priority function change)
  // case: norm || changing a max heap to min heap
  // desc: heap should change and pass tests
  bool maxToMinLeft(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 10; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    aQueue.setPriorityFn(priorityFn2, MINHEAP);
    result = checkHeap(aQueue, aQueue.m_heap);
    result = result && checkLeft(aQueue, aQueue.m_heap);
    return result;
  }

  // name: error (priority function change)
  // case: error || changing a max heap to max heap
  // desc: heap should not change
  bool priorFnErr(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    PQueue bQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 10; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
      bQueue.insertPatient(patient);
    }
    aQueue.setPriorityFn(priorityFn1, MAXHEAP);
    result = (equal(aQueue.m_heap, bQueue.m_heap));
    return result;
  }

  // name: merge edge
  // case: edge || merging an empty rhs to a populated lhs
  // desc: nothing should change
  bool mergeEdge(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    PQueue bQueue(priorityFn1, MAXHEAP, LEFTIST);
    PQueue cQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 10; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
      bQueue.insertPatient(patient);
    }
    aQueue.mergeWithQueue(cQueue);
    result = equal(aQueue.m_heap, bQueue.m_heap);
    return result;
  }

  // name: merge error
  // case: error || merging queues with different priFn
  // desc: domain error should be caught
  bool mergeErr(){
    bool result = false;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    PQueue bQueue(priorityFn2, MAXHEAP, LEFTIST);
    for(int i = 0; i < 10; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    bQueue.insertPatient(aQueue.m_heap->m_patient);
    try{
      aQueue.mergeWithQueue(bQueue);
    }catch(const domain_error& e){
      result = true;
      // catch the exception
    }
    return result;
  }

  // name: copy norm
  // case: norm || rhs populated, lhs empty
  // desc: should create an exact deep copy when finished
  bool copyNorm(){
    bool result = false;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 10; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    PQueue bQueue(aQueue);
    result = equal(aQueue.m_heap, bQueue.m_heap);
    return result;
  }

  // name: copy edge
  // case: edge || rhs empty, lhs empty
  // desc: should both be empty
  bool copyEdge(){
    bool result = false;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    PQueue bQueue(aQueue);
    result = equal(aQueue.m_heap, bQueue.m_heap);
    return result;
  }

  // name: assignment norm
  // case: norm || lhs empty, rhs populated
  // desc: should create an exact deep copy
  bool assignNorm(){
    bool result = false;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 10; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    PQueue bQueue(priorityFn1, MAXHEAP, LEFTIST);
    bQueue = aQueue;
    result = equal(aQueue.m_heap, bQueue.m_heap);
    return result;
  }

  // name: assignment edge
  // case: edge || lhs populated, rhs populated
  // desc: should clear out lhs data then create an exact
  //       deep copy
  bool assignEdge(){
    bool result = false;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    PQueue bQueue(priorityFn1, MAXHEAP, LEFTIST);
    for(int i = 0; i < 20; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      if(i < 10)
        aQueue.insertPatient(patient);
      else
        bQueue.insertPatient(patient);
    }
    bQueue = aQueue;
    result = equal(aQueue.m_heap, bQueue.m_heap);
    return result;
  }

  // name: insert log
  // case: norm || seeing if inserting runs on log n amortized
  //       time
  // desc: inserting 1000 nodes and 2000 nodes and seeing the
  //        ratio difference
  bool insertLog(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    // member variables for 1000
    double T = 0.0;
    clock_t start, stop;
    PQueue aQueue(priorityFn1, MAXHEAP, SKEW);
    int teamSize = 1000;
    // member variables for 2000
    double T2 = 0.0;
    clock_t start2, stop2;
    PQueue bQueue(priorityFn1, MAXHEAP, SKEW);
    int teamSize2 = 2000;
    start = clock();
    for(int i = 0; i < teamSize; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    stop = clock();
    T = stop - start;
    start2 = clock();
    for(int j = 0; j < teamSize2; j++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      bQueue.insertPatient(patient);
    }
    stop2 = clock();
    T2 = stop2 - start2;
    // finding the ratios
    double ratio = 1000 * log2(1000);
    double expectedRatio = (2000 * log2(2000) / ratio);
    double myratio = T2 / T;
    if((myratio > expectedRatio + 0.4) || (myratio < expectedRatio - 0.4))
      result = false;
    return result;
  }

  // name: remove log
  // case: norm || seeing if removing runs on log n amortized
  //       time
  // desc: removing 1000 nodes and 2000 nodes and seeing the
  //        ratio difference
  bool removeLog(){
    bool result = true;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    // member variables for 1000
    double T = 0.0;
    clock_t start, stop;
    PQueue aQueue(priorityFn1, MAXHEAP, SKEW);
    int teamSize = 1000;
    for(int i = 0; i < teamSize; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      aQueue.insertPatient(patient);
    }
    // member variables for 2000
    double T2 = 0.0;
    clock_t start2, stop2;
    PQueue bQueue(priorityFn1, MAXHEAP, SKEW);
    int teamSize2 = 2000;
    for(int i = 0; i < teamSize2; i++){
      Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
      bQueue.insertPatient(patient);
    }
    start = clock();
    for(int i = 0; i < teamSize; i++){
      aQueue.getNextPatient();
    }
    stop = clock();
    T = stop - start;
    start2 = clock();
    for(int j = 0; j < teamSize2; j++){
      bQueue.getNextPatient();
    }
    stop2 = clock();
    T2 = stop2 - start2;
    // finding the ratios
    double ratio = 1000 * log2(1000);
    double expectedRatio = (2000 * log2(2000) / ratio);
    double myratio = T2 / T;
    if((myratio > expectedRatio + 0.4) || (myratio < expectedRatio - 0.4))
      result = false;
    return result;
  }
};

int main(){
  Tester tester;
  if(tester.insertMinSkew())
    cout << NORM << MSG << "Insert 300 for Min Skew Heap    :: " << PASS << endl;
  else
    cout << NORM << MSG << "Insert 300 for Min Skew Heap    :: " << FAIL << endl;

  if(tester.insertMinSkewEdge())
    cout << EDGE << MSG << "Insertion for Min Skew Heap     :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Insertion for Min Skew Heap     :: " << FAIL << endl;

  if(tester.insertMinSkewErr())
    cout << ERR << MSG << "Insert empty for Min Skew Heap  :: " << PASS << endl;
  else
    cout << ERR << MSG << "Insert empty for Min Skew Heap  :: " << FAIL << endl;

  if(tester.insertMaxSkew())
    cout << NORM << MSG << "Insert 300 for Max Skew Heap    :: " << PASS << endl;
  else
    cout << NORM << MSG << "Insert 300 for Max Skew Heap    :: " << FAIL << endl;

  if(tester.insertMaxSkewEdge())
    cout << EDGE << MSG << "Insertion for Max Skew Heap     :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Insertion for Max Skew Heap     :: " << FAIL << endl;

  if(tester.insertMaxSkewErr())
    cout << ERR << MSG << "Insert empty for Max Skew Heap  :: " << PASS << endl;
  else
    cout << ERR << MSG << "Insert empty for Max Skew Heap  :: " << FAIL << endl;

  if(tester.insertMinLeft())
    cout << NORM << MSG << "Insert 300 for Leftist Min Heap :: " << PASS << endl;
  else
    cout << NORM << MSG << "Insert 300 for Leftist Min Heap :: " << FAIL << endl;

  if(tester.insertMinLeftEdge())
    cout << EDGE << MSG << "Insert for Leftist Min Heap     :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Insert for Leftist Min Heap     :: " << FAIL << endl;

  if(tester.insertMinLeftErr())
    cout << ERR << MSG << "Insert empty for Left Min Heap  :: " << PASS << endl;
  else
    cout << ERR << MSG << "Insert empty for Left Min Heap  :: " << FAIL << endl;

  if(tester.insertMaxLeft())
    cout << NORM << MSG << "Insert 300 for Leftist Max Heap :: " << PASS << endl;
  else
    cout << NORM << MSG << "Insert 300 for Leftist Max Heap :: " << FAIL << endl;

  if(tester.insertMaxLeftEdge())
    cout << EDGE << MSG << "Insert for Leftist Max Heap     :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Insert for Leftist Max Heap     :: " << FAIL << endl;

  if(tester.insertMaxLeftErr())
    cout << ERR << MSG << "Insert empty for Max Left Heap  :: " << PASS << endl;
  else
    cout << ERR << MSG << "Insert empty for Max Left Heap  :: " << FAIL << endl;

  if(tester.removeMinSkew())
    cout << NORM << MSG << "Remove 3 for Min Skew Heap      :: " << PASS << endl;
  else
    cout << NORM << MSG << "Remove 3 for Min Skew Heap      :: " << FAIL << endl;

  if(tester.removeMinSkewEdge())
    cout << EDGE << MSG << "Remove all for Min Skew Heap    :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Remove all for Min Skew Heap    :: " << FAIL << endl;

  if(tester.removeMaxSkew())
    cout << NORM << MSG << "Remove 3 for Max Skew Heap      :: " << PASS << endl;
  else
    cout << NORM << MSG << "Remove 3 for Max Skew Heap      :: " << FAIL << endl;

  if(tester.removeMaxSkewEdge())
    cout << EDGE << MSG << "Remove all for Max Skew Heap    :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Remove all for Max Skew Heap    :: " << FAIL << endl;

  if(tester.removeMinLeft())
    cout << NORM << MSG << "Remove 3 for Min Left Heap      :: " << PASS << endl;
  else
    cout << NORM << MSG << "Remove 3 for Min Left Heap      :: " << FAIL << endl;

  if(tester.removeMinLeftEdge())
    cout << EDGE << MSG << "Remove all for Min Left Heap    :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Remove all for Min Left Heap    :: " << FAIL << endl;

  if(tester.removeMaxLeft())
    cout << NORM << MSG << "Remove 3 for Max Left Heap      :: " << PASS << endl;
  else
    cout << NORM << MSG << "Remove 3 for Max Left Heap      :: " << FAIL << endl;

  if(tester.removeMaxLeftEdge())
    cout << EDGE << MSG << "Remove all for Max Left Heap    :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Remove all for Max Left Heap    :: " << FAIL << endl;

  if(tester.removeErr())
    cout << ERR << MSG << "Remove from empty heap          :: " << PASS << endl;
  else
    cout << ERR << MSG << "Remove from empty heap          :: " << FAIL << endl;

  if(tester.minToMaxSkew())
    cout << NORM << MSG << "PriFn min to max (skew)         :: " << PASS << endl;
  else
    cout << NORM << MSG << "PriFn min to max (skew)         :: " << FAIL << endl;

  if(tester.maxToMinLeft())
    cout << NORM << MSG << "PriFn max to min (left)         :: " << PASS << endl;
  else
    cout << NORM << MSG << "PriFn max to min (left)         :: " << FAIL << endl;

  if(tester.priorFnErr())
    cout << ERR << MSG << "PriFn max to max (left)         :: " << PASS << endl;
  else
    cout << ERR << MSG << "PriFn max to max (left)         :: " << FAIL << endl;

  if(tester.mergeEdge())
    cout << EDGE << MSG << "Merge lhs & empty rhs           :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Merge lhs & empty rhs           :: " << FAIL << endl;

  if(tester.mergeErr())
    cout << ERR << MSG << "Merge queues with diff priFn    :: " << PASS << endl;
  else
    cout << ERR << MSG << "Merge queues with diff priFn    :: " << FAIL << endl;

  if(tester.copyNorm())
    cout << NORM << MSG << "Copy Constructor                :: " << PASS << endl;
  else
    cout << NORM << MSG << "Copy Constructor                :: " << FAIL << endl;

  if(tester.copyEdge())
    cout << EDGE << MSG << "Copy Constructor rhs empty      :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Copy Constructor rhs empty      :: " << FAIL << endl;

  if(tester.assignNorm())
    cout << NORM << MSG << "Assignment Operator             :: " << PASS << endl;
  else
    cout << NORM << MSG << "Assignment Operator             :: " << FAIL << endl;

  if(tester.assignEdge())
    cout << EDGE << MSG << "Assign lhs & rhs populated      :: " << PASS << endl;
  else
    cout << EDGE << MSG << "Assign lhs & rhs populated      :: " << FAIL << endl;

  if(tester.insertLog())
    cout << NORM << MSG << "Running time O(log n) insert    :: " << PASS << endl;
  else
    cout << NORM << MSG << "Running time O(log n) insert    :: " << FAIL << endl;

  if(tester.removeLog())
    cout << NORM << MSG << "Running time O(log n) remove    :: " << PASS << endl;
  else
    cout << NORM << MSG << "Running time O(log n) rmeove    :: " << FAIL << endl;
  return 0;
}


// priority function definitions
int priorityFn1(const Patient & patient){
  int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
  return priority;
}
int priorityFn2(const Patient & patient){
  int priority = patient.getOpinion() + patient.getOxygen();
  return priority;
}
