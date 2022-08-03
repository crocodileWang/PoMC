//发现了一个问题，在本模块中获取另外一个模块的邻居节点时，假如一共有10个节点，无法获取到最后一个节点，原因未知
//TODO LIST
// - optimize evil node so that do transactions with node far
// - refactoring: implement enum to mark type of packets

#include "App.h"
#include "Routing.h"
#include <string.h>
#include <cmath>
#include <fstream>
#include <iomanip>
using namespace omnetpp;

Define_Module(App);


App::App()
{
    timerInitTransaction = nullptr;
    timerTransactionTimeout = nullptr;
    timerAnonymusAuditingTimeout = nullptr;
    timerAnonymiserDissemination = nullptr;
    timerSenatorDissemination = nullptr;
}
App::~App()
{
    cancelAndDelete(timerInitTransaction);
    cancelAndDelete(timerTransactionTimeout);
    cancelAndDelete(timerAnonymusAuditingTimeout);
    cancelAndDelete(timerAnonymiserDissemination);
    cancelAndDelete(timerSenatorDissemination);
}

void App::initialize()
{
    myAddress = par("address");
    packetLengthBytes = &par("packetLength");

    pkCounter = 0;
    tempBlockID = -1;
    tempPartnerSeqNum = 0;
    tempBlockTransaction = 0;
    transactionStage = 0;
    senatorRound = 0;
    miningNodeRound = 1;
    haveITakenConsensus = false;
    haveITakenSenatorLottery = false;
    receivedSenatorAck = 0;
    haveIProposedCandidate = false;
    confirmMsgCount = 0;
    haveISendConfirmMsg = false;
    haveIDisseminateMiningNode = false;
    blockDisseminationAck = 0;//reset when a mining node receives the ack msg whose counts are more than the half of total nodes && mining node changes
    keepReceivingTransaction = true;//reset when a mining node receives the ack msg whose counts are more than the half of total nodes && mining node changes

    rep = par("initialRep");

    blockSize = par("blockSize");

    can.nodeID = -1;
    can.time = -1;
    can.MF = -1;

    //gensis block init
    transactionID = 0;
    blockID = 1;
    PoMCblockElement genesisBlock;
    localBlockChain.push_back(genesisBlock);

    //first mining node
    MiningNode tNode(2,0,1);
    localMiningNode = tempLocalMiningNode = tNode;


    timerInitTransaction = new cMessage("InitNewTransaction");
    timerTransactionTimeout = new cMessage("TransactionTimeout");
    timerAnonymusAuditingTimeout = new cMessage("AnonymusAuditingTimeout");
    timerAnonymiserDissemination = new cMessage("TimerAnonymiserDissemination");
    timerSenatorDissemination = new cMessage("TimerSenatorDissemination");
    timerSenatorLifeTimeout = new cMessage("TimerSenatorLifeTimeout");
    timerMiningNodeDissemination = new cMessage("TimerMiningNodeDissemination");
    timerCheckNodeSelf = new cMessage("TimerCheckNodeSelf");
    nodeEnrollDissemination = new cMessage("nodeEnrollDissemination");
    displaySenatorList = new cMessage("displaySenatorList");
    senatorBufferToList = new cMessage("senatorBufferToList");
    timerTakePoMCConsensus = new cMessage("timerTakePoMCConsensus");
    timerConsensusInSenator = new cMessage("timerConsensusInSenator");
    miningNodeBufferToList = new cMessage("miningNodeBufferToList");

    //static
    numReceived = 0;
    numSent = 0;
    //hopCountStats.setName("hopCountStats");
    //hopCountStats.setRangeAutoUpper(0, 10, 1.5);
    //hopCountVector.setName("HopCount");

    //Information To Log
    WATCH(pkCounter);
    WATCH(myAddress);
    WATCH(chainTotalValue);
    WATCH(numReceived);
    WATCH(numSent);

    //module
    cModule *parent = getParentModule();
    posX = (int)parent->par("posX");
    posY = (int)parent->par("posY");


    /*std::ifstream infile;
    infile.open("D:\\coordinates.txt");
    if (!infile) {
        std::cerr << "Unable to open file";
        exit(1);   // call system to stop
    }
    std::vector<std::string> str;
    std::string line;
    while (getline(infile, line))
    {
        str.push_back(line);
    }
    infile.close();

    for(int i = 0; i < str.size(); i++)
    {
        const char *posStr = str[i].c_str();;
        cStringTokenizer tokenizer(posStr);
        const char *token;
        std::vector<int> tempStr;
        while ((token = tokenizer.nextToken()) != nullptr)
            tempStr.push_back(atoi(token));
        if(myAddress == tempStr[0])
        {
            posX = tempStr[1];
            posY = tempStr[2];
        }

    }*/
    cDisplayString& dispStr = parent->getDisplayString();
    dispStr.setTagArg("p", posX , posY);

    EV<<"My location is "<<posX<<","<<posY<<endl;
    NodePos tempNode(myAddress,posX,posY);
    totalNodeView.push_back(tempNode);
    EV<<"Parent module is :"<<parent->getFullName()<<"  grandparent module is "<< parent->getParentModule()->getName()<<endl;
    for(cModule::SubmoduleIterator it(parent);!it.end();it++)
    {
        cModule *submodule = *it;
        EV<<submodule->getName()<<endl;
    }
    cModule *tar = parent->getParentModule()->getSubmodule("rte",0);

    //utilize for testing to get neighbours of target nodes
    /*cModule *tar = parent->getParentModule()->getSubmodule("rte",10);
    EV<<parent->getFullName()<<" "<<parent->getParentModule()->getFullName()<<" "<<tar->getFullName()<<endl;
    cModule *mod = tar->getSubmodule("routing");
    EV<<mod->getName()<<endl;
    Routing *myRouting = check_and_cast<Routing*>(mod);
    EV<<myRouting->neighbourNodeAddresses.size()<<endl;
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;
    EV<<neighbourNodeAddresses.size()<<" "<<" neighbours of node 10 are:";
    for(int i=0 ;i < neighbourNodeAddresses.size(); i++){

          EV<<neighbourNodeAddresses[i]<<" ";

    }
    EV<<endl;*/

    //utilize for testing to get position of target nodes
    /*cModule *tar = parent->getParentModule()->getSubmodule("rte",10);
    EV<<parent->getFullName()<<" "<<parent->getParentModule()->getFullName()<<" "<<tar->getFullName()<<endl;
    cModule *mod = tar->getSubmodule("app");
    EV<<mod->getName()<<endl;
    App *myApp = check_and_cast<App*>(mod);
    EV<<"("<<myApp->posX<<" "<<myApp->posY<<")"<<endl;*/

    //utilize for testing build connection
    /*cGate *portin = parent->gate("port$i",1);
    cGate *portout = parent->gate("port$o",0);
    int gateinID = portin->getId();
    int gateoutID = portout->getId();
    EV<<portin->getFullName()<<" "<<gateinID<<endl;
    EV<<portout->getNameSuffix()<<" "<<portout->getBaseName()<<" "<<gateoutID<<endl;
    EV<<parent->gateSize("port")<<endl;
    EV<<*(portin->getPreviousGate())->getOwnerModule()<<endl;//portin->getPreviousGate()返回的是portin连接的gate的指针，指针指向的是连接的gate；getOwnerModule()返回该门（module、channel、connection）所在的模块
    EV<<(portin->getPreviousGate())->getFullPath()<<endl;//getFullPath()返回的是包含门和门所在的module的字符串

    cGate *porttest = parent->gate("port$o",2);
    porttest->disconnect();
    for(int i = 0;i<parent->gateSize("port");i++)
    {
        cGate *portin = parent->gate("port$i",i);
        int gateinID = portin->getId();
        EV<<portin->getFullName()<<" "<<gateinID<<endl;
    }*/

    //Neighbors definition
    for (int i = 0; i < (int) par("totalNodes"); i++) {
        destAddresses.push_back(i);
    }

    if (destAddresses.size() == 0)
        throw cRuntimeError("At least one address must be specified in the destAddresses parameter!");

    totalEvilTransactions = 0;
    //evil node mark
    const char *evilNodeIdsPar = par("evilNodeIDs");
    cStringTokenizer tokenizer(evilNodeIdsPar);
    const char *token;
    while ((token = tokenizer.nextToken()) != nullptr)//构造器接收一个字符串,被认为是通过字符串分隔符(缺省是空格),将单词分隔记号的序列.然后多次调用nextToken()方法将一个接一个地返回记号.在最后一个记号之后,将返回NULL.
        evilNodeIds.push_back(atoi(token));
    for(int i = 0; i < evilNodeIds.size(); i++)
    {
        EV<<evilNodeIds[i]<<" ";
    }
    EV<<endl;

    //mining node mark
    amIMiningNode = false;
    const char *miningNodeIdsPar = par("miningNodeIDs");
    cStringTokenizer mtokenizer(miningNodeIdsPar);
    const char *mtoken;
    while ((mtoken = mtokenizer.nextToken()) != nullptr)//构造器接收一个字符串,被认为是通过字符串分隔符(缺省是空格),将单词分隔记号的序列.然后多次调用nextToken()方法将一个接一个地返回记号.在最后一个记号之后,将返回NULL.
        miningNodeIds.push_back(atoi(mtoken));
    for(int i = 0; i < miningNodeIds.size(); i++)
    {
        EV<<miningNodeIds[i]<<" ";
    }
    EV<<endl;
    std::vector<int>::iterator mit = find(miningNodeIds.begin(),miningNodeIds.end(),myAddress);
    if(mit != miningNodeIds.end())
        amIMiningNode = true;

    //Start the recursive thread
    //scheduleAt(simTime() + par("sendIaTimeInit").doubleValue(), timerInitTransaction);
    //scheduleAt(simTime() + par("anonymizerDisseminationTimeInit").doubleValue(), timerAnonymiserDissemination);
    //scheduleAt(simTime() + par("SenatorDisseminationTimeInit").doubleValue(), timerSenatorDissemination);
    //scheduleAt(simTime() + par("MiningNodeDisseminationTimeInit").doubleValue(), timerMiningNodeDissemination);
    scheduleAt(simTime(),nodeEnrollDissemination);
    scheduleAt(simTime(),timerCheckNodeSelf);
    scheduleAt(simTime()+20,displaySenatorList);
    scheduleAt(simTime() + par("generateTransactionTimeInit").doubleValue(), timerInitTransaction);
    scheduleAt(simTime()+10,senatorBufferToList);
    scheduleAt(SimTime(miningNodeRound)*par("TermOfMiningNode"),miningNodeBufferToList);

    if(myAddress != localMiningNode.nodeID && intuniform(0,30) < 100) //first senator round lottery
        takeSenatorSortition();

    //endToEndDelaySignal = registerSignal("endToEndDelay");
    //hopCountSignal = registerSignal("hopCount");
    //sourceAddressSignal = registerSignal("sourceAddress");
    transDelaySignal = registerSignal("transactionDelay");
    if(parent == tar)
    {
        std::ofstream outfile;
        outfile.open("D:\\expData.txt",std::ios::out | std::ios::trunc);
        outfile<<(int)par("totalNodes")<<endl;
        outfile.close();

        std::ofstream outfile2;
        outfile2.open("D:\\delayData.txt",std::ios::out | std::ios::trunc);
        outfile2<<(int)par("totalNodes")<<endl;
        outfile2.close();

        std::ofstream outfile3;
        outfile3.open("D:\\consensusDelay.txt",std::ios::out | std::ios::trunc);
        outfile3<<"Mining Node num:"<<miningNodeIds.size()<<"  Senator Node num:"<<(int)par("numberOfSenatorThreshold")<<endl;
        outfile3.close();

        std::ofstream outfile4;
        outfile4.open("D:\\evilNodeCountsInMiningNodes.txt",std::ios::out | std::ios::trunc);
        outfile4<<"total evil nodes : "<<evilNodeIds.size()<<endl;
        outfile4.close();

    }
}

