#include "types.hpp"
#include "client.hpp"
#include <iostream>

class ApplicationLayer {

public:
    ApplicationLayer() = default;
    ~ApplicationLayer();

    void MessageToApp(Client * client);
private:
    
};

void ApplicationLayer::MessageToApp(Client * client){
	std::cout<<"test"<<std::endl;
	return;
}
