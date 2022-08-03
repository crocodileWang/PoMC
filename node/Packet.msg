

cplusplus {{
}}


packet Packet
{
    int srcAddr @packetData;
    int destAddr @packetData;
    int hopCount @packetData;   
   
    int packetType @packetData;  

    //Node View
    int nodeID @packetData;
    int posX @packetData;
    int posY @packetData;
    //Send Transaction
    int transactionValue @packetData;    
    double time @packetData;
    int userXID @packetData;
    int senatorAddress @packetData;
    int transactionSourceAddress @packetData;
    double TimeOfCreateTransaction @packetData;
    int transactionID @packetData;
    int transactionNodeID[] @packetData;
    int transactionIdMsg[] @packetData;
    simtime_t transactionTime[] @packetData;
    int transactionVal[] @packetData;
    //Transaction
    int blockID @packetData;
   	int preHash @packetData;
    //PoMF
    double MF @packetData;
    double candidateTime @packetData;
    int neighbours[] @packetData;
    bool vote[] @packetData;

    
    
}
    