void App::handleMessage(cMessage *msg)
{
    numReceived++;

    if(msg == nodeEnrollDissemination){
        disseminateMyEnrollInfo();
    }
    else if(msg == timerCheckNodeSelf){
        if(checkNeighbours())
        {
            checkSenatorLife();
            checkMiningNodeTerm();
        }
        scheduleAt(simTime() + 1,timerCheckNodeSelf);
    }
    else if (msg == senatorBufferToList)
    {
        senatorRound++;
        updateSenatorNodeList();
        scheduleAt(SimTime(senatorRound)*par("SenatorLifeTime"),senatorBufferToList);
    }
    else if (msg == miningNodeBufferToList)
    {
        votesOfCandidate.clear();//test
        candidateNodeList.clear();//test
        miningNodeRound++;
        haveITakenConsensus = false;
        haveISendConfirmMsg = false;
        haveIDisseminateMiningNode = false;
        confirmMsgCount = 0;
        keepReceivingTransaction = true;
        blockDisseminationAck = 0;
        localMiningNode = tempLocalMiningNode;
        scheduleAt(SimTime(miningNodeRound)*par("TermOfMiningNode"),miningNodeBufferToList);
    }
    else if (msg == displaySenatorList)
    {
        EV<<"local senator list :"<<endl;
        if(SenatorList.size()!=0)
        {
            for(int i=0;i<SenatorList.size();i++)
            {
                EV<<i+1<<":"<<SenatorList[i].nodeId<<" "<<SenatorList[i].time<<" ("<<SenatorList[i].posX<<","<<SenatorList[i].posY<<")."<<endl;
            }
        }
        if(SenatorList.size()!=0)
        {
            for(std::vector<SenatorNodeElement>::iterator it = SenatorList.begin();it!=SenatorList.end();it++)
            {
                EV<<it->nodeId<<" "<<it->time<<" ("<<it->posX<<","<<it->posY<<")."<<endl;
            }
        }
        EV<<"local Mining node is "<<localMiningNode.nodeID<<endl;
        scheduleAt(simTime()+10,displaySenatorList);
    }
    else if (msg == timerInitTransaction) {
            generateNewTransaction();
    }
    else if (msg == timerSenatorDissemination){
        disseminateMeAsSenator();
    }
    else if (msg == timerTakePoMCConsensus){
        proposeCandidate();
    }
    else if (msg == timerConsensusInSenator){
        voteToCandidate();
    }
    else {
        receiveMessage(msg);
    }
}
void App::receiveMessage(cMessage *msg)
{
    if (hasGUI())
        getParentModule()->bubble("Arrived!");
    Packet *pk = check_and_cast<Packet *>(msg);
    int hopcount = pk->getHopCount();
    //hopCountVector.record(hopcount);
    //hopCountStats.collect(hopcount);

    //emit(endToEndDelaySignal, simTime() - pk->getCreationTime());//当前仿真的时间减去该消息生成时的时间得到该消息传播的时间
    //emit(hopCountSignal, pk->getHopCount());//跳数
    //emit(sourceAddressSignal, pk->getSrcAddr());//源地址

    switch (pk->getPacketType()) {
        case 1: { // Senator dissemination Received
                    if((int)(SIMTIME_DBL(simTime())/(double)par("SenatorLifeTime")) == (int)(pk->getTime()/(double)par("SenatorLifeTime")))
                    {
                        if (pk->getUserXID() != myAddress) {
                            char key[128];
                            sprintf(key, "%d %f", pk->getUserXID(), pk->getTime());
                            if (!haveISeenThisSenatorDisseminationBefore(key)) {
                                //updateSenatorBuffer(pk->getUserXID());
                                updateSenatorBuffer(pk);
                                SenatorDisseminationMessageSet.insert(key);
                                reDisseminateSenatorMessage(pk);
                            }
                        }
                        break;
                    }
                }
        case 2: {//MiningNode received a transaction
                if(keepReceivingTransaction)
                {
                   EV<<"Mining node "<<myAddress<<" receive a new transaction from node "<<pk->getSrcAddr()<<",and the transaction ID is "<<pk->getTransactionID()<<endl;
                   dealTransaction(pk);
                }
                   break;
        }
        case 3:{//received a block dissemination
                int tempBlockID = pk->getBlockID();
                if(!haveISeenThisBlockDisseminationBefore(tempBlockID)){
                   std::vector<PoMCblockElement>::reverse_iterator it = localBlockChain.rbegin();
                   EV<<"Received block ID is :"<<pk->getBlockID()<<endl;
                   EV<<"LocalBlockChain PreHash is:"<<(*it).blockID<<endl;
                  //PoMCblockElement *p = &localBlockChain[localBlockChain.size()-1];
                  if(pk->getPreHash() ==  (*it).blockID)
                  {
                      PoMCblockElement tempBlock;
                      tempBlock.blockID = pk->getBlockID();
                      tempBlock.preHash = pk->getPreHash();
                      for(int i=0;i<pk->getTransactionIdMsgArraySize();i++){
                          PoMCtransactionElement tempTransaction(pk->getTransactionNodeID(i),pk->getTransactionIdMsg(i),pk->getTransactionVal(i),pk->getTransactionTime(i));
                          tempBlock.TransactionElement.push_back(tempTransaction);
                      }

                      localBlockChain.push_back(tempBlock);
                      EV<<"New block attaches to my local chain!"<<endl;
                      replyAckToMiningNode(localMiningNode.nodeID,tempBlock.blockID);

                          for(std::vector<PoMCblockElement>::iterator it = localBlockChain.begin();it!=localBlockChain.end();it++)
                              {
                                  EV<<"blockID="<<(*it).blockID<<"  preHash="<<(*it).preHash<<endl;
                              }
                      std::vector<PoMCblockElement>::iterator bl = localBlockChain.end()-1;
                      std::vector<PoMCtransactionElement>::iterator tr;
                      EV<<"the transactions of the new block are :"<<endl;
                      for(tr = (*bl).TransactionElement.begin(); tr!=(*bl).TransactionElement.end(); tr++)
                      {
                          EV<<"Node ID :"<<tr->nodeID<<" ,Transaction ID :"<<tr->transactionID<<" ,Transaction Value :"<<tr->transactionValue
                          <<" ,Transaction time :"<<tr->TimeOfCreateTransaction<<"."<<endl;
                          if(tr->nodeID == myAddress)
                          {
                              simtime_t transDelay = simTime() - tr->TimeOfCreateTransaction;
                              emit(transDelaySignal,transDelay);
                              std::ofstream outfile;
                              outfile.open("D:\\delayData.txt",std::ios::out | std::ios::app);
                              outfile<<transDelay<<endl;
                              outfile.close();
                          }
                      }
                  }
                  reDisseminateBlockMessage(pk);
                }
                break;
        }
        case 31:
        {
                if(pk->getBlockID() == blockID)
                {
                    blockDisseminationAck++;
                    if(blockDisseminationAck >= 0.5*par("totalNodes").intValue())
                    {
                        keepReceivingTransaction = true;
                        blockDisseminationAck = 0;
                        blockID++;
                    }
                }
                break;
        }
        case 4:{//timer for generating transaction
                if(timerInitTransaction->isScheduled())
                    cancelEvent(timerInitTransaction);
                scheduleAt(simTime() + par("generateTransactionTimeInit").doubleValue(), timerInitTransaction);
                break;
        }
        case 5:{//received a node enroll message
                int tempNodeID = pk->getNodeID();
                if(!haveIseenThisNodeDisseminationBefore(tempNodeID)){
                    int tempPosX = pk->getPosX();
                    int tempPosY = pk->getPosY();
                    NodePos tempNode(tempNodeID,tempPosX,tempPosY);
                    totalNodeView.push_back(tempNode);
                    reDisseminateEnrollInfo(pk);
                }
                break;
        }
        case 6:{//senator node received a consensus message
                if(simTime()<=pk->getTime()+10)
                {
                    EV<<"Received new consensus message from "<<pk->getNodeID()<<endl;
                    std::vector<int> canNeighbours;
                    std::vector<int> *p = &canNeighbours;
                    EV<<pk->getNeighboursArraySize()<<endl;
                    for(int i=0; i < pk->getNeighboursArraySize();i++)
                    {
                        canNeighbours.push_back(pk->getNeighbours(i));
                        EV<<pk->getNeighbours(i)<<" ";
                    }
                    EV<<endl;
                    CandidateNode tempNode(pk->getNodeID(),pk->getCandidateTime(),pk->getMF(),pk->getPosX(),pk->getPosY(),p);
                    if(tempNode.MF > can.MF)
                    {
                        can = tempNode;
                        EV<<"Current candidate node is "<<can.nodeID<<",candidate time:"<<can.time<<",MF:"<<pk->getMF()<<",coordinate:("<<pk->getPosX()<<","
                          <<pk->getPosY()<<"),neighbours:";
                        for(int i=0;i<pk->getNeighboursArraySize();i++)
                            EV<<can.neighbours[i]<<" ";
                        EV<<"."<<endl;
                    }
                }
                if(timerTakePoMCConsensus->isScheduled())
                    break;
                else
                {   scheduleAt(simTime()+5,timerTakePoMCConsensus);
                    break;
                }
        }
        case 7:{//senator node received a candidate node from other senator node
                if(pk->getNodeID() != -1)
                {
                    EV<<"Packet type 7 : received a candidate node "<<pk->getNodeID()<<" from senator "<<pk->getSrcAddr()<<endl;
                    std::vector<int> canNeighbours;
                    std::vector<int> *p = &canNeighbours;
                    for(int i=0;i<pk->getNeighboursArraySize();i++)
                    {
                        canNeighbours.push_back(pk->getNeighbours(i));
                    }
                    CandidateNode tempNode(pk->getNodeID(),pk->getCandidateTime(),pk->getMF(),pk->getPosX(),pk->getPosY(),p);
                    candidateNodeList.push_back(tempNode);
                    for(int i=0;i<candidateNodeList.size();i++){
                        EV<<" candidate node  "<<candidateNodeList[i].nodeID<<",candidate time:"<<candidateNodeList[i].time<<",MF:"<<candidateNodeList[i].MF
                          <<",coordinate:"<<pk->getPosX()<<","<<pk->getPosY()<<"),neighbours :";
                        for(int j=0;j<pk->getNeighboursArraySize();j++)
                            EV<<candidateNodeList[i].neighbours[j]<<" ";
                        EV<<endl;
                    }
                }
                if(can.nodeID == -1)
                {
                    proposeCandidate();
                    haveIProposedCandidate = true;
                }
                replySenatorAckTo(pk->getSrcAddr());
                break;
        }
        case 8:{//senator node received a reply from other senator nodes

                receivedSenatorAck++;
                if(receivedSenatorAck >= SenatorList.size()-1)
                {
                    if(timerConsensusInSenator->isScheduled())
                        break;
                    else
                        scheduleAt(simTime(),timerConsensusInSenator);
                    receivedSenatorAck = 0;
                }
                break;
        }
        case 9 :{//senator received the vote message on candidates from other senator nodes
                EV<<"Receive the votes of candidates from senator node"<<pk->getSrcAddr()<<endl;
                std::map<int,int>::iterator it = votesOfCandidate.begin();
                if(votesOfCandidate.size() == 0)
                {
                    for(int i = 0; i<candidateNodeList.size()-1;i++)//根据MF冒泡排序
                        for(int j = 0;j<candidateNodeList.size()-1;j++)
                        {
                            if(candidateNodeList[j].MF < candidateNodeList[j+1].MF)
                            {
                                CandidateNode temp = candidateNodeList[j];
                                candidateNodeList[j] = candidateNodeList[j+1];
                                candidateNodeList[j+1] = temp;
                            }
                        }
                    for(int i=0;i<pk->getVoteArraySize();i++)
                    {
                        if(pk->getVote(i))
                            votesOfCandidate[candidateNodeList[i].nodeID] = 1;
                        else
                            votesOfCandidate[candidateNodeList[i].nodeID] = 0;
                    }
                }
                else{
                    for(int i=0;i<pk->getVoteArraySize();i++)
                    {
                        if(pk->getVote(i))
                            it->second++;
                        it++;
                    }
                }
                for(it=votesOfCandidate.begin();it!=votesOfCandidate.end();it++)
                {
                    EV<<it->first<<" "<<it->second<<endl;
                }
                if(haveISendConfirmMsg)
                    EV<<"have I sent confirm msg? yes!"<<endl;
                else
                    EV<<"have I sent confirm msg? no!"<<endl;
                for(it=votesOfCandidate.begin();it!=votesOfCandidate.end();it++)
                {
                    if(it->second > SenatorList.size()*2/3 && !haveISendConfirmMsg)
                    {
                        sendConfirmMsg(it->first);
                        miningNodeBuffer = it->first;
                        EV<<"I am going to propose candidate "<<miningNodeBuffer<<endl;
                        haveISendConfirmMsg = true;
                        break;
                    }
                }
                break;
        }
        case 10:{
                //TODO:Here is a situation that when a sentor receive a confirm
                //message,the senator node might not determine the miningNodeBuffer.
                if(miningNodeBuffer == pk->getNodeID())
                {
                    replyConfirmMsg(pk->getSrcAddr());
                }
                break;
        }
        case 11:{//received a confirm msg and is going to decide whether disseminate a mining node
                confirmMsgCount++;
                EV<<"confirmMsgCount = "<<confirmMsgCount<<endl;
                if(confirmMsgCount > SenatorList.size()*2/3 && !haveIDisseminateMiningNode)
                {
                    EV<<"prepare to disseminate mining node "<<miningNodeBuffer<<endl;
                    disseminateMiningNode(miningNodeBuffer);
                    haveIDisseminateMiningNode = true;
                }
                break;
        }
        case 12:{//received a mining node dissemination from senator node
                int tempID = pk->getNodeID();
                if(!haveIseenThisMiningNodeDisseminationBefore(tempID))
                {
                    MiningNode tempNode(tempID,pk->getCandidateTime(),pk->getMF());
                    tempLocalMiningNode = tempNode;
                    reDisseminateMiningNode(pk);

                    simtime_t consensusDelay = simTime() - pk->getCandidateTime();
                    std::ofstream consdelayfile;
                    consdelayfile.open("D:\\consensusDelay.txt",std::ios::out | std::ios::app);
                    consdelayfile<<consensusDelay<<endl;
                    consdelayfile.close();

                    std::vector<int>::iterator eit = find(evilNodeIds.begin(),evilNodeIds.end(),tempID);
                    if(eit != evilNodeIds.end())
                    {
                        if(evilMiningNodes.size() == 0)
                            evilMiningNodes.push_back(tempID);
                        else
                        {
                            std::vector<int>::iterator e_it = find(evilMiningNodes.begin(),evilMiningNodes.end(),tempID);
                            if(e_it == evilMiningNodes.end())
                                evilMiningNodes.push_back(tempID);
                        }
                    }
                }
                break;
        }
        default: {
            break;
        }
    }
    delete pk;

}

