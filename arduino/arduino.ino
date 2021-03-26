#include <Servo.h>

#include "Sequencer.h"

// input buttons, with helper macros for button labels
#define WHITE 0
#define BLUE 1
#define WHITE_PIN 2
#define BLUE_PIN 4


// gesture timeouts
#define GESTURE_TIMEOUT 5000
long gesture_end_time;
bool gesture_set = false;


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Servo setup and positioning
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#define THUMB_PIN 3
#define INDEX_PIN 5
#define MIDDLE_PIN 6
#define RING_PIN 9
#define PINKY_PIN 10

#define SERVO_MIN_US 800
#define SERVO_MAX_US 2200
#define SERVO_DELTA_US SERVO_MIN_US - SERVO_MAX_US

Servo thumb, index, middle, ring, pinky;

// initial setup function for the servos
void setup_servos() {
    thumb.attach(THUMB_PIN);
    index.attach(INDEX_PIN);
    middle.attach(MIDDLE_PIN);
    ring.attach(RING_PIN);
    pinky.attach(PINKY_PIN);
}

// convert a position (0-1) to a microseconds value (SERVO_MIN_US - SERVO_MAX_US).
int position_to_us(float pos) {
    return (SERVO_DELTA_US * pos) + SERVO_MIN_US;
}

// set all finger positions. position 0 is fully retracted; 1 is fully extended.
void set_positions(float thumb_pos,
		   float index_pos,
		   float middle_pos,
		   float ring_pos,
		   float pinky_pos) {
    // set servo positions by the microsecond
    thumb.writeMicroseconds  ( position_to_us(thumb_pos)  );
    index.writeMicroseconds  ( position_to_us(1-index_pos)  ); // inverted position
    middle.writeMicroseconds ( position_to_us(1-middle_pos) ); // inverted position
    ring.writeMicroseconds   ( position_to_us(ring_pos)   );
    pinky.writeMicroseconds  ( position_to_us(pinky_pos)  );

    // set gesture timeout
    gesture_set = true;
    gesture_end_time = millis() + GESTURE_TIMEOUT;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Hand Gestures
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

void gesture_fist() {
    set_positions(0, 0, 0, 0, 0);
}

void gesture_open() {
    set_positions(1, 1, 1, 1, 1);
}

void gesture_point() {
    set_positions(0, 1, 0, 0, 0);
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Main program
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

// sequences
Sequencer s;
Sequence fist_sequence(WHITE, gesture_fist, nullptr);
Sequence open_sequence(BLUE, gesture_open, nullptr);
Sequence point_sequence(BLUE, BLUE, gesture_point, nullptr);

// setup
void setup() {
    setup_servos();

    s.setup(WHITE_PIN, BLUE_PIN, 500);
    s.addSequence(&fist_sequence);
    s.addSequence(&open_sequence);
    s.addSequence(&point_sequence);

    gesture_open();
}


// loop
void loop() {
    // update button sequencer
    s.update();

    if (gesture_set) {
	// check if gesture has timed out
	if (millis() > gesture_end_time) {
	    // reset position
	    gesture_open();
	    // stop checking for timeout
	    gesture_set = false;
	}
    }
}
