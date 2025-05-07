#ifndef mqtt_service_hpp
#define mqtt_service_hpp

#include <PubSubClient.h>


class mqtt_service
{
private:
    const char* user;
    const char* password;
    const char* MQTT_Host = "5cb42c9748844ca2bd6d0e57f6406124.s1.eu.hivemq.cloud";
    int port;
    PubSubClient client;

public:
    mqtt_service(const char* user,
    const char* pass,
    int port,
    PubSubClient client);
    void connect();
    void subscribe(const char* topic);
    void publish(const char* topic, const char* message);
};

#endif