//PoMC
void App::disseminateMiningNode(int nodeID)
{
    double MF;
    simtime_t candidateTime;
    for(int i=0;i<candidateNodeList.size();i++)
    {
        if(candidateNodeList[i].nodeID == nodeID)
        {
            MF = candidateNodeList[i].MF;
            candidateTime = candidateNodeList[i].time;
            break;
        }
    }
    cModule *mod = getParentModule()->getSubmodule("routing");
    Routing *myRouting = check_and_cast<Routing*>(mod);
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;
    RandomDistinctPicker *rand = new RandomDistinctPicker(0, neighbourNodeAddresses.size() - 1, par("randomSeed"));
    for (int i = 0; i < neighbourNodeAddresses.size(); i++)
    {
        int destAddress = neighbourNodeAddresses[rand->getRandomNumber()];
        char pkname[80];
        sprintf(pkname,"#%ld,Senator node %d ,disseminate mining node %d",pkCounter++,myAddress,nodeID);
        Packet *pk = new Packet(pkname);
        pk->setByteLength(packetLengthBytes->intValue());
        pk->setSrcAddr(myAddress);
        pk->setDestAddr(destAddress);
        pk->setTime(SIMTIME_DBL(simTime()));
        pk->setPacketType(12);
        pk->setMF(MF);
        pk->setNodeID(nodeID);
        pk->setCandidateTime(SIMTIME_DBL(candidateTime));
        send(pk,"out");
        numSent++;

    }
    delete rand;
    haveIProposedCandidate = false;
    candidateNodeList.clear();
    confirmMsgCount = 0;
    haveISendConfirmMsg = false;
    votesOfCandidate.clear();
}

