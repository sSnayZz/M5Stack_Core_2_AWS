/**
 * Chronomètre sur le M5Stack
 * Le bouton A (le plus à gauche) permet le contrôle :
 *		- Appuie court : Start / Pause
 *		- Appuie long (si en pause) : Remise à zéro
 * Version de base sur laquelle les étudiants vont ajouter des fonctionnalités
 * 
 * Consulter le fichier readme.md
 */
///////////////////////////////////////////////////////////////////////////////////////////////////
// 3. Utilisation des boutons 
//   3.2 chronomètre

#include <M5Unified.h>
#include "Chronometre.h"

//#define DEBUG // Décommenter pour avoir des infos de debuggage sur le moniteur série (115200 bauds)
#define BAUD_RATE 115200

#define INTENSITE_VIBREUR 255
#define DELAY_SHORT_VIBRATION  80
#define DELAY_LONG_VIBRATION  240

#define CHRONO_FONT_SIZE  5

const char AppTitle[] = "Chronometre";

// Les 3 états du chronomètre
typedef enum {STOP, START, PAUSE} EtatChrono_t;

///////////////////////////////////////////////////////////////////////////////////////////////////
// 3. Utilisation des boutons (chronomètre)

// SETUP ******************************************************************

void setup(void)
{
  #ifdef DEBUG
    auto cfg = M5.config();  // Assign a structure for initializing M5Stack
    // If config is to be set, set it here
    // Example.
    // cfg.external_spk = true;
    cfg.serial_baudrate = BAUD_RATE;
    M5.begin(cfg);    // Init M5Core 2 avec serial port pour debug
    M5.Log.setLogLevel(m5::log_target_t::log_target_serial, ESP_LOG_INFO);  
  #else
    M5.begin();        // Init M5Core 2
  #endif

  // Affichage "Chronometre" en haut de l'écran
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_YELLOW);
  M5.Lcd.setCursor((M5.Lcd.width() - M5.Lcd.textWidth(AppTitle)) / 2, 0); // Calcul pour centrer le texte et positionne le curseur
  M5.Lcd.print(AppTitle);

  // Affichage de la légende du bouton A (BtnA)
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(5, M5.Lcd.height() - M5.Lcd.fontHeight()); // Calcul pour afficher tout en bas de l'écran
  M5.Lcd.setTextColor(TFT_YELLOW);
  M5.Lcd.print("Start-Stop/Reset");

  #ifdef DEBUG  
    M5_LOGI("*** Fin setup ***");
  #endif
}

// LOOP *******************************************************************
void loop() {
  M5Canvas canvasChrono(&M5.Display);      // sprite d'affichage du chronomètre

  EtatChrono_t etatCourant = STOP;  // Le chronomètre démarre dans l'état STOP
  unsigned long startTime;
  unsigned long valChrono = 0;    // Etat STOP => Chrono = 0
  unsigned long lastDisplay = 0;
  uint cpt_aff_colors = 1;
  int aff_colors[5] = {TFT_YELLOW, TFT_RED, TFT_BLUE, TFT_GREEN, TFT_WHITE};
  bool state_aff_dixieme = 1;
  int bw_colors[2] = {TFT_BLACK, TFT_WHITE};
  bool state_bg_colors = 1;

  // Préparation du sprite d'affichage du chronomètre
  M5.Lcd.setTextSize(CHRONO_FONT_SIZE);     // Set the font size
  canvasChrono.createSprite(M5.Lcd.width(), M5.Lcd.fontHeight());
  canvasChrono.setTextColor(TFT_YELLOW);  // Set the font color to yellow
  canvasChrono.setTextSize(CHRONO_FONT_SIZE);     // Set the font size

  while(1) {
    // code exécuté quel que soit l'état
    if(millis() > lastDisplay + 100) {  // Affichage
      lastDisplay = millis();
      #ifdef DEBUG 
        M5_LOGI("etatCourant : %d\n", etatCourant);
      #endif
    }

    displayChrono(&canvasChrono, milisToTime(valChrono, state_aff_dixieme).c_str());
    bgColor(&canvasChrono, bw_colors[!state_bg_colors]);

    if(M5.BtnB.wasHold()) {
        bgColor(&canvasChrono, bw_colors[state_bg_colors]);
        state_bg_colors = !state_bg_colors;
        canvasChrono.setTextColor(bw_colors[state_bg_colors]);
      }
  
    M5.update();  // Pour lecture des boutons

    // Code exécuté selon l'état courant
    switch(etatCourant) {
      case STOP : // STOP State
        if(M5.BtnA.wasClicked()) { // Switch to START state
          startTime = millis();
          shortVibration(INTENSITE_VIBREUR, DELAY_SHORT_VIBRATION);
          etatCourant = START;
        }

        if(M5.BtnB.wasClicked()) {
          canvasChrono.setTextColor(aff_colors[cpt_aff_colors]);
          cpt_aff_colors = (cpt_aff_colors + 1) % 5;
        }

        if(M5.BtnC.wasClicked()) {
          state_aff_dixieme = !state_aff_dixieme;
        }

        break;

      case START : // START State
        valChrono = millis() - startTime;

        if(M5.BtnA.wasClicked()) { // Switch to PAUSE state
          etatCourant = PAUSE;
          shortVibration(INTENSITE_VIBREUR, DELAY_SHORT_VIBRATION);
          startTime = millis() - startTime;
        }
        break;

      case PAUSE : // PAUSE State
        if(M5.BtnA.wasClicked()) { // Switch to START state
          startTime = millis() - startTime;
          shortVibration(INTENSITE_VIBREUR, DELAY_SHORT_VIBRATION);
          etatCourant = START;
        }
        if(M5.BtnA.wasHold()) { // Switch to STOP state
          etatCourant = STOP;
          valChrono = 0;  // Remise à zéro du chrono
          shortVibration(INTENSITE_VIBREUR, DELAY_LONG_VIBRATION);
          displayChrono(&canvasChrono, milisToTime(valChrono, true).c_str());
        }              
        break;

    } // !switch

  } // !while

} // !loop