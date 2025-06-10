#ifndef mqtt_hpp
#define mqtt_hpp

#include <PubSubClient.h>

class MqttManager {
  public:
    MqttManager(const char *user, const char *pass, int port, const char *host);
    PubSubClient client;
    void connect(void callbackfunction(char *, byte *, unsigned int));
    void subscribe(const char *topic);
    void publish(const char *topic, const char *message);

  private:
    const char *user;
    const char *password;
    const char *host;
    const int port;
};

#endif
