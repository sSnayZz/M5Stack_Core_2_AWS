#include "Chronometre.h"

/**************************************************************************
 * Transforme une valeur en millisecondes en une chaine de
 * caractères au format  : 
 *         hh:mm:ss
 *     ou  hh:mm:ss.d si displayDs = true
 * @param ms : valeur en millisecondes à convertir
 * @param displayDs : boolean, pour afficher les dixièmes de secondes ou pas
 * @return chaine de caractère au format hh:mm:ss  ou  hh:mm:ss.d si displayDs = true 
*/
std::string milisToTime(uint32_t ms, bool displayDs = false) {
    uint32_t s = ms / 1000;
    uint32_t m = s / 60;
    uint32_t h = m / 60;

    s %= 60;
    m %= 60;

    char buffer[16];

    if (displayDs)
        std::snprintf(buffer, sizeof(buffer),
                      "%02lu:%02lu:%02lu.%02lu",
                      h, m, s, (ms % 1000) / 10);
    else
        std::snprintf(buffer, sizeof(buffer),
                      "%02lu:%02lu:%02lu",
                      h, m, s);

    return std::string(buffer);
  }

  /**************************************************************************
 * Affiche la chaine de caractère chrono en utilisant le sprite
 * refreshZone (évite de raffraichir tout l'écran)
 * @param refreshZone : sprite d'affichage du chronomètre
 * @param chrono : chaine de caractères à afficher
*/
void displayChrono(M5Canvas * refreshZone, String chrono) {
      refreshZone->fillSprite(TFT_BLACK);
      refreshZone->setCursor(10, 0);
      refreshZone->print(chrono);  

      refreshZone->pushSprite(0, 85);
}

/**************************************************************************
 * Active le vibreur du M5 - ATTENTION fonction BLOQUANTE (présence delay)
 * @param intensite : intensité de la vibration (valeur entre 0 et 255)
 * @param duree : Durée de la vibration en ms
*/
void shortVibration(uint8_t intensite, uint32_t duree)
{
  M5.Power.setVibration(intensite);
  delay(duree);
  M5.Power.setVibration(0);
}  