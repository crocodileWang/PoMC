//
// Generated file, do not edit! Created by nedtool 5.4 from node/Packet.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include "Packet_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace {
template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)(static_cast<const omnetpp::cObject *>(t));
}

template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && !std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)dynamic_cast<const void *>(t);
}

template <class T> inline
typename std::enable_if<!std::is_polymorphic<T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)static_cast<const void *>(t);
}

}


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule to generate operator<< for shared_ptr<T>
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const std::shared_ptr<T>& t) { return out << t.get(); }

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');

    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(Packet)

Packet::Packet(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

Packet::Packet(const Packet& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

Packet::~Packet()
{
    delete [] this->transactionNodeID;
    delete [] this->transactionIdMsg;
    delete [] this->transactionTime;
    delete [] this->transactionVal;
    delete [] this->neighbours;
    delete [] this->vote;
}

Packet& Packet::operator=(const Packet& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Packet::copy(const Packet& other)
{
    this->srcAddr = other.srcAddr;
    this->destAddr = other.destAddr;
    this->hopCount = other.hopCount;
    this->packetType = other.packetType;
    this->nodeID = other.nodeID;
    this->posX = other.posX;
    this->posY = other.posY;
    this->transactionValue = other.transactionValue;
    this->time = other.time;
    this->userXID = other.userXID;
    this->senatorAddress = other.senatorAddress;
    this->transactionSourceAddress = other.transactionSourceAddress;
    this->TimeOfCreateTransaction = other.TimeOfCreateTransaction;
    this->transactionID = other.transactionID;
    delete [] this->transactionNodeID;
    this->transactionNodeID = (other.transactionNodeID_arraysize==0) ? nullptr : new int[other.transactionNodeID_arraysize];
    transactionNodeID_arraysize = other.transactionNodeID_arraysize;
    for (size_t i = 0; i < transactionNodeID_arraysize; i++) {
        this->transactionNodeID[i] = other.transactionNodeID[i];
    }
    delete [] this->transactionIdMsg;
    this->transactionIdMsg = (other.transactionIdMsg_arraysize==0) ? nullptr : new int[other.transactionIdMsg_arraysize];
    transactionIdMsg_arraysize = other.transactionIdMsg_arraysize;
    for (size_t i = 0; i < transactionIdMsg_arraysize; i++) {
        this->transactionIdMsg[i] = other.transactionIdMsg[i];
    }
    delete [] this->transactionTime;
    this->transactionTime = (other.transactionTime_arraysize==0) ? nullptr : new omnetpp::simtime_t[other.transactionTime_arraysize];
    transactionTime_arraysize = other.transactionTime_arraysize;
    for (size_t i = 0; i < transactionTime_arraysize; i++) {
        this->transactionTime[i] = other.transactionTime[i];
    }
    delete [] this->transactionVal;
    this->transactionVal = (other.transactionVal_arraysize==0) ? nullptr : new int[other.transactionVal_arraysize];
    transactionVal_arraysize = other.transactionVal_arraysize;
    for (size_t i = 0; i < transactionVal_arraysize; i++) {
        this->transactionVal[i] = other.transactionVal[i];
    }
    this->blockID = other.blockID;
    this->preHash = other.preHash;
    this->MF = other.MF;
    this->candidateTime = other.candidateTime;
    delete [] this->neighbours;
    this->neighbours = (other.neighbours_arraysize==0) ? nullptr : new int[other.neighbours_arraysize];
    neighbours_arraysize = other.neighbours_arraysize;
    for (size_t i = 0; i < neighbours_arraysize; i++) {
        this->neighbours[i] = other.neighbours[i];
    }
    delete [] this->vote;
    this->vote = (other.vote_arraysize==0) ? nullptr : new bool[other.vote_arraysize];
    vote_arraysize = other.vote_arraysize;
    for (size_t i = 0; i < vote_arraysize; i++) {
        this->vote[i] = other.vote[i];
    }
}

void Packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->srcAddr);
    doParsimPacking(b,this->destAddr);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->packetType);
    doParsimPacking(b,this->nodeID);
    doParsimPacking(b,this->posX);
    doParsimPacking(b,this->posY);
    doParsimPacking(b,this->transactionValue);
    doParsimPacking(b,this->time);
    doParsimPacking(b,this->userXID);
    doParsimPacking(b,this->senatorAddress);
    doParsimPacking(b,this->transactionSourceAddress);
    doParsimPacking(b,this->TimeOfCreateTransaction);
    doParsimPacking(b,this->transactionID);
    b->pack(transactionNodeID_arraysize);
    doParsimArrayPacking(b,this->transactionNodeID,transactionNodeID_arraysize);
    b->pack(transactionIdMsg_arraysize);
    doParsimArrayPacking(b,this->transactionIdMsg,transactionIdMsg_arraysize);
    b->pack(transactionTime_arraysize);
    doParsimArrayPacking(b,this->transactionTime,transactionTime_arraysize);
    b->pack(transactionVal_arraysize);
    doParsimArrayPacking(b,this->transactionVal,transactionVal_arraysize);
    doParsimPacking(b,this->blockID);
    doParsimPacking(b,this->preHash);
    doParsimPacking(b,this->MF);
    doParsimPacking(b,this->candidateTime);
    b->pack(neighbours_arraysize);
    doParsimArrayPacking(b,this->neighbours,neighbours_arraysize);
    b->pack(vote_arraysize);
    doParsimArrayPacking(b,this->vote,vote_arraysize);
}

void Packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->srcAddr);
    doParsimUnpacking(b,this->destAddr);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->packetType);
    doParsimUnpacking(b,this->nodeID);
    doParsimUnpacking(b,this->posX);
    doParsimUnpacking(b,this->posY);
    doParsimUnpacking(b,this->transactionValue);
    doParsimUnpacking(b,this->time);
    doParsimUnpacking(b,this->userXID);
    doParsimUnpacking(b,this->senatorAddress);
    doParsimUnpacking(b,this->transactionSourceAddress);
    doParsimUnpacking(b,this->TimeOfCreateTransaction);
    doParsimUnpacking(b,this->transactionID);
    delete [] this->transactionNodeID;
    b->unpack(transactionNodeID_arraysize);
    if (transactionNodeID_arraysize == 0) {
        this->transactionNodeID = nullptr;
    } else {
        this->transactionNodeID = new int[transactionNodeID_arraysize];
        doParsimArrayUnpacking(b,this->transactionNodeID,transactionNodeID_arraysize);
    }
    delete [] this->transactionIdMsg;
    b->unpack(transactionIdMsg_arraysize);
    if (transactionIdMsg_arraysize == 0) {
        this->transactionIdMsg = nullptr;
    } else {
        this->transactionIdMsg = new int[transactionIdMsg_arraysize];
        doParsimArrayUnpacking(b,this->transactionIdMsg,transactionIdMsg_arraysize);
    }
    delete [] this->transactionTime;
    b->unpack(transactionTime_arraysize);
    if (transactionTime_arraysize == 0) {
        this->transactionTime = nullptr;
    } else {
        this->transactionTime = new omnetpp::simtime_t[transactionTime_arraysize];
        doParsimArrayUnpacking(b,this->transactionTime,transactionTime_arraysize);
    }
    delete [] this->transactionVal;
    b->unpack(transactionVal_arraysize);
    if (transactionVal_arraysize == 0) {
        this->transactionVal = nullptr;
    } else {
        this->transactionVal = new int[transactionVal_arraysize];
        doParsimArrayUnpacking(b,this->transactionVal,transactionVal_arraysize);
    }
    doParsimUnpacking(b,this->blockID);
    doParsimUnpacking(b,this->preHash);
    doParsimUnpacking(b,this->MF);
    doParsimUnpacking(b,this->candidateTime);
    delete [] this->neighbours;
    b->unpack(neighbours_arraysize);
    if (neighbours_arraysize == 0) {
        this->neighbours = nullptr;
    } else {
        this->neighbours = new int[neighbours_arraysize];
        doParsimArrayUnpacking(b,this->neighbours,neighbours_arraysize);
    }
    delete [] this->vote;
    b->unpack(vote_arraysize);
    if (vote_arraysize == 0) {
        this->vote = nullptr;
    } else {
        this->vote = new bool[vote_arraysize];
        doParsimArrayUnpacking(b,this->vote,vote_arraysize);
    }
}

int Packet::getSrcAddr() const
{
    return this->srcAddr;
}

void Packet::setSrcAddr(int srcAddr)
{
    this->srcAddr = srcAddr;
}

int Packet::getDestAddr() const
{
    return this->destAddr;
}

void Packet::setDestAddr(int destAddr)
{
    this->destAddr = destAddr;
}

int Packet::getHopCount() const
{
    return this->hopCount;
}

void Packet::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

int Packet::getPacketType() const
{
    return this->packetType;
}

void Packet::setPacketType(int packetType)
{
    this->packetType = packetType;
}

