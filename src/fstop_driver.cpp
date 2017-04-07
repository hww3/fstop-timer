#include "Arduino.h"
#include <Keypad_FStop.h>


#include <LiquidCrystalFast.h>

#include <FreqMeasure.h>


/*
    Copyright (C) 2011 William Brodie-Tyrrell
    william@brodie-tyrrell.org
  
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of   
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

/*
 * This sketch implements an f/stop enlarger timer; it requires 
 * an ATmega328-based Arduino; it will not fit in a -168.
 */

#ifndef _FSTOP_DRIVER
#define _FSTOP_DRIVER

// set this if using the new board layout, otherwise comment out for Rev.A
#define REVISION_B 1

#include <EEPROM.h>
#include <RotaryEncoder.h>
#include <FstopTimer.h>

/**
 * Set this according to the vagaries of your rotary encoder & keypad cable
 */
#define ROTARY_REVERSE 0
#define ROTARY_DBLSTEP 0
#define KEYPAD_REVERSE 0

/**
 * IO pin assignments for PCB Rev.B
 */

#define EXPOSE 17      // high=on
#define EXPOSEBTN 12   // low=depressed (internal pullup)
#define BEEP 24        // requires PWM, not used  
#define BACKLIGHT 17   // requires PWM; high=on

// keypad is 4x4: 3x3 numeric upper left with *0# below that and A-D down the right
// for physical keypad pinout, consider LHS (viewed from front) as pin 1
// 1-4 = rows starting at top
// 5-8 = columns starting at left

// output pins shared with LCD via resistors
#define SCANCOL0 11    // left column 147*
#define SCANCOL1 10
#define SCANCOL2 9
#define SCANCOL3 8    // right column ABCD

#define SCANROW0 4    // top row 123A
#define SCANROW1 5
#define SCANROW2 45
#define SCANROW3 7    // bottom row *0#D

// pins for HD44780 in 4-bit mode; RW grounded.
#define LCDD7 34
#define LCDD6 33
#define LCDD5 32
#define LCDD4 31
#define LCDEN 30
#define LCDRW 29
#define LCDRS 28
 
// light-sensor pins
#define TSLQ 16      // actually wired to both D5 and D8, to measure freq or period


// 
// 20 and 21 are taken by the rotary encoder - fixed assignment as they
// require interrupts

/**
 * Instances of static interface objects
 */
LiquidCrystalFast disp(LCDRS, LCDRW, LCDEN, LCDD4, LCDD5, LCDD6, LCDD7);
SMSKeypad keys(SCANCOL0, SCANCOL1, SCANCOL2, SCANCOL3, SCANROW0, SCANROW1, SCANROW2, SCANROW3, KEYPAD_REVERSE);
ButtonDebounce expbtn(EXPOSEBTN);
RotaryEncoder rotary(ROTARY_REVERSE, ROTARY_DBLSTEP);
// all the guts are in this object
FstopTimer fst(disp, keys, rotary, expbtn, EXPOSE, BEEP, BACKLIGHT);

/**
 * Arduino boot/init function
 */
void setup()
{
   // init raw IO for LCD
   pinMode(LCDRS, OUTPUT);
   pinMode(LCDEN, OUTPUT);
   pinMode(LCDD4, OUTPUT);
   pinMode(LCDD5, OUTPUT);
   pinMode(LCDD6, OUTPUT);
   pinMode(LCDD7, OUTPUT);
   
   disp.begin(16, 2);
   keys.begin();
   rotary.begin();
   fst.begin();
}

/**
 * Arduino main-program loop.
 * Processes the current state, looks for transitions to other states.
 */
void loop()
{
  fst.poll();
}

#endif
