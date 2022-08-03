#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "Routing.h"

using namespace omnetpp;

Define_Module(Routing);

//针对一个节点来讲，在初始化的时候发现与其他节点之间的连接，但是gate那里没看懂（可能
//是通过最短路径算法，计算出到达每个目标节点的最短路径后，计算出第一跳该向谁发送）。
//补充：可能是收到消息后，判断该消息该向谁发送，即判断该消息该发送到哪个端口
void Routing::initialize() {
    myAddress = getParentModule()->par("address");

    dropSignal = registerSignal("drop");
    outputIfSignal = registerSignal("outputIf");

    //
    // Brute force approach -- every node does topology discovery on its own,
    // and finds routes to all other nodes independently, at the beginning
    // of the simulation. This could be improved: (1) central routing database,
    // (2) on-demand route calculation
    //
    cTopology *topo = new cTopology("topo");

    std::vector<std::string> nedTypes;
    nedTypes.push_back(getParentModule()->getNedTypeName());
    topo->extractByNedTypeName(nedTypes);
    EV << "cTopology found " << topo->getNumNodes() << " nodes\n";

    cTopology::Node *thisNode = topo->getNodeFor(getParentModule());

    // find and store next hops
    for (int i = 0; i < topo->getNumNodes(); i++) {
        if (topo->getNode(i) == thisNode)
            continue;  // skip ourselves
        topo->calculateUnweightedSingleShortestPathsTo(topo->getNode(i));

        if (thisNode->getNumPaths() == 0)
            continue;  // not connected

        cGate *parentModuleGate = thisNode->getPath(0)->getLocalGate();
        int gateIndex = parentModuleGate->getIndex();
        int address = topo->getNode(i)->getModule()->par("address");
        rtable[address] = gateIndex;
        EV << "  towards address " << address << " gateIndex is " << gateIndex
                  << endl;

        if (thisNode->getDistanceToTarget() == 1) {
            neighbourNodeAddresses.push_back(i);
        }

    }

    delete topo;
}

void Routing::handleMessage(cMessage *msg) {
    Packet *pk = check_and_cast<Packet *>(msg);

    if (pk->getDestAddr() == myAddress) {
        EV << "local delivery of packet " << pk->getName() << endl;
        send(pk, "localOut");
        emit(outputIfSignal, -1);  // -1: local
        return;
    } else {
        sendOut(pk);
    }
}

void Routing::sendOut(Packet *pk) {
    RoutingTable::iterator it = rtable.find(pk->getDestAddr());//.find()返回该元素在容器中的位置
    if (it == rtable.end()) {//.end()指向的是容器尾部元素下一个位置的迭代器，也就是一个不存在的元素
        EV << "address " << pk->getDestAddr()
                  << " unreachable, discarding packet " << pk->getName()
                  << endl;
        emit(dropSignal, (long) pk->getByteLength());
        delete pk;
        return;
    }

    int outGateIndex = (*it).second;//it指向的是一个map类型的容器，该容器存储的数据是成对的，.second得到的是
                                    //一对数据中第二个数据。
    EV << "forwarding packet " << pk->getName() << " on gate index "
              << outGateIndex << endl;
    pk->setHopCount(pk->getHopCount() + 1);
    emit(outputIfSignal, outGateIndex);

    send(pk, "out", outGateIndex);
}
