package networks;

import node.Node;
import ned.DatarateChannel;


network Net5
{
    parameters:
        int totalNodes;
        int evilNodeIdSingle = intuniform(0, totalNodes-1);
        double delay @unit(s);
        double datarate @unit(kbps);
        //double connectedness;

    types:
        //here it set the connection speed and dealy (eventualy also packet drop)
        channel C extends DatarateChannel
        {
            delay = delay;
            datarate = datarate;
        }
    submodules:
        rte[totalNodes]: Node {
            address = index;
        }
    connections :
        for i=0..totalNodes-1,for j=0..totalNodes-1{
        	rte[i].port++ <--> C <--> rte[j].port++ if i!=j && ((pow((rte[i].posX-rte[j].posX),2)+pow((rte[i].posY-rte[j].posY),2)) <40000) ;
        	//&& uniform(0,1) < connectedness  && ((pow((rte[i].posX-rte[j].posX),2)+pow((rte[i].posY-rte[j].posY),2)) <10000);
        }
//        rte[0].port++ <--> C <--> rte[1].port++;
//        rte[0].port++ <--> C <--> rte[2].port++;
//        rte[0].port++ <--> C <--> rte[4].port++;
//        rte[0].port++ <--> C <--> rte[5].port++;
//        rte[1].port++ <--> C <--> rte[4].port++;
//        rte[1].port++ <--> C <--> rte[6].port++;
//        rte[1].port++ <--> C <--> rte[0].port++;
//        rte[1].port++ <--> C <--> rte[2].port++;       
//        rte[2].port++ <--> C <--> rte[1].port++;
//        rte[2].port++ <--> C <--> rte[0].port++;
//        rte[2].port++ <--> C <--> rte[3].port++;
//        rte[2].port++ <--> C <--> rte[4].port++;
//        rte[2].port++ <--> C <--> rte[5].port++;
//        rte[2].port++ <--> C <--> rte[9].port++;
//        rte[3].port++ <--> C <--> rte[0].port++;
//        rte[3].port++ <--> C <--> rte[9].port++;
//        rte[3].port++ <--> C <--> rte[6].port++;
//        rte[4].port++ <--> C <--> rte[5].port++;
//        rte[4].port++ <--> C <--> rte[3].port++;
//        rte[4].port++ <--> C <--> rte[2].port++;
//        rte[4].port++ <--> C <--> rte[1].port++;
//        rte[5].port++ <--> C <--> rte[2].port++;
//        rte[5].port++ <--> C <--> rte[3].port++;
//        rte[5].port++ <--> C <--> rte[8].port++;
//        rte[5].port++ <--> C <--> rte[9].port++;
//        rte[5].port++ <--> C <--> rte[1].port++;
//        rte[6].port++ <--> C <--> rte[3].port++;
//        rte[6].port++ <--> C <--> rte[9].port++;
//        rte[7].port++ <--> C <--> rte[8].port++;
//        rte[7].port++ <--> C <--> rte[2].port++;
//        rte[7].port++ <--> C <--> rte[4].port++;
//        rte[8].port++ <--> C <--> rte[1].port++;
//        rte[8].port++ <--> C <--> rte[9].port++;
//        rte[8].port++ <--> C <--> rte[6].port++;
//        rte[9].port++ <--> C <--> rte[4].port++;
//        rte[9].port++ <--> C <--> rte[3].port++;
//        rte[9].port++ <--> C <--> rte[1].port++;

}