int Packet::getNodeID() const
{
    return this->nodeID;
}

void Packet::setNodeID(int nodeID)
{
    this->nodeID = nodeID;
}

int Packet::getPosX() const
{
    return this->posX;
}

void Packet::setPosX(int posX)
{
    this->posX = posX;
}

int Packet::getPosY() const
{
    return this->posY;
}

void Packet::setPosY(int posY)
{
    this->posY = posY;
}

int Packet::getTransactionValue() const
{
    return this->transactionValue;
}

void Packet::setTransactionValue(int transactionValue)
{
    this->transactionValue = transactionValue;
}

double Packet::getTime() const
{
    return this->time;
}

void Packet::setTime(double time)
{
    this->time = time;
}

int Packet::getUserXID() const
{
    return this->userXID;
}

void Packet::setUserXID(int userXID)
{
    this->userXID = userXID;
}

int Packet::getSenatorAddress() const
{
    return this->senatorAddress;
}

void Packet::setSenatorAddress(int senatorAddress)
{
    this->senatorAddress = senatorAddress;
}

int Packet::getTransactionSourceAddress() const
{
    return this->transactionSourceAddress;
}

void Packet::setTransactionSourceAddress(int transactionSourceAddress)
{
    this->transactionSourceAddress = transactionSourceAddress;
}

double Packet::getTimeOfCreateTransaction() const
{
    return this->TimeOfCreateTransaction;
}

void Packet::setTimeOfCreateTransaction(double TimeOfCreateTransaction)
{
    this->TimeOfCreateTransaction = TimeOfCreateTransaction;
}

int Packet::getTransactionID() const
{
    return this->transactionID;
}

void Packet::setTransactionID(int transactionID)
{
    this->transactionID = transactionID;
}

size_t Packet::getTransactionNodeIDArraySize() const
{
    return transactionNodeID_arraysize;
}

int Packet::getTransactionNodeID(size_t k) const
{
    if (k >= transactionNodeID_arraysize) throw omnetpp::cRuntimeError("Array of size transactionNodeID_arraysize indexed by %lu", (unsigned long)k);
    return this->transactionNodeID[k];
}

void Packet::setTransactionNodeIDArraySize(size_t newSize)
{
    int *transactionNodeID2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = transactionNodeID_arraysize < newSize ? transactionNodeID_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        transactionNodeID2[i] = this->transactionNodeID[i];
    for (size_t i = minSize; i < newSize; i++)
        transactionNodeID2[i] = 0;
    delete [] this->transactionNodeID;
    this->transactionNodeID = transactionNodeID2;
    transactionNodeID_arraysize = newSize;
}

