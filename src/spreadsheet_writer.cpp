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

void SpreadsheetWriter::sendData(char data[][4]) {
    if (!GSheet.ready()) {
        Serial.println("[SpreadsheetWriter] GSheet not ready");
        return;
    }
    FirebaseJsonArray row;
    for (int i = 0; i < 4; i++) {
        row.add(data[i]);
    }
    FirebaseJsonArray values;
    values.add(row);
    FirebaseJson valueRange;
    valueRange.set("values", values);
    String payload;
    valueRange.toString(payload, false);
    Serial.print("[SpreadsheetWriter] payload: ");
    Serial.println(payload);
    FirebaseJson response;
    bool ok = GSheet.values.append(&response, spreadsheetId,
                                   "Sheet1!A1",
                                   &valueRange);
    if (!ok) {
        Serial.print("[SpreadsheetWriter] append error: ");
        Serial.println(GSheet.errorReason());
        
        String resp;
        response.toString(resp, false);
        Serial.print("[SpreadsheetWriter] response: ");
        Serial.println(resp);
    } else {
        Serial.println("[SpreadsheetWriter] row appended");
    }
}

