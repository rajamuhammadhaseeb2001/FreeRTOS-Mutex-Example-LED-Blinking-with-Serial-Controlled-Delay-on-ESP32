#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1;
#endif

static SemaphoreHandle_t mutex;
static int delay_arg = 0; // Default delay (ms)
const int minDelay = 1;    // Minimum delay to avoid zero delay

// Define the LED pin
const int ledPin = 2; // Use the appropriate pin for your LED

void incTask(void *parameters)
{
  while (1)
  {
    int num;

    // Take the mutex to access the shared resource
    xSemaphoreTake(mutex, portMAX_DELAY);

    // Read the delay_arg (shared resource)
    num = delay_arg;
    //Serial.print("Using delay: ");
    //Serial.println(num);

    // Release the mutex
    xSemaphoreGive(mutex);

    // Configure the LED pin as an output
    pinMode(ledPin, OUTPUT);
    if(num > 0)
    {
    // Blink the LED with the specified delay
    digitalWrite(ledPin, HIGH);
    vTaskDelay(num / portTICK_PERIOD_MS); // Delay for `num` milliseconds
    digitalWrite(ledPin, LOW);
    vTaskDelay(num / portTICK_PERIOD_MS); // Delay for `num` milliseconds
    }
  }
}

void setup()
{
  // Initialize the serial communication
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("-----FreeRTOS MUTEX DEMO-----");
  Serial.println("ENTER A NUMBER FOR DELAY (ms)");

  // Create the mutex
  mutex = xSemaphoreCreateMutex();

  // Create the task and pass NULL as the parameter (task uses global delay_arg)
  xTaskCreatePinnedToCore(
    incTask,
    "BLINK LED",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  Serial.print("DONE");
}

void loop() 
{
  while (1)
  {
    if (Serial.available() > 0)
    {
      // Take the mutex to update the delay_arg safely
      xSemaphoreTake(mutex, portMAX_DELAY);

      // Read the new parameter
      int newDelay = Serial.parseInt();
      
      // Check if the new delay is valid and greater than the minimum delay
      if (newDelay >= minDelay)
      {
        delay_arg = newDelay;
        Serial.print("Updated delay to: ");
        Serial.println(delay_arg);
      }
      else
      {
        //Serial.println("Invalid delay. Must be greater than minimum delay.");
      }

      // Release the mutex
      xSemaphoreGive(mutex);

      // Small delay to prevent rapid serial reads
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
