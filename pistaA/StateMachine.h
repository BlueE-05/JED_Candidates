// Dana Elizabeth Torres Estrada ._.
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

// Definición de estados
enum State {
    START,
    FETCH_BALL,
    GRAB_BALL,
    FETCH_EXIT,
    FINISH
};

class StateMachine {
private:
    State currentState;
    void phaseStart();
    void phaseFetchBall();
    void phaseGrabBall();
    void phaseFetchExit();
    void phaseFinish();

public:
    StateMachine() : currentState(START) {} // Constructor de la clase

    // Controlador de estados
    void run() {
        switch (currentState) {
            case START:
                Serial.println("Iniciando fase START");
                phaseStart();
                break;
            case FETCH_BALL:
                Serial.println("Iniciando fase FETCH BALL");
                phaseFetchBall();
                break;
            case GRAB_BALL:
                Serial.println("Iniciando fase GRAB BALL");
                phaseGrabBall();
                break;
            case FETCH_EXIT:
                Serial.println("Iniciando fase FETCH EXIT");
                phaseFetchExit();
                break;
            case FINISH:
                Serial.println("Iniciando fase FINISH");
                phaseFinish();
                break;
        }
    }
};

#endif // STATEMACHINE_H