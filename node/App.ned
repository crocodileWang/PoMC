package node;

//
// Application model to generate traffic for the network.
//
simple App like IApp
{
    parameters:
        int address;  // local node address
        int totalNodes;
        int initialRep;
        int transaction_limit;
        int randomSeed = intuniform(0, 1000000);
        string evilNodeIDs;
        string miningNodeIDs;
        volatile int packetLength @unit(byte);  // length of one message (fixed! no "volatile" modifier)

        //Senator
        int numberOfSenatorThreshold;
        volatile double SenatorDisseminationTimeInit @unit(s);
        volatile double SenatorDisseminationTime @unit(s);
        volatile double SenatorLifeTime @unit(s);
        
        volatile double MiningNodeDisseminationTimeInit @unit(s);
        volatile double TermOfMiningNode @unit(s);
        volatile double RedundantTimeOfConsensus @unit(s);
        volatile double NoisePower;
        volatile double PathLossExponent;
        volatile double SINRThreshold;
        volatile double RepThreshold;
        volatile double RepMean;
        volatile double RepStandardDeviation;
        
        volatile double generateTransactionTimeInit @unit(s);
        int blockSize;
        
        
        @display("i=block/browser");
        //@signal[endToEndDelay](type="simtime_t");
        //@signal[hopCount](type="long");
        //@signal[sourceAddress](type="long");
        //@statistic[endToEndDelay](title="end-to-end delay of arrived packets"; unit=s; record=vector,mean,max; interpolationmode=none);
        //@statistic[hopCount](title="hop count of arrived packets"; interpolationmode=none; record=vector?,mean,max);
        //@statistic[sourceAddress](title="source address of arrived packets"; interpolationmode=none; record=vector?);
        @signal[transactionDelay](type="simtime_t");
        @statistic[transactionDelay](title="Transaction verify delay";unit=s;record=mean,max;interpolationmode=none);
        //@signal[numTransantion](type="int");
        //@statistic[numTransanction](title="number of total transanctions";record=last);
    gates:
        input in;
        output out;
        output direct;
}