bool App::haveIseenThisMiningNodeDisseminationBefore(int nodeID)
{
    if(tempLocalMiningNode.nodeID == nodeID)
        return true;
    else
        return false;
}

void App::reDisseminateMiningNode(Packet *pk)
{
    //TODO: this might need anti-loop control system
        cModule *mod = getParentModule()->getSubmodule("routing");
        Routing *myRouting = check_and_cast<Routing*>(mod);
        std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;

        for (int i = 0; i < neighbourNodeAddresses.size(); i++) {
            if (pk->getSrcAddr() == neighbourNodeAddresses[i])
                continue;

            Packet *copy = (Packet *) pk->dup();
            char pkname[80];
            sprintf(pkname, "#%ld from-%d-to-%d Mining node dissemination", pkCounter++, myAddress, neighbourNodeAddresses[i]);

            if (hasGUI())
                getParentModule()->bubble("Generating packet...");

            copy->setName(pkname);
            copy->setSrcAddr(myAddress);
            copy->setDestAddr(neighbourNodeAddresses[i]);
            send(copy, "out");

            numSent++;
        }
}


void App::replyConfirmMsg(int dest)
{
    //Create Packet
    char pkname[80];
    sprintf(pkname,"#%ld,Senator node %d reply a confirm at %f",pkCounter++,myAddress,SIMTIME_DBL(simTime()));
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(dest);
    pk->setTime(SIMTIME_DBL(simTime()));
    pk->setPacketType(11);
    send(pk,"out");


    numSent++;
}

void App::sendConfirmMsg(int nodeID)
{
    for(int i=0;i<SenatorList.size();i++)
    {
        if(SenatorList[i].nodeId!=myAddress)
        {
            //Create Packet
            char pkname[80];
            sprintf(pkname,"#%ld,%d to %d,Senator node %d is going to disseminate candidate node %d at %f",pkCounter++,myAddress,SenatorList[i].nodeId,myAddress,nodeID,SIMTIME_DBL(simTime()));
            Packet *pk = new Packet(pkname);
            pk->setByteLength(packetLengthBytes->intValue());
            pk->setSrcAddr(myAddress);
            pk->setDestAddr(SenatorList[i].nodeId);
            pk->setTime(SIMTIME_DBL(simTime()));
            pk->setNodeID(nodeID);
            pk->setPacketType(10);
            send(pk,"out");


            numSent++;
        }
    }
}

void App::voteToCandidate()
{
    int i;
    int j;
    can.nodeID = -1;
    can.MF = -1;
    haveIProposedCandidate = false;//the situation that a senator do not receive any consensus message.
    EV<<"Beginning to vote to candidates!"<<endl;
    bool voteAggregation[candidateNodeList.size()];
    bool SINRVerify,RepVerify = false;
    //std::vector<CandidateNode>::iterator it = candidateNodeList.begin();
    for(i = 0; i<candidateNodeList.size()-1;i++)//根据MF冒泡排序
        for(j = 0;j<candidateNodeList.size()-1;j++)
        {
            if(candidateNodeList[j].MF < candidateNodeList[j+1].MF)
            {
                CandidateNode temp = candidateNodeList[j];
                candidateNodeList[j] = candidateNodeList[j+1];
                candidateNodeList[j+1] = temp;
            }
        }
    for(i=0;i<candidateNodeList.size();i++)
    {
        EV<<" candidate node  "<<candidateNodeList[i].nodeID<<",candidate time:"<<candidateNodeList[i].time<<",MF:"<<candidateNodeList[i].MF
        <<",coordinate:("<<candidateNodeList[i].posX<<","<<candidateNodeList[i].posY<<"),neighbours :";
        for(j=0;j<candidateNodeList[i].neighbours.size();j++)
            EV<<candidateNodeList[i].neighbours[j]<<" ";
        EV<<endl;
    }

    for(std::vector<CandidateNode>::iterator it = candidateNodeList.begin(); it!=candidateNodeList.end();it++)
    {
        if(checkSINR(it) && checkRep((it)))
        {
            EV<<"validation : TRUE!"<<endl;
            voteList.push_back(true);
        }
        else
        {
            EV<<"validation : False!"<<endl;
            voteList.push_back(false);
        }
    }
    for(i=0;i<voteList.size();i++)
    {
        if(voteList[i])
            EV<<"true";
        else
            EV<<"false";
    }
    EV<<endl;
    if(votesOfCandidate.size() == 0)
    {
        for(i=0;i<voteList.size();i++)
        {
            if(voteList[i])
                votesOfCandidate[candidateNodeList[i].nodeID] = 1;
            else
                votesOfCandidate[candidateNodeList[i].nodeID] = 0;

        }
    }
    else
    {
        i = 0;
        for(std::map<int,int>::iterator it=votesOfCandidate.begin();it!=votesOfCandidate.end();it++)
        {
            if(voteList[i])
                (*it).second++;
            i++;
        }
    }
    for(std::map<int,int>::iterator it=votesOfCandidate.begin();it!=votesOfCandidate.end();it++)
    {
        EV<<it->first<<" "<<it->second<<endl;
    }
    voteList.clear();
    for(i=0;i<SenatorList.size();i++)
    {
        if(SenatorList[i].nodeId!=myAddress)
        {
            //Create Packet
            char pkname[80];
            sprintf(pkname,"#%ld,%d to %d,Senator node %d has voted to the candidate nodes at %f",pkCounter++,myAddress,SenatorList[i].nodeId,myAddress,SIMTIME_DBL(simTime()));
            Packet *pk = new Packet(pkname);
            pk->setByteLength(packetLengthBytes->intValue());
            pk->setSrcAddr(myAddress);
            pk->setDestAddr(SenatorList[i].nodeId);
            pk->setTime(SIMTIME_DBL(simTime()));
            pk->setVoteArraySize(candidateNodeList.size());
            for(int i=0;i<candidateNodeList.size();i++)
                pk->setVote(i, voteList[i]);
            pk->setPacketType(9);
            send(pk,"out");

            numSent++;
        }
    }
}

