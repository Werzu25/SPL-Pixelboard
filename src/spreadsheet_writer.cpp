#include <Arduino.h>
#include <ESP_Google_Sheet_Client.h>
#include <GS_SDHelper.h>
#include "spreadsheet_writer.hpp"

void tokenStatusCallback(TokenInfo info) {
    if (info.status == token_status_error) {
        Serial.print("[Spreadsheet_writer] Token error: ");
        Serial.println(info.error.message.c_str());
    } else {
        Serial.print("[Spreadsheet_writer] Token status: ");
        Serial.println(String(info.status));
    }
}

SpreadsheetWriter::SpreadsheetWriter(const char *project_id,
                                       const char *client_email,
                                       const char private_key[],
                                       const char spreadsheet_id[])
    : PROJECT_ID(project_id), CLIENT_EMAIL(client_email),
      PRIVATE_KEY(private_key), spreadsheetId(spreadsheet_id) {
    GSheet.setTokenCallback(tokenStatusCallback);
    GSheet.setPrerefreshSeconds(10 * 60);
}

void SpreadsheetWriter::sendData(char* data[]) {
    if (!GSheet.ready()) {
        Serial.println("[SpreadsheetWriter] GSheet not ready");
        return;
    }

    FirebaseJsonArray* row = new FirebaseJsonArray();
    if (!row) {
        Serial.println("[SpreadsheetWriter] Failed to allocate row");
        return;
    }

    for (int i = 0; i < 4; i++) {
        row->add(data[i] ? data[i] : "");
    }

    FirebaseJsonArray* values = new FirebaseJsonArray();
    if (!values) {
        delete row;
        Serial.println("[SpreadsheetWriter] Failed to allocate values");
        return;
    }
    values->add(*row);

    FirebaseJson* valueRange = new FirebaseJson();
    if (!valueRange) {
        delete row;
        delete values;
        Serial.println("[SpreadsheetWriter] Failed to allocate valueRange");
        return;
    }
    valueRange->set("values", *values);

    FirebaseJson response;
    bool ok = GSheet.values.append(&response, spreadsheetId, "Sheet1!A1", valueRange);

    delete row;
    delete values;
    delete valueRange;
    
    if (!ok) {
        Serial.printf("[SpreadsheetWriter] Error: %s\n", GSheet.errorReason());
    } else {
        Serial.println("[SpreadsheetWriter] Row appended successfully");
    }
}

