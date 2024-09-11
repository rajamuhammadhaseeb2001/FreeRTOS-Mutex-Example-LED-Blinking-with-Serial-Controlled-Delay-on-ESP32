# FreeRTOS-Mutex-Example-LED-Blinking-with-Serial-Controlled-Delay-on-ESP32
This FreeRTOS project demonstrates task synchronization using a mutex. An LED blinks with a user-defined delay, adjustable via serial input. The incTask uses a semaphore to safely access the shared delay variable, ensuring thread-safe operations across tasks on an ESP32 microcontroller.
