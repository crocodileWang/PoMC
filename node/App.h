#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif


#include <bits/stdc++.h>
#include <string.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <iostream>
#include <unordered_set>
#include <omnetpp.h>
#include "Packet_m.h"


using namespace omnetpp;

class CandidateNode
{
public:
    int nodeID;
    simtime_t time;
    double MF;
    int posX;
    int posY;
    std::vector<int> neighbours;
    //std::vector<int> *p = &neighbours;
    CandidateNode(){}
    CandidateNode(int nodeID,simtime_t time,double MF,int posX,int posY,std::vector<int> *p)
    {
        this->nodeID = nodeID;
        this->time = time;
        this->MF = MF;
        this->posX = posX;
        this->posY = posY;
        for(int i=0;i<p->size();i++)
        {
            this->neighbours.push_back(p->at(i));
        }
    }
    CandidateNode(const CandidateNode& b)
    {
           this->nodeID = b.nodeID;
           this->time = b.time;
           this->MF = b.MF;
           this->posX = b.posX;
           this->posY = b.posY;
           //const std::vector<int>* q = &b.neighbours;
           for (int i = 0; i < b.neighbours.size(); i++)
           {
               this->neighbours.push_back(b.neighbours.at(i));
           }
    }
};

class CandidateMiningNode
{
public:
    int nodeID;
    simtime_t time;
    double MF;
    CandidateMiningNode(int nodeID,simtime_t time,double MF)
    {
        this->nodeID = nodeID;
        this->time = time;
        this->MF = MF;
    }
};

class NodePos
{
public:
    int nodeID;
    int posX;
    int posY;
    NodePos(int nodeID,int posX,int posY)
    {
        this->nodeID = nodeID;
        this->posX = posX;
        this->posY = posY;
    }
};

class SenatorNodeElement
{
public:
    int nodeId;
    simtime_t time;
    int posX;
    int posY;

    SenatorNodeElement(int nodeId,simtime_t time,int posX,int posY)
    {
        this->nodeId = nodeId;
        this->time = time;
        this->posX = posX;
        this->posY = posY;
    }
    SenatorNodeElement(const SenatorNodeElement& b)
    {
        this->nodeId = b.nodeId;
        this->posX = b.posX;
        this->posY = b.posY;
        this->time = b.time;
    }
};

class PoMCtransactionElement
{
public:
    int nodeID,transactionValue,transactionID;
    //std::string transactionVal;
    //char *transactionVal;
    simtime_t TimeOfCreateTransaction;

    PoMCtransactionElement(){}

    PoMCtransactionElement(int nodeID, int transactionID, int transactionValue, simtime_t TimeOfCreateTransaction)
    {
        this->nodeID = nodeID;
        this->transactionID = transactionID;
        this->transactionValue = transactionValue;
        this->TimeOfCreateTransaction = TimeOfCreateTransaction;
    }
};

class PoMCblockElement
{
public:
    int blockID = 0;
    //PoMCblockElement *preHash;
    int preHash = 0;
    std::vector<PoMCtransactionElement> TransactionElement = std::vector<PoMCtransactionElement>();
    PoMCblockElement(int blockID,int preHash,std::vector<PoMCtransactionElement>* TransactionElement)
    {
        this->blockID = blockID;
        this->preHash = preHash;
        for(int i=0;i<TransactionElement->size();i++)
        {
            this->TransactionElement[i] = TransactionElement->at(i);
        }
    }
    PoMCblockElement()
    {
       // this->blockID = 0;
       // this->preHash = 0;
       // this->TransactionElement = std::vector<PoMCtransactionElement>();
    }
    /*PoMCblockElement(const PoMCblockElement& b)
    {
        this->blockID = b.blockID;
        this->preHash = b.preHash;
        for(int i=0;i<b.TransactionElement.size();i++)
        {
            this->TransactionElement[i] = b.TransactionElement.at(i);
        }
    }*/

};