void Packet::setTransactionNodeID(size_t k, int transactionNodeID)
{
    if (k >= transactionNodeID_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->transactionNodeID[k] = transactionNodeID;
}

void Packet::insertTransactionNodeID(size_t k, int transactionNodeID)
{
    if (k > transactionNodeID_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = transactionNodeID_arraysize + 1;
    int *transactionNodeID2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        transactionNodeID2[i] = this->transactionNodeID[i];
    transactionNodeID2[k] = transactionNodeID;
    for (i = k + 1; i < newSize; i++)
        transactionNodeID2[i] = this->transactionNodeID[i-1];
    delete [] this->transactionNodeID;
    this->transactionNodeID = transactionNodeID2;
    transactionNodeID_arraysize = newSize;
}

void Packet::insertTransactionNodeID(int transactionNodeID)
{
    insertTransactionNodeID(transactionNodeID_arraysize, transactionNodeID);
}

void Packet::eraseTransactionNodeID(size_t k)
{
    if (k >= transactionNodeID_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = transactionNodeID_arraysize - 1;
    int *transactionNodeID2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        transactionNodeID2[i] = this->transactionNodeID[i];
    for (i = k; i < newSize; i++)
        transactionNodeID2[i] = this->transactionNodeID[i+1];
    delete [] this->transactionNodeID;
    this->transactionNodeID = transactionNodeID2;
    transactionNodeID_arraysize = newSize;
}

size_t Packet::getTransactionIdMsgArraySize() const
{
    return transactionIdMsg_arraysize;
}

int Packet::getTransactionIdMsg(size_t k) const
{
    if (k >= transactionIdMsg_arraysize) throw omnetpp::cRuntimeError("Array of size transactionIdMsg_arraysize indexed by %lu", (unsigned long)k);
    return this->transactionIdMsg[k];
}

void Packet::setTransactionIdMsgArraySize(size_t newSize)
{
    int *transactionIdMsg2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = transactionIdMsg_arraysize < newSize ? transactionIdMsg_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        transactionIdMsg2[i] = this->transactionIdMsg[i];
    for (size_t i = minSize; i < newSize; i++)
        transactionIdMsg2[i] = 0;
    delete [] this->transactionIdMsg;
    this->transactionIdMsg = transactionIdMsg2;
    transactionIdMsg_arraysize = newSize;
}

void Packet::setTransactionIdMsg(size_t k, int transactionIdMsg)
{
    if (k >= transactionIdMsg_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->transactionIdMsg[k] = transactionIdMsg;
}

void Packet::insertTransactionIdMsg(size_t k, int transactionIdMsg)
{
    if (k > transactionIdMsg_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = transactionIdMsg_arraysize + 1;
    int *transactionIdMsg2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        transactionIdMsg2[i] = this->transactionIdMsg[i];
    transactionIdMsg2[k] = transactionIdMsg;
    for (i = k + 1; i < newSize; i++)
        transactionIdMsg2[i] = this->transactionIdMsg[i-1];
    delete [] this->transactionIdMsg;
    this->transactionIdMsg = transactionIdMsg2;
    transactionIdMsg_arraysize = newSize;
}

void Packet::insertTransactionIdMsg(int transactionIdMsg)
{
    insertTransactionIdMsg(transactionIdMsg_arraysize, transactionIdMsg);
}

void Packet::eraseTransactionIdMsg(size_t k)
{
    if (k >= transactionIdMsg_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = transactionIdMsg_arraysize - 1;
    int *transactionIdMsg2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        transactionIdMsg2[i] = this->transactionIdMsg[i];
    for (i = k; i < newSize; i++)
        transactionIdMsg2[i] = this->transactionIdMsg[i+1];
    delete [] this->transactionIdMsg;
    this->transactionIdMsg = transactionIdMsg2;
    transactionIdMsg_arraysize = newSize;
}

size_t Packet::getTransactionTimeArraySize() const
{
    return transactionTime_arraysize;
}

omnetpp::simtime_t Packet::getTransactionTime(size_t k) const
{
    if (k >= transactionTime_arraysize) throw omnetpp::cRuntimeError("Array of size transactionTime_arraysize indexed by %lu", (unsigned long)k);
    return this->transactionTime[k];
}

void Packet::setTransactionTimeArraySize(size_t newSize)
{
    omnetpp::simtime_t *transactionTime2 = (newSize==0) ? nullptr : new omnetpp::simtime_t[newSize];
    size_t minSize = transactionTime_arraysize < newSize ? transactionTime_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        transactionTime2[i] = this->transactionTime[i];
    for (size_t i = minSize; i < newSize; i++)
        transactionTime2[i] = SIMTIME_ZERO;
    delete [] this->transactionTime;
    this->transactionTime = transactionTime2;
    transactionTime_arraysize = newSize;
}

void Packet::setTransactionTime(size_t k, omnetpp::simtime_t transactionTime)
{
    if (k >= transactionTime_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->transactionTime[k] = transactionTime;
}

void Packet::insertTransactionTime(size_t k, omnetpp::simtime_t transactionTime)
{
    if (k > transactionTime_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = transactionTime_arraysize + 1;
    omnetpp::simtime_t *transactionTime2 = new omnetpp::simtime_t[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        transactionTime2[i] = this->transactionTime[i];
    transactionTime2[k] = transactionTime;
    for (i = k + 1; i < newSize; i++)
        transactionTime2[i] = this->transactionTime[i-1];
    delete [] this->transactionTime;
    this->transactionTime = transactionTime2;
    transactionTime_arraysize = newSize;
}

void Packet::insertTransactionTime(omnetpp::simtime_t transactionTime)
{
    insertTransactionTime(transactionTime_arraysize, transactionTime);
}

void Packet::eraseTransactionTime(size_t k)
{
    if (k >= transactionTime_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = transactionTime_arraysize - 1;
    omnetpp::simtime_t *transactionTime2 = (newSize == 0) ? nullptr : new omnetpp::simtime_t[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        transactionTime2[i] = this->transactionTime[i];
    for (i = k; i < newSize; i++)
        transactionTime2[i] = this->transactionTime[i+1];
    delete [] this->transactionTime;
    this->transactionTime = transactionTime2;
    transactionTime_arraysize = newSize;
}

size_t Packet::getTransactionValArraySize() const
{
    return transactionVal_arraysize;
}

int Packet::getTransactionVal(size_t k) const
{
    if (k >= transactionVal_arraysize) throw omnetpp::cRuntimeError("Array of size transactionVal_arraysize indexed by %lu", (unsigned long)k);
    return this->transactionVal[k];
}

void Packet::setTransactionValArraySize(size_t newSize)
{
    int *transactionVal2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = transactionVal_arraysize < newSize ? transactionVal_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        transactionVal2[i] = this->transactionVal[i];
    for (size_t i = minSize; i < newSize; i++)
        transactionVal2[i] = 0;
    delete [] this->transactionVal;
    this->transactionVal = transactionVal2;
    transactionVal_arraysize = newSize;
}

void Packet::setTransactionVal(size_t k, int transactionVal)
{
    if (k >= transactionVal_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->transactionVal[k] = transactionVal;
}

void Packet::insertTransactionVal(size_t k, int transactionVal)
{
    if (k > transactionVal_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = transactionVal_arraysize + 1;
    int *transactionVal2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        transactionVal2[i] = this->transactionVal[i];
    transactionVal2[k] = transactionVal;
    for (i = k + 1; i < newSize; i++)
        transactionVal2[i] = this->transactionVal[i-1];
    delete [] this->transactionVal;
    this->transactionVal = transactionVal2;
    transactionVal_arraysize = newSize;
}

void Packet::insertTransactionVal(int transactionVal)
{
    insertTransactionVal(transactionVal_arraysize, transactionVal);
}

void Packet::eraseTransactionVal(size_t k)
{
    if (k >= transactionVal_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = transactionVal_arraysize - 1;
    int *transactionVal2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        transactionVal2[i] = this->transactionVal[i];
    for (i = k; i < newSize; i++)
        transactionVal2[i] = this->transactionVal[i+1];
    delete [] this->transactionVal;
    this->transactionVal = transactionVal2;
    transactionVal_arraysize = newSize;
}

int Packet::getBlockID() const
{
    return this->blockID;
}

void Packet::setBlockID(int blockID)
{
    this->blockID = blockID;
}

int Packet::getPreHash() const
{
    return this->preHash;
}

void Packet::setPreHash(int preHash)
{
    this->preHash = preHash;
}

double Packet::getMF() const
{
    return this->MF;
}

void Packet::setMF(double MF)
{
    this->MF = MF;
}

double Packet::getCandidateTime() const
{
    return this->candidateTime;
}

void Packet::setCandidateTime(double candidateTime)
{
    this->candidateTime = candidateTime;
}

size_t Packet::getNeighboursArraySize() const
{
    return neighbours_arraysize;
}

int Packet::getNeighbours(size_t k) const
{
    if (k >= neighbours_arraysize) throw omnetpp::cRuntimeError("Array of size neighbours_arraysize indexed by %lu", (unsigned long)k);
    return this->neighbours[k];
}

void Packet::setNeighboursArraySize(size_t newSize)
{
    int *neighbours2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = neighbours_arraysize < newSize ? neighbours_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        neighbours2[i] = this->neighbours[i];
    for (size_t i = minSize; i < newSize; i++)
        neighbours2[i] = 0;
    delete [] this->neighbours;
    this->neighbours = neighbours2;
    neighbours_arraysize = newSize;
}

void Packet::setNeighbours(size_t k, int neighbours)
{
    if (k >= neighbours_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->neighbours[k] = neighbours;
}

void Packet::insertNeighbours(size_t k, int neighbours)
{
    if (k > neighbours_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = neighbours_arraysize + 1;
    int *neighbours2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        neighbours2[i] = this->neighbours[i];
    neighbours2[k] = neighbours;
    for (i = k + 1; i < newSize; i++)
        neighbours2[i] = this->neighbours[i-1];
    delete [] this->neighbours;
    this->neighbours = neighbours2;
    neighbours_arraysize = newSize;
}

void Packet::insertNeighbours(int neighbours)
{
    insertNeighbours(neighbours_arraysize, neighbours);
}

void Packet::eraseNeighbours(size_t k)
{
    if (k >= neighbours_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = neighbours_arraysize - 1;
    int *neighbours2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        neighbours2[i] = this->neighbours[i];
    for (i = k; i < newSize; i++)
        neighbours2[i] = this->neighbours[i+1];
    delete [] this->neighbours;
    this->neighbours = neighbours2;
    neighbours_arraysize = newSize;
}

size_t Packet::getVoteArraySize() const
{
    return vote_arraysize;
}

bool Packet::getVote(size_t k) const
{
    if (k >= vote_arraysize) throw omnetpp::cRuntimeError("Array of size vote_arraysize indexed by %lu", (unsigned long)k);
    return this->vote[k];
}

void Packet::setVoteArraySize(size_t newSize)
{
    bool *vote2 = (newSize==0) ? nullptr : new bool[newSize];
    size_t minSize = vote_arraysize < newSize ? vote_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        vote2[i] = this->vote[i];
    for (size_t i = minSize; i < newSize; i++)
        vote2[i] = false;
    delete [] this->vote;
    this->vote = vote2;
    vote_arraysize = newSize;
}

void Packet::setVote(size_t k, bool vote)
{
    if (k >= vote_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->vote[k] = vote;
}

void Packet::insertVote(size_t k, bool vote)
{
    if (k > vote_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = vote_arraysize + 1;
    bool *vote2 = new bool[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        vote2[i] = this->vote[i];
    vote2[k] = vote;
    for (i = k + 1; i < newSize; i++)
        vote2[i] = this->vote[i-1];
    delete [] this->vote;
    this->vote = vote2;
    vote_arraysize = newSize;
}

void Packet::insertVote(bool vote)
{
    insertVote(vote_arraysize, vote);
}

void Packet::eraseVote(size_t k)
{
    if (k >= vote_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = vote_arraysize - 1;
    bool *vote2 = (newSize == 0) ? nullptr : new bool[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        vote2[i] = this->vote[i];
    for (i = k; i < newSize; i++)
        vote2[i] = this->vote[i+1];
    delete [] this->vote;
    this->vote = vote2;
    vote_arraysize = newSize;
}

class PacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_srcAddr,
        FIELD_destAddr,
        FIELD_hopCount,
        FIELD_packetType,
        FIELD_nodeID,
        FIELD_posX,
        FIELD_posY,
        FIELD_transactionValue,
        FIELD_time,
        FIELD_userXID,
        FIELD_senatorAddress,
        FIELD_transactionSourceAddress,
        FIELD_TimeOfCreateTransaction,
        FIELD_transactionID,
        FIELD_transactionNodeID,
        FIELD_transactionIdMsg,
        FIELD_transactionTime,
        FIELD_transactionVal,
        FIELD_blockID,
        FIELD_preHash,
        FIELD_MF,
        FIELD_candidateTime,
        FIELD_neighbours,
        FIELD_vote,
    };
  public:
    PacketDescriptor();
    virtual ~PacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(PacketDescriptor)

PacketDescriptor::PacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(Packet)), "omnetpp::cPacket")
{
    propertynames = nullptr;
}

PacketDescriptor::~PacketDescriptor()
{
    delete[] propertynames;
}

bool PacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Packet *>(obj)!=nullptr;
}

const char **PacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *PacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int PacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 24+basedesc->getFieldCount() : 24;
}

unsigned int PacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcAddr
        FD_ISEDITABLE,    // FIELD_destAddr
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_packetType
        FD_ISEDITABLE,    // FIELD_nodeID
        FD_ISEDITABLE,    // FIELD_posX
        FD_ISEDITABLE,    // FIELD_posY
        FD_ISEDITABLE,    // FIELD_transactionValue
        FD_ISEDITABLE,    // FIELD_time
        FD_ISEDITABLE,    // FIELD_userXID
        FD_ISEDITABLE,    // FIELD_senatorAddress
        FD_ISEDITABLE,    // FIELD_transactionSourceAddress
        FD_ISEDITABLE,    // FIELD_TimeOfCreateTransaction
        FD_ISEDITABLE,    // FIELD_transactionID
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_transactionNodeID
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_transactionIdMsg
        FD_ISARRAY,    // FIELD_transactionTime
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_transactionVal
        FD_ISEDITABLE,    // FIELD_blockID
        FD_ISEDITABLE,    // FIELD_preHash
        FD_ISEDITABLE,    // FIELD_MF
        FD_ISEDITABLE,    // FIELD_candidateTime
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_neighbours
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_vote
    };
    return (field >= 0 && field < 24) ? fieldTypeFlags[field] : 0;
}

const char *PacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcAddr",
        "destAddr",
        "hopCount",
        "packetType",
        "nodeID",
        "posX",
        "posY",
        "transactionValue",
        "time",
        "userXID",
        "senatorAddress",
        "transactionSourceAddress",
        "TimeOfCreateTransaction",
        "transactionID",
        "transactionNodeID",
        "transactionIdMsg",
        "transactionTime",
        "transactionVal",
        "blockID",
        "preHash",
        "MF",
        "candidateTime",
        "neighbours",
        "vote",
    };
    return (field >= 0 && field < 24) ? fieldNames[field] : nullptr;
}

int PacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 's' && strcmp(fieldName, "srcAddr") == 0) return base+0;
    if (fieldName[0] == 'd' && strcmp(fieldName, "destAddr") == 0) return base+1;
    if (fieldName[0] == 'h' && strcmp(fieldName, "hopCount") == 0) return base+2;
    if (fieldName[0] == 'p' && strcmp(fieldName, "packetType") == 0) return base+3;
    if (fieldName[0] == 'n' && strcmp(fieldName, "nodeID") == 0) return base+4;
    if (fieldName[0] == 'p' && strcmp(fieldName, "posX") == 0) return base+5;
    if (fieldName[0] == 'p' && strcmp(fieldName, "posY") == 0) return base+6;
    if (fieldName[0] == 't' && strcmp(fieldName, "transactionValue") == 0) return base+7;
    if (fieldName[0] == 't' && strcmp(fieldName, "time") == 0) return base+8;
    if (fieldName[0] == 'u' && strcmp(fieldName, "userXID") == 0) return base+9;
    if (fieldName[0] == 's' && strcmp(fieldName, "senatorAddress") == 0) return base+10;
    if (fieldName[0] == 't' && strcmp(fieldName, "transactionSourceAddress") == 0) return base+11;
    if (fieldName[0] == 'T' && strcmp(fieldName, "TimeOfCreateTransaction") == 0) return base+12;
    if (fieldName[0] == 't' && strcmp(fieldName, "transactionID") == 0) return base+13;
    if (fieldName[0] == 't' && strcmp(fieldName, "transactionNodeID") == 0) return base+14;
    if (fieldName[0] == 't' && strcmp(fieldName, "transactionIdMsg") == 0) return base+15;
    if (fieldName[0] == 't' && strcmp(fieldName, "transactionTime") == 0) return base+16;
    if (fieldName[0] == 't' && strcmp(fieldName, "transactionVal") == 0) return base+17;
    if (fieldName[0] == 'b' && strcmp(fieldName, "blockID") == 0) return base+18;
    if (fieldName[0] == 'p' && strcmp(fieldName, "preHash") == 0) return base+19;
    if (fieldName[0] == 'M' && strcmp(fieldName, "MF") == 0) return base+20;
    if (fieldName[0] == 'c' && strcmp(fieldName, "candidateTime") == 0) return base+21;
    if (fieldName[0] == 'n' && strcmp(fieldName, "neighbours") == 0) return base+22;
    if (fieldName[0] == 'v' && strcmp(fieldName, "vote") == 0) return base+23;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *PacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcAddr
        "int",    // FIELD_destAddr
        "int",    // FIELD_hopCount
        "int",    // FIELD_packetType
        "int",    // FIELD_nodeID
        "int",    // FIELD_posX
        "int",    // FIELD_posY
        "int",    // FIELD_transactionValue
        "double",    // FIELD_time
        "int",    // FIELD_userXID
        "int",    // FIELD_senatorAddress
        "int",    // FIELD_transactionSourceAddress
        "double",    // FIELD_TimeOfCreateTransaction
        "int",    // FIELD_transactionID
        "int",    // FIELD_transactionNodeID
        "int",    // FIELD_transactionIdMsg
        "omnetpp::simtime_t",    // FIELD_transactionTime
        "int",    // FIELD_transactionVal
        "int",    // FIELD_blockID
        "int",    // FIELD_preHash
        "double",    // FIELD_MF
        "double",    // FIELD_candidateTime
        "int",    // FIELD_neighbours
        "bool",    // FIELD_vote
    };
    return (field >= 0 && field < 24) ? fieldTypeStrings[field] : nullptr;
}

const char **PacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case FIELD_srcAddr: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_destAddr: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_hopCount: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_packetType: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_nodeID: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_posX: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_posY: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_transactionValue: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_time: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_userXID: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_senatorAddress: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_transactionSourceAddress: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_TimeOfCreateTransaction: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_transactionID: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_transactionNodeID: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_transactionIdMsg: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_transactionTime: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_transactionVal: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_blockID: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_preHash: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_MF: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_candidateTime: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_neighbours: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_vote: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *PacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case FIELD_srcAddr:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_destAddr:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_hopCount:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_packetType:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_nodeID:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_posX:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_posY:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_transactionValue:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_time:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_userXID:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_senatorAddress:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_transactionSourceAddress:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_TimeOfCreateTransaction:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_transactionID:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_transactionNodeID:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_transactionIdMsg:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_transactionTime:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_transactionVal:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_blockID:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_preHash:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_MF:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_candidateTime:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_neighbours:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_vote:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        default: return nullptr;
    }
}

int PacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        case FIELD_transactionNodeID: return pp->getTransactionNodeIDArraySize();
        case FIELD_transactionIdMsg: return pp->getTransactionIdMsgArraySize();
        case FIELD_transactionTime: return pp->getTransactionTimeArraySize();
        case FIELD_transactionVal: return pp->getTransactionValArraySize();
        case FIELD_neighbours: return pp->getNeighboursArraySize();
        case FIELD_vote: return pp->getVoteArraySize();
        default: return 0;
    }
}

const char *PacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        case FIELD_srcAddr: return long2string(pp->getSrcAddr());
        case FIELD_destAddr: return long2string(pp->getDestAddr());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_packetType: return long2string(pp->getPacketType());
        case FIELD_nodeID: return long2string(pp->getNodeID());
        case FIELD_posX: return long2string(pp->getPosX());
        case FIELD_posY: return long2string(pp->getPosY());
        case FIELD_transactionValue: return long2string(pp->getTransactionValue());
        case FIELD_time: return double2string(pp->getTime());
        case FIELD_userXID: return long2string(pp->getUserXID());
        case FIELD_senatorAddress: return long2string(pp->getSenatorAddress());
        case FIELD_transactionSourceAddress: return long2string(pp->getTransactionSourceAddress());
        case FIELD_TimeOfCreateTransaction: return double2string(pp->getTimeOfCreateTransaction());
        case FIELD_transactionID: return long2string(pp->getTransactionID());
        case FIELD_transactionNodeID: return long2string(pp->getTransactionNodeID(i));
        case FIELD_transactionIdMsg: return long2string(pp->getTransactionIdMsg(i));
        case FIELD_transactionTime: return simtime2string(pp->getTransactionTime(i));
        case FIELD_transactionVal: return long2string(pp->getTransactionVal(i));
        case FIELD_blockID: return long2string(pp->getBlockID());
        case FIELD_preHash: return long2string(pp->getPreHash());
        case FIELD_MF: return double2string(pp->getMF());
        case FIELD_candidateTime: return double2string(pp->getCandidateTime());
        case FIELD_neighbours: return long2string(pp->getNeighbours(i));
        case FIELD_vote: return bool2string(pp->getVote(i));
        default: return "";
    }
}

bool PacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        case FIELD_srcAddr: pp->setSrcAddr(string2long(value)); return true;
        case FIELD_destAddr: pp->setDestAddr(string2long(value)); return true;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); return true;
        case FIELD_packetType: pp->setPacketType(string2long(value)); return true;
        case FIELD_nodeID: pp->setNodeID(string2long(value)); return true;
        case FIELD_posX: pp->setPosX(string2long(value)); return true;
        case FIELD_posY: pp->setPosY(string2long(value)); return true;
        case FIELD_transactionValue: pp->setTransactionValue(string2long(value)); return true;
        case FIELD_time: pp->setTime(string2double(value)); return true;
        case FIELD_userXID: pp->setUserXID(string2long(value)); return true;
        case FIELD_senatorAddress: pp->setSenatorAddress(string2long(value)); return true;
        case FIELD_transactionSourceAddress: pp->setTransactionSourceAddress(string2long(value)); return true;
        case FIELD_TimeOfCreateTransaction: pp->setTimeOfCreateTransaction(string2double(value)); return true;
        case FIELD_transactionID: pp->setTransactionID(string2long(value)); return true;
        case FIELD_transactionNodeID: pp->setTransactionNodeID(i,string2long(value)); return true;
        case FIELD_transactionIdMsg: pp->setTransactionIdMsg(i,string2long(value)); return true;
        case FIELD_transactionVal: pp->setTransactionVal(i,string2long(value)); return true;
        case FIELD_blockID: pp->setBlockID(string2long(value)); return true;
        case FIELD_preHash: pp->setPreHash(string2long(value)); return true;
        case FIELD_MF: pp->setMF(string2double(value)); return true;
        case FIELD_candidateTime: pp->setCandidateTime(string2double(value)); return true;
        case FIELD_neighbours: pp->setNeighbours(i,string2long(value)); return true;
        case FIELD_vote: pp->setVote(i,string2bool(value)); return true;
        default: return false;
    }
}

const char *PacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *PacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

