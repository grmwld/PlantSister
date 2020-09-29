void connectToWifi();
void setupServer();
void handle_NotFound();
void handle_OnConnect();
String SendHTML(float temperature, float humidity, String last_update);
String processor(const String& var);