bool App::checkRep(std::vector<CandidateNode>::iterator p)
{
    int canNodeID = (*p).nodeID;
    cModule *parent = getParentModule();
    cModule *tar = parent->getParentModule()->getSubmodule("rte",canNodeID);
    EV<<parent->getFullName()<<" "<<parent->getParentModule()->getFullName()<<" "<<tar->getFullName()<<endl;
    cModule *mod = tar->getSubmodule("app");
    EV<<mod->getName()<<endl;
    App *myApp = check_and_cast<App*>(mod);
    EV<<"the rep of node "<<canNodeID<<" is "<<myApp->rep<<endl;
    if(myApp->rep>5)
        return true;
    else
        return false;
}

bool App::checkSINR(std::vector<CandidateNode>::iterator p)
{
    int canNodeID = (*p).nodeID;
    //get the neighbours of candidate node
    cModule *parent = getParentModule();
    cModule *tar = parent->getParentModule()->getSubmodule("rte",canNodeID);
    EV<<parent->getFullName()<<" "<<parent->getParentModule()->getFullName()<<" "<<tar->getFullName()<<endl;
    cModule *mod = tar->getSubmodule("routing");
    EV<<mod->getName()<<endl;
    Routing *myRouting = check_and_cast<Routing*>(mod);
    EV<<myRouting->neighbourNodeAddresses.size()<<endl;
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;
    if((*p).neighbours.size()!=neighbourNodeAddresses.size())
    {
        EV<<"number of neighbours doesn't match."<<(*p).neighbours.size()<<" "<<neighbourNodeAddresses.size()<<endl;
        return false;
    }
    else
    {
        for(int i=0;i<neighbourNodeAddresses.size()-1;i++)
            for(int j=0;j<neighbourNodeAddresses.size()-1;j++)
            {
                if(neighbourNodeAddresses[j]<neighbourNodeAddresses[j+1])
                {
                    int temp = neighbourNodeAddresses[j];
                    neighbourNodeAddresses[j]  = neighbourNodeAddresses[j+1];
                    neighbourNodeAddresses[j+1] = temp;
                }
            }
        for(int i=0;i<(*p).neighbours.size()-1;i++)
            for(int j=0;j<(*p).neighbours.size()-1;j++)
            {
                if((*p).neighbours[j]<(*p).neighbours[j+1])
                {
                    int temp = (*p).neighbours[j];
                    (*p).neighbours[j] = (*p).neighbours[j+1];
                    (*p).neighbours[j+1] = temp;
                }
            }
        for(int i=0;i<neighbourNodeAddresses.size();i++)
            EV<<neighbourNodeAddresses[i]<<" ";
        EV<<endl;
        for(int i=0;i<(*p).neighbours.size();i++)
            EV<<(*p).neighbours[i]<<" ";
        EV<<endl;
        if((*p).neighbours == neighbourNodeAddresses)
            return true;
        else
            return false;
    }
}

void App::replySenatorAckTo(int nodeID)
{
    //Create Packet
    char pkname[80];
    sprintf(pkname,"#%ld,Senator node %d has received your candidate node messeage at %f",pkCounter++,myAddress,SIMTIME_DBL(simTime()));
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(nodeID);
    pk->setTime(SIMTIME_DBL(simTime()));
    pk->setPacketType(8);
    send(pk,"out");

    numSent++;
}

void App::proposeCandidate()
{

    if(can.nodeID != -1)
    {
        candidateNodeList.push_back(can);
        for(int i=0;i<SenatorList.size();i++)
        {
            if(SenatorList[i].nodeId != myAddress)
            {
                //Create Packet
                char pkname[80];
                sprintf(pkname,"#%ld,Senator node %d proposes a candidate node at %f",pkCounter++,myAddress,SIMTIME_DBL(simTime()));
                Packet *pk = new Packet(pkname);
                pk->setByteLength(packetLengthBytes->intValue());
                pk->setSrcAddr(myAddress);
                pk->setDestAddr(SenatorList[i].nodeId);
                pk->setNodeID(can.nodeID);
                pk->setCandidateTime(SIMTIME_DBL(can.time));
                pk->setMF(can.MF);
                pk->setPosX(can.posX);
                pk->setPosY(can.posY);
                pk->setNeighboursArraySize(can.neighbours.size());
                for(int i=0;i<can.neighbours.size();i++)
                    pk->setNeighbours(i, can.neighbours[i]);
                pk->setTime(SIMTIME_DBL(simTime()));
                pk->setPacketType(7);
                send(pk,"out");

                numSent++;
            }
        }
    }
    else
    {
        if(!haveIProposedCandidate)
        {
            for(int i=0;i<SenatorList.size();i++)
            {
                if(SenatorList[i].nodeId != myAddress)
                {
                    //Create Packet
                    char pkname[50];
                    sprintf(pkname,"#%ld,Senator node %d receives none",pkCounter++,myAddress);
                    Packet *pk = new Packet(pkname);
                    pk->setByteLength(packetLengthBytes->intValue());
                    pk->setSrcAddr(myAddress);
                    pk->setDestAddr(SenatorList[i].nodeId);
                    pk->setNodeID(can.nodeID);
                    pk->setTime(SIMTIME_DBL(simTime()));
                    pk->setPacketType(7);
                    send(pk,"out");

                    numSent++;
                }
            }
        }
    }

}

void App::takePoMCConsensus()
{
    //TODO:动态创建连接更符合方案，但是目前做不出来。
    SenatorNodeElement targetSenator = SenatorList[0];
    double dis,tempDis;
    dis = sqrt(pow(posX-SenatorList[0].posX,2)+pow(posY-SenatorList[0].posY,2));
    EV<<"Distance to Senator Node "<<SenatorList[0].nodeId<<" is "<<dis<<endl;
    for(int i=1;i<SenatorList.size();i++)
    {
        tempDis = sqrt(pow(posX-SenatorList[i].posX,2)+pow(posY-SenatorList[i].posY,2));
        EV<<"Distance to Senator Node "<<SenatorList[i].nodeId<<" is "<<tempDis<<endl;
        if(tempDis < dis)
        {
            dis = tempDis;
            targetSenator = SenatorList[i];
        }
    }
    EV<<"Target Senator Node is "<<targetSenator.nodeId<<endl;

    double MF = calculateMF(targetSenator);

    cModule *mod = getParentModule()->getSubmodule("routing");
    Routing *myRouting = check_and_cast<Routing*>(mod);
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;
    EV<<"taking consensus,number of my neighbours is "<<neighbourNodeAddresses.size()<<", and the neighbours are :";
    for(int i=0 ;i < neighbourNodeAddresses.size(); i++)
        EV<<neighbourNodeAddresses[i]<<",";
    EV<<endl;


    //Create Packet
    char pkname[40];
    sprintf(pkname,"#%ld,node%d takes consensus at %f",pkCounter++,myAddress,SIMTIME_DBL(simTime()));
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(targetSenator.nodeId);
    pk->setNodeID(myAddress);
    pk->setMF(MF);
    pk->setNeighboursArraySize(neighbourNodeAddresses.size());
    for(int i=0 ;i < neighbourNodeAddresses.size(); i++){
       pk->setNeighbours(i, neighbourNodeAddresses[i]);
    }
    pk->setPosX(posX);
    pk->setPosY(posY);
    pk->setCandidateTime(SIMTIME_DBL(simTime()));
    pk->setTime(SIMTIME_DBL(simTime()));
    pk->setPacketType(6);
    send(pk,"out");

    numSent++;
}

