#ifndef SMM_SEQUENCER_H
#define SMM_SEQUENCER_H

#include "Button.h"
#include "Sequence.h"

class Sequencer;

struct ButtonData
{
    Sequencer* sequencer;
    int id;

    ButtonData(Sequencer* s, int id) : sequencer(s), id(id) {}
};

class Sequencer
{
 public:
    Button b0, b1;
    
    bool timingOut;
    int timeoutLength;
    unsigned long timeoutEndpoint;
    
    Sequence sequence;
    int numSequences;
    Sequence* sequences[N_MAX_SEQUENCES];

    Sequencer() : timingOut(false), numSequences(0) {}

    void setup(int b0pin, int b1pin, int timeoutLength)
    {
	b0.setup(b0pin, Sequencer::buttonCallback, new ButtonData(this, 0));
	b1.setup(b1pin, Sequencer::buttonCallback, new ButtonData(this, 1));
	this->timeoutLength = timeoutLength;
    }

    static void buttonCallback(int pressed, void* data)
    {
	ButtonData* d = data;
	Sequencer* self = d->sequencer;

	if (pressed) {
	    self->sequence.push(d->id);
	}
	else { // released
	    self->timingOut = true;
	    self->timeoutEndpoint = millis() + self->timeoutLength;
	}
    }

    void addSequence(Sequence* s) {
	if (numSequences < N_MAX_SEQUENCES) {
	    sequences[numSequences] = s;
	    numSequences += 1;
	}
    }

    void processSequence()
    {
	for (int i=0; i<numSequences; i++) {
	    if (sequence == *(sequences[i]))
		sequences[i]->action(sequences[i]->data);
	}
	
	sequence.length = 0;
	timingOut = false;
    }

    void update()
    {
	b0.update();
	b1.update();

	if (timingOut && millis() > timeoutEndpoint)
	    processSequence();
    }
};

#endif
