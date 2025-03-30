# AODV & PCF MAC Implementation in NS-3

## 📌 Installation Steps
1. Install NS-3 dependencies:
   ```sh
   sudo apt update && sudo apt install g++ python3 cmake ninja-build
2. Clone ns-3 
    git clone https://gitlab.com/nsnam/ns-3-dev.git
    cd ns-3-dev
3. Configure and build NS-3
               ./ns3 configure
              ./ns3 build

4. Copy AODV & PCF-MAC code to scratch/  :
    cp aodv-manet.cc scratch/
    cp pcf-mac.cc scratch/
5. Run the simulation :-
     ./ns3 run scratch/aodv-manet
     ./ns3 run scratch/pcf-mac
6. Analyze packet tracing :

 a) for AODV 
      

    tcpdump -r aodv-manet-0-0.pcap
    tcpdump -r aodv-manet-0-0.pcap | grep "aodv"
 b) for PCF-MAC 

tcpdump -n -tt -r pcf-mac-1-0.pcap
     

 






































