double App::calculateMF(SenatorNodeElement targetNode)
{
    SenatorNodeElement targetSenator = targetNode;
    double MF;
    int N = (int)par("totalNodes");
    //int Ns = (int)par("numberOfSenatorThreshold");
    int Ns = SenatorList.size();
    double Pi = acos(-1);
    double a = par("PathLossExponent").doubleValue();
    double Pnoise = par("NoisePower").doubleValue();
    double beta1 = par("SINRThreshold").doubleValue();
    double beta2 = par("RepThreshold").doubleValue();
    double RepM = par("RepMean").doubleValue();
    double RepSD = par("RepStandardDeviation").doubleValue();
    EV<<"N="<<N<<" "<<"Ns="<<Ns<<" "<<"Pi="<<Pi<<" "<<"a="<<a<<" "<<"Pnoise="<<Pnoise<<" "<<"beta1="<<beta1<<" "<<"beta2="<<beta2<<endl;
    //获取senator的邻居节点地址及数量
    cModule *parent = getParentModule();
    cModule *tar = parent->getParentModule()->getSubmodule("rte",targetSenator.nodeId);
    EV<<parent->getFullName()<<" "<<parent->getParentModule()->getFullName()<<" "<<tar->getFullName()<<endl;
    cModule *mod = tar->getSubmodule("routing");
    EV<<mod->getName()<<endl;
    Routing *myRouting = check_and_cast<Routing*>(mod);
    EV<<myRouting->neighbourNodeAddresses.size()<<endl;
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;
    EV<<neighbourNodeAddresses.size()<<" "<<" neighbours of node "<<targetSenator.nodeId<<" are:";
    for(int i=0 ;i < neighbourNodeAddresses.size(); i++){

          EV<<neighbourNodeAddresses[i]<<" ";

    }
    EV<<endl;

    //Calculate the Probability of SINR >= beta1
    double accumulate = 0;
    //utilize for test node view
    /*for(int i=0;i<totalNodeView.size();i++)
    {
        EV<<totalNodeView[i].nodeID<<" ("<<totalNodeView[i].posX<<","<<totalNodeView[i].posY<<")"<<endl;
    }*/
    //Calculate the accumulating part of SINR
    for(int i=0;i<neighbourNodeAddresses.size();i++)
    {
        if(neighbourNodeAddresses[i] != myAddress)
        {
            for(int j = 0; j<totalNodeView.size(); j++)
            {
                if(totalNodeView[j].nodeID == neighbourNodeAddresses[i])
                {
                    accumulate += pow(sqrt(pow(totalNodeView[j].posX-targetSenator.posX,2)+pow(totalNodeView[j].posY-targetSenator.posY,2)),a);
                    EV<<"Distance between node "<<totalNodeView[j].nodeID<<" ("<<totalNodeView[j].posX<<","<<totalNodeView[j].posY<<")"<<" and target Senator "
                    <<targetSenator.nodeId<<" ("<<targetSenator.posX<<","<<targetSenator.posY<<") is "
                    <<sqrt(pow(totalNodeView[j].posX-targetSenator.posX,2)+pow(totalNodeView[j].posY-targetSenator.posY,2))<<endl;
                    break;
                }
            }
        }
    }
    double ita = pow(beta1*(Pnoise+accumulate),1/a);
    EV<<accumulate<<" "<<ita<<endl;
    double PrSINR = 1-exp(-Pi*Ns*ita*ita/N);

    //Calculate the probability of Rep>= beta2
    double PrRep = 0.5-0.5*(erf((beta2-RepM)/(sqrt(2)*RepSD)));

    EV<<"Pr(SINR>=beta1)="<<PrSINR<<",Pr(Rep>=beta2)="<<PrRep<<",MF="<<PrSINR*PrRep<<endl;

    MF = PrSINR*PrRep;

    return MF;
}

void App::checkMiningNodeTerm()
{
    bool amISenator = false;
    for(int i = 0; i<SenatorList.size(); i++)
    {
        if(myAddress == SenatorList[i].nodeId  || myAddress == SenatorBuffer[i].nodeId)
        {    amISenator = true;
             break;
        }
    }

    /*EV<<"am I senator?"<<endl;
    if(amISenator)
        EV<<"yes!"<<endl;
    else
        EV<<"no!"<<endl;

    EV<<"have I taken consensus?"<<endl;
    if(haveITakenConsensus)
        EV<<"yes!"<<endl;
    else
        EV<<"no!"<<endl;
    EV<<SimTime(miningNodeRound*(double)par("TermOfMiningNode")-(double)par("RedundantTimeOfConsensus"))<<endl;*/

    if(SimTime(miningNodeRound*(double)par("TermOfMiningNode")-(double)par("RedundantTimeOfConsensus")) <= simTime())
    {
        if(!amISenator && !haveITakenConsensus && myAddress != localMiningNode.nodeID && amIMiningNode)
        {
            EV<<"current mining node round is"<<miningNodeRound<<endl;
            takePoMCConsensus();
            haveITakenConsensus = true;
        }
    }
}

//Node Enroll
void App::disseminateMyEnrollInfo()
{
    cModule *mod = getParentModule()->getSubmodule("routing");
    Routing *myRouting = check_and_cast<Routing*>(mod);
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;
    RandomDistinctPicker *rand = new RandomDistinctPicker(0, neighbourNodeAddresses.size() - 1, par("randomSeed"));
    for (int i = 0; i < neighbourNodeAddresses.size(); i++) {
        int destAddress = neighbourNodeAddresses[rand->getRandomNumber()];
        char pkname[40];
        sprintf(pkname,"#%ld,Node %d ,coordinate(%d,%d)",pkCounter++,myAddress,posX,posY);
        if (hasGUI())
            getParentModule()->bubble("Generating packet...");
        Packet *pk = new Packet(pkname);
        pk->setByteLength(packetLengthBytes->intValue());
        pk->setSrcAddr(myAddress);
        pk->setDestAddr(destAddress);
        pk->setTime(SIMTIME_DBL(simTime()));
        pk->setPacketType(5);
        pk->setNodeID(myAddress);
        pk->setPosX(posX);
        pk->setPosY(posY);
        send(pk,"out");

        numSent++;
    }
    delete rand;
}

bool App::haveIseenThisNodeDisseminationBefore(int nodeID)
{
    for(std::vector<NodePos>::reverse_iterator it = totalNodeView.rbegin();it!=totalNodeView.rend();it++)
    {
        if((*it).nodeID == nodeID)
            return true;
    }
    return false;
}

void App::reDisseminateEnrollInfo(Packet *pk)
{
    //TODO: this might need anti-loop control system
        cModule *mod = getParentModule()->getSubmodule("routing");
        Routing *myRouting = check_and_cast<Routing*>(mod);
        std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;

        for (int i = 0; i < neighbourNodeAddresses.size(); i++) {
            if (pk->getSrcAddr() == neighbourNodeAddresses[i])
                continue;

            Packet *copy = (Packet *) pk->dup();
            char pkname[40];
            sprintf(pkname, "#%ld from-%d-to-%d Node Enroll dissemination", pkCounter++, myAddress, neighbourNodeAddresses[i]);

            if (hasGUI())
                getParentModule()->bubble("Generating packet...");

            copy->setName(pkname);
            copy->setSrcAddr(myAddress);
            copy->setDestAddr(neighbourNodeAddresses[i]);
            send(copy, "out");

            numSent++;
        }
}

//Senator
void App::takeSenatorSortition()
{
    if(timerSenatorDissemination->isScheduled())
        cancelEvent(timerSenatorDissemination);
    if(myAddress != (int)par("totalNodes")-1 && intuniform(0,100) < 30){
        scheduleAt(simTime() + par("SenatorDisseminationTimeInit").doubleValue(), timerSenatorDissemination);
    }
}
void App::disseminateMeAsSenator()
{
    cModule *mod = getParentModule()->getSubmodule("routing");
    Routing *myRouting = check_and_cast<Routing*>(mod);
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;
    RandomDistinctPicker *rand = new RandomDistinctPicker(0, neighbourNodeAddresses.size() - 1, par("randomSeed"));
    EV<<"I am node "<<myAddress<<" my neighbours are:";
    for(int i=0 ;i < neighbourNodeAddresses.size(); i++){

        EV<<neighbourNodeAddresses[i]<<" ";

    }
    EV<<endl;
    for (int i = 0; i < neighbourNodeAddresses.size(); i++) {
            // User selection
            int destAddress = neighbourNodeAddresses[rand->getRandomNumber()];

            char pkname[40];
            sprintf(pkname, "#%ld from-%d-to-%d Senator dissemination", pkCounter++, myAddress, destAddress);

            if (hasGUI())
                getParentModule()->bubble("Generating packet...");

            //Packet Creation
            Packet *pk = new Packet(pkname);
            pk->setByteLength(packetLengthBytes->intValue());
            pk->setSrcAddr(myAddress);
            pk->setDestAddr(destAddress);
            pk->setPacketType(1);
            pk->setTime(SIMTIME_DBL(simTime()));
            pk->setUserXID(myAddress);
            pk->setSenatorAddress(myAddress);
            pk->setPosX(posX);
            pk->setPosY(posY);
            sendDelayed(pk, uniform(0, 0.1), "out");
            updateSenatorBuffer(pk);

            numSent++;
        }
        delete rand;
        haveITakenSenatorLottery = true;
        //scheduleAt(simTime() + par("SenatorDisseminationTime").doubleValue(), timerSenatorDissemination);
}

void App::updateSenatorBuffer(int nodeId)
{
    bool alreadyPresent = false;
    for (int i = 0; i < SenatorList.size(); i++) {
        if (SenatorList[i].nodeId == nodeId) {
            //SenatorList[i].time = SIMTIME_DBL(simTime());
            alreadyPresent = true;
        }
    }

    if (alreadyPresent == false) {
       // SenatorList.push_back(*new SenatorNodeElement(nodeId, SIMTIME_DBL(simTime())));
    }
}

