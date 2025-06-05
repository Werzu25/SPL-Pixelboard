#ifndef SPREADSHEET_WRITER_HPP
#define SPREADSHEET_WRITER_HPP

#include <Arduino.h>
#include <ESP_Google_Sheet_Client.h>

class SpreadsheetWriter
{
private:
    const char* PROJECT_ID;
    const char* CLIENT_EMAIL;
    const char* PRIVATE_KEY;
    const char* spreadsheetId;
public:
    void sendData(char* data[]);
    SpreadsheetWriter(const char* project_id, const char* client_email, const char private_key[], const char spreadsheet_id[]);
};


#endif
