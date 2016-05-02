/******************************************************************************
 * Copyright (c) ArduinoDynAmbi 2016. All rights reserved.                    *
 *                                                                            *
 * @author Tim Visee                                                          *
 * @website http://timvisee.com/                                              *
 *                                                                            *
 * Open Source != No Copyright                                                *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included    *
 * in all copies or substantial portions of the Software.                     *
 *                                                                            *
 * You should have received a copy of The MIT License (MIT) along with this   *
 * program. If not, see <http://opensource.org/licenses/MIT/>.                *
 ******************************************************************************/

#include "Core.h"
#include "Random.h"
#include "Timer.h"
#include "StripController.h"

StripController c = StripController();

Core::Core() {
    // Field initialization
    this->started = false;
}

void Core::setup() {
    // Initial startup delay
    delay(200);

    // Enable the serial connection
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"
    if(SERIAL_USB_ENABLED)
        Serial.begin(SERIAL_USB_BAUD);
#pragma clang diagnostic pop

    // Randomize the random seed
    Random::randomize();

    // Set up the status LED
    LedManager::statusLed.setupPin();

    // Enable pulsing for the status LED
    LedManager::statusLed.setPulsing(true);

    // Initialize the LED strip
    c.init();

    // The device has been started, update the flag
    this->started = true;

    // Start the data stream
    // TODO: Replace this with proper handshaking!
    Serial.print("ozy");
}

void Core::loop() {
    // Wait for the begin bytes
    // TODO: Replace this with proper handshaking!
    while(Serial.read() != 'o');
    if(Serial.read() != 'z')
        return;

    // Stream the strip data
    c.stream();

    // End
    // TODO: Replace this with proper packet handling
    Serial.write('y');

    // Update everything
    updateLogic();
}

void Core::updateLogic() {
    // Update the LED manager
    LedManager::update();
}

void Core::smartDelay(int delay) {
    // Create a timer, to track the passed time
    Timer timer(delay);

    // Start the timer
    timer.start();

    // Call the update method until the timer has passed the specified delay
    while(!timer.isFinished())
        updateLogic();
}