void App::updateSenatorBuffer(Packet *pk)
{
    bool alreadyPresent = false;
    for (int i = 0; i < SenatorBuffer.size(); i++) {
        if (SenatorBuffer[i].nodeId == pk->getSenatorAddress()) {
            //SenatorList[i].time = SIMTIME_DBL(simTime());
            alreadyPresent = true;
            EV<<"This senator already in my local buffer list."<<endl;
            break;
        }
    }
    if (alreadyPresent == false) {//收到的申请全都加入buffer，在buffertolist时再排序
        int nodeId = pk->getSenatorAddress();
        SenatorBuffer.push_back(*new SenatorNodeElement(nodeId, pk->getTime(),pk->getPosX(),pk->getPosY()));
        EV<<"new senator joins in my local buffer list.address:"<<pk->getSenatorAddress()<<" time:"<<pk->getTime()<<",coordinate is ("<<pk->getPosX()<<","<<pk->getPosY()<<")."<<endl;
        EV<<"local senator buffer list :"<<endl;
        for(int i=0;i<SenatorBuffer.size();i++){
             EV<<i+1<<":"<<SenatorBuffer[i].nodeId<<" "<<SenatorBuffer[i].time<<" ("<<SenatorBuffer[i].posX<<","<<SenatorBuffer[i].posY<<")."<<endl;
        }
    }
}

void App::updateSenatorNodeList()
{
    SenatorList.clear();
    if(SenatorBuffer.size()!=0){
        for(int i = 0; i<SenatorBuffer.size()-1;i++)//根据时间冒泡排序
            for(int j = 0;j<SenatorBuffer.size()-1;j++)
            {
                if(SenatorBuffer[j].time > SenatorBuffer[j+1].time)
                {
                    SenatorNodeElement temp (SenatorBuffer[j]);
                    SenatorBuffer[j] = SenatorBuffer[j+1];
                    SenatorBuffer[j+1] = temp;
                }
            }
        EV<<"local senator buffer list :"<<endl;
        for(int i=0;i<SenatorBuffer.size();i++)
        {
            EV<<i+1<<":"<<SenatorBuffer[i].nodeId<<" "<<SenatorBuffer[i].time<<" ("<<SenatorBuffer[i].posX<<","<<SenatorBuffer[i].posY<<")."<<endl;
        }
        for(int i = 0; i< (int)par("numberOfSenatorThreshold") && i<SenatorBuffer.size() ; i++)
        {
            SenatorNodeElement temp(SenatorBuffer[i]);
            SenatorList.push_back(temp);
        }
        SenatorBuffer.clear();
    }
    haveITakenSenatorLottery = false;

    /*if(myAddress != localMiningNode.nodeID)
    {
        std::vector<SenatorNodeElement>::iterator it;
        for(it = SenatorList.begin(); it != SenatorList.end(); it++)
        {
            if(it->nodeId == myAddress)
                break;
        }
        cModule *parent = getParentModule();
        cDisplayString& dispStr = parent->getDisplayString();

            if(it == SenatorList.end())
                dispStr.setTagArg("i", 0 , "misc/node_vs,white");
            else
                dispStr.setTagArg("i", 0 , "misc/node_vs,blue");
    }*/

}

void App::checkSenatorLife()
{

    int round = SIMTIME_DBL(simTime())/(double)par("SenatorLifeTime")+1;
    for(int i=0;i<SenatorList.size();i++){
        if(SimTime(round*(double)par("SenatorLifeTime")-(double)par("SenatorDisseminationTime")) <= simTime())//在SenatorRound结束前SenatorDisseminationTime时间内进行下一个round的预抽签
        {
            if(myAddress != localMiningNode.nodeID && myAddress != tempLocalMiningNode.nodeID &&!haveITakenSenatorLottery)
            {
                takeSenatorSortition();
                break;
            }
        }
    }
}

bool App::checkNeighbours()
{
    cModule *mod = getParentModule()->getSubmodule("routing");
    Routing *myRouting = check_and_cast<Routing*>(mod);
    if(myRouting->neighbourNodeAddresses.size() != 0)
        return true;
    else
        return false;
}

bool App::haveISeenThisSenatorDisseminationBefore(std::string key)
{
    if (SenatorDisseminationMessageSet.find(key) == SenatorDisseminationMessageSet.end())//.find()遍历容器，如果没找到就到容器末尾元素的下一位。.end()指向容器末尾元素的下一位
        return false;
    else
        return true;
}

void App::reDisseminateSenatorMessage(Packet *pk)
{
    //TODO: this might need anti-loop control system
    cModule *mod = getParentModule()->getSubmodule("routing");
    Routing *myRouting = check_and_cast<Routing*>(mod);
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;

    for (int i = 0; i < neighbourNodeAddresses.size(); i++) {
        if (pk->getSrcAddr() == neighbourNodeAddresses[i])
            continue;

        Packet *copy = (Packet *) pk->dup();
        char pkname[40];
        sprintf(pkname, "#%ld from-%d-to-%d Senator dissemination", pkCounter++, myAddress, neighbourNodeAddresses[i]);

        if (hasGUI())
            getParentModule()->bubble("Generating packet...");

        copy->setName(pkname);
        copy->setSrcAddr(myAddress);
        copy->setDestAddr(neighbourNodeAddresses[i]);
        send(copy, "out");

        numSent++;
    }
}

//transaction
void App::generateNewTransaction(){
    /*checkMiningNodeTerm();
    if(localMiningNode.size() == 0){
        scheduleAt(simTime() + par("generateTransactionTimeInit").doubleValue(), timerInitTransaction);
        return;
    }*/
    int transactionValue = uniform(0,1000);
    transactionID++;
    //Create Packet
    char pkname[40];
    sprintf(pkname,"node%d generates a new transaction at %f",myAddress,SIMTIME_DBL(simTime()));
    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(localMiningNode.nodeID);
    pk->setTime(SIMTIME_DBL(simTime()));
    pk->setTimeOfCreateTransaction(SIMTIME_DBL(simTime()));
    //pk->setTimeOfCreateTransaction(simTime());
    pk->setTransactionValue(transactionValue);
    pk->setTransactionSourceAddress(myAddress);
    pk->setTransactionID(transactionID);
    pk->setPacketType(2);
    send(pk,"out");

    numSent++;

}

void App::dealTransaction(Packet *pk)
{
    for(int i=0;i<TransactionPool.size();i++)
    {
        if(pk->getTransactionID() == TransactionPool[i].transactionID){
            return;
        }
    }


    int nodeID = pk->getTransactionSourceAddress();
    int transactionID = pk->getTransactionID();
    int transactionValue = pk->getTransactionValue();
    simtime_t time = SimTime(pk->getTimeOfCreateTransaction());
    PoMCtransactionElement *newTransaction = new PoMCtransactionElement(nodeID,transactionID,transactionValue,time);
    TransactionPool.push_back(*newTransaction);
    //PoMCblockElement *tempPreBlockHash = &(localBlockChain.back());

    //Reply an acknowledgement to src node.
    int destAddr,tempTransactionID;
    destAddr = pk->getTransactionSourceAddress();
    tempTransactionID = pk->getTransactionID();
    replyAckToSrcNode(destAddr,tempTransactionID);


    if(TransactionPool.size() >= blockSize && keepReceivingTransaction)//starting to pack the transactions in pool
    {
        keepReceivingTransaction = false;
        //blockID++;
        PoMCblockElement newblock;
        newblock.blockID = blockID;
        //newblock.preHash = tempPreBlockHash;
        newblock.preHash = blockID-1;
        for(int i=0;i<TransactionPool.size();i++){
            int tempNodeID = TransactionPool[i].nodeID;
            int tempTransactionID = TransactionPool[i].transactionID;
            int tempTransactionValue = TransactionPool[i].transactionValue;
            simtime_t tempTimeOfCreateTransaction = TransactionPool[i].TimeOfCreateTransaction;
            PoMCtransactionElement tempTransaction(tempNodeID,tempTransactionID,tempTransactionValue,tempTimeOfCreateTransaction);
            newblock.TransactionElement.push_back(tempTransaction);
        }
        localBlockChain.push_back(newblock);
        EV<<"New block attaches to my local chain!"<<endl;
        for(std::vector<PoMCblockElement>::iterator it = localBlockChain.begin();it!=localBlockChain.end();it++)
        {
            EV<<"blockID="<<(*it).blockID<<"  preHash="<<(*it).preHash<<endl;
        }

        //disseminate new block
        cModule *mod = getParentModule()->getSubmodule("routing");
        Routing *myRouting = check_and_cast<Routing*>(mod);
        std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;
        RandomDistinctPicker *rand = new RandomDistinctPicker(0, neighbourNodeAddresses.size() - 1, par("randomSeed"));
            for (int i = 0; i < neighbourNodeAddresses.size(); i++) {
                    // User selection
                    int destAddress = neighbourNodeAddresses[rand->getRandomNumber()];

                    char pkname[40];
                    sprintf(pkname, "#%ld from-%d-to-%d Block dissemination", pkCounter++, myAddress, destAddress);

                    if (hasGUI())
                        getParentModule()->bubble("Generating packet...");

                    //Packet Creation
                    Packet *pk = new Packet(pkname);
                    pk->setByteLength(packetLengthBytes->intValue());
                    pk->setSrcAddr(myAddress);
                    pk->setDestAddr(destAddress);
                    pk->setPacketType(3);
                    pk->setTime(SIMTIME_DBL(simTime()));
                    pk->setBlockID(newblock.blockID);
                    pk->setPreHash(newblock.preHash);
                    pk->setTransactionNodeIDArraySize(TransactionPool.size());
                    pk->setTransactionIdMsgArraySize(TransactionPool.size());
                    pk->setTransactionTimeArraySize(TransactionPool.size());
                    pk->setTransactionValArraySize(TransactionPool.size());
                    for(int j=0;j<TransactionPool.size();j++){
                        pk->setTransactionNodeID(j, TransactionPool[j].nodeID);
                        pk->setTransactionIdMsg(j, TransactionPool[j].transactionID);
                        pk->setTransactionTime(j, TransactionPool[j].TimeOfCreateTransaction);
                        pk->setTransactionVal(j, TransactionPool[j].transactionValue);
                    }
                    sendDelayed(pk, uniform(0, 0.1), "out");

                    rep += 1;
                    numSent++;
                }
                delete rand;



        TransactionPool.clear();


    }

    EV<<"TransactionPool.size()="<<TransactionPool.size()<<endl;
    EV<<"blockSize="<<blockSize<<endl;


}