class MiningNode
{
public:
    int nodeID;
    simtime_t electTime;
    double MF;
    MiningNode(){}
    MiningNode(int nodeID, simtime_t electTime,double MF){
        this->nodeID = nodeID;
        this->electTime = electTime;
        this->MF = MF;
    }
    MiningNode(const MiningNode& b){
        this->nodeID = b.nodeID;
        this->electTime = b.electTime;
        this->MF = b.MF;
    }

};

class localCandidateNode
{
public:
    int nodeID;
    double timeOfTakingConsensus;
};

class consensusCandidateNode
{
public:
    int nodeID;
    double timeOfTakingConsensus;
    std::map<int, bool> voteSet;
};

class SimulationTiming
{
public:
    int nodeId;
    simtime_t transactionTime, detectionTime;

    SimulationTiming(int nodeId, simtime_t transactionTime, simtime_t detectionTime)
    {
        this->nodeId = nodeId;
        this->transactionTime = transactionTime;
        this->detectionTime = detectionTime;
    }
};

class RandomDistinctPicker
{
public:
    int seedIncremental;
    int seedCore;
    int min, max;
    std::unordered_set<int> intSet;
    int resultProided;

    RandomDistinctPicker(int min, int max, int seedCore)
    {
        this->min = min;
        this->max = max;
        this->seedCore = seedCore;
        seedIncremental = 0;
        resultProided = 0;
    }

    int getRandomNumber()
    {
        if (resultProided >= (max - min + 1))
            return min;

        int posibleRandomNum = rand();
        while (intSet.find(posibleRandomNum) != intSet.end()) {
            posibleRandomNum = rand();
        }
        intSet.insert(posibleRandomNum);
        resultProided++;
        return posibleRandomNum;
    }
private:
    int rand()
    {
        seedIncremental++;
        std::mt19937 gen(seedIncremental + seedCore);
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

};

static std::vector<SimulationTiming> simulationTiming; // This variable contains the evil nodes last transaction and detection timing

static long numberOfTransactionCounter= 0;

static int transactionID;
static int blockID;

class App : public omnetpp::cSimpleModule
{
private:
    // configuration
    int myAddress;
    std::vector<int> destAddresses;
    cPar *sendIATime;
    cPar *anonymusAuditingTimeoutTime;
    cPar *sendIaTimeEvil;
    cPar *packetLengthBytes;
    int rep;//ÐÅÓþ¶È
    int blockSize;
    int senatorRound;
    int miningNodeRound;
    bool haveITakenConsensus;
    bool haveITakenSenatorLottery;
    int posX,posY;
    int receivedSenatorAck;
    bool haveIProposedCandidate;//the situation that a senator do not receive any consensus message
    int miningNodeBuffer;
    int confirmMsgCount;
    bool haveISendConfirmMsg;
    bool haveIDisseminateMiningNode;
    MiningNode localMiningNode;
    MiningNode tempLocalMiningNode;
    bool amIMiningNode;
    int blockDisseminationAck;
    bool keepReceivingTransaction;
    // state
    //cMessage *generatePacket;
    //cMessage *timerThread;
    cMessage *timerInitTransaction;
    cMessage *timerTransactionTimeout;
    cMessage *timerAnonymusAuditingTimeout;
    cMessage *timerAnonymiserDissemination;
    cMessage *timerSenatorDissemination;
    cMessage *timerSenatorLifeTimeout;
    cMessage *timerMiningNodeDissemination;
    cMessage *timerCheckNodeSelf;
    cMessage *nodeEnrollDissemination;
    cMessage *displaySenatorList;
    cMessage *senatorBufferToList;
    cMessage *timerTakePoMCConsensus;
    cMessage *timerConsensusInSenator;
    cMessage *miningNodeBufferToList;

    //cMessage *timerThreadCompleteAuditing;

    CandidateNode can;

