#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>

#define LAP_SIGNAL  SIGUSR1
#define STOP_SIGNAL SIGUSR2

#define LAP_CHAR  ' '
#define STOP_CHAR '\n'

// Flags to tell the sleepLoop which "button" has been pressed
int lapPressed = 0;
int stopPressed = 0;

/**
 * Signal handler for signals passed to us by the other thread
 */
void sleepLoopSignalHandler(const int signalNumber) {
   switch (signalNumber) {
      case LAP_SIGNAL : {
         lapPressed = 1;

         break;
      }

      case STOP_SIGNAL : {
         stopPressed = 1;

         break;
      }

      default : {
         fprintf(stderr, "Unhandled signal : %i", signalNumber);
      }
   }
}

/**
 * Convert a timeval structure to an unsigned long
 */
unsigned long long timevalToUll(const struct timeval time) {
   return time.tv_sec * 1000000llu + time.tv_usec;
}

/**
 * Get the current time as an unsigned long long
 */
unsigned long long getTimeNowUs() {
   struct timeval timeNow;
   gettimeofday(&timeNow, NULL);

   return timevalToUll(timeNow);
}

/**
 * Turn line buffering back on
 */ 
void turnOnLineBuffering() {
   struct termios termiosStruct;

   tcgetattr(STDIN_FILENO, &termiosStruct);
   termiosStruct.c_lflag |= (ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &termiosStruct);
}

/**
 * Turn off line buffering so we can listen for single characters from the user
 */
void turnOffLineBuffering() {
   struct termios termiosStruct;

   tcgetattr(STDIN_FILENO, &termiosStruct);
   termiosStruct.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &termiosStruct);
}

/**
 * This function runs in its own thread and periodically prints out the elapsed time.
 * It waits for signals sent to it from its parent thread to tell it what to do
 */
void* sleepLoop() {
   unsigned long long increment = 1000000;
   unsigned long long uStartTime = getTimeNowUs();
   unsigned long long uTimeNow;
   unsigned long long uLastLapTime = uStartTime;
   unsigned long long uTimeNext = uStartTime + increment;

   // Register signal handlers
   signal(LAP_SIGNAL, sleepLoopSignalHandler);
   signal(STOP_SIGNAL, sleepLoopSignalHandler);

   // For ever
   while (1) {
      uTimeNow = getTimeNowUs();

      // Sleep for interval microseconds (minus our processing time)
      if (usleep(uTimeNext - uTimeNow) == 0) {
         // If no signal caught then print out elapsed time
         uTimeNow = getTimeNowUs();
         fprintf(stdout, "\rTime Elapsed : %0.3fs", (uTimeNow - uStartTime) / 1000000.0);
         fflush(stdout);

         uTimeNext += increment;
      } else { // Signal caught
         if (lapPressed == 1) {
            // If the lap button has been pressed
            uTimeNow = getTimeNowUs();
            fprintf(stdout, "\rLap Time : %0.3f, Split Time : %0.3fs\n", (uTimeNow - uLastLapTime) / 1000000.0, (uTimeNow - uStartTime) / 1000000.0);
            fflush(stdout);

            // Reset lap time
            uLastLapTime = uTimeNow;
            lapPressed = 0;
         } else if (stopPressed == 1) {
            // If the stop button has been pressed
            uTimeNow = getTimeNowUs();
            fprintf(stdout, "\rTotal Time Elapsed : %0.3fs\n", (uTimeNow - uStartTime) / 1000000.0);
            fflush(stdout);

            // Exit loop as we have been told to stop
            break;
         }
      }
   }
   
   return NULL;
}

/**
 * Main function
 */
int main() {
   int shouldExit = 0;
   pthread_t printThread;

   // Turn off line buffering so we can listen for single characters from the user
   turnOffLineBuffering();

   // Create thread which sleeps and waits for our signals
   if(pthread_create(&printThread, NULL, sleepLoop, NULL)) {
      fprintf(stderr, "Error creating thread\n");

      exit(EXIT_FAILURE);
   }

   // Loop until told to stop, waiting for input from the user
   while (!shouldExit) {
      switch (getchar()) {
         case LAP_CHAR : {
            // Send the relevant signal to the other thread
            pthread_kill(printThread, LAP_SIGNAL);

            break;
         }

         // Enter means "stop"
         case STOP_CHAR : {
            // Send the relevant signal to the other thread
            pthread_kill(printThread, STOP_SIGNAL);
            shouldExit = 1;

            break;
         }

         // Ignore everything else
         default : {
            // Ignore
            
            break;
         }
      }
   }

   // Wait for other thread to end
   pthread_join(printThread, NULL);

   // Reset line buffering
   turnOnLineBuffering();
   
   return EXIT_SUCCESS;
}