bool App::haveISeenThisBlockDisseminationBefore(int blockID)
{
    for(std::vector<PoMCblockElement>::reverse_iterator it = localBlockChain.rbegin();it!=localBlockChain.rend();it++)
    {
        if ((*it).blockID == blockID)
            return true;
    }
    return false;
}

void App::reDisseminateBlockMessage(Packet *pk)
{
    //TODO: this might need anti-loop control system
    cModule *mod = getParentModule()->getSubmodule("routing");
    Routing *myRouting = check_and_cast<Routing*>(mod);
    std::vector<int> neighbourNodeAddresses = myRouting->neighbourNodeAddresses;

    for (int i = 0; i < neighbourNodeAddresses.size(); i++) {
        if (pk->getSrcAddr() == neighbourNodeAddresses[i])
            continue;

        Packet *copy = (Packet *) pk->dup();
        char pkname[40];
        sprintf(pkname, "#%ld from-%d-to-%d Block dissemination", pkCounter++, myAddress, neighbourNodeAddresses[i]);

        if (hasGUI())
            getParentModule()->bubble("Generating packet...");

        copy->setName(pkname);
        copy->setSrcAddr(myAddress);
        copy->setDestAddr(neighbourNodeAddresses[i]);
        send(copy, "out");

        numSent++;
    }
}

void App::replyAckToSrcNode(int destAddr , int tempTransactionID)
{
    char pkname[40];
    sprintf(pkname,"#%ld from %d to %d ,Mining node %d receives the transaction %d.",pkCounter++,myAddress,destAddr,myAddress,tempTransactionID);
    Packet *pk = new Packet(pkname);
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddr);
    pk->setTime(SIMTIME_DBL(simTime()));
    pk->setPacketType(4);
    send(pk,"out");

    numSent++;
}

void App::replyAckToMiningNode(int nodeID, int blockID)
{
    char pkname[40];
    sprintf(pkname,"#%ld from %d to %d , node %d receives the block %d.",pkCounter++,myAddress,nodeID,myAddress,blockID);
    Packet *pk = new Packet(pkname);
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(nodeID);
    pk->setTime(SIMTIME_DBL(simTime()));
    pk->setBlockID(blockID);
    pk->setPacketType(31);
    send(pk,"out");

    numSent++;
}

//SIMULATION

void App::simulationRegisterTransactionTime(int nodeId)    // Here are recorded the transaction starting times for all evil nodes
{
    int i = 0, alreadyRegistered = 0;

    for (i = 0; i < simulationTiming.size(); i++) {
        if (simulationTiming[i].nodeId == nodeId) {
            alreadyRegistered = 1;
            simulationTiming[i].transactionTime = simTime();
        }
    }

    if (alreadyRegistered == 0) {
        SimulationTiming *timeLog = new SimulationTiming(nodeId, simTime(), 0);
        simulationTiming.push_back(*timeLog);
    }
}
void App::simulationRegisterDetectionTime(int nodeId) // Here are logged the evil nodes detection times
{
    int i = 0;

    for (i = 0; i < simulationTiming.size(); i++) {
        if (simulationTiming[i].nodeId == nodeId) {
            if (simulationTiming[i].detectionTime == 0) { // In case of multiple detection caused by dissemination_detection we just need the quickest time
                simulationTiming[i].detectionTime = simTime();
            }
        }
    }
}
void App::stopSimulation() // This function is being called when ever an evil node is being detected
{
    int i, nodesDetected = 0;
    for (i = 0; i < simulationTiming.size(); i++) {
        if (simulationTiming[i].detectionTime != 0) {
            nodesDetected++;
        }
    }

// If there has been detected enough nodes equal to the evil node numbers stop simulation
    if (nodesDetected == evilNodeIds.size()) {
        char text[128];
        for (i = 0; i < simulationTiming.size(); i++) {
            sprintf(text, "Simulation: evil node #%d delta detection time:  %s", simulationTiming[i].nodeId, SIMTIME_STR(simulationTiming[i].detectionTime - simulationTiming[i].transactionTime));
            getSimulation()->getActiveEnvir()->alert(text);
        }

        simulationTiming.clear();
        endSimulation();
    }
}

//GENERAL USE
void App::createDirectChannel(int nodeId)
{
    cModule *target = getParentModule()->getParentModule()->getSubmodule("rte", nodeId)->getSubmodule("queue", 0);
    cDatarateChannel *channel = cDatarateChannel::create("channel");
    channel->setDelay((double) getParentModule()->getParentModule()->par("delay"));
    channel->setDatarate((double) getParentModule()->getParentModule()->par("datarate") * 1000);
    this->gate("direct")->connectTo(target->gate("direct"), channel);
}
void App::closeDirectChannel()
{
    this->gate("direct")->disconnect();
}

int App::randomNodeAddressPicker()
{
    int destAddress = -1;

    while (destAddress == -1 || destAddress == myAddress) {
        destAddress = destAddresses[intuniform(0, destAddresses.size() - 1)];
    }

    return destAddress;
}


void App::refreshDisplay() const
{
    char buf[40];
    sprintf(buf,"received: %ld sent: %ld",numReceived,numSent);
    getDisplayString().setTagArg("t",0,buf);
}

void App::finish()
{
    // This function is called by OMNeT++ at the end of the simulation.
    EV << "Sent:     " << numSent << endl;
    EV << "Received: " << numReceived << endl;
    //EV<<"Hop count, min:    "<<hopCountStats.getMin()<<endl;
    //EV<<"Hop count, max:    "<<hopCountStats.getMax()<<endl;
    //EV<<"Hop count, mean:    "<<hopCountStats.getMean()<<endl;
    //EV<<"Hop count, stddev:    "<<hopCountStats.getStddev()<<endl;

    std::ofstream outfile;
    outfile.open("D:\\expData.txt",std::ios::out | std::ios::app);
    outfile<<numSent<<","<<numReceived<<","<<blockID<<","<<transactionID<<endl;
    outfile.close();
    if(myAddress == 0)
    {
        std::ofstream evilNodeCounts;
        evilNodeCounts.open("D:\\evilNodeCountsInMiningNodes.txt",std::ios::out | std::ios::app);
        evilNodeCounts<<evilMiningNodes.size()<<endl;
        for(int i = 0; i < evilMiningNodes.size(); i++)
            evilNodeCounts<<evilMiningNodes[i]<<" ";
        evilNodeCounts<<endl;
        evilNodeCounts.close();
    }

    recordScalar("#sent", numSent);//recordScalar() writes into the .sca file
    recordScalar("#received", numReceived);
    recordScalar("total Block",blockID);
    recordScalar("total transaction",transactionID);
    //hopCountStats.recordAs("hop count");

}
