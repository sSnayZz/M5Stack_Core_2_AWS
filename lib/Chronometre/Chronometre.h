#pragma once // Evite les inclusions multiples

#include <string>
#include <M5Unified.h>

std::string milisToTime(uint32_t ms, bool displayDs);
void displayChrono(M5Canvas * refreshZone, String chrono);
void shortVibration(uint8_t intensite, uint32_t duree);
void bgColor(M5Canvas * refreshzone, int color);
