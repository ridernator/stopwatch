#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>

int lapPressed = 0;
int stopPressed = 0;

void sleepLoopSignalHandler(const int signalNumber) {
   switch (signalNumber) {
      case SIGUSR1 : {
         lapPressed = 1;

         break;
      }

      case SIGUSR2 : {
         stopPressed = 1;

         break;
      }

      default : {
         fprintf(stderr, "Unhandled signal : %i", signalNumber);
      }
   }
}

unsigned long long timevalToUll(const struct timeval time) {
   return time.tv_sec * 1000000llu + time.tv_usec;
}

unsigned long long getTimeNowUs() {
   struct timeval timeNow;
   gettimeofday(&timeNow, NULL);

   return timevalToUll(timeNow);
}

void *sleepLoop() {
   unsigned long long increment = 1000000;
   unsigned long long uStartTime = getTimeNowUs();
   unsigned long long uTimeNow;
   unsigned long long uLastLapTime = uStartTime;
   unsigned long long uTimeNext = uStartTime + increment;

   signal(SIGUSR1, sleepLoopSignalHandler);
   signal(SIGUSR2, sleepLoopSignalHandler);

   while (1) {
      uTimeNow = getTimeNowUs();

      if (usleep(uTimeNext - uTimeNow) == 0) {
         uTimeNow = getTimeNowUs();
         fprintf(stdout, "Time Elapsed : %0.3fs\r", (uTimeNow - uStartTime) / 1000000.0);
         fflush(stdout);

         uTimeNext += increment;
      } else {
         if (lapPressed == 1) {
            uTimeNow = getTimeNowUs();
            fprintf(stdout, "Lap Time : %0.3f, Split Time : %0.3fs\n", (uTimeNow - uLastLapTime) / 1000000.0, (uTimeNow - uStartTime) / 1000000.0);
            fflush(stdout);

            uLastLapTime = uTimeNow;
            lapPressed = 0;
         } else if (stopPressed == 1) {
            uTimeNow = getTimeNowUs();
            fprintf(stdout, "Total Time Elapsed : %0.3fs\n", (uTimeNow - uStartTime)/1000000.0);
            fflush(stdout);

            break;
         }
      }
   }
}

void turnOnLineBuffering() {
   struct termios termiosStruct;

   tcgetattr(STDIN_FILENO, &termiosStruct);
   termiosStruct.c_lflag |= (ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &termiosStruct);
}

void turnOffLineBuffering() {
   struct termios termiosStruct;

   tcgetattr(STDIN_FILENO, &termiosStruct);
   termiosStruct.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &termiosStruct);
}

int main() {
   int shouldExit = 1;
   pthread_t printThread;

   turnOffLineBuffering();

   if(pthread_create(&printThread, NULL, sleepLoop, NULL)) {
      fprintf(stderr, "Error creating thread\n");
      exit(EXIT_FAILURE);
   }

   while (shouldExit) {
      switch (getchar()) {
         case ' ' : {
            pthread_kill(printThread, SIGUSR1);

            break;
         }

         case '\n' : {
            pthread_kill(printThread, SIGUSR2);
            shouldExit = 0;

            break;
         }

         default : {
            // meh
         }
      }
   }

   pthread_join(printThread, NULL);

   turnOnLineBuffering();
}