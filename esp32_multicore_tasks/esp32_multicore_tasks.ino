#define BUILTIN_LED 37
#define TASK1_RATE 500
#define TASK2_RATE 500

TaskHandle_t Task1, Task2;
SemaphoreHandle_t baton;

int t2, t3;
unsigned long loops1 = 10, loops2 = 10, qq = 0;
float t1;


void artificialSWLoad(){
  for(long i = 0; i < loops1; i++){
    for(long j = 1; j < loops2; j++){
      qq++;
      t1 = 5000.0 * i;
      t2 = 150 * 1234 * i;
      t3 = j % 554;
    }
  }
  delay(1000);
}

void artificialHWLoad(){
  digitalWrite(BUILTIN_LED, HIGH);
  delay(500);
  digitalWrite(BUILTIN_LED, LOW);
  delay(500);
}

void codeForTask1(void *parameter){                                 // Loop function for the task in core 1
  for(;;){
    long start = millis();

    artificialHWLoad();

    Serial.print("Finish Task 1 which runs on Core "); Serial.println(xPortGetCoreID());
    Serial.print("Time "); Serial.println(millis() - start);
    delay(TASK1_RATE);
  }
}

void codeForTask2(void *parameter){
  for(;;){
    long start = millis();

    artificialSWLoad();

    Serial.print("\tFinish Task 2 which runs on Core "); Serial.println(xPortGetCoreID());
    Serial.print("\tVariable values t1 = "); Serial.print(t1); Serial.print(", t2 = "); Serial.print(t2); Serial.print(" and t3 = "); Serial.println(t3);
    Serial.print("\tTime "); Serial.println(millis() - start);
    delay(TASK2_RATE);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);

  baton = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
    codeForTask1,                                                    // Task function
    "Task1",                                                         // Task name
    10000,                                                           // Task stack size
    NULL,                                                            // Task parameter
    1,                                                               // Task priority
    &Task1,                                                          // Task handle to keep track of it
    0                                                                // Task core
  );

  delay(500);                                                        // Needed to comfortably start-up Task1

  xTaskCreatePinnedToCore(codeForTask2, "Task2", 10000, NULL, 1, &Task2, 1);
}

void loop() {
  delay(1000);                                                       // If no delay is added, as loop() runs in core 1, the high demand can slow the activity on that same core
}
