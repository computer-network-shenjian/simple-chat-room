
// A simple class responsible for the conversion of data between the forms of CircularQueue
// and DataPacket
class PresentationLayer {
private:
	PacketType curPacket;

public:
    PresentationLayer() = default;
    ~PresentationLayer();

    // Public methods called by the lower layer (session layer).
    int	recvFrame(int clientID, int frameLength, char * frame, PacketType &curPacket);

    // Public methods called by the high layer (application layer)
    int sendFrame(int clientID, int frameLength, char * frame);
    
    int refuseClient(int clientID);
    int syncEnd(int clientID);
    int infoResponse(int clientID, ResponseType response);
    int passwdResponse(int clientID, ResponseType response);
    int syncHistory(int clientID, chatHistory hisory);
    int updateConf(int clientID, configure conf);

    //int unPack()   

private:

};