    long pkCounter;
    long chainTotalValue;
    bool amIEvil;
    //int kindsOfNode;
    std::vector<int> victimDestAddresses; //used to keep track of whom the evil node has attacked
    int totalEvilTransactions;
    std::vector<int> evilNodeIds;
    int transactionStage; // 0-none 1-waiting for anonymizer 2-disseminating

    //Transactions related par
    int tempBlockID;
    int tempPartnerSeqNum;
    int tempBlockTransaction;


    //static
    long numReceived;
    long numSent;
    //cLongHistogram hopCountStats;
    //cOutVector hopCountVector;

    //Signals
    //simsignal_t endToEndDelaySignal;
    //simsignal_t hopCountSignal;
    //simsignal_t sourceAddressSignal;
    simsignal_t transDelaySignal;

    std::vector<int> disseminationNodeAddresses;
    std::vector<int> nodeWithMissingTransaction;
    std::unordered_set<std::string> anonyDisseminationMessageSet;

    std::unordered_set<std::string> SenatorDisseminationMessageSet;
    std::vector<SenatorNodeElement> SenatorList;
    std::vector<SenatorNodeElement> SenatorBuffer;
    std::vector<PoMCtransactionElement> TransactionPool;
    std::vector<PoMCblockElement> localBlockChain;
    std::vector<NodePos> totalNodeView;
    std::vector<CandidateNode> candidateNodeList;
    std::vector<bool> voteList;
    std::map<int,int> votesOfCandidate;
    std::vector<int> miningNodeIds;
    std::vector<int> evilMiningNodes;


public:
    App();
    virtual ~App();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    virtual void receiveMessage(cMessage *msg);
    virtual int randomNodeAddressPicker();

    //Messaging

    virtual void createDirectChannel(int nodeId);
    virtual void closeDirectChannel();

    //Node Enroll
    virtual void disseminateMyEnrollInfo();
    virtual bool haveIseenThisNodeDisseminationBefore(int nodeID);
    virtual void reDisseminateEnrollInfo(Packet *pk);

    //Senator
    virtual void disseminateMeAsSenator();
    virtual void updateSenatorBuffer(int nodeId);
    virtual void updateSenatorBuffer(Packet *pk);
    virtual void updateSenatorNodeList();
    virtual bool haveISeenThisSenatorDisseminationBefore(std::string key);
    virtual void reDisseminateSenatorMessage(Packet *pk);
    virtual void checkSenatorLife();
    virtual void takeSenatorSortition();
    virtual bool checkNeighbours();

    //transaction
    virtual void generateNewTransaction();
    virtual void dealTransaction(Packet *pk);
    virtual void replyAckToSrcNode(int destAddr , int tempTransactionID);
    virtual bool haveISeenThisBlockDisseminationBefore(int blockID);
    virtual void reDisseminateBlockMessage(Packet *pk);
    virtual void replyAckToMiningNode(int nodeID, int blockID);

    //PoMC
    virtual void checkMiningNodeTerm();
    virtual void takePoMCConsensus();
    virtual double calculateMF(SenatorNodeElement node);
    virtual void proposeCandidate();
    virtual void voteToCandidate();
    virtual bool checkSINR(std::vector<CandidateNode>::iterator p);
    virtual bool checkRep(std::vector<CandidateNode>::iterator p);
    virtual void replySenatorAckTo(int nodeID);
    virtual void sendConfirmMsg(int nodeID);
    virtual void replyConfirmMsg(int dest);
    virtual void disseminateMiningNode(int nodeID);
    virtual bool haveIseenThisMiningNodeDisseminationBefore(int nodeID);
    virtual void reDisseminateMiningNode(Packet *pk);

    //static
    virtual void refreshDisplay() const override;
    virtual void finish() override;


    //Data logging
    virtual void simulationRegisterTransactionTime(int idNode);
    virtual void simulationRegisterDetectionTime(int idNode);
    virtual void stopSimulation();


};
