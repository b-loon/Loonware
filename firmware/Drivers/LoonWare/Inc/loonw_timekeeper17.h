/*
 * loonw_timekeeper17.h
 *
 *  Created on: May 25, 2019
 *      Author: chapd
 */
#include <functional>
#include <stdint.h>
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_lptim.h"

#ifndef LOONWARE_INC_LOONW_TIMEKEEPER17_H_
#define LOONWARE_INC_LOONW_TIMEKEEPER17_H_

#define MAX_STORED_TIMERS 8

enum class DriverState {
	kUnknown = 0,
	kUninitialized = 1,
	kInitializing = 2,
	kReady = 3,
	kSuspended = 4,
	kError = 5
};

enum class LPAlarmState {
	kUnknown = 0,
	kUninitialized = 1,
	kActive = 2,
	kSlept = 3,
};

class LPAlarm {
	LPAlarm(uint16_t delay, bool recur) : state_(LPAlarmState::kUninitialized) {
		delay_ticks = delay;
		recurring = recur;
	}
	// when we time out do we go again?
	bool recurring;
	// Callback when your LPAlarm Expires
	void timExpCallback(std::function<void(int)> callback);
	// Timer ID is a unique LPAlarm reference so that
	// you can search for your unique instance
	uint16_t timerid;
	// ticks for delay
	uint16_t delay_ticks;

	LPAlarmState getState() {return state_;};
protected:
	// only allow our close friends to touch our state
	void setState(LPAlarmState s) {state_ = s;};

private:
	LPAlarmState state_;
};

class TimeKeeper {
	friend class LPAlarm;
public:
	/////////////
	//
	//	Driver API
	//
	/////////////
	TimeKeeper(LPTIM_HandleTypeDef * lptim) : state_(DriverState::kUninitialized) {
		LPTIMx = lptim;
	};
	~TimeKeeper() {};
	// initializer for the driver
	bool init();
	// Reports the Driver State
	DriverState getState()  {return state_;};
	//////////////
	//
	//	Alarm Scheduler API
	//
	/////////////
	// Adds an alarm to our TimeKeeper
	// updates LPAlarm with a unique ID
	bool addAlarm(LPAlarm * alarm);
	// temporarily sleep alarm
	bool sleepAlarm(LPAlarm * alarm);
	// Resume slept alarm
	bool resumeAlarm(LPAlarm * alarm);

	bool stopAlarms();
	bool resumeAlarms();


protected:
	DriverState state_;
	LPTIM_HandleTypeDef * LPTIMx;
};



#endif /* LOONWARE_INC_LOONW_TIMEKEEPER17_H_ */
