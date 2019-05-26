/*
 * loonw_timekeeper17.cc
 *
 *  Created on: May 25, 2019
 *      Author: chapd
 */

#include "loonw_timekeeper17.h"

// global API

bool TimeKeeper::init () {
	this->timerid_watermark = 0;
	lpalarm_array = new LPAlarm*[MAX_STORED_TIMERS];
	return true;
}

LPAlarmState TimeKeeper::addAlarm(LPAlarm * alarm) {
	// check if delay is in a reasonable range
	if (alarm->delay_ticks == 0) {
		alarm->setState(LPAlarmState::kErrorInvalid);
		return LPAlarmState::kErrorInvalid;
	}
	// are we out of timers?
	if (this->timerid_watermark >= MAX_STORED_TIMERS)
	{
		alarm->setState( LPAlarmState::kErrorOverFlow);
		return LPAlarmState::kErrorOverFlow;
	}
	// assign a valid timer's ID and increment for the next timer
	alarm->timerid = timerid_watermark;
	lpalarm_array[timerid_watermark] = alarm;

	return alarm->getState();
}

void TimeKeeper::setNextWakeEvent() {
	uint16_t shortest_tcks = 0xFFFF;
	for (uint8_t i = 0; i <= this->timerid_watermark; i++){
		if(shortest_tcks > this->lpalarm_array[i]->delay_ticks) {
			shortest_tcks = this->lpalarm_array[i]->delay_ticks;
		}
	}

}

// local functions
