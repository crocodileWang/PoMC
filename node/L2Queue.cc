#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * Point-to-point interface module. While one frame is transmitted,
 * additional frames get queued up; see NED file for more info.
 */
class L2Queue : public cSimpleModule
{
  private:
    long frameCapacity;

    cQueue queue;
    cMessage *endTransmissionEvent;
    bool isBusy;

    simsignal_t qlenSignal;
    simsignal_t busySignal;
    simsignal_t queueingTimeSignal;
    simsignal_t dropSignal;
    simsignal_t txBytesSignal;
    simsignal_t rxBytesSignal;

  public:
    L2Queue();
    virtual ~L2Queue();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    virtual void startTransmitting(cMessage *msg);
};

Define_Module(L2Queue);

L2Queue::L2Queue()
{
    endTransmissionEvent = nullptr;
}

L2Queue::~L2Queue()
{
    cancelAndDelete(endTransmissionEvent);
}
//该程序创建了一个消息queue，并创建了不同的signal
void L2Queue::initialize()
{
    queue.setName("queue");
    endTransmissionEvent = new cMessage("endTxEvent");

    if (par("useCutThroughSwitching"))
      // 此方法只能在简单模块的输入门上调用。长度不为零的消息
      //则具有非零传输持续时间（因此，连接另一端的接收持续时间）。
      //默认情况下，将消息传递到模块标志着接收的结束。设置此位将导
      //致通道在接收开始时将消息传递到模块。接收将采取的持续时间可
      //以通过其 getDuration（） 方法从消息对象中提取。
        gate("line$i")->setDeliverOnReceptionStart(true);//意思就是接收者在接收到消息时立刻发送一个消息，而不是等待消息全部接收完毕才发送消息

    frameCapacity = par("frameCapacity");

    qlenSignal = registerSignal("qlen");//registerSignal（）方法将信号名称作为参数，并返回相应的simsignal_t值。
    busySignal = registerSignal("busy");
    queueingTimeSignal = registerSignal("queueingTime");
    dropSignal = registerSignal("drop");
    txBytesSignal = registerSignal("txBytes");
    rxBytesSignal = registerSignal("rxBytes");

    emit(qlenSignal, queue.getLength());
    emit(busySignal, false);
    isBusy = false;
}

void L2Queue::startTransmitting(cMessage *msg)
{
    EV << "Starting transmission of " << msg << endl;
    isBusy = true;
    int64_t numBytes = check_and_cast<cPacket *>(msg)->getByteLength();//check_and_cast<A *>:转换为A类型的指针，失败则返回NULL
    send(msg, "line$o");

    emit(txBytesSignal, (long)numBytes);

    // Schedule an event for the time when last bit will leave the gate.
    simtime_t endTransmission = gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
    scheduleAt(endTransmission, endTransmissionEvent);//当传输完成时自己给自己传输一个信息
}

void L2Queue::handleMessage(cMessage *msg)
{
    if (msg == endTransmissionEvent) {//情况1：收到了自己发送给自己的消息，该消息为line88发送的。
                                      //收到该消息后开始传送queue内下一则消息。
        // Transmission finished, we can start next one.
        EV << "Transmission finished.\n";
        isBusy = false;
        if (queue.isEmpty()) {//isEmpty():如果为空返回true，其他情况返回false
            emit(busySignal, false);
        }
        else {
            msg = (cMessage *)queue.pop();//pop()可以返回被删除的元素
            emit(queueingTimeSignal, simTime() - msg->getTimestamp());
            emit(qlenSignal, queue.getLength());
            startTransmitting(msg);
        }
    }
    else if (msg->arrivedOn("line$i") || msg->arrivedOn("direct")) {//情况2：可能是转发其它模块发来的消息
        // pass up
        emit(rxBytesSignal, (long)check_and_cast<cPacket *>(msg)->getByteLength());
        send(msg, "out");
    }
    else {  // arrived on gate "in"//情况3：收到消息
        //若有endTransmissionEvent在安排中（在计划中），判断队列是否满，如果满了则丢弃，不满则入栈
        if (endTransmissionEvent->isScheduled()) {//The isScheduled() method returns true if the message is currently scheduled.
            // We are currently busy, so just queue up the packet.
            if (frameCapacity && queue.getLength() >= frameCapacity) {
                EV << "Received " << msg << " but transmitter busy and queue full: discarding\n";
                emit(dropSignal, (long)check_and_cast<cPacket *>(msg)->getByteLength());
                delete msg;
            }
            else {
                EV << "Received " << msg << " but transmitter busy: queueing up\n";
                msg->setTimestamp();
                queue.insert(msg);
                emit(qlenSignal, queue.getLength());
            }
        }
        //若没有endTransmissionEvent在安排中（在计划中），则直接接受该消息并开始发送，不需要入栈
        else {
            // We are idle, so we can start transmitting right away.
            EV << "Received " << msg << endl;
            emit(queueingTimeSignal, SIMTIME_ZERO);
            startTransmitting(msg);
            emit(busySignal, true);
        }
    }
}

void L2Queue::refreshDisplay() const
{
    getDisplayString().setTagArg("t", 0, isBusy ? "transmitting" : "idle");
    getDisplayString().setTagArg("i", 1, isBusy ? (queue.getLength() >= 3 ? "red" : "yellow") : "");
}

