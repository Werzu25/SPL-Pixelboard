#ifndef mqtt_service_hpp
#define mqtt_service_hpp

#include <PubSubClient.h>


const char* MQTT_Host = "5cb42c9748844ca2bd6d0e57f6406124.s1.eu.hivemq.cloud";

class mqtt_service
{
private:
    char* user;
    char* password;
    int port;
    PubSubClient client;

public:
    mqtt_service(char* user, char* password, int port, PubSubClient client);
    void connect();
    void subscribe(const char* topic);
    void publish(const char* topic, const char* message);
};

#endif