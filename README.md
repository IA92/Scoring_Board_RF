# Scoring_Board_RF
An archive of old scoring board project for badminton games.
Note: Code are not well written and managed. Instructions are not fully accurate.

## Version of arduino files
- scoringBoard.ino: Testing LED panel
- scoringIR.ino, scoringIR_1 and scoringIR_2: 1 LED panel with IR instructions
- HunterBadminton_ProjectRF: 1 LED panel with RF instructions
- HunterBadminton_ProjectRF_Larger: 2 LED panel with RF instructions

## Getting Started with the Project RF Version
### Keyfob
- Get 433MHz key fobs from ebay.
- Use a 433MHz transmitter to transmit the desired code to be cloned by the key fob
- Set the key fob to be in programming mode and clone the transmitted code for each button. 
  - Reference video: https://www.youtube.com/watch?v=l-k_XvkZ0jE
### Code
- Ensure that `buttonCodes` array corresponds to the programmed instructions of the 4 buttons in the key fob

## References for the code in the libraries
- Freetronics lib https://github.com/freetronics/DMD/blob/master/DMD.cpp
- TimerOne lib http://code.google.com/p/arduino-timerone/downloads/list
- RF lib https://github.com/sui77/rc-switch/blob/master/RCSwitch.h
