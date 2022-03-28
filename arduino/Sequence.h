#ifndef SMM_SEQUENCE_H
#define SMM_SEQUENCE_H

#ifndef MAX_SEQUENCE_LENGTH
#define MAX_SEQUENCE_LENGTH 10
#endif

#ifndef N_MAX_SEQUENCES
#define N_MAX_SEQUENCES 10
#endif

class Sequence
{
public:
    int buttons[MAX_SEQUENCE_LENGTH];
    int length;

    void (*action)(void*);
    void* data;

    Sequence()
	: length(0), action(nullptr), data(nullptr)
    {}
    Sequence(int button, void (*action)(void*), void* data)
	: length(1), action(action), data(data)
    { buttons[0] = button; }
    Sequence(int b0, int b1, void (*action)(void*), void* data)
	: length(2), action(action), data(data)
    { buttons[0] = b0; buttons[1] = b1; }
    Sequence(int b0, int b1, int b2, void (*action)(void*), void* data)
	: length(3), action(action), data(data)
    { buttons[0] = b0; buttons[1] = b1; buttons[2] = b2; }
    Sequence(int b0, int b1, int b2, int b3, void (*action)(void*), void* data)
	: length(4), action(action), data(data)
    { buttons[0] = b0; buttons[1] = b1; buttons[2] = b2; buttons[3] = b3; }
    

    void push(int button)
    {
	if (length >= MAX_SEQUENCE_LENGTH) return;
	buttons[length] = button;
	length += 1;
    }

    void print()
    {
	Serial.print("Sequence [ ");
	for(int i=0; i<length-1; i++) {
	    Serial.print(buttons[i]);
	    Serial.print(", ");
	}
	Serial.print(buttons[length-1]);
	Serial.println(" ]");
    }
	    

    bool operator==(Sequence& other)
    {
	if (length != other.length)
	    return false;

	for (int i=0; i<length; i++) {
	    if (buttons[i] != other.buttons[i])
		return false;
	}
	return true;
    }
};

#endif
