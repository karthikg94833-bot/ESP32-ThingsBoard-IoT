#include "State.h"


String bayStatus = "FREE";
float voltage = 0.0, current = 0.0, power = 0.0, energyWh = 0.0, temperature = 0.0;

unsigned long  sessionStartMs=0;
float predictedArrivalProb=0.0;
int   predictedDurationMin=0;

int lastHourOfDay=12;
String loadDecision = "ALLOW";
int throttleLevel = 100;
int peakTariffStartHr = 18;
int peakTariffEndHr = 21;
bool overloadActive = false;
float predictionThreshold = 0.5;
int overloadCurrentA = 10;
int maxStationLoadW = 3000;
bool manualOverrideActive = 0;